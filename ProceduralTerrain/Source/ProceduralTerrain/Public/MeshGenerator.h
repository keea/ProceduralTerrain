// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MatrixObject.h"
#include "MeshData.h"
#include "MapColorDataAsset.h"
#include "MeshGenerator.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROCEDURALTERRAIN_API UMeshGenerator : public UObject
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintCallable, Category = "MeshGenerator")
	static FMeshData GeneratorTerrainMesh(MatrixObject<float> heightMap, float heightMultiplier, int levelOfDetail, TArray<FMapColor> regions);

	static FMeshData GeneratorTerrainMesh(MatrixObject<float> heightMap, float heightMultiplier, int levelOfDetail);
};