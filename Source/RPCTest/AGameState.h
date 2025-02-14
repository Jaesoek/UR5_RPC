// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "AGameState.generated.h"

UCLASS()
class RPCTEST_API AAGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Event)
	bool IsMyTurn(int iUserID) const;

	UFUNCTION(BlueprintCallable, Category = Event)
	void PossToPlacer();
};
