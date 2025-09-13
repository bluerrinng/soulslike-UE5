// Fill out your copyright notice in the Description page of Project Settings.
// LookAtPlayerAnimNotifyState.h
// 애니메이션 노티파이 상태 클래스.
// 특정 애니메이션 동안 캐릭터가 플레이어를 바라보도록 처리하는 기능을 제공합니다.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "LookAtPlayerAnimNotifyState.generated.h"

/**
 * ULookAtPlayerAnimNotifyState
 * 애니메이션이 시작될 때 캐릭터가 플레이어를 바라보게 하고,
 * 애니메이션이 끝날 때 원래 상태로 되돌리는 역할을 수행합니다.
 */
UCLASS()
class SOULDEMO_API ULookAtPlayerAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	// 애니메이션 노티파이가 시작될 때 호출
	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference
	) override;

	// 애니메이션 노티파이가 종료될 때 호출
	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;
};
