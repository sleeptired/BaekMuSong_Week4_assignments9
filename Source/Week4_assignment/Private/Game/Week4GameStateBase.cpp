// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Week4GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include  "Player/Week4PlayerController.h"

void AWeek4GameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (GetNetMode() != NM_DedicatedServer)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			AWeek4PlayerController* Week4PC = Cast<AWeek4PlayerController>(PC);
			if (IsValid(Week4PC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				Week4PC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
