// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Week4Pawn.h"
#include "Week4_assignment/Week4_assignment.h"


// Called when the game starts or when spawned
void AWeek4Pawn::BeginPlay()
{
	Super::BeginPlay();
	FString NetRoleString =  Week4FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::BeginPlay() %s [%s]"), * Week4FunctionLibrary::GetNetModeString(this), *NetRoleString);
	Week4FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void AWeek4Pawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = Week4FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::PossessedBy() %s [%s]"), *Week4FunctionLibrary::GetNetModeString(this), *NetRoleString);
	Week4FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}


