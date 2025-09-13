// Fill out your copyright notice in the Description page of Project Settings.
// EEnemyState.h
// 적의 상태를 정의하는 열거형입니다.
// Behavior Tree나 AI 로직에서 보스/적 캐릭터 상태를 관리할 때 사용됩니다.

#pragma once

#include "CoreMinimal.h"

// 적의 상태를 나타내는 Enum
UENUM(BlueprintType)
enum EEnemyState
{
	Idle UMETA(DisplayName = "Idle"),       // 대기 상태, 아무 행동도 하지 않음
	Range UMETA(DisplayName = "Range"),     // 원거리 공격 상태
	Charge UMETA(DisplayName = "Charge"),   // 돌진 공격 상태
	Melee UMETA(DisplayName = "Melee"),     // 근접 공격 상태
	GameOver UMETA(DisplayName = "Game Over") // 게임 종료 상태
};
