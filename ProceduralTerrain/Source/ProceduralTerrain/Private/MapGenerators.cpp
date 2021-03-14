// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerators.h"
#include "EngineUtils.h"
#include "MapDisplay.h"
#include "MeshGenerator.h"
#include "MapDataThread.h"
#include "MeshDataThread.h"
#include "CreateMeshThread.h"
#include "HAL/RunnableThread.h"

void UMapGenerators::GenerateMap(AMapDisplay *mapDisplay, MatrixObject<float> heightMap, float HeightMultiplier, int LevelOfDetail, TArray<FMapColor> regions)
{
    mapDisplay->DrawMesh(UMeshGenerator::GeneratorTerrainMesh(heightMap, HeightMultiplier, LevelOfDetail, regions));
}

void UMapGenerators::UpdateMap(AMapDisplay *mapDisplay, MatrixObject<float> heightMap, float HeightMultiplier, int LevelOfDetail, TArray<FMapColor> regions)
{
    mapDisplay->UpdateMesh(UMeshGenerator::GeneratorTerrainMesh(heightMap, HeightMultiplier, LevelOfDetail, regions));
}

void UMapGenerators::GenerateMap(MatrixObject<float> heightMap, float HeightMultiplier, int LevelOfDetail)
{
    // MapDisplay를 찾고
    UWorld *CurrentWorld = GEngine->GetWorldContexts()[0].World();
    for (TActorIterator<AMapDisplay> It(CurrentWorld); It; ++It)
    {
        // 값을 넘겨준다.
        (*It)->DrawMesh(UMeshGenerator::GeneratorTerrainMesh(heightMap, HeightMultiplier, LevelOfDetail));
        break;
    }
}

void UMapGenerators::GenerateMap(AMapDisplay *mapDisplay, MatrixObject<float> heightMap, float HeightMultiplier, int LevelOfDetail)
{
    mapDisplay->DrawMesh(UMeshGenerator::GeneratorTerrainMesh(heightMap, HeightMultiplier, LevelOfDetail));
}

void UMapGenerators::UpdateMap(AMapDisplay *mapDisplay, MatrixObject<float> heightMap, float HeightMultiplier, int LevelOfDetail)
{
    mapDisplay->UpdateMesh(UMeshGenerator::GeneratorTerrainMesh(heightMap, HeightMultiplier, LevelOfDetail));
}

void UMapGenerators::RequestMapData(FVector2D viewedChunkCoord, AEndlessTerrains *endlessTerrain)
{
    EndlessTerrains = endlessTerrain;

    mapDataId++;
    ChunkCoordMap.Add(mapDataId, viewedChunkCoord);

    MapDataThread *mapThread = new MapDataThread(mapDataId, MapSetting, viewedChunkCoord, &mapDataThreadInfoQueue);
    mapDataThreadArray.Push(mapThread);

    FRunnableThread::Create(mapDataThreadArray[mapDataThreadArray.Num() - 1], TEXT("MapDataThread"));

    if (!isTimer) //타이머가 실행중이 아니라면
    {
        CallTimer();
        isTimer = true;
    }
}

void UMapGenerators::RequestMeshData(FVector2D viewedChunkCoord, MatrixObject<float> mapData, int lodIndex, int lod, AEndlessTerrains *endlessTerrain)
{
    EndlessTerrains = endlessTerrain;

    FMeshDataInfo info;
    info.lodIndex = lodIndex;
    info.viewedChunkCoord = viewedChunkCoord;

    meshDataId++;
    meshDataMap.Add(meshDataId, info);

    //여기서 LOD를 받아서 변경함.
    MapSetting.LevelOfDetail = lod;

    UE_LOG(LogTemp, Log, TEXT("MapSetting.LevelOfDetail : %d"), MapSetting.LevelOfDetail);

    if (MapColorDataAsset != nullptr)
    {
        meshDataThread = new MeshDataThread(meshDataId, MapSetting, mapData, MapColorDataAsset->Regions, &meshDataThreadInfoQueue);
    }
    else{
        TArray<FMapColor> Regions;
        meshDataThread = new MeshDataThread(meshDataId, MapSetting, mapData, Regions, &meshDataThreadInfoQueue);
    }

    FRunnableThread::Create(meshDataThread, TEXT("MeshDataThread"));

    if (!isTimer) //타이머가 실행중이 아니라면
    {
        CallTimer();
        isTimer = true;
    }
}

void UMapGenerators::CallTimer()
{
    const float Delay = 1.0f;
    GetWorld()->GetTimerManager().SetTimer(tHandle, this, &UMapGenerators::TestTimer, Delay, true);
}

void UMapGenerators::TestTimer()
{
    if (mapDataThreadInfoQueue.IsEmpty() && meshDataThreadInfoQueue.IsEmpty())
    {
        if (isTimer)
        {
            isTimer = false;
            GetWorld()->GetTimerManager().ClearTimer(tHandle); //타이머 해제함.
        }
        return;
    }

    while (!mapDataThreadInfoQueue.IsEmpty())
    {
        FMapThreadInfo threadInfo;
        mapDataThreadInfoQueue.Dequeue(threadInfo);
        int32 viewedChunkCoordId = threadInfo.callbackId;

        if (ChunkCoordMap.Find(viewedChunkCoordId))
        {
            //월드에 있는 엑터 가져오기

            if (EndlessTerrains)
            {
                FVector2D coord = ChunkCoordMap[viewedChunkCoordId];
                UE_LOG(LogTemp, Log, TEXT("Send :: %s"), *coord.ToString());
                //EndlessTerrains 액터를 가져와서 함수 실행.
                EndlessTerrains->OnMapDataReceived(coord, threadInfo.noiseMapData);
            }
        }
    }

    while (!meshDataThreadInfoQueue.IsEmpty())
    {
        /* code */
        FMeshThreadInfo threadMeshInfo;
        meshDataThreadInfoQueue.Dequeue(threadMeshInfo);
        int32 viewedChunkCoordId = threadMeshInfo.callbackId;

        if (meshDataMap.Find(viewedChunkCoordId))
        {
            //월드에 있는 엑터 가져오기

            if (EndlessTerrains)
            {
                FVector2D coord = meshDataMap[viewedChunkCoordId].viewedChunkCoord;
                //UE_LOG(LogTemp, Log, TEXT("Send :: %s"), *coord.ToString());
                //EndlessTerrains 액터를 가져와서 함수 실행.

                //여기서 meshData에 머티리얼의 값을 추가하도록 한다.
                //MapMaterial
                threadMeshInfo.meshData.MapMaterial = MapMaterial;

                EndlessTerrains->OnMeshDataReceived(coord, threadMeshInfo.meshData, meshDataMap[viewedChunkCoordId].lodIndex);
            }
        }
    }
}

void UMapGenerators::Tick(float DeltaTime)
{
    //GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, FString::SanitizeFloat(TestCounter));
}

bool UMapGenerators::IsTickable() const
{
    return true;
}

bool UMapGenerators::IsTickableInEditor() const
{
    return false;
}

bool UMapGenerators::IsTickableWhenPaused() const
{
    return false;
}

TStatId UMapGenerators::GetStatId() const
{
    return TStatId();
}

UWorld *UMapGenerators::GetWorld() const
{
    return GetOuter()->GetWorld();
}

#if WITH_EDITOR
void UMapGenerators::PostEditChangeProperty(struct FPropertyChangedEvent &PropertyChangedEvent)
{
    static const FName NAME_MapSettingDataAsset = FName(TEXT("MapSettingDataAsset"));

    if (PropertyChangedEvent.Property->GetFName() == NAME_MapSettingDataAsset)
    {
        if (MapSettingDataAsset)
            MapSetting = MapSettingDataAsset->MapSetting;
        else
        {
            MapSetting.Init();
        }
    }

    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif