// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPerson415Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AFirstPerson415Projectile::AFirstPerson415Projectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFirstPerson415Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Creation of default subobject for "ballMesh"
	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>("Ball Mesh");

	// Set as root component
	RootComponent = CollisionComp;

	// Setup for ballMesh attachment to the collision component
	ballMesh->SetupAttachment(CollisionComp);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 1.0f;
}

void AFirstPerson415Projectile::BeginPlay()
{
	Super::BeginPlay();
	// Determines color parameter variables using randomized numbers generated within a range and then sets them equal to randColor variable
	randColor = FLinearColor(UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), 1.f);

	// Creates dynamic material instance using projMat and stores value in dmiMat variable
	dmiMat = UMaterialInstanceDynamic::Create(projMat, this);

	// Sets ballMesh material to created dynamic material instance stored in dmiMat
	ballMesh->SetMaterial(0, dmiMat);

	// Sets vector parameter value for projectile color from dmiMat using the randColor variable and applying this to ProjColor material blueprint parameter
	dmiMat->SetVectorParameterValue("ProjColor", randColor);
}

void AFirstPerson415Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}

	// If projectile hits something (aka not nothing), projectile collision will apply splatter effect with randomized color to impacted surface
	if (OtherActor != nullptr)
	{
		if (colorP) 
		{
			UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, HitComp, NAME_None, FVector(-20.f, 0.f, 0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);
			particleComp->SetNiagaraVariableLinearColor(FString("RandomColor"), randColor);
			ballMesh->DestroyComponent();
			CollisionComp->BodyInstance.SetCollisionProfileName("NoCollision");
		}

		// frame variable to allow for random selection of splatter texture decal from texture table
		float frameNum = UKismetMathLibrary::RandomFloatInRange(0.f, 3.f);

		// Decal variable determines placement location and rotation values of splatter decal
		auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), baseMat, FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f)), Hit.Location, Hit.Normal.Rotation(), 0.f);
		
		// Creates dynamic material instance of splatter decal and places it at location value from decal variable. This is location value is then set equal to MatInstance variable
		auto MatInstance = Decal->CreateDynamicMaterialInstance();

		// Sets parameter values for Splat_MAT material blueprint based on C++ variable values randColor and frameNum
		MatInstance->SetVectorParameterValue("Color", randColor);
		MatInstance->SetScalarParameterValue("Frame", frameNum);
	}
}