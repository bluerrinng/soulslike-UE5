// Fill out your copyright notice in the Description page of Project Settings.

// BTT_RangeAttack.cpp
// Behavior Tree Task 클래스
// 보스 AI 또는 적이 원거리 공격(Range) 행동을 수행하는 Task
// 1. ExecuteTask:
//    - 플레이어와의 거리를 블랙보드에서 가져옴
//    - 플레이어가 근접 공격 범위 안이면 원거리 공격 중단, 근접 상태로 전환
//    - 원거리 공격 애니메이션 재생
//    - 일정 확률(Threshold)에 따라 다음 상태를 돌진(Charge)으로 변경
//    - Threshold 값 조정으로 다음 판단에 영향

#include "Characters/AI/BTT_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"
#include "Interfaces/Fighter.h"

// 원거리 공격 Task 실행 시 호출
EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AI가 소유한 캐릭터 가져오기
    ACharacter* CharacterRef{ OwnerComp.GetAIOwner()->GetPawn<ACharacter>() };

    if (!IsValid(CharacterRef))
    {
        return EBTNodeResult::Failed; // 캐릭터가 없으면 Task 실패
    }

    // 블랙보드에서 플레이어와의 거리 가져오기
    float Distance
    {
        OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"))
    };

    AAIController* AIRef = OwnerComp.GetAIOwner();
    IFighter* FighterRef = Cast<IFighter>(CharacterRef);

    // 플레이어가 근접 공격 범위 안에 있으면 원거리 공격 중단, 근접 상태로 전환
    if (Distance < FighterRef->GetMeleeRange())
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Melee);

        AbortTask(OwnerComp, NodeMemory);

        return EBTNodeResult::Aborted;
    }

    // 원거리 공격 애니메이션 재생
