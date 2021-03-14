// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/BoxSphereBounds.h"
#include "MatrixObject.h"
#include "MeshData.h"
#include "MapDisplay.h"
#include <functional>
#include "EndlessTerrains.generated.h"

USTRUCT(BlueprintType)
struct FLODInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MapGenerator")
	int lod;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MapGenerator")
	float visibleDstThreshold;
};

USTRUCT(BlueprintType)
struct FLODMesh
{
	GENERATED_BODY()

public:
	FMeshData MeshData;
	bool hasRequestedMesh = false;
	bool hasMesh = false;
	int lod;

	FLODMesh() : FLODMesh(0) {}

	FLODMesh(int lod);

	void OnMeshDataReceived(FMeshData meshData);
	void RequestMeshData();
};

USTRUCT(BlueprintType)
struct FTerrainChunks
{
	GENERATED_BODY()

public:
	FTerrainChunks() : FTerrainChunks(nullptr, FVector2D::ZeroVector, 0) {}
	FTerrainChunks(AEndlessTerrains *owner, FVector2D coord, int size);
	void UpdateTerrainChunk(AEndlessTerrains *owner, FVector viewerPosition, float maxViewDst);
	void SetVisible(bool visible);
	bool GetVisible();
	void OnMapDataReceived(AEndlessTerrains *owner, MatrixObject<float> noiseMapData);
	void OnMeshDataReceived(FMeshData meshData, int lodIndex);

public:
	//스폰할 엑터 선언
	AMapDisplay *MeshObject;

	//std::function<void(MatrixObject<float>, UMapGenerators*)> Rq_MapData;
private:
	FVector2D position;
	FBoxSphereBounds bounds;
	FVector positionV3;
	class UMapGenerators *cMapGenerators;

	int Size;
	bool Visible;

private:
	int previousLODIndex = -1;

public:
	TArray<FLODMesh> lodMeshes;
	bool mapDataReceived = false;
	MatrixObject<float> NoiseMapData;
	TArray<FLODInfo> detailLevels;

	FVector2D viewedChunkCoord;
	AEndlessTerrains *Owner;
};

UCLASS()
class PROCEDURALTERRAIN_API AEndlessTerrains : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEndlessTerrains();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EndlessTerrain")
	float MaxViewDst = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EndlessTerrain")
	AActor *viewerActor;

	FVector viewerPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EndlessTerrain")
	int ChunkSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EndlessTerrain")
	int ChunkVisibleInViewDst;

private:
	void UpdateVisibleChunks();

	TMap<FVector2D, FTerrainChunks> TerrainChunkMap;
	TArray<FTerrainChunks> terrainChunksVisibleLastUpdate;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EndlessTerrain")
	float scale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapDisplay)
	TSubclassOf<class AMapDisplay> MapDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapData)
	TSubclassOf<class UMapGenerators> MapGenerators;

	UPROPERTY()
	class UMapGenerators *cMapGenerators;

	void OnMapDataReceived(FVector2D viewedChunkCoord, MatrixObject<float> noiseMapData);
	void OnMeshDataReceived(FVector2D viewedChunkCoord, FMeshData meshData, int lodIndex);

	void AddTerrainChunksVisibleLastUpdate(FTerrainChunks * terrainChunks);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EndlessTerrain")
	TArray<FLODInfo> detailLevels;

private:
	// 세팅이 다 되어 있는 상태인가?
	bool isSettingDone = true;
};