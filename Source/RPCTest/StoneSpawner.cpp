#include "StoneSpawner.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>

AStoneSpawner::AStoneSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStoneSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStoneSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


