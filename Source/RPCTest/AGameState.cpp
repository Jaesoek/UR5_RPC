// Fill out your copyright notice in the Description page of Project Settings.


#include "AGameState.h"
#include "RPCTestPlayerController.h"

#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"

void AAGameState::PossToPlacer()
{
    UGameInstance* GameInstance = GetWorld()->GetGameInstance();
    if (GameInstance)
    {
        ARPCTestPlayerController* LocalPlayer = Cast<ARPCTestPlayerController>(GameInstance->GetFirstLocalPlayerController());
    }
}
