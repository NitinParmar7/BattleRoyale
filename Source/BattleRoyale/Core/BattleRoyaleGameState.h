#pragma once
#include "..\BattleRoyaleEnums.h"
#include "GameFramework/GameState.h"
#include "BattleRoyaleGameState.generated.h"





DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EGameState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCountChanged, int32, NewPlayeCount);

UCLASS()
class BATTLEROYALE_API ABattleRoyaleGameState : public AGameState
{


private:
	GENERATED_BODY()

public:

	ABattleRoyaleGameState();
	
	UPROPERTY(BlueprintAssignable)
	FOnGameStateChanged OnGameStateChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerCountChanged OnTotalPlayerCountChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerCountChanged OnCurrentPlayerCountChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,ReplicatedUsing = OnRep_OnGameStateUpdated, Category = "BattleRoyaleGameState")
	EGameState CurrentGameState = EGameState::UNINITIALIZE;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_OnTotalPlayerCountUpdated,Category = "BattleRoyaleGameState")
	int32 TotalPlayerCount;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_OnCurrentPlayerCountUpdated,Category = "BattleRoyaleGameState")
	int32 CurrentPlayerCount;

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_OnTimeRequiredToConnectUpdated,BlueprintReadOnly, Category = "BattleRoyaleGameMode")
	float TimeRequiredToConnect = 60.f;

	UPROPERTY(BlueprintReadOnly, Category = "BattleRoyaleGameState")
	float TimeRemainingToConnect;

	/**
	 * @brief Update the state of the Game
	 * @param NewGameState 
	 */
	UFUNCTION(BlueprintCallable, Category = "BattleRoyaleGameState")
	void UpdateGameState(EGameState NewGameState);

	/**
	 * @brief Get Current State of the game
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category = "BattleRoyaleGameState")
	EGameState GetCurrentGameState() const;

	/**
	 * @brief Sets the current player count
	 * @param NewCurrentPlayerCount 
	 */
	void SetCurrentPlayerCount(int32 NewCurrentPlayerCount);

	/**
	 * @brief Sets the total player count
	 * @param NewTotalPlayerCount 
	 */
	void SetTotalPlayerCount(int32 NewTotalPlayerCount);

	/**
	 * @brief Add Player to total player count
	 */
	void AddPlayer();

	/**
	 * @brief Remove the current player 
	 */
	void RemovePlayer();

	virtual void Tick(float DeltaSeconds) override;

protected:

	UFUNCTION()
	void OnRep_OnGameStateUpdated();

	UFUNCTION()
	void OnRep_OnTotalPlayerCountUpdated() const;

	UFUNCTION()
	void OnRep_OnCurrentPlayerCountUpdated() const;

	UFUNCTION()
	void OnRep_OnTimeRequiredToConnectUpdated();

private:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;

	FTimerHandle ConnectionTimer;

};
