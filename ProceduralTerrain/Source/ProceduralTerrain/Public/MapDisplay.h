// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshData.h"
#include "MapDisplay.generated.h"

//맵을 보이게 한다.
UCLASS()
class PROCEDURALTERRAIN_API AMapDisplay : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapDisplay();

	UPROPERTY(VisibleAnywhere, Category = "MyGeneratorMesh")
	class UProceduralMeshComponent *pm;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "MapDisplay")
	void DrawMesh(FMeshData mapData);

	void DrawMesh(int lodIndex, FMeshData mapData);

	UFUNCTION(BlueprintCallable, Category = "MapDisplay")
	void UpdateMesh(FMeshData mapData);

	UFUNCTION(BlueprintCallable, Category = "MapDisplay")
	void DrawMeshTest(float width, float height);

public:
	int LodIndex = -1;
};