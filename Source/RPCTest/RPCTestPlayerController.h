// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "RPCTestPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class AAGameState;
class ARPCTestCharacter;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class ARPCTestPlayerController : public APlayerController
{
	GENERATED_BODY()

	friend class ARPCTestGameMode;

public:
	ARPCTestPlayerController();

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	/** Check */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float FollowTime; // For how long it has been pressed

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	// About key inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetAttackPressedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetMovePressedAction;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	// Input handlers for checking...
	void Move(const FInputActionValue& Value);

	// Attack KeyInput
	void OnClickKeyQ();
	void OnPressingKeyQ();
	void OnReleasedKeyQ();

public:
	void PossToPlacer();
	void PossToGameCharacter();

private:
	FVector CachedDestination;
	bool bIsTouch; // Is it a touch device

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	TObjectPtr<AAGameState> m_GameState;

private:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = Team, meta = (AllowPrivateAccess = "true"))
	int m_iMyID;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Team, meta = (AllowPrivateAccess = "true"))
	bool m_isBlack;

private:
	UPROPERTY(BlueprintReadOnly, Category = Team, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APawn> m_pPlacer;

	UPROPERTY(BlueprintReadOnly, Category = Team, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ARPCTestCharacter> m_pGameCharacter;
};

