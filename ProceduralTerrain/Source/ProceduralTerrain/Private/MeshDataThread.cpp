#pragma once

#include "MeshDataThread.h"
#include "MeshData.h"
#include "GenerateNoiseMap.h"
#include "MapGenerators.h"
#include "MatrixObject.h"
#include "MeshGenerator.h"

MeshDataThread::MeshDataThread(int32 callbackId, FMapSetting mapSetting, MatrixObject<float> mapData, TArray<FMapColor> regions, TQueue<FMeshThreadInfo, EQueueMode::Spsc> *meshDataThreadInfoQueue) 
{
    CallbackId = callbackId;
    MapSetting = mapSetting;
    MapData = mapData;
    MeshDataThreadInfoQueue = meshDataThreadInfoQueue;
}

uint32 MeshDataThread::Run() 
{
    bStopThread = false;
    FMeshData meshData = UMeshGenerator::GeneratorTerrainMesh(MapData, MapSetting.HeightMultiplier, MapSetting.LevelOfDetail, Regions);

    FScopeLock ScopeLock(&Mutex);
    MeshDataThreadInfoQueue->Enqueue(FMeshThreadInfo(CallbackId, meshData));

    return 0;
}

void MeshDataThread::Stop() 
{
    bStopThread = true;
}