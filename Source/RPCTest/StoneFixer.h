#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "StoneFixer.generated.h"

class AStoneActor;
class ARPCTestPlayerController;

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
	TObjectPtr<ARPCTestPlayerController> m_pController;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Event)
	void PlaceStones(FVector vPos);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AStoneActor> m_TempStoneClass;

	TObjectPtr<AStoneActor> m_pTempStone;

private:
	FVector m_vCachedPos;
};
