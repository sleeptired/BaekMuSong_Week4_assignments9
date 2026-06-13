// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Week4PlayerController.h"
#include "UI/Week4ChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Week4_assignment/Week4_assignment.h"
#include "EngineUtils.h"

void AWeek4PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == true)
	{
		FInputModeUIOnly InputModeUIOnly;
		SetInputMode(InputModeUIOnly);

		if (IsValid(ChatInputWidgetClass) == true)
		{
			ChatInputWidgetInstance = CreateWidget<UWeek4ChatInput>(this, ChatInputWidgetClass);
			if (IsValid(ChatInputWidgetInstance) == true)
			{
				ChatInputWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		return;
	}
}

void AWeek4PlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	PrintChatMessageString(ChatMessageString);
}

void AWeek4PlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	Week4FunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}
