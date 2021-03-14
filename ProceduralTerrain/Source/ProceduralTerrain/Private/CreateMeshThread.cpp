// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateMeshThread.h"
#include "MapDisplay.h"

CreateMeshThread::CreateMeshThread(class AMapDisplay * mapDisplay) 
{
	this->mapDisplay = mapDisplay;
}

uint32 CreateMeshThread::Run() 
{
    bStopThread = false;

	//여기서 메시를 만든다....
    //mapDisplay::DrawMesh
    
    FScopeLock ScopeLock(&Mutex);
    mapDisplay->DrawMeshTest(161, 161);

    return 0;
}

void CreateMeshThread::Stop() 
{
	// 스레드를 일찍 중지하도록 강제.
    bStopThread = true;
}