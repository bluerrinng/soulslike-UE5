// Fill out your copyright notice in the Description page of Project Settings.
// BTT_MeleeAttack.h
// Behavior Tree Task 클래스
// AI 보스가 플레이어에게 근접 공격(Melee Attack)을 수행하도록 구현하는 Task입니다.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MeleeAttack.generated.h"

/**
 * UBTT_MeleeAttack
 * AI 보스가 플레이어를 향해 근접 공격을 수행하도록 Behavior Tree Task를 구현.
 * TickTask를 통해 공격 진행 상태를 체크하고 완료 시 Task를 종료합니다.
 */
UCLASS()
class SOULDEMO_API UBTT_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()

	// 공격 가능한 최대 거리
	UPROPERTY(EditAnywhere)
	float AttackRadius{ 200.0f };

	// Task 완료를 판단할 최소 거리
	UPROPERTY(EditAnywhere)
	float AcceptableRadius{ 100.0f };

	// Task 완료 여부
	bool bIsFinished{ false };

	// 이동 완료 시 호출될 델리게이트
	FScriptDelegate MoveDelegate;

protected:
	// Task 실행 시 호출
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Task 수행 중 매 Tick마다 호출
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// 생성자
	UBTT_MeleeAttack();

	// Task 종료 처리 함수
	UFUNCTION()
	void FinishAttackTask();
	
};
