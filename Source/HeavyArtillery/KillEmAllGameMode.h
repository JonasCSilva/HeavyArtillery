// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeavyArtilleryGameMode.h"
#include "KillEmAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HEAVYARTILLERY_API AKillEmAllGameMode : public AHeavyArtilleryGameMode
{
	GENERATED_BODY()

public:
	AKillEmAllGameMode();
	
	virtual void PawnKilled(APawn* PawnKilled) override;

public:
	virtual void Tick(float DeltaSeconds) override;

private:
	void EndGame(bool bIsPlayerWinner);
};
