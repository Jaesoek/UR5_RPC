
#include "StoneFixer.h"

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
}

void AStoneFixer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AStoneFixer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 필요시
 	//SetOwner(NewController);

	// TODO: UI 출력하기

}
