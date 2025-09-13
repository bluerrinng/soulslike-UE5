// Fill out your copyright notice in the Description page of Project Settings.
// BossAnimInstance.h
// 보스 캐릭터 전용 애니메이션 인스턴스 클래스.
// 보스의 이동 속도(CurrentSpeed)와 공격 준비 상태(bIsCharging)를 관리합니다.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * UBossAnimInstance
 * 보스 캐릭터의 애니메이션 관련 변수와 함수를 관리.
 * - CurrentSpeed: 현재 이동 속도
 * - bIsCharging: 공격 준비 상태 여부
 * - UpdateSpeed(): 애니메이션 블루프린트에서 현재 속도를 업데이트
 */
UCLASS()
class SOULDEMO_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	// 캐릭터의 현재 이동 속도
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentSpeed{ 0.0f };

	// 이동 속도를 갱신하는 함수 (블루프린트 호출 가능)
	UFUNCTION(BlueprintCallable)
	void UpdateSpeed();

public:
	// 보스가 공격 준비(차징) 중인지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCharging{ false };
};
