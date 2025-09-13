// Fill out your copyright notice in the Description page of Project Settings.
// ToggleTraceNotifyState.h
// 애니메이션 NotifyState 클래스.
// 공격 애니메이션 중 Trace(충돌 체크)를 켜고 끄는 역할을 합니다.
// NotifyBegin에서 트레이스 활성화, NotifyEnd에서 트레이스 비활성화 처리.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ToggleTraceNotifyState.generated.h"

/**
 * UToggleTraceNotifyState
 * 공격 애니메이션 시 특정 구간에서 트레이스를 활성화/비활성화하는 AnimNotifyState 클래스.
 * 주로 콤보 공격이나 범위 공격 시 캐릭터의 무기 충돌 판정을 켜고 끄는 데 사용됩니다.
 */
UCLASS()
class SOULDEMO_API UToggleTraceNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	// 애니메이션 Notify 시작 시 호출: 트레이스를 활성화
	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,          // 애니메이션이 재생되는 SkeletalMesh
		UAnimSequenceBase* Animation,              // 실행 중인 애니메이션
		float TotalDuration,                        // Notify 지속 시간
		const FAnimNotifyEventReference& EventReference // 이벤트 참조
	) override;

	// 애니메이션 Notify 종료 시 호출: 트레이스를 비활성화
	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference
	) override;
};
