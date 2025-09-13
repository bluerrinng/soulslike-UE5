// Fill out your copyright notice in the Description page of Project Settings.

// BossAnimInstance.cpp
// 보스 캐릭터 전용 애니메이션 인스턴스 클래스
// 이동 속도(CurrentSpeed)를 계산하여 애니메이션 블루프린트에서 사용될 수 있도록 갱신함

#include "Animations/BossAnimInstance.h"

// 보스의 애니메이션 인스턴스에서 이동 속도를 갱신하는 함수
void UBossAnimInstance::UpdateSpeed()
{
    // 현재 이 애니메이션 인스턴스를 소유하고 있는 Pawn 객체를 가져옴
    APawn* PawnRef{ TryGetPawnOwner() };

    // Pawn이 유효하지 않으면(= 캐릭터가 존재하지 않으면) 함수 종료
    if (!IsValid(PawnRef))
    {
        return;
    }

    // Pawn의 현재 속도 벡터를 가져옴
    FVector Velocity{ PawnRef->GetVelocity() };

    // 속도 벡터의 길이(크기)를 구해서 CurrentSpeed 변수에 저장
    // → 이 값은 애니메이션 블루프린트에서 이동 상태 전환 등에 활용될 수 있음
    CurrentSpeed = static_cast<float>(Velocity.Length());
}
