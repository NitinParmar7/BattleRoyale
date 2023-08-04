// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "BattleRoyaleGameMode.generated.h"

class ABattleRoyaleCharacter;

UCLASS()
class ABattleRoyaleGameMode : public AGameMode
{

private:
	GENERATED_BODY()

public:

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	                                 const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual bool ReadyToStartMatch_Implementation() override;

	/**
	 * @brief Called when player died
	 * @param Character 
	 */
	UFUNCTION()
	void OnPlayerDied(ABattleRoyaleCharacter* Character);
	
	virtual void StartMatch() override;
	virtual void EndMatch() override;

protected:
	virtual void BeginPlay() override;


private:

	FTimerHandle GameStateTimerHandle;


public:
	ABattleRoyaleGameMode();

	/**
	 * @brief Returns the IP of this server in string
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	FString GetIP()const;

	TArray<APlayerController*> ConnectedPlayers;
};



