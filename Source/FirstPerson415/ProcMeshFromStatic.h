// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProcMeshFromStatic.generated.h"

UCLASS()
class FIRSTPERSON415_API AProcMeshFromStatic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcMeshFromStatic();

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
	UPROPERTY()
		TArray<FVector> Vertices;

	/* An array of integers called Triangles*/
	UPROPERTY()
		TArray<int> Triangles;

	UPROPERTY()
		/* An array of vectors to indicate 3D directional facing of surfaces called Normals*/
		TArray<FVector> Normals;

		/* An array for vector coordinates for 2D texture mapping on 3D meshes*/
		TArray<FVector2D> UV0;

	UPROPERTY()
		
		/* Array of vectors of a floating point representation of color parameters*/
		TArray<FLinearColor> VertexColors;

		/* Array of vectors representing stored color values represented by 8-bit integer value (aka 0 - 256)*/
		TArray<FColor> UpVertexColors;

		/* Array of vectors representing mesh tangent vectors. Allows for the modification/customization of mesh geometry*/
		TArray<FProcMeshTangent> Tangents;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* baseMesh;

private:

	/* Initialization of procedural mesh component*/
	UProceduralMeshComponent* procMesh;

	/* Initialization of GetMeshData function that is further defined in C++ file*/
	void GetMeshData();

	/* Initialization of CreateMesh function that is further defined in C++ file*/
	void CreateMesh();
};
