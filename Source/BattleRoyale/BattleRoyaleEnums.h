#pragma once

UENUM(BlueprintType)
enum class EGameState : uint8
{
	UNINITIALIZE = 0 UMETA(DisplayName = "Uninitialize"),
	CONNECTION = 1 UMETA(DisplayName = "Connection"),
	PREGAME = 2 UMETA(DisplayName = "Pre game"),
	INGAME = 3 UMETA(DisplayName = "In Game"),
	ENDGAME = 4 UMETA(DisplayName = "End Game")
};


UENUM(BlueprintType)
enum class EElectricZoneState : uint8
{
	IDLE = 0 UMETA(DisplayName = "Idle"),
	SHRINK = 1 UMETA(DisplayName = "Shrink")
};
