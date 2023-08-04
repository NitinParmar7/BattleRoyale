#include "BattleRoyaleGameState.h"

#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

ABattleRoyaleGameState::ABattleRoyaleGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABattleRoyaleGameState::UpdateGameState(EGameState NewGameState)
{
	if(HasAuthority())
	{
		CurrentGameState = NewGameState;
		OnRep_OnGameStateUpdated();

		switch (CurrentGameState)
		{
		case EGameState::INGAME:
			SetCurrentPlayerCount(TotalPlayerCount);
			break;
		default:
			break;
		}
	}
}

EGameState ABattleRoyaleGameState::GetCurrentGameState() const
{
	return CurrentGameState;
}

void ABattleRoyaleGameState::SetCurrentPlayerCount(int32 NewCurrentPlayerCount)
{
	if(HasAuthority())
	{
		CurrentPlayerCount = NewCurrentPlayerCount;
		OnRep_OnCurrentPlayerCountUpdated();
	}
}

void ABattleRoyaleGameState::SetTotalPlayerCount(int32 NewTotalPlayerCount)
{
	if(HasAuthority())
	{
		TotalPlayerCount = NewTotalPlayerCount;
		OnRep_OnTotalPlayerCountUpdated();
	}
}

void ABattleRoyaleGameState::AddPlayer()
{
	if(HasAuthority())
	{
		TotalPlayerCount += 1;
		OnRep_OnTotalPlayerCountUpdated();
	}
}

void ABattleRoyaleGameState::RemovePlayer()
{
	if(HasAuthority())
	{
		CurrentPlayerCount -= 1;
		OnRep_OnCurrentPlayerCountUpdated();
	}
}

void ABattleRoyaleGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(GetWorldTimerManager().IsTimerActive(ConnectionTimer))
	{
		TimeRemainingToConnect = GetWorldTimerManager().GetTimerRemaining(ConnectionTimer);
	}
}


void ABattleRoyaleGameState::OnRep_OnGameStateUpdated()
{
	OnGameStateChanged.Broadcast(CurrentGameState);
	if(CurrentGameState == EGameState::CONNECTION)
	{
		if(!GetWorldTimerManager().IsTimerActive(ConnectionTimer))
			GetWorldTimerManager().SetTimer(ConnectionTimer, TimeRequiredToConnect, false);
	}
}

void ABattleRoyaleGameState::OnRep_OnTotalPlayerCountUpdated() const
{
	OnTotalPlayerCountChanged.Broadcast(TotalPlayerCount);
}

void ABattleRoyaleGameState::OnRep_OnCurrentPlayerCountUpdated() const
{
	OnCurrentPlayerCountChanged.Broadcast(CurrentPlayerCount);
}

void ABattleRoyaleGameState::OnRep_OnTimeRequiredToConnectUpdated()
{
	GetWorldTimerManager().SetTimer(ConnectionTimer, TimeRequiredToConnect, false);
}

void ABattleRoyaleGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABattleRoyaleGameState, CurrentGameState);
	DOREPLIFETIME(ABattleRoyaleGameState, TotalPlayerCount);
	DOREPLIFETIME(ABattleRoyaleGameState, CurrentPlayerCount);
	DOREPLIFETIME(ABattleRoyaleGameState, TimeRequiredToConnect);
}
