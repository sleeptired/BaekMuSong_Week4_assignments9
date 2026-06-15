// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Week4PlayerController.h"
#include "UI/Week4ChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Week4_assignment/Week4_assignment.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Game/Week4GameModeBase.h"
#include "Player/Week4PlayerState.h"
#include "Net/UnrealNetwork.h"

AWeek4PlayerController::AWeek4PlayerController()
{
	bReplicates = true;
}

void AWeek4PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// UI는 오직 '내 컴퓨터 화면'에만 띄워야 합니다.
	// 방장 컴퓨터(Listen Server)에는 내 컨트롤러와 남의 컨트롤러가 같이 존재하므로,
	// IsLocalController()를 쓰지 않으면 내 화면에 남의 UI까지 2~3개가 겹침
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

	// [공지사항 위젯] 얘는 모두에게 화면을 띄워야 함
	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void AWeek4PlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		//ServerRPCPrintChatMessageString(InChatMessageString);
		AWeek4PlayerState* Week4PS = GetPlayerState<AWeek4PlayerState>();
		if (IsValid(Week4PS) == true)
		{
			FString CombinedMessageString = Week4PS->PlayerNameString + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void AWeek4PlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	Week4FunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void AWeek4PlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
	DOREPLIFETIME(ThisClass, bIsReady);
}

//추가 부분
void AWeek4PlayerController::ServerRPCPlayerReady_Implementation()
{
	if (bIsReady == true) // 이미 눌렀으면 무시
	{
		return;
	}

	bIsReady = true;

	// 게임 모드에게 "나 준비했으니 인원 체크해봐!" 라고 전달
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (AWeek4GameModeBase* Week4GM = Cast<AWeek4GameModeBase>(GM))
	{
		Week4GM->CheckAllPlayersReady();
	}
}

void AWeek4PlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{

	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		AWeek4GameModeBase* Week4GM = Cast<AWeek4GameModeBase>(GM);
		if (IsValid(Week4GM) == true)
		{
			Week4GM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void AWeek4PlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}
