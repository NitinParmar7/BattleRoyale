// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

UCLASS()
class BATTLEROYALE_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMenuGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/**
	 * @brief Host game as listen server
	 * @param LevelName 
	 */
	UFUNCTION(BlueprintCallable)
	void HostGame(FName LevelName);

	/**
	 * @brief Join game via IP of listen server
	 * @param IP 
	 */
	UFUNCTION(BlueprintCallable)
	void JoinGame(FString IP);

	
	
};
