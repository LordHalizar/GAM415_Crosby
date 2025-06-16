// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "FirstPerson415Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Creation of default subobjects for later attachments*/
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Comp");
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture");
	rootArrow = CreateDefaultSubobject<UArrowComponent>("Root Arrow");


	/* Creation of attachments for subobjects*/
	RootComponent = boxComp;
	mesh->SetupAttachment(boxComp);
	sceneCapture->SetupAttachment(mesh);
	rootArrow->SetupAttachment(RootComponent);

	/* Disables mesh collisions to allow player to walk through and into overlap event for portal*/
	mesh->SetCollisionResponseToAllChannels(ECR_Ignore);

}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
	/* Sets up begin overlap event responsible for teleportation function*/
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
	mesh->SetHiddenInSceneCapture(true);

	/* Validation check to make sure material is valid*/
	if (mat) 
	{
		mesh->SetMaterial(0, mat);
	}
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Updates portal image render on each tick to rotate with players perspective*/
	UpdatePortals();
}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFirstPerson415Character* playerChar = Cast<AFirstPerson415Character>(OtherActor);

	/* Validation check to see if cast to player character is valid*/
	if (playerChar) 
	{
		/* Validation check to see if OtherPortal variable is valid*/
		if (OtherPortal) 
		{
			/* Validation check to see is isTeleporting variable is False. If so, allow for teleportation*/
			if (!playerChar->isTeleporting) 
			{
				playerChar->isTeleporting = true;
				FVector loc = OtherPortal->rootArrow->GetComponentLocation();
				playerChar->SetActorLocation(loc);

				/* Sets timer by function name in order to reset isTeleporting to false to prevent infinite loop crash*/
				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUFunction(this, "SetBool", playerChar);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false);
			}
		}
	}
}

void APortal::SetBool(AFirstPerson415Character* playerChar)
{
	if (playerChar) 
	{
		playerChar->isTeleporting = false;
	}
}

void APortal::UpdatePortals()
{
	if (OtherPortal != nullptr) 
	{
		/* Gets the vector difference between player character location and the location of OtherPortal*/
		FVector Location = this->GetActorLocation() - OtherPortal->GetActorLocation();

		/* Gets player camera location in order to proceduraly update portal view to orient to players vision direction*/
		FVector camLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentLocation();

		/* Gets player camera rotation in order to proceduraly update angle of portal view based on player camera view rotation*/
		FRotator camRotation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentRotation();

		/* Calculates the combined location*/
		FVector CombinedLocation = camLocation + Location;

		/* Sets the world location and rotation of scene capture component*/
		sceneCapture->SetWorldLocationAndRotation(CombinedLocation, camRotation);
	}
}

