// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProceduralTerrain.h"
#include "LevelEditor.h"
#include "SMapGeneratorWidget.h"
#include "Interfaces/IMainFrameModule.h"

#define LOCTEXT_NAMESPACE "FProceduralTerrainModule"

static const FName CreateMapDataSettingEditorTabName("MapData Setting Editor");
#define LOCTEXT_NAMESPACE "FProceduralTerrainModule"

void FProceduralTerrainModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FLevelEditorModule &LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());

	//메뉴 항목 생성 코드
	MenuExtender->AddMenuBarExtension(
		"Help",
		EExtensionHook::After,
		nullptr,
		FMenuBarExtensionDelegate::CreateRaw(this, &FProceduralTerrainModule::AddMenu));
		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	//새 탭 생성 및 레지스트.
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CreateMapDataSettingEditorTabName, FOnSpawnTab::CreateRaw(this, &FProceduralTerrainModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("MapDataSettingEditor", "MapDataSettingEditorWindow"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FProceduralTerrainModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FProceduralTerrainModule::AddMenu(FMenuBarBuilder &MenuBuilder) 
{
	MenuBuilder.AddMenuEntry(
		LOCTEXT("MenuLocKey", "MapDataSettingEditor"),
		LOCTEXT("MenuTooltipKey", "Opens menu for MapData Setting Editor"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FProceduralTerrainModule::MenuCallback)));
}

void FProceduralTerrainModule::MenuCallback() 
{
	UE_LOG(LogTemp, Log, TEXT("Action! MapDataSettingEditor"));

	TSharedRef<SDockTab> NewTap = FGlobalTabmanager::Get()->InvokeTab(CreateMapDataSettingEditorTabName);
}

TSharedRef<SDockTab> FProceduralTerrainModule::OnSpawnPluginTab(const FSpawnTabArgs &SpawnTabArgs) 
{
	return SNew(SDockTab)
	.TabRole(ETabRole::NomadTab)
	[
		SNew(SMapGeneratorWidget)
	];
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FProceduralTerrainModule, ProceduralTerrain)