// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MatrixObject.h"
#include "GenerateNoiseMap.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ENormalizeMode : uint8
{
	Local UMETA(DisplayName = "Local"),
	Global UMETA(DisplayName = "Global"),
};

UCLASS()
class PROCEDURALTERRAIN_API UGenerateNoiseMap : public UObject
{
	GENERATED_BODY()

public:
	static MatrixObject<float> GenerateNoiseMap(int mapWidth, int mapHeight, float NoiseScale, int Seed, float Octaves, float Persistance, float Lacunarity, FVector2D Offset, ENormalizeMode NormalizeMode);

	static MatrixObject<float> GenerateNoiseMap(int mapWidth, int mapHeight, FVector2D coord, float NoiseScale, int Seed, float Octaves, float Persistance, float Lacunarity, FVector2D Offset, ENormalizeMode NormalizeMode);
};