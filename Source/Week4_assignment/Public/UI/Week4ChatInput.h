// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Week4ChatInput.generated.h"

class UEditableTextBox;
class UButton;
/**
 *
 */
UCLASS()
class WEEK4_ASSIGNMENT_API UWeek4ChatInput : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnReadyButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();

public:
	// meta = (BindWidget): 블루프린트 위젯(UMG)에 있는 요소와 이 C++ 변수를 이름으로 묶어줍니다.
	// 블루프린트에서 만든 입력창의 이름이 반드시 "EditableTextBox_ChatInput"이어야만 에러가 나지 않음
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Ready;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Quit;
};
