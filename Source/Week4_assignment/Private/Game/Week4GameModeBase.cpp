// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Week4GameModeBase.h"
#include "Game/Week4GameStateBase.h"
#include "Player/Week4PlayerController.h"
#include "Player/Week4PlayerState.h"
#include "EngineUtils.h"

void AWeek4GameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AWeek4PlayerController* Week4PlayerController = Cast<AWeek4PlayerController>(NewPlayer);
	if (IsValid(Week4PlayerController) == true)
	{
		// 접속한 유저의 화면 중앙 공지사항을 출력
		Week4PlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));

		// 서버의 참가자 배열에 이 유저를 등록합니다. 나중에 채팅을 뿌릴 때 사용.
		AllPlayerControllers.Add(Week4PlayerController);

		// static 변수를 사용해 접속한 순서대로 번호 제공
		static int32 PlayerNumber = 0;
		 AWeek4PlayerState* Week4PS = Week4PlayerController->GetPlayerState< AWeek4PlayerState>();
		if (IsValid(Week4PS) == true)
		{
			// 이 변수는 Replicated이므로 서버에서 바꾸면 해당 클라이언트 화면에서도 자동으로 이름이 바뀜
			Week4PS->PlayerNameString = TEXT("Player") + FString::FromInt(++PlayerNumber);
		}

		// GameState를 통해 현재 방에 있는 모든 유저에게 접속했다고 알림을 보냄 (Multicast RPC).
		AWeek4GameStateBase* Week4GameStateBase = GetGameState<AWeek4GameStateBase>();
		if (IsValid(Week4GameStateBase) == true)
		{
			Week4GameStateBase->MulticastRPCBroadcastLoginMessage(Week4PS->PlayerNameString);
		}
	}
}

FString AWeek4GameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);// 1~9까지 숫자를 배열에 넣음
	}

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		// 배열에서 랜덤하게 하나를 뽑아 결과 문자열에 붙임
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		// 한 번 뽑은 숫자는 배열에서 삭제하여 '중복'이 발생하지 않도록 함
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool AWeek4GameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	// 중복 검사를 위한 Set 자료구조 선언
	TSet<TCHAR> UniqueDigits;

	// 문자열의 각 문자를 하나씩 검사
	for (TCHAR C : InNumberString)
	{
		// 1. 숫자가 아니거나, '0'이 포함되어 있으면 실패 (false 반환)
		if (FChar::IsDigit(C) == false || C == '0')
		{
			return false;
		}

		// Set에 문자 추가 (Set은 중복된 값을 무시함)
		UniqueDigits.Add(C);
	}

	// 2. 입력된 문자열의 길이(3)와 Set의 크기가 다르다면 중복된 문자가 있었다는 뜻이므로 실패
	if (UniqueDigits.Num() != InNumberString.Len())
	{
		return false;
	}

	// 모든 검사를 무사히 통과했다면 정상적인 야구 게임 숫자!
	return true;
}

FString AWeek4GameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			// 자리와 숫자가 모두 같은 경우
			StrikeCount++;
		}
		else
		{
			// 정답 문자열에 맞는 숫자는 있지만 자리가 달랐을 때 경우
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		// 겹치는 게 없는 경우
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void AWeek4GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// 서버가 시작되면 정답을 생성
	SecretNumberString = GenerateSecretNumber();
}

void AWeek4GameModeBase::PrintChatMessageString(AWeek4PlayerController* InChattingPlayerController,const FString& InChatMessageString)
{
	/*
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		IncreaseGuessCount(InChattingPlayerController);

		for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
		{
			AWeek4PlayerController* Week4PlayerController = AllPlayerControllers[i].Get();
			if (IsValid(Week4PlayerController) == false)
			{
				AllPlayerControllers.RemoveAt(i);
				continue;
			}

			//FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
			//Week4PlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
			AWeek4PlayerState* Week4PS = InChattingPlayerController->GetPlayerState<AWeek4PlayerState>();
			if (IsValid(Week4PS) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString + Week4PS->GetPlayerInfoString();
				Week4PlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}
		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
		JudgeGame(InChattingPlayerController, StrikeCount);
	}
	else
	{
		for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
		{
			AWeek4PlayerController* Week4PlayerController = AllPlayerControllers[i].Get();
			if (IsValid(Week4PlayerController) == false)
			{
				AllPlayerControllers.RemoveAt(i);
				continue;
			}

			Week4PlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
		}
	}
	*/
	AWeek4PlayerState* Week4PS = InChattingPlayerController->GetPlayerState<AWeek4PlayerState>();

	// [예외 처리] 이미 기회를 모두 소진한 플레이어의 입력 차단
	if (IsValid(Week4PS) && Week4PS->CurrentGuessCount >= Week4PS->MaxGuessCount)
	{
		// 입력한 본인에게만 경고 메시지를 보냅니다.
		InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("System: 기회를 모두 소진했습니다. 결과를 기다려주세요."));
		return; // 더 이상 아래 판정 로직을 실행하지 않고 함수를 종료합니다.
	}

	// 입력된 채팅에서 숫자 부분만 추출 ("Player1: 123" -> "123" 추출)
	FString GuessNumberString;
	FString PlayerNameString;
	// ": "를 기준으로 이름과 채팅 내용을 분리하여 안전하게 추출합니다.
	if (InChatMessageString.Split(TEXT(": "), &PlayerNameString, &GuessNumberString) == false)
	{
		GuessNumberString = InChatMessageString;
	}

	// [예외 처리] 입력 형식 상세 경고 (기회 소진 X)
	if (GuessNumberString.Len() != 3)
	{
		InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("System: 3자리 숫자를 입력해주세요."));
		return;
	}

	if (IsGuessNumberString(GuessNumberString) == false)
	{
		InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("System: 중복되지 않은 1~9 사이의 숫자를 입력해주세요."));
		return;
	}

	// === 여기서부터는 기존에 작성하신 정상 판정 로직입니다 ===
	FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

	// 정상적인 입력이므로 시도 횟수 1 증가
	IncreaseGuessCount(InChattingPlayerController);

	for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
	{
		AWeek4PlayerController* Week4PlayerController = AllPlayerControllers[i].Get();
		if (IsValid(Week4PlayerController) == false)
		{
			AllPlayerControllers.RemoveAt(i);
			continue;
		}

		if (IsValid(Week4PS) == true)
		{
			FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString + Week4PS->GetPlayerInfoString();
			Week4PlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
		}
	}

	// 문자열에서 앞글자(스트라이크 숫자)만 뽑아서 숫자로 변환 후 승리/무승부 판정
	int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
	JudgeGame(InChattingPlayerController, StrikeCount);
}

void AWeek4GameModeBase::IncreaseGuessCount(AWeek4PlayerController* InChattingPlayerController)
{
	AWeek4PlayerState* Week4PS = InChattingPlayerController->GetPlayerState<AWeek4PlayerState>();
	if (IsValid(Week4PS) == true)
	{
		Week4PS->CurrentGuessCount++;
	}
}

void AWeek4GameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber(); // 새 정답 뽑기

	// 모든 플레이어의 시도 횟수를 0으로 초기화
	for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
	{
		AWeek4PlayerController* Week4PlayerController = AllPlayerControllers[i].Get();
		if (IsValid(Week4PlayerController) == true)
		{
			AWeek4PlayerState* Week4PS = Week4PlayerController->GetPlayerState<AWeek4PlayerState>();
			if (IsValid(Week4PS) == true)
			{
				Week4PS->CurrentGuessCount = 0;
			}
			Week4PlayerController->NotificationText = FText::GetEmpty();

			// 새 게임이 시작되었다고 모든 유저의 채팅창에 알려줍니다.
			Week4PlayerController->ClientRPCPrintChatMessageString(TEXT("System: 새로운 게임이 시작되었습니다! 정답을 맞춰보세요."));
		}
	}
}

void AWeek4GameModeBase::JudgeGame(AWeek4PlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount) // 3스트라이크 = 승리
	{
		AWeek4PlayerState* Week4PS = InChattingPlayerController->GetPlayerState<AWeek4PlayerState>();

		// 모든 사람의 화면 중앙 공지를 승리자 이름으로 변경.
		for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
		{
			AWeek4PlayerController* Week4PlayerController = AllPlayerControllers[i].Get();
			if (IsValid(Week4PlayerController) == true)
			{
				if (IsValid(Week4PS) == true)
				{
					FString CombinedMessageString = Week4PS->PlayerNameString + TEXT(" has won the game.");
					Week4PlayerController->NotificationText = FText::FromString(CombinedMessageString);
				}
			}
		}

		// 5초 대기 후 리셋
		GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, this, &AWeek4GameModeBase::ResetGame, 5.0f, false);
	}
	else
	{
		// 아무도 3스트라이크가 아니라면 무승부를 체크합니다.
		bool bIsDraw = true;
		for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
		{
			AWeek4PlayerController* Week4PlayerController = AllPlayerControllers[i].Get();
			if (IsValid(Week4PlayerController) == true)
			{
				AWeek4PlayerState* Week4PS = Week4PlayerController->GetPlayerState<AWeek4PlayerState>();
				if (IsValid(Week4PS) == true)
				{
					// 아직 기회를 다 안 쓴 사람이 단 1명이라도 있다면 무승부가 아님
					if (Week4PS->CurrentGuessCount < Week4PS->MaxGuessCount)
					{
						bIsDraw = false;
						break;
					}
				}
			}
		}
		// 무승부(모두가 3번 다 틀림)일 때
		if (true == bIsDraw)
		{
			for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
			{
				AWeek4PlayerController* Week4PlayerController = AllPlayerControllers[i].Get();
				if (IsValid(Week4PlayerController) == true)
				{
					// 화면 중앙에 무승부 띄우기
					Week4PlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
				}
			}

			ResetGame(); // 새 게임 준비
		}
	}
}
