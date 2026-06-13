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

	if (IsLocalController() == true)
	{
		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void AWeek4PlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	Week4FunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void AWeek4PlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	for (TActorIterator<AWeek4PlayerController> It(GetWorld()); It; ++It)
	{
		AWeek4PlayerController* Week4PlayerController = *It;
		if (IsValid(Week4PlayerController) == true)
		{
			// 찾은 각 플레이어들에게 "네 화면에 이 메시지 띄워!" 라고 명령합니다.
			Week4PlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
		}
	}
}

void AWeek4PlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}
