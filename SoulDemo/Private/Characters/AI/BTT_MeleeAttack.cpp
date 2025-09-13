// Fill out your copyright notice in the Description page of Project Settings.

// BTT_MeleeAttack.cpp
// Behavior Tree Task 클래스
// 보스 AI 또는 적이 근접 공격(Melee) 행동을 수행하는 Task
// 1. ExecuteTask:
//    - 플레이어와의 거리를 확인
//    - 공격 범위 밖이면 플레이어 쪽으로 이동 시작
//    - 공격 범위 안이면 즉시 공격 수행, 애니메이션 길이만큼 대기 후 Task 완료
// 2. TickTask:
//    - 플레이어가 공격 범위를 벗어나면 원거리(Range) 상태로 전환
//    - 공격 완료 여부 확인 후 Task 완료 처리
// 3. FinishAttackTask: 공격 완료 플래그 설정

#include "Characters/AI/BTT_MeleeAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Interfaces/Fighter.h"
#include "GameFramework/Character.h"
#include "Characters/EEnemyState.h"

// Task 실행 시 호출: 근접 공격 수행 또는 이동
EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    bIsFinished = false;

    // 블랙보드에서 플레이어와의 거리 가져오기
    float Distance{ OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance")) };

    AAIController* AIRef{ OwnerComp.GetAIOwner() };

    // 공격 범위 밖이면 플레이어에게 이동
    if (Distance > AttackRadius)
    {
        APawn* PlayerRef{ GetWorld()->GetFirstPlayerController()->GetPawn() };
        FAIMoveRequest MoveRequest{ PlayerRef };
        MoveRequest.SetUsePathfinding(true);           // 경로 탐색 사용
        MoveRequest.SetAcceptanceRadius(AcceptableRadius); // 목표 근처에서 멈춤

        AIRef->MoveTo(MoveRequest);
        AIRef->SetFocus(PlayerRef);                    // 플레이어 포커스

        // 이동 완료 시 호출될 델리게이트 등록
        AIRef->ReceiveMoveCompleted.AddUnique(MoveDelegate);
    }
    // 공격 범위 안이면 즉시 공격
    else
    {
        IFighter* FighterRef{ Cast<IFighter>(AIRef->GetCharacter()) };
        FighterRef->Attack();

        FTimerHandle AttackTimerHandle;

        // 공격 애니메이션 길이만큼 지연 후 Task 완료 처리
        AIRef->GetCharacter()->GetWorldTimerManager().SetTimer(
            AttackTimerHandle,
            this,
            &UBTT_MeleeAttack::FinishAttackTask,
            FighterRef->GetAnimDuration(),
            false
        );
    }

    return EBTNodeResult::InProgress; // Task 진행 중 상태 반환
}

// 매 프레임 실행되는 TickTask: 공격 범위 이탈 체크 및 Task 완료 처리
void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"));
    AAIController* AIRef = OwnerComp.GetAIOwner();
    IFighter* FighterRef = Cast<IFighter>(AIRef->GetCharacter());

    // 플레이어가 근접 공격 범위를 벗어나면 원거리 상태로 전환
    if (Distance > FighterRef->GetMeleeRange())
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(
            TEXT("CurrentState"), EEnemyState::Range);

        // Task 중단
        AbortTask(OwnerComp, NodeMemory);
        FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);

        // 이동 및 포커스 초기화
        AIRef->StopMovement();
        AIRef->ClearFocus(EAIFocusPriority::Gameplay);
        AIRef->ReceiveMoveCompleted.Remove(MoveDelegate);
    }

    // 공격이 아직 완료되지 않았다면 종료
    if (!bIsFinished)
    {
        return;
    }

    // Task 완료 처리
    OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Remove(MoveDelegate);
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

// 생성자: Tick 알림 및 MoveDelegate 바인딩
UBTT_MeleeAttack::UBTT_MeleeAttack()
{
    MoveDelegate.BindUFunction(this, "FinishAttackTask");
    bNotifyTick = true;
}

// 공격 Task 완료 플래그 설정
void UBTT_MeleeAttack::FinishAttackTask()
{
    bIsFinished = true;
}
