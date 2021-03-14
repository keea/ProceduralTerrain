// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGeneratorEditor.h"
#include "MapGenerators.h"
#include "MeshGenerator.h"
#include "MapSettingDataAsset.h"
#include "GenerateNoiseMap.h"
#include "UnrealEd.h"
#include "UObject/Package.h"
#include "MapColorDataAsset.h"
#include "AssetRegistryModule.h"

void UMapGeneratorEditor::GenerateMapDataDisplay(AMapDisplay *mapDispaly, UMapSettingDataAsset *MapSettingDataAsset)
{
	UMapGenerators::GenerateMap(mapDispaly,
								UGenerateNoiseMap::GenerateNoiseMap(MapSettingDataAsset->MapSetting.MapChunkSize,
																	MapSettingDataAsset->MapSetting.MapChunkSize,
																	MapSettingDataAsset->MapSetting.NoiseScale,
																	MapSettingDataAsset->MapSetting.Seed,
																	MapSettingDataAsset->MapSetting.Octaves,
																	MapSettingDataAsset->MapSetting.Persistance,
																	MapSettingDataAsset->MapSetting.Lacunarity,
																	MapSettingDataAsset->MapSetting.Offset,
																	MapSettingDataAsset->MapSetting.NormalizeMode),
								MapSettingDataAsset->MapSetting.HeightMultiplier,
								MapSettingDataAsset->MapSetting.LevelOfDetail);
}

void UMapGeneratorEditor::UpdateMapDataDisplay(AMapDisplay *mapDispaly, class UMapSettingDataAsset *MapSettingDataAsset)
{
	UMapGenerators::UpdateMap(mapDispaly,
							  UGenerateNoiseMap::GenerateNoiseMap(MapSettingDataAsset->MapSetting.MapChunkSize,
																  MapSettingDataAsset->MapSetting.MapChunkSize,
																  MapSettingDataAsset->MapSetting.NoiseScale,
																  MapSettingDataAsset->MapSetting.Seed,
																  MapSettingDataAsset->MapSetting.Octaves,
																  MapSettingDataAsset->MapSetting.Persistance,
																  MapSettingDataAsset->MapSetting.Lacunarity,
																  MapSettingDataAsset->MapSetting.Offset,
																  MapSettingDataAsset->MapSetting.NormalizeMode),
							  MapSettingDataAsset->MapSetting.HeightMultiplier,
							  MapSettingDataAsset->MapSetting.LevelOfDetail);
}

void UMapGeneratorEditor::SaveMapSettingDataAsset(FString AssetName, UMapSettingDataAsset *MapSettingDataAsset)
{
	FString PackageName = TEXT("/Game/Test/MapSettingData/");
	PackageName += AssetName;

	UPackage *Package = CreatePackage(NULL, *PackageName);
	Package->FullyLoad();

	UMapSettingDataAsset *MapSettingAsset = NewObject<UMapSettingDataAsset>(Package, *AssetName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	MapSettingAsset->MapSetting = MapSettingDataAsset->MapSetting;

	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(MapSettingAsset);

	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

	bool bSaved = UPackage::SavePackage(
		Package,
		MapSettingAsset,
		EObjectFlags::RF_Public | EObjectFlags::RF_Standalone,
		*PackageFileName, 
		GError, nullptr, true, true, SAVE_NoError);

	TArray<UObject*> ObjectsToSync;
	ObjectsToSync.Add(MapSettingAsset);
	GEditor->SyncBrowserToObjects(ObjectsToSync);
}

void UMapGeneratorEditor::UpdateMapDataDisplayWithColor(AMapDisplay * mapDispaly, class UMapSettingDataAsset* MapSettingDataAsset, class UMapColorDataAsset* MapColorDataAsset) 
{
	UMapGenerators::UpdateMap(mapDispaly,
								UGenerateNoiseMap::GenerateNoiseMap(MapSettingDataAsset->MapSetting.MapChunkSize,
																	MapSettingDataAsset->MapSetting.MapChunkSize,
																	MapSettingDataAsset->MapSetting.NoiseScale,
																	MapSettingDataAsset->MapSetting.Seed,
																	MapSettingDataAsset->MapSetting.Octaves,
																	MapSettingDataAsset->MapSetting.Persistance,
																	MapSettingDataAsset->MapSetting.Lacunarity,
																	MapSettingDataAsset->MapSetting.Offset,
																	MapSettingDataAsset->MapSetting.NormalizeMode),
								MapSettingDataAsset->MapSetting.HeightMultiplier,
								MapSettingDataAsset->MapSetting.LevelOfDetail,
								MapColorDataAsset->Regions);
}

void UMapGeneratorEditor::GenerateMapDataDisplayWithColor(AMapDisplay * mapDispaly, class UMapSettingDataAsset* MapSettingDataAsset, UMapColorDataAsset * MapColorDataAsset) 
{
	UMapGenerators::GenerateMap(mapDispaly,
								UGenerateNoiseMap::GenerateNoiseMap(MapSettingDataAsset->MapSetting.MapChunkSize,
																	MapSettingDataAsset->MapSetting.MapChunkSize,
																	MapSettingDataAsset->MapSetting.NoiseScale,
																	MapSettingDataAsset->MapSetting.Seed,
																	MapSettingDataAsset->MapSetting.Octaves,
																	MapSettingDataAsset->MapSetting.Persistance,
																	MapSettingDataAsset->MapSetting.Lacunarity,
																	MapSettingDataAsset->MapSetting.Offset,
																	MapSettingDataAsset->MapSetting.NormalizeMode),
								MapSettingDataAsset->MapSetting.HeightMultiplier,
								MapSettingDataAsset->MapSetting.LevelOfDetail,
								MapColorDataAsset->Regions);
}

void UMapGeneratorEditor::SaveMapColorDataAsset(FString AssetName, UMapColorDataAsset * MapColorDataAsset) 
{
	FString PackageName = TEXT("/Game/Test/MapColorData/");
	PackageName += AssetName;

	UPackage *Package = CreatePackage(NULL, *PackageName);
	Package->FullyLoad();

	UMapColorDataAsset *MapColorAsset = NewObject<UMapColorDataAsset>(Package, *AssetName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	MapColorAsset->Regions = MapColorDataAsset->Regions;

	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(MapColorAsset);

	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

	bool bSaved = UPackage::SavePackage(
		Package,
		MapColorAsset,
		EObjectFlags::RF_Public | EObjectFlags::RF_Standalone,
		*PackageFileName, 
		GError, nullptr, true, true, SAVE_NoError);

	TArray<UObject*> ObjectsToSync;
	ObjectsToSync.Add(MapColorAsset);
	GEditor->SyncBrowserToObjects(ObjectsToSync);
}