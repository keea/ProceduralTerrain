// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessTerrains.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/Blueprint.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "MapDisplay.h"
#include "MapGenerators.h"

// Sets default values
AEndlessTerrains::AEndlessTerrains()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEndlessTerrains::BeginPlay()
{
	Super::BeginPlay();

	// viewerActor, MapGenerators, detailLevels 체크
	if (viewerActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("viewerActor 를 설정해주기 바랍니다."));
		isSettingDone = false;
	}

	if (MapGenerators == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MapGenerators 를 설정해주기 바랍니다."));
		isSettingDone = false;
	}

	if (detailLevels.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("detailLevels을 설정해주기 바랍니다."));
		isSettingDone = false;
	}

	if (isSettingDone)
	{
		ChunkSize = 161 - 1;

		MaxViewDst = detailLevels[detailLevels.Num() - 1].visibleDstThreshold;
		ChunkVisibleInViewDst = FMath::RoundToInt(MaxViewDst / ChunkSize);
		cMapGenerators = NewObject<UMapGenerators>(this, *(MapGenerators));
	}
}

// Called every frame
void AEndlessTerrains::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isSettingDone)
		return;

	if (viewerActor)
		viewerPosition = FVector(viewerActor->GetActorLocation().X, viewerActor->GetActorLocation().Y, 0.0f) / scale;
	else
	{
		ACharacter *myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		viewerPosition = myCharacter->GetActorLocation() / scale;
	}
	UpdateVisibleChunks();
}

void AEndlessTerrains::UpdateVisibleChunks()
{
	for (int i = 0; i < terrainChunksVisibleLastUpdate.Num(); i++)
	{
		terrainChunksVisibleLastUpdate[i].SetVisible(false);
	}

	terrainChunksVisibleLastUpdate.Empty();

	//청크의 좌표 얻기.
	int currentChunkCoordX = FMath::RoundToInt(viewerPosition.X / ChunkSize);
	int currentChunkCoordY = FMath::RoundToInt(viewerPosition.Y / ChunkSize);

	for (int yOffset = -ChunkVisibleInViewDst; yOffset <= ChunkVisibleInViewDst; yOffset++)
	{
		for (int xOffset = -ChunkVisibleInViewDst; xOffset <= ChunkVisibleInViewDst; xOffset++)
		{
			FVector2D viewedChunkCoord = FVector2D(currentChunkCoordX + xOffset, currentChunkCoordY + yOffset);

			if (TerrainChunkMap.Contains(viewedChunkCoord))
			{
				TerrainChunkMap[viewedChunkCoord].UpdateTerrainChunk(this, viewerPosition, MaxViewDst);

				if (TerrainChunkMap[viewedChunkCoord].GetVisible())
				{
					//terrainChunksVisibleLastUpdate.Add(TerrainChunkMap[viewedChunkCoord]);
				}
			}
			else
			{
				TerrainChunkMap.Add(viewedChunkCoord, FTerrainChunks(this, viewedChunkCoord, ChunkSize));
				FVector2D positionV2 = viewedChunkCoord * ChunkSize;
				//UE_LOG(LogTemp, Log, TEXT("positionV2 :: %s"), *positionV2.ToString());
				cMapGenerators->RequestMapData(viewedChunkCoord, this);
			}
		}
	}
}

void AEndlessTerrains::OnMapDataReceived(FVector2D viewedChunkCoord, MatrixObject<float> noiseMapData)
{
	if (TerrainChunkMap.Find(viewedChunkCoord))
	{
		TerrainChunkMap[viewedChunkCoord].OnMapDataReceived(this, noiseMapData);
	}
}

void AEndlessTerrains::OnMeshDataReceived(FVector2D viewedChunkCoord, FMeshData meshData, int lodIndex)
{
	if (TerrainChunkMap.Find(viewedChunkCoord))
	{
		TerrainChunkMap[viewedChunkCoord].OnMeshDataReceived(meshData, lodIndex);
	}
}

void AEndlessTerrains::AddTerrainChunksVisibleLastUpdate(FTerrainChunks *terrainChunks)
{
	terrainChunksVisibleLastUpdate.Add(*terrainChunks);
}

FTerrainChunks::FTerrainChunks(AEndlessTerrains *Owner, FVector2D coord, int size)
{
	if (Owner == nullptr)
		return;

	this->Owner = Owner;
	viewedChunkCoord = coord;

	detailLevels = Owner->detailLevels;

	Size = size;
	position = coord * size;

	bounds.Origin = FVector(position, 0.0);
	bounds.BoxExtent = FVector(1, 1, 0) * size;

	positionV3 = FVector(position.X * Owner->scale, position.Y * Owner->scale, 0);

	if (GEngine)
	{
		UWorld *world = GEngine->GetWorldContexts()[0].World();
		if (world)
		{
			// 액터 스폰
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Owner;
			FRotator rotator;

			if (Owner->MapDisplay)
			{
				//스폰된 엑터 저장
				MeshObject = world->SpawnActor<AMapDisplay>(Owner->MapDisplay, positionV3, rotator, SpawnParams);
				MeshObject->SetActorScale3D(FVector::OneVector * Owner->scale);
				SetVisible(false);

				lodMeshes.SetNumZeroed(Owner->detailLevels.Num());
				for (int i = 0; i < Owner->detailLevels.Num(); i++)
				{
					lodMeshes[i] = FLODMesh(Owner->detailLevels[i].lod);
				}
			}
		}
	}
}

void FTerrainChunks::OnMapDataReceived(AEndlessTerrains *owner, MatrixObject<float> noiseMapData)
{
	UE_LOG(LogTemp, Log, TEXT("OnMapDataReceived"));

	NoiseMapData = noiseMapData;
	mapDataReceived = true;
	UpdateTerrainChunk(owner, owner->viewerPosition, owner->MaxViewDst);
}

void FTerrainChunks::OnMeshDataReceived(FMeshData meshData, int lodIndex)
{
	UE_LOG(LogTemp, Log, TEXT("OnMeshDataReceived"));

	lodMeshes[lodIndex].OnMeshDataReceived(meshData);
}

void FTerrainChunks::UpdateTerrainChunk(AEndlessTerrains *owner, FVector viewerPosition, float maxViewDst)
{
	if (mapDataReceived)
	{
		//경계면에 대한 거리를 구함.
		float viewerDstFromNearestEdge = FMath::Sqrt(bounds.ComputeSquaredDistanceFromBoxToPoint(viewerPosition));

		bool visible = viewerDstFromNearestEdge <= maxViewDst;

		if (visible)
		{
			int lodIndex = 0;
			for (int i = 0; i < detailLevels.Num() - 1; i++)
			{
				if (viewerDstFromNearestEdge > detailLevels[i].visibleDstThreshold)
				{
					lodIndex = i + 1;
				}
				else
				{
					break;
				}
			}

			//lod 관련 작업
			if (lodIndex != previousLODIndex)
			{
				if (lodMeshes[lodIndex].hasMesh)
				{
					previousLODIndex = lodIndex;
					MeshObject->DrawMesh(lodIndex, lodMeshes[lodIndex].MeshData);
				}
				else if (!lodMeshes[lodIndex].hasRequestedMesh)
				{
					//lodMesh의 키를 보내고 받을 때 그 키(lodIndex) 값을 가지고 설정한다.
					lodMeshes[lodIndex].RequestMeshData();
					Owner->cMapGenerators->RequestMeshData(viewedChunkCoord, NoiseMapData, lodIndex, lodMeshes[lodIndex].lod, Owner);
				}
			}

			owner->AddTerrainChunksVisibleLastUpdate(this);
		}

		SetVisible(visible);
	}
}

void FTerrainChunks::SetVisible(bool visible)
{
	if (MeshObject != nullptr)
	{
		// 메시오브젝트를 활성/비활성
		MeshObject->SetActorHiddenInGame(!visible);	  //액터를 보임(false)/감춤(true)
		MeshObject->SetActorEnableCollision(visible); //충돌 계산 활성(true)/비활성화(false)
		MeshObject->SetActorTickEnabled(visible);	  //틱(업데이트 함수) 작동(true)/중지(false)
	}
}

bool FTerrainChunks::GetVisible()
{
	if (MeshObject)
		return (!MeshObject->IsHidden());
	else
		return false;
}

FLODMesh::FLODMesh(int lod)
{
	this->lod = lod;
}

void FLODMesh::RequestMeshData()
{
	hasRequestedMesh = true;
}

void FLODMesh::OnMeshDataReceived(FMeshData meshData)
{
	MeshData = meshData;
	hasMesh = true;
}