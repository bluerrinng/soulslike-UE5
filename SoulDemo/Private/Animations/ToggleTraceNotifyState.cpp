// Fill out your copyright notice in the Description page of Project Settings.

// ToggleTraceNotifyState.cpp
// 애니메이션 NotifyState 클래스
// 공격 애니메이션 구간 동안 TraceComponent를 활성화/비활성화하여
// 공격 충돌 처리와 로직을 제어

#include "Animations/ToggleTraceNotifyState.h"
#include "Components/TraceComponent.h"

// AnimNotifyState 시작 시 호출: 공격 상태 활성화
void UToggleTraceNotifyState::NotifyBegin(
    USkeletalMeshComponent* MeshComp, 
    UAnimSequenceBase* Animation, 
    float TotalDuration, 
    const FAnimNotifyEventReference& EventReference)
{
    // 메시 컴포넌트가 없으면 종료
    if (!MeshComp) { return; }

    // 메시 소유 액터 가져오기
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) { return; }

    // 액터에 붙어있는 TraceComponent 가져오기
    UTraceComponent* TraceComp = Owner->FindComponentByClass<UTraceComponent>();
    if (!IsValid(TraceComp)) { return; }

    // 공격 상태를 true로 설정 → 충돌 처리 등 공격 로직 활성화
    TraceComp->bIsAttacking = true;
}

// AnimNotifyState 끝날 때 호출: 공격 상태 비활성화
void UToggleTraceNotifyState::NotifyEnd(
    USkeletalMeshComponent* MeshComp, 
    UAnimSequenceBase* Animation, 
    const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp) { return; }

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) { return; }

    UTraceComponent* TraceComp = Owner->FindComponentByClass<UTraceComponent>();
    if (!IsValid(TraceComp)) { return; }

    // 공격 상태를 false로 설정 → 공격 로직 종료
    TraceComp->bIsAttacking = false;
}
