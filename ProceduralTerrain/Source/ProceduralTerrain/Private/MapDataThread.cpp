// Fill out your copyright notice in the Description page of Project Settings.


#include "MapDataThread.h"
#include "MeshData.h"
#include "GenerateNoiseMap.h"
#include "MapGenerators.h"
#include "MatrixObject.h"

MapDataThread::MapDataThread(int32 callbackId, FMapSetting mapSetting, FVector2D coord, TQueue<FMapThreadInfo, EQueueMode::Spsc> * mapDataThreadInfoQueue)
{
    CallbackId = callbackId;
    MapSetting = mapSetting;
    MapDataThreadInfoQueue = mapDataThreadInfoQueue;
    Coord = coord;
}

uint32 MapDataThread::Run() 
{
	bStopThread = false;

    FVector2D coordPosition = Coord * (MapSetting.MapChunkSize - 1);

    //일단 맵을 만드는 계산하는 것이 있어야한다.
    MatrixObject<float> NoiseMap = UGenerateNoiseMap::GenerateNoiseMap(MapSetting.MapChunkSize, MapSetting.MapChunkSize, coordPosition, MapSetting.NoiseScale, MapSetting.Seed, MapSetting.Octaves, MapSetting.Persistance, MapSetting.Lacunarity, MapSetting.Offset, MapSetting.NormalizeMode);
    
    FScopeLock ScopeLock(&Mutex);
    MapDataThreadInfoQueue->Enqueue(FMapThreadInfo(CallbackId, NoiseMap));
    
    //반환 성공
    return 0;
}

void MapDataThread::Stop() 
{
	// 스레드를 일찍 중지하도록 강제.
    bStopThread = true;
}
