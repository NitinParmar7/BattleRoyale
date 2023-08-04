// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ElectricZone.generated.h"

UCLASS()
class BATTLEROYALE_API AElectricZone : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElectricZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Update The size of electric zone
	 * @param ShrinkRadiusPercent 
	 * @param ShrinkTimer 
	 * @param Duration 
	 */
	void UpdateSize(float ShrinkRadiusPercent, FTimerHandle ShrinkTimer, float Duration);

private:


	/**
	 * @brief New Size of Electric Zone
	 */
	float NewSize;

	/**
	 * @brief Current Size of Electric Zone
	 */
	float CurrentSize;

	/**
	 * @brief Initial Size of Electric Zone
	 */
	float InitialSize;

	
	/**
	 * @brief time take to shrink from current size to new size
	 */
	float ShrinkDuration;

	FTimerHandle ShrinkTimerHandle; 
};
