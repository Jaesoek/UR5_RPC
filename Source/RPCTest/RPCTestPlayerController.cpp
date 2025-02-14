// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPCTestPlayerController.h"
#include "RPCTestCharacter.h"
#include "AGameState.h"

#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ARPCTestPlayerController::ARPCTestPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void ARPCTestPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARPCTestPlayerController, m_isBlack);
	DOREPLIFETIME(ARPCTestPlayerController, m_iMyID);
}

void ARPCTestPlayerController::BeginPlay()
{
	Super::BeginPlay();

	m_GameState = GetWorld()->GetGameState<AAGameState>();
}

void ARPCTestPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ARPCTestPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ARPCTestPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ARPCTestPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ARPCTestPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ARPCTestPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ARPCTestPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ARPCTestPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ARPCTestPlayerController::OnTouchReleased);

		// Setup WASD Move input events
		EnhancedInputComponent->BindAction(SetMovePressedAction, ETriggerEvent::Triggered, this, &ARPCTestPlayerController::Move);

		// Setup Attack(KeyF) input events
		EnhancedInputComponent->BindAction(SetAttackPressedAction, ETriggerEvent::Started, this, &ARPCTestPlayerController::OnClickKeyQ);
		EnhancedInputComponent->BindAction(SetAttackPressedAction, ETriggerEvent::Triggered, this, &ARPCTestPlayerController::OnPressingKeyQ);
		EnhancedInputComponent->BindAction(SetAttackPressedAction, ETriggerEvent::Completed, this, &ARPCTestPlayerController::OnReleasedKeyQ);
		EnhancedInputComponent->BindAction(SetAttackPressedAction, ETriggerEvent::Canceled, this, &ARPCTestPlayerController::OnReleasedKeyQ);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARPCTestPlayerController::OnInputStarted()
{
	StopMovement();
}

void ARPCTestPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ARPCTestPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

void ARPCTestPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ARPCTestPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void ARPCTestPlayerController::Move(const FInputActionValue& Value)
{
	auto vPath = Value.Get<FVector>();
	ARPCTestCharacter* ControlledPawn = Cast<ARPCTestCharacter>(GetPawn());
	if (ControlledPawn == nullptr)
		return;
	ControlledPawn->AddMovementInput(vPath);
}

void ARPCTestPlayerController::OnClickKeyQ()
{
	if (!m_GameState->IsMyTurn(m_iMyID))
		return;	// TODO: Print ui(Not your turn)

	ARPCTestCharacter* ControlledPawn = Cast<ARPCTestCharacter>(GetPawn());
	if (ControlledPawn == nullptr)
		return;

	ControlledPawn->AttackStart();
}

void ARPCTestPlayerController::OnPressingKeyQ()
{
	if (!m_GameState->IsMyTurn(m_iMyID))
		return;	// TODO: Print ui(Not your turn)

	ARPCTestCharacter* ControlledPawn = Cast<ARPCTestCharacter>(GetPawn());
	if (ControlledPawn == nullptr)
		return;

	ControlledPawn->AttackCheck();
}

void ARPCTestPlayerController::OnReleasedKeyQ()
{
	if (!m_GameState->IsMyTurn(m_iMyID))
		return;	// TODO: Print ui(Not your turn)

	ARPCTestCharacter* ControlledPawn = Cast<ARPCTestCharacter>(GetPawn());
	if (ControlledPawn == nullptr)
		return;

	ControlledPawn->AttackFire();
}

void ARPCTestPlayerController::PossToPlacer()
{
	
	Possess(m_pPlacer);
}
void ARPCTestPlayerController::PossToGameCharacter()
{
	Possess(m_pGameCharacter);
}