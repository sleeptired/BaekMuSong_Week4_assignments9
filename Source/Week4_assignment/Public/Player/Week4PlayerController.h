// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Week4PlayerController.generated.h"

class UWeek4ChatInput;
/**
 *
 */
UCLASS()
class WEEK4_ASSIGNMENT_API AWeek4PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWeek4PlayerController();

	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString); // 채팅로직

	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString); // 서버가 호출하지만, 실제 코드는 컨트롤러를 가진 클라이언트 컴퓨터 화면에서만 돌아가는 로직

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString); // 클라이언트가 호출하지만, 실제 코드는 '서버 컴퓨터'에서만 돌아가는 로직

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override; // 네트워크로 복제할 변수를 등록하는 로직
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWeek4ChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UWeek4ChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
