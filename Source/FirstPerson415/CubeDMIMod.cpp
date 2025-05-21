// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeDMIMod.h"
#include "firstperson415Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ACubeDMIMod::ACubeDMIMod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creation of subobject components
	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Component");
	cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cube Mesh");

	//Attachments for object classes
	RootComponent = boxComp;
	cubeMesh->SetupAttachment(boxComp);
}

// Called when the game starts or when spawned
void ACubeDMIMod::BeginPlay()
{
	Super::BeginPlay();

	//Initialization of function on overlap with box component object
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ACubeDMIMod::OnOverLapBegin);
	
	// Verify whether or not the base material for the box component object is valid
	if (baseMat) 
	{
		// Creates dynamic material instance for the base material and then sets it equal to the dmiMat variable for later use
		dmiMat = UMaterialInstanceDynamic::Create(baseMat, this);
	}

	// Verify whether the cube mesh is valid
	if (cubeMesh) 
	{
		// Sets cubeMesh equal to the previously determined dmiMat variable value
		cubeMesh->SetMaterial(0, dmiMat);
	}

}

// Called every frame
void ACubeDMIMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubeDMIMod::OnOverLapBegin(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//
{
	// Casting to player character in order to grab and use associated class variables
	AFirstPerson415Character* overlappedActor = Cast<AFirstPerson415Character>(OtherActor);

	// Determines if the overlapped actor is valid and then executes following code
	if (overlappedActor) 
	{
		//If valid, three random numerical values are determined within a given float range
		float ranNumX = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumY = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumZ = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);

		// Determined random numerical float values are assigned as vector inputs for a 3 vector color parameter
		FLinearColor randColor = FLinearColor(ranNumX, ranNumY, ranNumZ, 1.f);
		
		// If dmiMat is valid, values for dmiMat parameters "Color," "Darkness," and "Opacity" are assigned values from previously determined randomized variable values
		if (dmiMat) 
		{
			dmiMat->SetVectorParameterValue("Color", randColor);
			dmiMat->SetScalarParameterValue("Darkness", ranNumX);
			//dmiMat->SetScalarParameterValue("Opacity", ranNumY);

			if (colorP) 
			{
				UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, OtherComp, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);

				particleComp->SetNiagaraVariableLinearColor(FString("RandColor"), randColor);
			}
		}
	}
}

