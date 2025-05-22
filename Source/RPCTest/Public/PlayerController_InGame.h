#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "PlayerController_InGame.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(TEST, Log, All);

UCLASS()
class RPCTEST_API APlayerController_InGame : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerController_InGame();

protected:
	virtual void BeginPlay() override;
	virtual void Possess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

protected:
	void HandleMouseMoving(const FInputActionValue& Value);
	void HandleMouseClick(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MouseMoving;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MouseClick;
};
