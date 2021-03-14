// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapColorDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMapColor
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name = FString("None");
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Height = 1.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor Color = FColor::Black;
};

UCLASS()
class PROCEDURALTERRAIN_API UMapColorDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapGenerator")
	TArray<FMapColor> Regions;
};
