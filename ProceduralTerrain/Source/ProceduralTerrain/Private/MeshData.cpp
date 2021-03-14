// Fill out your copyright notice in the Description page of Project Settings.

#include "MeshData.h"

void FMeshData::AddTriangle(int a, int b, int c)
{
    triangles[triangleIndex] = a;
    triangles[triangleIndex + 1] = b;
    triangles[triangleIndex + 2] = c;

    triangleIndex += 3;
}

void FMapSetting::Init() 
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