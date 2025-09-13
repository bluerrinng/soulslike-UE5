// Fill out your copyright notice in the Description page of Project Settings.
// BTT_ChargeAttack.h
// Behavior Tree Task 클래스
// 보스 AI가 플레이어를 향해 돌진(Charge Attack)하도록 하는 기능을 수행합니다.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChargeAttack.generated.h"

/**
 * UBTT_ChargeAttack
 * AI 보스가 플레이어를 향해 돌진 공격을 수행하도록 Behavior Tree Task를 구현.
 * TickTask를 통해 이동 진행 상황을 체크하며 공격 종료 여부를 관리합니다.
 */
UCLASS()
class SOULDEMO_API UBTT_ChargeAttack : public UBTTaskNode
{
	GENERATED_BODY()

	// Task 수행 시 필요한 AI 컨트롤러 참조
	AAIController* ControllerRef;

	// 보스 캐릭터 참조
	ACharacter* CharacterRef;

	// 보스 애니메이션 인스턴스 참조
	class UBossAnimInstance* BossAnim;

	// 돌진 종료를 판단할 수 있는 거리
	UPROPERTY(EditAnywhere)
	float AcceptableRadius{ 100.0f };

	// MoveTo가 완료되었을 때 호출될 델리게이트
	FScriptDelegate MoveCompletedDelegate;

	// Task 실행 전 원래 보스의 이동 속도 저장
	float OriginalWalkSpeed;

	// 돌진 시 이동 속도
	UPROPERTY(EditAnywhere)
	float ChargeWalkSpeed{ 2000.0f };

	// Task 완료 여부
	bool bIsFinished{ false };

protected:
	// Task 수행 중 매 Tick마다 호출
	// 이동 상태 확인 및 Task 완료 여부 판단
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// 생성자
	UBTT_ChargeAttack();

	// Task 시작 시 호출
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 플레이어를 향해 돌진 실행
	void ChargeAtPlayer();

	// MoveTo 완료 시 호출되는 함수
	UFUNCTION()
	void HandleMoveCompleted();

	// Task 종료 처리
	UFUNCTION()
	void FinishAttackTask();
	
};
