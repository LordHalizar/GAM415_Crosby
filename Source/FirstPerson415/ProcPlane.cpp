// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcPlane.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AProcPlane::AProcPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	/* Establishment of procmesh derived value by creating associated subobject*/
	procMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Proc Mesh");

}

// Called when the game starts or when spawned
void AProcPlane::BeginPlay()
{
	Super::BeginPlay();
	
}

/* Runs and creates mesh through CreateMesh function before it is then preloaded into game session with PostLoad*/
void AProcPlane::PostActorCreated()
{
	Super::PostActorCreated();
	CreateMesh();

	/* Check to verify that PlaneMat is valid. If so, the following code block is initiated*/
	if (PlaneMat)
	{
		procMesh->SetMaterial(0, PlaneMat);
	}
}

/* Preloads mesh into game session, initially created through PostActorCreated function*/
void AProcPlane::PostLoad()
{
	Super::PostLoad();
	CreateMesh();

	if (PlaneMat)
	{
		procMesh->SetMaterial(0, PlaneMat);
	}
}

// Called every frame
void AProcPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/* Function used to procedurally create meshes based on entered array parameters*/
void AProcPlane::CreateMesh()
{
	procMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

