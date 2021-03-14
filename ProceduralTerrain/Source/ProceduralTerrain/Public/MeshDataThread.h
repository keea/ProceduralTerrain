// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAL/Runnable.h"
#include "Containers/Array.h"
#include "MeshData.h"
#include "Containers/Queue.h"
#include "EndlessTerrains.h"
#include "MapGenerators.h"
#include "MatrixObject.h"
#include "Misc/ScopeLock.h"
#include "MapColorDataAsset.h"

/**
 * 
 */
class PROCEDURALTERRAIN_API MeshDataThread : public FRunnable
{
public:
    int32 CallbackId;
    FMapSetting MapSetting;
    MatrixObject<float> MapData;
    TArray<FMapColor> Regions;

    MeshDataThread(int32 callback, FMapSetting mapSetting, MatrixObject<float> mapData, TArray<FMapColor> regions, TQueue<FMeshThreadInfo, EQueueMode::Spsc> *meshDataThreadInfoQueue);

    // FRunnable 함수
    virtual uint32 Run() override;
    virtual void Stop() override;
    // FRunnable

protected:
    bool bStopThread = false;
    FCriticalSection Mutex;
    TQueue<FMeshThreadInfo, EQueueMode::Spsc> *MeshDataThreadInfoQueue;
};