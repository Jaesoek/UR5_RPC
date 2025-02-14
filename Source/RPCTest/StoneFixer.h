#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "StoneFixer.generated.h"

UCLASS()
class RPCTEST_API AStoneFixer : public APawn
{
	GENERATED_BODY()

public:
	AStoneFixer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = Event)
	void PlaceStones(FVector vPos);
};
