	// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Week4PlayerState.h"
#include "Net/UnrealNetwork.h"

AWeek4PlayerState::AWeek4PlayerState():PlayerNameString(TEXT("None")), CurrentGuessCount(0), MaxGuessCount(3)
{
	bReplicates = true;
}

void AWeek4PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

FString AWeek4PlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = TEXT(" (") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}
