// Fill out your copyright notice in the Description page of Project Settings.
// EEnemyState.cpp / EEnemyState.h
// 적 캐릭터의 현재 상태를 정의하는 열거형(Enum)
// AI 및 Behavior Tree에서 적 캐릭터의 행동을 결정하는데 사용됨
// 예시 상태:
// - Idle: 대기 상태, 플레이어를 탐지하지 않은 상태
// - Range: 원거리 공격 상태
// - Melee: 근접 공격 상태
// - Charge: 돌진 공격 상태
// - GameOver: 플레이어 사망 또는 게임 종료 상태
#include "Characters/EEnemyState.h"

// EEnemyState는 적 캐릭터의 상태를 나타내는 Enum입니다.
// 예: Idle, Range, Melee, Charge, GameOver 등
