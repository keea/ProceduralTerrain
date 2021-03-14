// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshData.h"
#include "MapSettingDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALTERRAIN_API UMapSettingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FMapSetting MapSetting;
};
