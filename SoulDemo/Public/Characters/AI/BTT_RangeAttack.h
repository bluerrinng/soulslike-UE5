// Fill out your copyright notice in the Description page of Project Settings.
// BTT_RangeAttack.h
// Behavior Tree Task 클래스
// AI 보스가 플레이어를 향해 원거리 공격(Range Attack)을 수행하도록 구현하는 Task입니다.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RangeAttack.generated.h"

/**
 * UBTT_RangeAttack
 * AI 보스가 플레이어에게 원거리 공격을 수행하도록 Behavior Tree Task를 구현.
 * ExecuteTask에서 공격 애니메이션을 재생하고 Task를 완료합니다.
 */
UCLASS()
class SOULDEMO_API UBTT_RangeAttack : public UBTTaskNode
{
	GENERATED_BODY()

	// 원거리 공격 애니메이션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage;

	// 공격 성공 판단 임계값 (예: 애니메이션 재생 비율 기준)
	double Threshold{ 0.9 };

public:
	// Task 실행 시 호출되는 함수
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;
};
