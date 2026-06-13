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
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWeek4ChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UWeek4ChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;

};
