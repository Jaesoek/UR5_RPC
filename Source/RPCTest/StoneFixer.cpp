
#include "StoneFixer.h"
#include "RPCTestPlayerController.h"
#include "StoneActor.h"

AStoneFixer::AStoneFixer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStoneFixer::BeginPlay()
{
	Super::BeginPlay();
}

void AStoneFixer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetController() != nullptr)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (!PlayerController)
			return;

		FHitResult Hit;
		bool bHitSuccessful = false;
		bHitSuccessful = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
		if (bHitSuccessful)
		{
			m_vCachedPos = Hit.Location;
			m_pTempStone->MoveTo(m_vCachedPos);
		}
	}
}

void AStoneFixer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AStoneFixer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	m_pController = Cast<ARPCTestPlayerController>(NewController);

	m_pTempStone = GetWorld()->SpawnActor<AStoneActor>(m_TempStoneClass, m_vCachedPos, GetActorRotation());
	m_pTempStone->SetOwner(this);
}
