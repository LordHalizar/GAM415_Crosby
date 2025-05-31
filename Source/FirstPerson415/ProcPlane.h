// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcPlane.generated.h"

class UProceduralMeshComponent;

UCLASS()
class FIRSTPERSON415_API AProcPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Allows for the calling of needed function at the start of a game session*/
	virtual void PostActorCreated() override;

	/* Loads specified functions before a game session loads*/
	virtual void PostLoad() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* An array of vectors called Vertices*/
	UPROPERTY(EditAnywhere)
		TArray<FVector> Vertices;
	
	/* An array of integers called Triangles*/
	UPROPERTY(EditAnywhere)
		TArray<int> Triangles;

	/* An array for vector coordinates for 2D texture mapping on 3D meshes*/
	UPROPERTY(EditAnywhere)
		TArray<FVector2D> UV0;

	/* Initialization of material interface that will be used for the application of materials to procedurally created planes/environments*/
	UPROPERTY(EditAnywhere)
		UMaterialInterface* PlaneMat;
	
	/* Initialization of CreateMesh function that is further defined in ProcPlane C++ file*/
	UFUNCTION()
		void CreateMesh();

private:

	/* Procedural mesh component used as a variable for the generation of procedural meshes*/
	UProceduralMeshComponent* procMesh;
};
