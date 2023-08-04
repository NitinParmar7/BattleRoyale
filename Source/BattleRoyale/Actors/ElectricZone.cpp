// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectricZone.h"


// Sets default values
AElectricZone::AElectricZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AElectricZone::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(GetStaticMeshComponent()))
	{
		InitialSize = GetStaticMeshComponent()->GetComponentScale().X;
		NewSize = CurrentSize =InitialSize;
	}
}

// Called every frame
void AElectricZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GetWorldTimerManager().IsTimerActive(ShrinkTimerHandle))
	{
		const float ElapseTime = GetWorldTimerManager().GetTimerElapsed(ShrinkTimerHandle);
		const float Progress = ElapseTime/ ShrinkDuration;
		float SizeToSet = FMath::Lerp(CurrentSize, NewSize, Progress);
		GetStaticMeshComponent()->SetWorldScale3D(FVector(SizeToSet, SizeToSet, GetStaticMeshComponent()->GetComponentScale().Z));
	}
}

void AElectricZone::UpdateSize(float ShrinkRadiusPercent, FTimerHandle ShrinkTimer, float Duration)
{
	CurrentSize = NewSize;
	NewSize = NewSize - (InitialSize * ShrinkRadiusPercent);
	ShrinkTimerHandle = ShrinkTimer;
	ShrinkDuration = FMath::IsNearlyZero(Duration) ? 1 : Duration;
}



