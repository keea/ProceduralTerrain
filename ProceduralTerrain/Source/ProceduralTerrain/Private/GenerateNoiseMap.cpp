// Fill out your copyright notice in the Description page of Project Settings.

#include "GenerateNoiseMap.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

MatrixObject<float> UGenerateNoiseMap::GenerateNoiseMap(int mapWidth, int mapHeight, float NoiseScale, int Seed, float Octaves, float Persistance, float Lacunarity, FVector2D Offset, ENormalizeMode NormalizeMode)
{
    MatrixObject<float> noiseMap(mapWidth, mapHeight);

    FRandomStream Stream(Seed);
    TArray<FVector2D> octaveOffsets;
    octaveOffsets.SetNumZeroed(Octaves);

    float maxPossibleHeight = 0;
    float amplitude = 1;
    float frequency = 1;

    for (int i = 0; i < Octaves; i++)
    {
        float offsetX = Stream.RandRange(-100000, 100000) + Offset.X;
        float offsetY = Stream.RandRange(-100000, 100000) - Offset.Y;
        octaveOffsets[i] = FVector2D(offsetX, offsetY);

        maxPossibleHeight += amplitude;
        amplitude *= Persistance;
    }

    //scale이 0로면 에러남.
    if (NoiseScale <= 0)
    {
        NoiseScale = 0.0001f;
    }

    float maxLocalNoiseHeight = TNumericLimits<float>::Min();
    float minLocalNoiseHeight = TNumericLimits<float>::Max();

    float halfWidth = mapWidth / 2.0f;
    float halfHeight = mapHeight / 2.0f;

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            amplitude = 1;
            frequency = 1;
            float noiseHeight = 0;

            for (int i = 0; i < Octaves; i++)
            {
                float sampleX = (x - halfWidth + octaveOffsets[i].X) / NoiseScale * frequency;
                float sampleY = (y - halfHeight + octaveOffsets[i].Y) / NoiseScale * frequency;

                //float perlinValue = FMath::PerlinNoise2D(FVector2D(sampleX, sampleY)) * 0.5f + 1.0f;
                float perlinValue = FMath::PerlinNoise2D(FVector2D(sampleX, sampleY)) * 2.0f - 1.0f;
                noiseHeight += perlinValue * amplitude;

                amplitude *= Persistance;
                frequency *= Lacunarity;
            }

            if (noiseHeight > maxLocalNoiseHeight)
            {
                maxLocalNoiseHeight = noiseHeight;
            }

            else if (noiseHeight < minLocalNoiseHeight)
            {
                minLocalNoiseHeight = noiseHeight;
            }
            noiseMap(x, y) = noiseHeight;
        }
    }

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (NormalizeMode == ENormalizeMode::Local)
            {
                noiseMap(x, y) = UKismetMathLibrary::InverseLerp(minLocalNoiseHeight, maxLocalNoiseHeight, noiseMap(x, y));
            }
            else
            {
                float normalizedHeight = (noiseMap(x, y) + 1.0f) / (maxPossibleHeight / 0.9f);
                noiseMap(x, y) = FMath::Clamp(normalizedHeight, 0.0f, TNumericLimits<float>::Max());
                //noiseMap(x, y) = normalizedHeight;
                //UE_LOG(LogTemp, Log, TEXT("noiseMap(%d, %d) = %f"), x,y, noiseMap(x, y));
            }
        }
    }
    return noiseMap;
}

MatrixObject<float> UGenerateNoiseMap::GenerateNoiseMap(int mapWidth, int mapHeight, FVector2D coord, float NoiseScale, int Seed, float Octaves, float Persistance, float Lacunarity, FVector2D Offset, ENormalizeMode NormalizeMode) 
{
	MatrixObject<float> noiseMap(mapWidth, mapHeight);

    FRandomStream Stream(Seed);
    TArray<FVector2D> octaveOffsets;
    octaveOffsets.SetNumZeroed(Octaves);

    float maxPossibleHeight = 0;
    float amplitude = 1;
    float frequency = 1;

    for (int i = 0; i < Octaves; i++)
    {
        float offsetX = Stream.RandRange(-100000, 100000) + Offset.X;
        float offsetY = Stream.RandRange(-100000, 100000) - Offset.Y;
        octaveOffsets[i] = FVector2D(offsetX, offsetY);

        maxPossibleHeight += amplitude;
        amplitude *= Persistance;
    }

    //scale이 0로면 에러남.
    if (NoiseScale <= 0)
    {
        NoiseScale = 0.0001f;
    }

    float maxLocalNoiseHeight = TNumericLimits<float>::Min();
    float minLocalNoiseHeight = TNumericLimits<float>::Max();

    float halfWidth = mapWidth / 2.0f;
    float halfHeight = mapHeight / 2.0f;

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            amplitude = 1;
            frequency = 1;
            float noiseHeight = 0;

            for (int i = 0; i < Octaves; i++)
            {
                float sampleX = ((x + coord.X) - halfWidth + octaveOffsets[i].X) / NoiseScale * frequency;
                float sampleY = ((y + coord.Y) - halfHeight + octaveOffsets[i].Y) / NoiseScale * frequency;

                //float perlinValue = FMath::PerlinNoise2D(FVector2D(sampleX, sampleY)) * 0.5f + 1.0f;
                float perlinValue = FMath::PerlinNoise2D(FVector2D(sampleX, sampleY)) * 2.0f - 1.0f;
                noiseHeight += perlinValue * amplitude;

                amplitude *= Persistance;
                frequency *= Lacunarity;
            }

            if (noiseHeight > maxLocalNoiseHeight)
            {
                maxLocalNoiseHeight = noiseHeight;
            }

            else if (noiseHeight < minLocalNoiseHeight)
            {
                minLocalNoiseHeight = noiseHeight;
            }
            noiseMap(x, y) = noiseHeight;
        }
    }

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (NormalizeMode == ENormalizeMode::Local)
            {
                noiseMap(x, y) = UKismetMathLibrary::InverseLerp(minLocalNoiseHeight, maxLocalNoiseHeight, noiseMap(x, y));
            }
            else
            {
                float normalizedHeight = (noiseMap(x, y) + 1.0f) / (maxPossibleHeight / 0.9f);
                noiseMap(x, y) = FMath::Clamp(normalizedHeight, 0.0f, TNumericLimits<float>::Max());
                //noiseMap(x, y) = normalizedHeight;
                //UE_LOG(LogTemp, Log, TEXT("noiseMap(%d, %d) = %f"), x,y, noiseMap(x, y));
            }
        }
    }
    return noiseMap;
}