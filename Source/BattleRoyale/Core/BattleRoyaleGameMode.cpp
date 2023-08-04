// Copyright Epic Games, Inc. All Rights Reserved.

#include "BattleRoyaleGameMode.h"

#include "BattleRoyaleGameState.h"
#include "SocketSubsystem.h"
#include "BattleRoyale/BattleRoyaleHUD.h"
#include "BattleRoyale/Components/BattleRoyaleElectricZone.h"
#include "BattleRoyale/BattleRoyaleCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABattleRoyaleGameMode::ABattleRoyaleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ABattleRoyaleHUD::StaticClass();
}

FString ABattleRoyaleGameMode::GetIP() const
{
	bool canBind = false;
	TSharedRef<FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
	FString IP = localIp->IsValid() ? localIp->ToString(false) : "";
	UE_LOG(LogTemp, Warning, TEXT("My IP: %s") , *IP);
	return IP;
}


bool ABattleRoyaleGameMode::ReadyToStartMatch_Implementation()
{
	if(const ABattleRoyaleGameState* BattleRoyaleGameState = GetGameState<ABattleRoyaleGameState>())
	{
		return (BattleRoyaleGameState->GetCurrentGameState() != EGameState::CONNECTION && BattleRoyaleGameState->GetCurrentGameState() != EGameState::UNINITIALIZE) && Super::ReadyToStartMatch_Implementation();
		
	}
	return  false;
}

void ABattleRoyaleGameMode::OnPlayerDied(ABattleRoyaleCharacter* Character)
{
	if(ABattleRoyaleGameState* BattleRoyaleGameState = GetGameState<ABattleRoyaleGameState>())
	{
		BattleRoyaleGameState->RemovePlayer();
		Character->OnPlayerDeath.RemoveAll(this);
		if(BattleRoyaleGameState->CurrentPlayerCount == 0)
		{
			EndMatch();
		}
	}
}

void ABattleRoyaleGameMode::StartMatch()
{
	if(ABattleRoyaleGameState* BattleRoyaleGameState = GetGameState<ABattleRoyaleGameState>())
	{
		if(BattleRoyaleGameState->GetCurrentGameState() == EGameState::CONNECTION)
		{
			BattleRoyaleGameState->UpdateGameState(EGameState::INGAME);
			Super::StartMatch();
			for(auto PC: ConnectedPlayers)
			{
				if(ABattleRoyaleCharacter* Character = PC->GetPawn<ABattleRoyaleCharacter>())
				{
					Character->OnPlayerDeath.AddUniqueDynamic(this, &ThisClass::OnPlayerDied);
				}
			}
		}
	}
}

void ABattleRoyaleGameMode::EndMatch()
{
	if(ABattleRoyaleGameState* BattleRoyaleGameState = GetGameState<ABattleRoyaleGameState>())
	{
		BattleRoyaleGameState->UpdateGameState(EGameState::ENDGAME);
	}
	Super::EndMatch();
}

APlayerController* ABattleRoyaleGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
                                                const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if(const ABattleRoyaleGameState* BattleRoyaleGameState = GetGameState<ABattleRoyaleGameState>())
	{
		if(BattleRoyaleGameState->GetCurrentGameState() != EGameState::CONNECTION && BattleRoyaleGameState->GetCurrentGameState() != EGameState::UNINITIALIZE) 
		{
			return nullptr;
		}
	}
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

void ABattleRoyaleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if(ABattleRoyaleGameState* BattleRoyaleGameState = GetGameState<ABattleRoyaleGameState>())
	{
	 	if(BattleRoyaleGameState->GetCurrentGameState() == EGameState::UNINITIALIZE)
	 	{
	 		BattleRoyaleGameState->UpdateGameState(EGameState::CONNECTION);
	 		GetWorldTimerManager().SetTimer(GameStateTimerHandle, this, &ThisClass::StartMatch, BattleRoyaleGameState->TimeRequiredToConnect, false);
	 	}
		BattleRoyaleGameState->AddPlayer();
		BattleRoyaleGameState->TimeRequiredToConnect = GetWorldTimerManager().GetTimerRemaining(GameStateTimerHandle);
		ConnectedPlayers.AddUnique(NewPlayer);
	}
}

void ABattleRoyaleGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetIP();
}



