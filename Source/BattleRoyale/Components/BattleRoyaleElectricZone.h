// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleRoyale/BattleRoyaleEnums.h"
#include "Components/ActorComponent.h"
#include "BattleRoyaleElectricZone.generated.h"


class ABattleRoyaleCharacter;
class AElectricZone;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BATTLEROYALE_API UBattleRoyaleElectricZoneComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBattleRoyaleElectricZoneComponent();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * @brief Called when Shrink Timer expired
	 */
	void OnElectricZoneShrinkExpired();
	/**
	 * @brief Called when Shrink Idle time expired
	 */
	void OnElectricZoneIdleExpired();
	UFUNCTION()
	void OnElectricZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
	                                const FHitResult& SweepResult);
	/**
	 * @brief Damages the player inside the electric zone
	 */
	void DamageTick();

	UFUNCTION()
	void OnElectricZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);
	UFUNCTION()
	void OnGameStateChanged(EGameState NewState);

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ElectricZone")
	EElectricZoneState InitialState = EElectricZoneState::IDLE; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ElectricZone")
	int32 MaxShrinkCount = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ElectricZone")
	float ShrinkDuration = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ElectricZone")
	float ShrinkDelay = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ElectricZone", meta = (ClampMin = 0, ClampMax = 1))
	float ShrinkRadiusPercent = 0.15f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ElectricZone")
	float DamagePerSec = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ElectricZone")
	EElectricZoneState CurrentState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ElectricZone")
	TSubclassOf<AElectricZone> ElectricZoneClass;

private:

	int32 CurrentShrinkCount = 0;

	UPROPERTY()
	AElectricZone* ElectricZone;

	FTimerHandle ElectricZoneTimer;

	FTimerHandle DamageTimer;

	UPROPERTY()
	TArray<ABattleRoyaleCharacter*> CharactersArray;
	
};
