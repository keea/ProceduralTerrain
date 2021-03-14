// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAL/Runnable.h"
#include "CoreMinimal.h"
#include "Misc/ScopeLock.h"

/**
 * 
 */
class PROCEDURALTERRAIN_API CreateMeshThread : public FRunnable
{
public:
	CreateMeshThread(class AMapDisplay * mapDisplay);

	// FRunnable 함수
	virtual uint32 Run() override;
	virtual void Stop() override;
	// FRunnable

protected:
	bool bStopThread = false;
	FCriticalSection Mutex;
	class AMapDisplay * mapDisplay;
};