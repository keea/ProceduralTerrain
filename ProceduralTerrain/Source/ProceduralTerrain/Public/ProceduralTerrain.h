// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FProceduralTerrainModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void AddMenu(FMenuBarBuilder &MenuBuilder);
	void MenuCallback();
	TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs &SpawnTabArgs);
};