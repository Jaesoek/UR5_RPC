// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPCTestGameMode.h"
#include "RPCTestPlayerController.h"
#include "RPCTestCharacter.h"
#include "StoneSpawner.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>

ARPCTestGameMode::ARPCTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	// set default stone spawner
	static ConstructorHelpers::FClassFinder<AActor> StoneSpawnerBPClass(TEXT("/Game/TopDown/Blueprints/BP_Spawner_Stone"));
	if (StoneSpawnerBPClass.Class != NULL)
	{
		DefaultStoneSpawnerClass = StoneSpawnerBPClass.Class;
	}
}

void ARPCTestGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (m_pSpawner == nullptr)
		m_pSpawner = Cast<AStoneSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), DefaultStoneSpawnerClass));
}

void ARPCTestGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto pNewRPCPlayer = Cast<ARPCTestPlayerController>(NewPlayer);
	pNewRPCPlayer->m_isBlack = m_iCurrentUser % 2 == 1;
	pNewRPCPlayer->m_iMyID = m_iCurrentUser;

	m_iCurrentUser++;
	if (m_iCurrentUser == m_MaxUserCnt)	// Spawn when all players login
	{
		m_pSpawner->SpawnStones(m_iCurrentLevel);
	}
}

void ARPCTestGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	m_iCurrentUser--;
}
