// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MatrixObject.h"
#include "EndlessTerrains.h"
#include "MapDisplay.h"
#include "MeshData.h"
#include "Tickable.h"
#include <functional>
#include "MapColorDataAsset.h"
#include "Materials/MaterialInterface.h"
#include "MapSettingDataAsset.h"
#include "MapGenerators.generated.h"

USTRUCT(BlueprintType)
struct FMeshDataInfo
{
	GENERATED_BODY()
public:
	FVector2D viewedChunkCoord;
	int lodIndex = -1;
};

USTRUCT(BlueprintType)
struct FMapThreadInfo
{
	GENERATED_BODY()

public:
	FMapThreadInfo()
	{
		this->callbackId = -1;
		MatrixObject<float> noiseMap(0, 0);
		this->noiseMapData = noiseMap;
	}

	FMapThreadInfo(int32 callbackId, MatrixObject<float> noiseMapData)
	{
		this->callbackId = callbackId;
		this->noiseMapData = noiseMapData;
	}

	int32 callbackId = -1;
	MatrixObject<float> noiseMapData;
};

USTRUCT(BlueprintType)
struct FMeshThreadInfo
{
	GENERATED_BODY()

public:
	FMeshThreadInfo()
	{
		this->callbackId = -1;
	}

	FMeshThreadInfo(int32 callbackId, FMeshData meshData)
	{
		this->callbackId = callbackId;
		this->meshData = meshData;
	}

	int32 callbackId = -1;
	FMeshData meshData;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROCEDURALTERRAIN_API UMapGenerators : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	static void GenerateMap(MatrixObject<float> heightMap, float HeightMultiplier, int LevelOfDetail);
	static void GenerateMap(AMapDisplay * mapDisplay, MatrixObject<float> heightMap, float HeightMultiplier, int LevelOfDetail);

	static void GenerateMap(AMapDisplay * mapDisplay, MatrixObject<float> heightMap, float HeightMultiplier, int LevelOfDetail, TArray<FMapColor> regions);

	static void UpdateMap(AMapDisplay * mapDisplay, MatrixObject<float> heightMap, float HeightMultiplier, int LevelOfDetail);
	static void UpdateMap(AMapDisplay * mapDisplay, MatrixObject<float> heightMap, float HeightMultiplier, int LevelOfDetail, TArray<FMapColor> regions);

	void RequestMapData(FVector2D viewedChunkCoord, AEndlessTerrains* endlessTerrain);
	
	void RequestMeshData(FVector2D viewedChunkCoord, MatrixObject<float> mapData, int lodIndex, int lod, AEndlessTerrains* endlessTerrain);

protected:
	class MapDataThread * mapDataThread = nullptr;
	class MeshDataThread * meshDataThread = nullptr;

	TArray<class MapDataThread*> mapDataThreadArray;

	FRunnableThread *CurrentThread = nullptr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapSetting")
	UMapSettingDataAsset * MapSettingDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapSetting")
	UMaterialInterface * MapMaterial;

	//맵 정보 프로퍼티
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MapSetting")
	FMapSetting MapSetting;

	//컬러 정보 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapSetting")
	UMapColorDataAsset * MapColorDataAsset;


	TQueue<FMapThreadInfo, EQueueMode::Spsc> mapDataThreadInfoQueue;
	TQueue<FMeshThreadInfo, EQueueMode::Spsc> meshDataThreadInfoQueue;

public:
	UFUNCTION(BlueprintCallable, Category = Test)
	void CallTimer();
	void TestTimer();
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	UWorld* GetWorld() const override;
	float TestCounter;
	bool isTimer = false;
	FTimerHandle tHandle;

	AEndlessTerrains * EndlessTerrains;

private:
	TMap<int32, FVector2D> ChunkCoordMap;

	TMap<int32, FMeshDataInfo> meshDataMap;

	int32 mapDataId = 0;
	int32 meshDataId = 0;

	//테스트 부분
public:
#if WITH_EDITOR
	//프로퍼티가 변경될 경우 처리
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
#endif
};