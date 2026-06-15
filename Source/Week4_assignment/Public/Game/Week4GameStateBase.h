// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Week4GameStateBase.generated.h"

class AWeek4PlayerState;
/**
 *
 */

UENUM(BlueprintType)
enum class EWeek4MatchState : uint8
{
	Waiting,
	Playing
};

UCLASS()
class WEEK4_ASSIGNMENT_API AWeek4GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AWeek4GameStateBase();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));

	//  모든 유저가 공유할 남은 시간
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 TurnTimeRemaining;

	//  UI에서 누구 턴인지 확인할떄 사용
	UPROPERTY(Replicated, BlueprintReadOnly)
	TObjectPtr<AWeek4PlayerState> CurrentTurnPlayerState;

	// [추가] 게임 준비, 종료 상태
	UPROPERTY(Replicated, BlueprintReadOnly)
	EWeek4MatchState MatchState;
};
