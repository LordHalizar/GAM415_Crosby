// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CubeDMIMod.generated.h"

// Forward Declaration of Niagara system
class UNiagaraSystem;

UCLASS()
class FIRSTPERSON415_API ACubeDMIMod : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeDMIMod();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Establishment of box component for later use in dynamic material
	UPROPERTY(EditAnywhere)
		UBoxComponent* boxComp;

	// Establishment of static mesh component for later use in dynamic material
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* cubeMesh;

	// Establishment of base material from which the dynamic material will be instanced from
	UPROPERTY(EditAnywhere)
		UMaterialInterface* baseMat;

	UPROPERTY()
		UMaterialInstanceDynamic* dmiMat;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* colorP;

	// Overlap function for the implementation of dynamic material effects
	UFUNCTION()
		void OnOverLapBegin(class UPrimitiveComponent* OverLappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
