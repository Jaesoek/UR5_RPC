// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StoneActor.generated.h"

UCLASS()
class RPCTEST_API AStoneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AStoneActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
