// Fill out your copyright notice in the Description page of Project Settings.


#include "SMapGeneratorWidget.h"
#include "SlateOptMacros.h"
#include "EditorUtilityWidget.h"
#include "WidgetBlueprint.h"
#include "Widgets/SWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMapGeneratorWidget::Construct(const FArguments& InArgs)
{
	//에셋 로드
	auto WidgetBlueprint = LoadObject<UBlueprint>(NULL, TEXT("EditorUtilityWidgetBlueprint'/ProceduralTerrain/Editor/MapDataSettingEditor.MapDataSettingEditor'"), NULL, LOAD_None, NULL);

	// UEditorUtilityWidget 클래스 캐스트
	TSubclassOf<UEditorUtilityWidget> WidgetClass = (TSubclassOf<UEditorUtilityWidget>)WidgetBlueprint->GeneratedClass;

	//위젯 만들기
	UWorld *  World  =  GEditor->GetEditorWorldContext().World(); 
	UEditorUtilityWidget *  CreatedUMGWidget  =  CreateWidget <UEditorUtilityWidget> ( World ,  WidgetClass );
	TSharedRef<SWidget> CreatedSlateWidget = CreatedUMGWidget->TakeWidget();

	ChildSlot
	[
		// Populate the widget
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
            .HAlign(HAlign_Fill)
            [
                CreatedSlateWidget
            ]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
