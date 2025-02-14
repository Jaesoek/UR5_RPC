// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPCTestCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>


ARPCTestCharacter::ARPCTestCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ARPCTestCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ARPCTestCharacter::AttackStart()
{
	// Bottom sweep으로 위치 선정
	auto vLocation = GetActorLocation();
	FHitResult hitResult{};
	FCollisionQueryParams queryParam{};
	GetWorld()->LineTraceSingleByChannel(hitResult, vLocation, vLocation - FVector(0.f, 0.f, 100.f), ECollisionChannel::ECC_Visibility);

	if (hitResult.bBlockingHit)
	{
		// Spawn Actor
		m_pAttackRange = GetWorld()->SpawnActor<AActor>(ClassAttackRange, hitResult.Location, GetActorRotation());
		m_pAttackRange->SetOwner(GetController());
		FAttachmentTransformRules attachRule(EAttachmentRule::KeepWorld, true);
		m_pAttackRange->AttachToActor(this, attachRule);
	}
}

void ARPCTestCharacter::AttackCheck()
{
	if (!IsValid(m_pAttackRange))
		return;

	FProperty* Property = m_pAttackRange->GetClass()->FindPropertyByName("IsFoolCharge");
	if (Property)
	{
		bool* pIsFoolCharge = Property->ContainerPtrToValuePtr<bool>(m_pAttackRange);
		if (*pIsFoolCharge)
			AttackFire();
	}
}

void ARPCTestCharacter::AttackFire()
{
	if (IsValid(m_pAttackRange))
	{
		FName FunctionName = "SpawnAttack"; // 블루프린트에서 정의된 함수명
		UFunction* Function = m_pAttackRange->FindFunction(FunctionName);
		if (Function)
			m_pAttackRange->ProcessEvent(Function, nullptr);

		m_pAttackRange->Destroy();
		m_pAttackRange = nullptr;

		// Mode
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		animInstance->Montage_Play(AttackActionMontage);
	}
}