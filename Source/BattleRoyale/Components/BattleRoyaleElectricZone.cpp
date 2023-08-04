// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleRoyaleElectricZone.h"

#include "BattleRoyale/BattleRoyaleCharacter.h"
#include "BattleRoyale/Actors/ElectricZone.h"
#include "BattleRoyale/Core/BattleRoyaleGameState.h"


// Sets default values for this component's properties
UBattleRoyaleElectricZoneComponent::UBattleRoyaleElectricZoneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UBattleRoyaleElectricZoneComponent::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(GetOwner()))
	{
		if(ABattleRoyaleGameState* BattleRoyaleGameState = Cast<ABattleRoyaleGameState>(GetOwner()))
		{
			BattleRoyaleGameState->OnGameStateChanged.AddUniqueDynamic(this, &ThisClass::OnGameStateChanged);
		}
	}

}

void UBattleRoyaleElectricZoneComponent::OnElectricZoneShrinkExpired()
{
	CurrentState = EElectricZoneState::IDLE;
	GetWorld()->GetTimerManager().SetTimer(ElectricZoneTimer, this, &ThisClass::OnElectricZoneIdleExpired, ShrinkDelay, false);
}

void UBattleRoyaleElectricZoneComponent::OnElectricZoneIdleExpired()
{
	CurrentShrinkCount++;
	if(CurrentShrinkCount <= MaxShrinkCount)
	{
		CurrentState = EElectricZoneState::SHRINK;
		GetWorld()->GetTimerManager().SetTimer(ElectricZoneTimer, this, &ThisClass::OnElectricZoneShrinkExpired, ShrinkDuration, false);
		ElectricZone->UpdateSize(ShrinkRadiusPercent, ElectricZoneTimer, ShrinkDuration);
		
	}
}

void UBattleRoyaleElectricZoneComponent::OnElectricZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if(ABattleRoyaleCharacter* Character = Cast<ABattleRoyaleCharacter>(OtherActor))
	{
		CharactersArray.Remove(Character);
		if(CharactersArray.Num() == 0)
		{
			GetWorld()->GetTimerManager().PauseTimer(DamageTimer);
		}
	}
}

void UBattleRoyaleElectricZoneComponent::DamageTick()
{
	for(const auto Character : CharactersArray)
	{
		if(IsValid(Character))
		{
			Character->DamagePlayer(CurrentShrinkCount * DamagePerSec);
		}
	}
}



void UBattleRoyaleElectricZoneComponent::OnElectricZoneEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                                  AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if(ABattleRoyaleCharacter* Character = Cast<ABattleRoyaleCharacter>(OtherActor))
	{
		CharactersArray.AddUnique(Character);
		if(!GetWorld()->GetTimerManager().IsTimerActive(DamageTimer))
		{
			if(GetWorld()->GetTimerManager().IsTimerPaused(DamageTimer))
			{
				GetWorld()->GetTimerManager().UnPauseTimer(DamageTimer);
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(DamageTimer, this, &ThisClass::DamageTick, 1, true);
			}
		}
	}
}

void UBattleRoyaleElectricZoneComponent::OnGameStateChanged(EGameState NewState)
{
	if(NewState == EGameState::INGAME)
	{
		ElectricZone = GetWorld()->SpawnActor<AElectricZone>(ElectricZoneClass);
		if(CurrentState == EElectricZoneState::IDLE)
		{
			GetWorld()->GetTimerManager().SetTimer(ElectricZoneTimer, this, &ThisClass::OnElectricZoneIdleExpired, ShrinkDelay, false);
		}
		if(GetOwner()->HasAuthority())
		{
			ElectricZone->GetStaticMeshComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnElectricZoneBeginOverlap);
			ElectricZone->GetStaticMeshComponent()->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnElectricZoneEndOverlap);
		}
	}
}


