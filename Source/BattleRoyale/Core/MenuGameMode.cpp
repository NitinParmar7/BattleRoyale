// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AMenuGameMode::AMenuGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMenuGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMenuGameMode::HostGame(FName LevelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName, true, "bIsLanMatch=1?listen?port=9999");
}

void AMenuGameMode::JoinGame(FString IP)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->ClientTravel(IP, ETravelType::TRAVEL_Absolute);
}

