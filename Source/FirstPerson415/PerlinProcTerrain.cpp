// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinProcTerrain.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
APerlinProcTerrain::APerlinProcTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/* Setup for subobject attachments*/
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Procedural Mesh");
	ProcMesh->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void APerlinProcTerrain::BeginPlay()
{
	Super::BeginPlay();
	
	/* Calling functions for the creation of vertices and triangles*/
	CreateVertices();
	CreateTriangles();

	/* Creates mesh section and sets the material for it*/
	ProcMesh->CreateMeshSection(sectionID, Vertices, Triangles, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>(), true);
	ProcMesh->SetMaterial(0, Mat);

}

// Called every frame
void APerlinProcTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APerlinProcTerrain::AlterMesh(FVector impactpoint)
{
	/* For loop that checks the total number of vertices on the terrain mesh*/
	for (int i = 0; i < Vertices.Num(); i++) 
	{
		FVector tempVector = impactpoint - this->GetActorLocation();

		if (FVector(Vertices[i] - tempVector).Size() < radius) 
		{
			Vertices[i] = Vertices[i] - Depth;

			/* Updates terrain mesh based on determined Vertices value determined in previously line statement*/
			ProcMesh->UpdateMeshSection(sectionID, Vertices, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>());
		}
	}
}

void APerlinProcTerrain::CreateVertices()
{
	/* Nested loop that will increment X and Y values until they equal determined procedural terrain parameters*/
	for (int X = 0; X <= XSize; X++)
	{
		for (int Y = 0; Y <= YSize; Y++) 
		{
			/* random scale values determined by FMath PerlinNoise library function that are then set equal to float variable Z*/
			float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale + 0.1, Y * NoiseScale + 0.1)) * ZMultiplier;
			
			/* Prints out generated Z value*/
			GEngine->AddOnScreenDebugMessage(-1, 999.0f, FColor::Yellow, FString::Printf(TEXT("Z %f"), Z));
			
			/* Vertices generated based on determined scale values from previous line statements*/
			Vertices.Add(FVector(X * Scale, Y * Scale, Z));
			
			/* UVs generated based on determined UV scale values from previous line statements*/
			UV0.Add(FVector2D(X * UVScale, Y * UVScale));
		}
	}
}

void APerlinProcTerrain::CreateTriangles()
{
	int Vertex = 0;

	/* Nested for loop that dynamically updates index values for triangle creation based on generated vertex value from CreateVertices() funciton*/
	for (int X = 0; X < XSize; X++) 
	{
		for (int Y = 0; Y < YSize; Y++) 
		{
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 1);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);

			Vertex++;
		}

		Vertex++;
	}
}

