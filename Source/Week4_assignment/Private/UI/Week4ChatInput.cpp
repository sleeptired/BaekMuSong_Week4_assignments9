// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Week4ChatInput.h"
#include "Components/EditableTextBox.h"
#include "Player/Week4PlayerController.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UWeek4ChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}

	// [추가] 버튼 클릭 이벤트 바인딩
	if (!Button_Ready->OnClicked.IsAlreadyBound(this, &ThisClass::OnReadyButtonClicked))
	{
		Button_Ready->OnClicked.AddDynamic(this, &ThisClass::OnReadyButtonClicked);
	}

	if (!Button_Quit->OnClicked.IsAlreadyBound(this, &ThisClass::OnQuitButtonClicked))
	{
		Button_Quit->OnClicked.AddDynamic(this, &ThisClass::OnQuitButtonClicked);
	}
}

void UWeek4ChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UWeek4ChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			AWeek4PlayerController* OwningWeek4PlayerController = Cast<AWeek4PlayerController>(OwningPlayerController);
			if (IsValid(OwningWeek4PlayerController) == true)
			{
				OwningWeek4PlayerController->SetChatMessageString(Text.ToString());

				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}

void UWeek4ChatInput::OnReadyButtonClicked()
{
	APlayerController* OwningPlayerController = GetOwningPlayer();
	if (AWeek4PlayerController* OwningWeek4PC = Cast<AWeek4PlayerController>(OwningPlayerController))
	{
		OwningWeek4PC->ServerRPCPlayerReady(); // 서버로 나 준비됐다고 전달!
	}
}

void UWeek4ChatInput::OnQuitButtonClicked()
{
	APlayerController* OwningPlayerController = GetOwningPlayer();
	// 현재 게임을 즉시 종료하고 창을 닫습니다.
	UKismetSystemLibrary::QuitGame(this, OwningPlayerController, EQuitPreference::Quit, true);
}
