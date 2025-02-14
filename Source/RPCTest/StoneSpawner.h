#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StoneSpawner.generated.h"

UCLASS()
class RPCTEST_API AStoneSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	AStoneSpawner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Event)
	void SpawnStones(int iLevel);
};
