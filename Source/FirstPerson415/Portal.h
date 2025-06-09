// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/BoxComponent.h"
#include "FirstPerson415Character.h"
#include "Components/ArrowComponent.h"
#include "Portal.generated.h"


/* Forward declarations to allow access to local variables in associated classes*/
class FirstPerson415Character;


UCLASS()
class FIRSTPERSON415_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Initialization of static mesh component*/
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;

	/* Initialization of scene capture component*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneCaptureComponent2D* sceneCapture;

	/**/
	UPROPERTY(EditAnywhere)
		UArrowComponent* rootArrow;
	
	/* Initialization of render target*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextureRenderTarget2D* renderTarget;

	/* Initializing box component*/
	UPROPERTY(EditAnywhere)
		UBoxComponent* boxComp;

	/* Initialize OtherPortal variable creation for later use as eye dropper for selecting portal in scene this is connected to*/
	UPROPERTY(EditAnywhere)
		APortal* OtherPortal;
	/* Initializing base material interface*/
	UPROPERTY(EditAnywhere)
		UMaterialInterface* mat;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void SetBool(AFirstPerson415Character* playerChar);

	UFUNCTION()
		void UpdatePortals();

};
