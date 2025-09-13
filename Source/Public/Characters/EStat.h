// Fill out your copyright notice in the Description page of Project Settings.
// EStat.h
// 플레이어 및 적 캐릭터의 능력치(Stats)를 정의하는 Enum입니다.
// StatsComponent 등에서 캐릭터 능력치에 접근하거나 관리할 때 사용됩니다.

#pragma once

#include "CoreMinimal.h"

// 캐릭터 능력치를 나타내는 Enum
UENUM(BlueprintType)
enum EStat
{
	None UMETA(DisplayName = "None Selected"),    // 선택되지 않은 상태
	Health UMETA(DisplayName = "Health"),        // 현재 체력
	MaxHealth UMETA(DisplayName = "MaxHealth"),  // 최대 체력
	Strength UMETA(DisplayName = "Strength"),    // 공격력
	Stamina UMETA(DisplayName = "Stamina"),      // 현재 스태미너
	MaxStamina UMETA(DisplayName = "MaxStamina"),// 최대 스태미너
	MeleeRange UMETA(DisplayName = "MeleeRange") // 근접 공격 범위
};
