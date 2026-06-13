// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Week4GameModeBase.h"
#include "Game/Week4GameStateBase.h"

void AWeek4GameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	AWeek4GameStateBase* Week4GameStateBase =  GetGameState<AWeek4GameStateBase>();
	if (IsValid(Week4GameStateBase) == true)
	{
		Week4GameStateBase->MulticastRPCBroadcastLoginMessage(TEXT("XXXXXXX"));
	}
}
