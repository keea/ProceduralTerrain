// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAL/Runnable.h"
#include "Containers/Array.h"
#include "MeshData.h"
#include "Containers/Queue.h"
#include "EndlessTerrains.h"
#include "MapGenerators.h"
#include "Misc/ScopeLock.h"

/**
 * 
 */
class PROCEDURALTERRAIN_API MapDataThread : public FRunnable
{
public:
	int32 CallbackId;
	FMapSetting MapSetting;
	FVector2D Coord;

	MapDataThread(int32 callbackId, FMapSetting mapSetting, FVector2D coord, TQueue<FMapThreadInfo, EQueueMode::Spsc> * mapDataThreadInfoQueue);

	// FRunnable 함수
	virtual uint32 Run() override;
	virtual void Stop() override;
	// FRunnable

protected:
	bool bStopThread = false;
	FCriticalSection Mutex;
	TQueue<FMapThreadInfo, EQueueMode::Spsc> * MapDataThreadInfoQueue;
};