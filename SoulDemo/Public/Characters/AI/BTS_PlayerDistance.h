// Fill out your copyright notice in the Description page of Project Settings.
// BTS_PlayerDistance.h
// Behavior Tree Service 클래스
// 플레이어와 AI 캐릭터 사이의 거리를 체크하여 Blackboard에 저장하는 역할을 합니다.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_PlayerDistance.generated.h"

/**
 * UBTS_PlayerDistance
 * AI 캐릭터가 플레이어와의 거리를 지속적으로 확인하도록 하는 Behavior Tree Service.
 * TickNode 함수가 매 프레임 호출되어 거리 계산 후 Blackboard에 값을 업데이트합니다.
 */
UCLASS()
class SOULDEMO_API UBTS_PlayerDistance : public UBTService
{
	GENERATED_BODY()
	
protected:
	// Behavior Tree Service의 핵심 함수.
	// 매 Tick마다 호출되어 플레이어와 AI 사이의 거리를 계산하고 Blackboard에 저장.
	virtual void TickNode(
		UBehaviorTreeComponent& OwnerComp,  // Behavior Tree를 실행하는 컴포넌트
		uint8* NodeMemory,                  // 이 서비스가 사용하는 메모리
		float DeltaSeconds                   // 지난 프레임과의 시간 차이
	) override; 
};
