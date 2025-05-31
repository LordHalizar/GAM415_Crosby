// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcMeshFromStatic.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
AProcMeshFromStatic::AProcMeshFromStatic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/* Initializes default subobjects for procMesh*/
	procMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Proc Mesh");

	/* Initializes default subobjects for baseMesh*/
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");

	/* Attachments for procMesh subobject*/
	RootComponent = procMesh;
	// procMesh->SetupAttachment(GetRootComponent());  alternate way of doing the above line statement
	baseMesh->SetupAttachment(procMesh);
}

// Called when the game starts or when spawned
void AProcMeshFromStatic::BeginPlay()
{
	Super::BeginPlay();
	
}

/* Runs and creates function outputs before it is then preloaded into game session with PostLoad*/
void AProcMeshFromStatic::PostActorCreated()
{
	Super::PostActorCreated();
	GetMeshData();
}

/* Preloads function outputs into game session, initially created through PostActorCreated function*/
void AProcMeshFromStatic::PostLoad()
{
	Super::PostLoad();
	GetMeshData();
}

// Called every frame
void AProcMeshFromStatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProcMeshFromStatic::GetMeshData()
{
	/* Creates new local static mesh variable, pulling the static mesh from my static mesh component*/
	UStaticMesh* mesh = baseMesh->GetStaticMesh();
	if (mesh) 
	{
		/* Pulls static mesh data from static mesh*/
		UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(mesh, 0, 0, Vertices, Triangles, Normals, UV0, Tangents);
		
		/* Creates procedural static mesh using pulled static mesh data from previous line statement*/
		procMesh->UpdateMeshSection(0, Vertices, Normals, UV0, UpVertexColors, Tangents);
		CreateMesh();
	}
}

void AProcMeshFromStatic::CreateMesh()
{
	/* Check to make sure if baseMesh is valid. If so, code block is run and mesh section is created based on entered variable parameters*/
	if (baseMesh) 
	{
		procMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, UpVertexColors, Tangents, true);
	}
}

