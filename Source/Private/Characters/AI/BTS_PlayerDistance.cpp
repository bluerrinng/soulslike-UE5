// Fill out your copyright notice in the Description page of Project Settings.

// BTS_PlayerDistance.cpp
// Behavior Tree Service 클래스
// AI가 플레이어와의 거리를 지속적으로 계산하여 블랙보드에 업데이트
// → 다른 BT 노드들이 이 거리 값을 기반으로 행동 결정 가능

#include "Characters/AI/BTS_PlayerDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// AI가 플레이어와의 거리를 계속 계산하여 블랙보드에 업데이트하는 TickNode
void UBTS_PlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // AI 소유 Pawn의 현재 위치 가져오기
    FVector CurrentLocation{
        OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()
    };

    // 월드에서 플레이어 Pawn의 위치 가져오기 (첫 번째 플레이어 컨트롤러 기준)
    FVector PlayerLocation{ 
        GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() 
    };

    // AI와 플레이어 사이의 거리 계산
    float Distance{ static_cast<float>(FVector::Distance(CurrentLocation, PlayerLocation)) };

    // 계산된 거리를 블랙보드에 저장 → Behavior Tree 조건이나 서비스에서 사용 가능
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("Distance"), Distance);
}
