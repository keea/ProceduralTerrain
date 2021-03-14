// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProceduralMeshComponent.h"
#include "MatrixObject.h"
#include "GenerateNoiseMap.h"
#include "MeshData.generated.h"

/**
 * Mesh Data
 */
USTRUCT(BlueprintType)
struct FMeshData
{
	GENERATED_BODY()

	FMeshData(int meshWidth = 0, int meshHeight = 0)
	{
		triangleIndex = 0;
		MapMaterial = nullptr;

		vertices.Init(FVector::ZeroVector, meshWidth * meshHeight);
		normals.Init(FVector::ZeroVector, meshWidth * meshHeight);
		uvs.Init(FVector2D::ZeroVector, meshWidth * meshHeight);
		vertexColors.Init(FLinearColor::Black, meshWidth * meshHeight);
		tangents.Init(FProcMeshTangent(0.0f, 0.0f, 0.0f), meshWidth * meshHeight);

		triangles.Init(0, ((meshWidth - 1) * (meshHeight - 1)) * 6);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> vertices; //정점

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> normals; //법선

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> triangles; // 삼각형

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> uvs; //uvs

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLinearColor> vertexColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FProcMeshTangent> tangents; //접선

	UPROPERTY()
	int triangleIndex;

	UPROPERTY()
	UMaterialInterface *MapMaterial;

	void AddTriangle(int a, int b, int c);
};

/* MapSetting */
USTRUCT(BlueprintType)
struct FMapSetting
{
	GENERATED_BODY()

	FMapSetting()
	{
		MapChunkSize = 161;
		HeightMultiplier = 1.0f;
		LevelOfDetail = 0;
		NoiseScale = 0.3f;
		Seed = 0;
		Octaves = 2;
		Persistance = 0.8f;
		Lacunarity = 1.0f;
		Offset = FVector2D::ZeroVector;
		NormalizeMode = ENormalizeMode::Global;
	}

	FMapSetting(int MapChunkSize, float HeightMultiplier, int LevelOfDetail, float NoiseScale, int Seed, int Octaves, float Persistance, float Lacunarity, FVector2D Offset, ENormalizeMode NormalizeMode) : MapChunkSize(MapChunkSize), HeightMultiplier(HeightMultiplier), LevelOfDetail(LevelOfDetail), NoiseScale(NoiseScale), Seed(Seed), Octaves(Octaves), Persistance(Persistance), Lacunarity(Lacunarity), Offset(Offset), NormalizeMode(NormalizeMode){};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MapGenerator")
	int MapChunkSize = 161;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MapGenerator")
	int LevelOfDetail;

	//높이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapGenerator")
	float HeightMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Setting")
	float NoiseScale = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Setting")
	int Seed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Setting", meta = (UIMIN = "0.0"))
	int Octaves = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Setting")
	float Persistance = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Setting", meta = (UIMIN = "1.0"))
	float Lacunarity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Setting")
	FVector2D Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Setting")
	ENormalizeMode NormalizeMode;

	void Init(); //값 초기화 하기
};