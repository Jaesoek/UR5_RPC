#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spectator_InGame.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(TEST, Log, All);

UCLASS()
class RPCTEST_API ASpectator_InGame : public APawn
{
	GENERATED_BODY()

public:
	ASpectator_InGame();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (DisableEditOnInstance))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (DisableEditOnInstance))
	class USpringArmComponent* CameraBoom;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MovementInfo)
	float m_fCamSpeed;
};
