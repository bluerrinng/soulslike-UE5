// Fill out your copyright notice in the Description page of Project Settings.

// BTT_ChargeAttack.cpp
// Behavior Tree Task 클래스
// 보스 AI가 플레이어를 향해 돌진(Charge)하는 행동을 수행
// 1. TickTask에서 돌진 준비 여부를 확인
// 2. 준비 완료 시 ChargeAtPlayer()로 플레이어 방향 이동 시작
// 3. 돌진 완료 시 블랙보드 상태를 Melee로 변경하고 Task 종료
// 4. 돌진 속도, 애니메이션, 이동 완료 처리 등을 관리

#include "Characters/AI/BTT_ChargeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animations/BossAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/EEnemyState.h"

// 매 프레임마다 실행되는 TickTask
void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 블랙보드에서 돌진 준비 상태 확인
    bool bIsReadyToCharge{ OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsReadyToCharge")) };

    if (bIsReadyToCharge)
    {
        // 준비 완료 플래그 초기화
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsReadyToCharge"), false);

        // 플레이어를 향해 돌진 시작
        ChargeAtPlayer();
    }

    // 돌진이 아직 끝나지 않았다면 함수 종료
    if (!bIsFinished)
    {
        return;
    }

    // 돌진 완료 후, 적 상태를 근접 전투(Melee)로 변경
    OwnerComp.GetBlackboardComponent()->SetValueAsEnum(
        TEXT("CurrentState"), EEnemyState::Melee
    );
    
    // MoveCompleted 델리게이트 제거
    ControllerRef->ReceiveMoveCompleted.Remove(MoveCompletedDelegate);

    // Behavior Tree Task 종료
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

// 생성자: Tick 알림 설정 및 MoveCompleted 델리게이트 바인딩
UBTT_ChargeAttack::UBTT_ChargeAttack()
{
    bNotifyTick = true;
    
    MoveCompletedDelegate.BindUFunction(
        this, "HandleMoveCompleted"
    );
}

// Task 실행 시 호출
EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ControllerRef = OwnerComp.GetAIOwner();
    CharacterRef = ControllerRef->GetCharacter();

    // 보스 애니메이션 인스턴스 가져오기
    BossAnim = Cast<UBossAnimInstance>(CharacterRef->GetMesh()->GetAnimInstance());

    // 돌진 애니메이션 활성화
    BossAnim->bIsCharging = true;

    // 돌진 준비 플래그 초기화
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsReadyToCharge"), false);

    bIsFinished = false;

    return EBTNodeResult::InProgress; // Task 진행 중 상태 반환
}

// 실제 플레이어를 향해 돌진 시작
void UBTT_ChargeAttack::ChargeAtPlayer()
{
    APawn* PlayerRef{ GetWorld()->GetFirstPlayerController()->GetPawn() };
    FVector PlayerLocation{ PlayerRef->GetActorLocation() };

    FAIMoveRequest MoveRequest{ PlayerLocation };
    MoveRequest.SetUsePathfinding(true);          // 경로 탐색 사용
    MoveRequest.SetAcceptanceRadius(AcceptableRadius); // 목표 근처에서 멈춤

    // AI 이동 시작 및 플레이어에 포커스 설정
    ControllerRef->MoveTo(MoveRequest);
    ControllerRef->SetFocus(PlayerRef);

    // 돌진 완료 시 호출될 델리게이트 등록
    ControllerRef->ReceiveMoveCompleted.AddUnique(MoveCompletedDelegate);

    // 기존 이동 속도 저장 후, 돌진 속도로 변경
    OriginalWalkSpeed = CharacterRef->GetCharacterMovement()->MaxWalkSpeed;
    CharacterRef->GetCharacterMovement()->MaxWalkSpeed = ChargeWalkSpeed;
}

// 돌진 완료 시 호출되는 함수
void UBTT_ChargeAttack::HandleMoveCompleted()
{
    // 돌진 애니메이션 종료
    BossAnim->bIsCharging = false;

    FTimerHandle AttackTimerHandle;

    // 1초 후 공격 완료 처리
    CharacterRef->GetWorldTimerManager().SetTimer(
        AttackTimerHandle,
        this,
        &UBTT_ChargeAttack::FinishAttackTask,
        1.0f,
        false
    );

    // 이동 속도 원래 값으로 복구
    CharacterRef->GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
}

// 공격 Task 완료 플래그 설정
void UBTT_ChargeAttack::FinishAttackTask()
{
    bIsFinished = true;
}
