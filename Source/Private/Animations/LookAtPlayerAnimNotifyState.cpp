// Fill out your copyright notice in the Description page of Project Settings.

// LookAtPlayerAnimNotifyState.cpp
// AnimNotifyState를 활용하여 특정 애니메이션 구간 동안 액터가 플레이어를 바라보게 만드는 기능을 담당

#include "Animations/LookAtPlayerAnimNotifyState.h"
#include "Characters/LookAtPlayerComponent.h"

// 애니메이션 재생 중 특정 구간에서 플레이어를 바라보게 만드는 AnimNotifyState
void ULookAtPlayerAnimNotifyState::NotifyBegin(
    USkeletalMeshComponent* MeshComp, 
    UAnimSequenceBase* Animation, 
    float TotalDuration, 
    const FAnimNotifyEventReference& EventReference)
{
    // 애니메이션이 적용된 스켈레탈 메시의 소유 액터를 가져옴
    AActor* OwnerRef{ MeshComp->GetOwner() };

    if (!IsValid(OwnerRef)) { return; }

    // 액터에 붙어있는 LookAtPlayerComponent를 가져옴
    ULookAtPlayerComponent* RotationComp{
        OwnerRef->FindComponentByClass<ULookAtPlayerComponent>() };

    if (!IsValid(RotationComp)) { return; }

    // 애니메이션 구간이 시작될 때 플레이어를 바라보도록 회전 허용
    RotationComp->bCanRotate = true;
}

// AnimNotifyState 구간이 끝날 때 호출됨
void ULookAtPlayerAnimNotifyState::NotifyEnd(
    USkeletalMeshComponent* MeshComp, 
    UAnimSequenceBase* Animation, 
    const FAnimNotifyEventReference& EventReference)
{
    AActor* OwnerRef{ MeshComp->GetOwner() };

    if (!IsValid(OwnerRef)) { return; }

    ULookAtPlayerComponent* RotationComp{
        OwnerRef->FindComponentByClass<ULookAtPlayerComponent>() };

    if (!IsValid(RotationComp)) { return; }

    // 애니메이션 구간이 끝나면 더 이상 플레이어를 바라보지 않도록 회전 비활성화
    RotationComp->bCanRotate = false;
}
