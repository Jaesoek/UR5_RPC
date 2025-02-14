// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPCTestGameMode.generated.h"

UCLASS(minimalapi)
class ARPCTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARPCTestGameMode();

protected:
	UFUNCTION(Blueprintcallable)
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

protected:
	TSubclassOf<AActor> DefaultStoneSpawnerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	TObjectPtr<class AStoneSpawner> m_pSpawner;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	int64 m_MaxUserCnt = 2;

private:
	int64 m_iCurrentUser = 0;
	int64 m_iCurrentLevel = 1;
};



