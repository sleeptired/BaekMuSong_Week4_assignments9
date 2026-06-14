// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Week4GameModeBase.generated.h"

class AWeek4PlayerController;
/**
 *
 */
UCLASS()
class WEEK4_ASSIGNMENT_API AWeek4GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override; // 클라이언트가 게임 방(서버)에 접속을 완료했을 때 언리얼 엔진이 호출함.

	FString GenerateSecretNumber(); // 게임이 처음 시작되거나(BeginPlay), 리셋(ResetGame)될 때 정답을 만듬

	bool IsGuessNumberString(const FString& InNumberString); // 사용자가 숫자를 입력했을 때 정상적인 숫자인지 논리 검사

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString); // 유저 입력이 모든 검사를 통과했을 때, 서버의 정답과 비교하여 S/B을 계산합니다.

	virtual void BeginPlay() override;

	void PrintChatMessageString(AWeek4PlayerController* InChattingPlayerController, const FString& InChatMessageString); // 플레이어 컨트롤러가 Server RPC를 통해 뭘 입력했는지 서버에 보내는 로직

	void IncreaseGuessCount(AWeek4PlayerController* InChattingPlayerController); // 횟수 증가

	void ResetGame();

	void JudgeGame(AWeek4PlayerController* InChattingPlayerController, int InStrikeCount); // 정상 입력이 들어올 때마다 승리,무승부 여부를 판단합니다.

protected:
	FString SecretNumberString;

	TArray<TWeakObjectPtr<AWeek4PlayerController>> AllPlayerControllers;

	FTimerHandle ResetTimerHandle;
};
