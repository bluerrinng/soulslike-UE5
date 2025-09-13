// Fill out your copyright notice in the Description page of Project Settings.

// PlayerAnimInstance.cpp
// 플레이어 캐릭터의 애니메이션 인스턴스 클래스
// 이동 속도(CurrentSpeed), 전투 상태(bIsInCombat), 이동 방향(CurrentDirection) 등을 애니메이션 블루프린트와 연동하여 갱신

#include "Animations/PlayerAnimInstance.h"

// 플레이어의 이동 속도를 갱신하는 함수
void UPlayerAnimInstance::UpdateSpeed()
{
    // 현재 이 애니메이션 인스턴스를 소유하고 있는 Pawn 가져오기
    APawn* PawnRef{ TryGetPawnOwner() };

    if (!IsValid(PawnRef))
    {
        return;
    }

    // Pawn의 속도 벡터를 가져옴
    FVector Velocity{ PawnRef->GetVelocity() };

    // 속도 벡터의 길이(크기)를 구해 현재 속도 변수에 저장
    CurrentSpeed = static_cast<float>(Velocity.Length());
}

// 전투 상태 여부를 갱신하는 함수
void UPlayerAnimInstance::HandleUpdatedTarget(AActor* NewTargetActorRef)
{
    // 전투 대상이 유효하면 전투 상태로 간주
    bIsInCombat = IsValid(NewTargetActorRef);
}

// 플레이어의 이동 방향을 갱신하는 함수 (전투 상태일 때만 동작)
void UPlayerAnimInstance::UpdateDirection()
{
    APawn* PawnRef{ TryGetPawnOwner() };
    if (!IsValid(PawnRef)) { return; }

    // 전투 상태가 아니면 방향 계산을 하지 않음
    if (!bIsInCombat)
    {
        return;
    }

    // 속도 벡터와 현재 회전값을 기반으로 이동 방향 계산
    CurrentDirection = CalculateDirection(
        PawnRef->GetVelocity(), 
        PawnRef->GetActorRotation());
}
