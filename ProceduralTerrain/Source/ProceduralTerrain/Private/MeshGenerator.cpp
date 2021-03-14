// Fill out your copyright notice in the Description page of Project Settings.
#include "MeshGenerator.h"
#include "MatrixObject.h"

FMeshData UMeshGenerator::GeneratorTerrainMesh(MatrixObject<float> heightMap, float heightMultiplier, int levelOfDetail, TArray<FMapColor> regions)
{
    //여기서 LOD 관련 작업을 함.
    int width = heightMap.GetColumnsSize();
    int height = heightMap.GetRowsSize();
    UE_LOG(LogTemp, Log, TEXT("heightMap :: %d %d"), width, height);

    int meshSimpleificationIncrement = (levelOfDetail == 0) ? 1 : levelOfDetail * 2;
    int cellSize = (width - 1) / meshSimpleificationIncrement + 1;
    UE_LOG(LogTemp, Log, TEXT("cellSizeMap :: %d"), cellSize);

    FMeshData data = FMeshData(cellSize, cellSize);
    int vertexIndex = 0;

    for (int32 y = 0; y < height; y += meshSimpleificationIncrement)
    {
        for (int32 x = 0; x < width; x += meshSimpleificationIncrement)
        {
            float Mapheight = heightMap(x, y) * heightMultiplier;

            float currentHeight = heightMap(x, y);
            for (int i = 0; i < regions.Num(); i++)
            {
                if (currentHeight >= regions[i].Height)
                {
                    data.vertexColors[vertexIndex] = regions[i].Color;
                }
                else
                {
                    break;
                }
            }

            data.vertices[vertexIndex] = FVector(x, y, Mapheight);
            data.uvs[vertexIndex] = FVector2D((x / (float)width), (y / (float)height));

            if (x < width - 1 && y < height - 1)
            {
                //current vertex ,  current vertex + row, current vertex + row + one right
                data.AddTriangle(vertexIndex, vertexIndex + cellSize, vertexIndex + cellSize + 1);
                //current vertex, current vertex + row + one right, current vertex + one right
                data.AddTriangle(vertexIndex, vertexIndex + cellSize + 1, vertexIndex + 1);

                //노멀 구하기 위한 버텍스 계산
                FVector TopLeftVector = data.vertices[vertexIndex];                                             //current
                FVector BottomLeftVerctor = FVector(x, y + 1, heightMap(x, y + 1) * heightMultiplier);          //current + row
                FVector BottomRightVerctor = FVector(x + 1, y + 1, heightMap(x + 1, y + 1) * heightMultiplier); //current + row + 1
                FVector TopRightVerctor = FVector(x + 1, y, heightMap(x + 1, y) * heightMultiplier);            //current + 1

                data.normals[vertexIndex] = FVector::CrossProduct(BottomRightVerctor - TopLeftVector, BottomLeftVerctor - TopLeftVector).GetSafeNormal();
            }
            vertexIndex++;
        }
    }
    return data;
}

FMeshData UMeshGenerator::GeneratorTerrainMesh(MatrixObject<float> heightMap, float heightMultiplier, int levelOfDetail) 
{
	//여기서 LOD 관련 작업을 함.
    int width = heightMap.GetColumnsSize();
    int height = heightMap.GetRowsSize();
    UE_LOG(LogTemp, Log, TEXT("heightMap :: %d %d"), width, height);

    int meshSimpleificationIncrement = (levelOfDetail == 0) ? 1 : levelOfDetail * 2;
    int cellSize = (width - 1) / meshSimpleificationIncrement + 1;
    UE_LOG(LogTemp, Log, TEXT("cellSizeMap :: %d"), cellSize);

    FMeshData data = FMeshData(cellSize, cellSize);
    int vertexIndex = 0;

    for (int32 y = 0; y < height; y += meshSimpleificationIncrement)
    {
        for (int32 x = 0; x < width; x += meshSimpleificationIncrement)
        {
            float Mapheight = heightMap(x, y) * heightMultiplier;

            data.vertices[vertexIndex] = FVector(x, y, Mapheight);
            data.uvs[vertexIndex] = FVector2D((x / (float)width), (y / (float)height));

            if (x < width - 1 && y < height - 1)
            {
                //current vertex ,  current vertex + row, current vertex + row + one right
                data.AddTriangle(vertexIndex, vertexIndex + cellSize, vertexIndex + cellSize + 1);
                //current vertex, current vertex + row + one right, current vertex + one right
                data.AddTriangle(vertexIndex, vertexIndex + cellSize + 1, vertexIndex + 1);

                //노멀 구하기 위한 버텍스 계산
                FVector TopLeftVector = data.vertices[vertexIndex];                                             //current
                FVector BottomLeftVerctor = FVector(x, y + 1, heightMap(x, y + 1) * heightMultiplier);          //current + row
                FVector BottomRightVerctor = FVector(x + 1, y + 1, heightMap(x + 1, y + 1) * heightMultiplier); //current + row + 1
                FVector TopRightVerctor = FVector(x + 1, y, heightMap(x + 1, y) * heightMultiplier);            //current + 1

                data.normals[vertexIndex] = FVector::CrossProduct(BottomRightVerctor - TopLeftVector, BottomLeftVerctor - TopLeftVector).GetSafeNormal();
            }
            vertexIndex++;
        }
    }
    return data;
}