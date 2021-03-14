// Fill out your copyright notice in the Description page of Project Settings.

#include "MapDisplay.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AMapDisplay::AMapDisplay()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	pm = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = pm;
	//pm->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	//SetRootComponent(pm);
}

// Called when the game starts or when spawned
void AMapDisplay::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMapDisplay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//메시를 그린다.
void AMapDisplay::DrawMesh(FMeshData mapData)
{
	pm->ClearAllMeshSections();
	pm->CreateMeshSection_LinearColor(0, mapData.vertices, mapData.triangles, mapData.normals, mapData.uvs, mapData.vertexColors, mapData.tangents, true);

	// Enable collision data
	//pm->ContainsPhysicsTriMeshData(true);
	if (mapData.MapMaterial)
	{
		pm->SetMaterial(0, mapData.MapMaterial);
	}
}

void AMapDisplay::UpdateMesh(FMeshData mapData) 
{
	pm->UpdateMeshSection_LinearColor(0, mapData.vertices, mapData.normals, mapData.uvs, mapData.vertexColors, mapData.tangents);
}


void AMapDisplay::DrawMesh(int lodIndex, FMeshData mapData)
{
	if (LodIndex != lodIndex)
	{
		LodIndex = lodIndex;
		DrawMesh(mapData);
	}
}

//테스트용 메시 그리기.
void AMapDisplay::DrawMeshTest(float width, float height)
{
	TArray<FVector> vertices; //정점

	TArray<FVector> normals; //법선

	TArray<int32> triangles; // 삼각형

	TArray<FVector2D> uvs; //uvs

	TArray<FLinearColor> vertexColors;

	TArray<FProcMeshTangent> tangents; //접선

	//정점 추가
	vertices.Add(FVector(0.0f, 0.0f, 0.0f));	//0
	vertices.Add(FVector(width, 0.0f, 0.0f));	//1
	vertices.Add(FVector(0.0f, height, 0.0f));	//2
	vertices.Add(FVector(width, height, 0.0f)); //3

	//정점의 인덱스 추가
	triangles.Add(0);
	triangles.Add(2);
	triangles.Add(3);
	triangles.Add(0);
	triangles.Add(3);
	triangles.Add(1);

	uvs.Init(FVector2D(0.0f, 0.0f), vertices.Num());
	normals.Init(FVector(0.0f, 0.0f, 0.0f), vertices.Num());
	vertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), vertices.Num());
	tangents.Init(FProcMeshTangent(1.0f, 0.0f, 0.0f), vertices.Num());

	pm->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uvs, vertexColors, tangents, false);
}