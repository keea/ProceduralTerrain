// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapDisplay.h"
#include "MeshData.h"
#include "GenerateNoiseMap.h"
#include "MapGeneratorEditor.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALTERRAIN_API UMapGeneratorEditor : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MapGenerator")
	static void GenerateMapDataDisplay(AMapDisplay * mapDispaly, class UMapSettingDataAsset* MapSettingDataAsset);

	UFUNCTION(BlueprintCallable, Category = "MapGenerator")
	static void GenerateMapDataDisplayWithColor(AMapDisplay * mapDispaly, class UMapSettingDataAsset* MapSettingDataAsset, class UMapColorDataAsset * MapColorDataAsset);

	UFUNCTION(BlueprintCallable, Category = "MapGenerator")
	static void UpdateMapDataDisplay(AMapDisplay * mapDispaly, class UMapSettingDataAsset* MapSettingDataAsset);

	UFUNCTION(BlueprintCallable, Category = "MapGenerator")
	static void UpdateMapDataDisplayWithColor(AMapDisplay * mapDispaly, class UMapSettingDataAsset* MapSettingDataAsset, class UMapColorDataAsset* MapColorDataAsset);

	//만들어진 데이터 에셋을 저장한다.
	UFUNCTION(BlueprintCallable, Category = "MapGenerator")
	static void SaveMapSettingDataAsset(FString AssetName, UMapSettingDataAsset * MapSettingDataAsset);

	//만들어진 컬러맵 데이터 에셋을 저장시킨다.
	UFUNCTION(BlueprintCallable, Category = "MapGenerator")
	static void SaveMapColorDataAsset(FString AssetName, UMapColorDataAsset * MapColorDataAsset);
};