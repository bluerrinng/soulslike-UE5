// Fill out your copyright notice in the Description page of Project Settings.
// PlayerAnimInstance.h
// 플레이어 캐릭터의 애니메이션 인스턴스 클래스.
// 캐릭터의 이동 속도, 방향, 전투 상태, 방어 상태 등을 애니메이션 블루프린트와 연동합니다.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * UPlayerAnimInstance
 * 플레이어 캐릭터의 애니메이션 상태를 관리하는 클래스.
 * 이동 속도(CurrentSpeed), 방향(CurrentDirection), 전투 상태(bIsInCombat),
 * 블록 상태(bIsBlocking) 등을 애니메이션 블루프린트와 연동합니다.
 */
UCLASS()
class SOULDEMO_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	// 캐릭터 현재 이동 속도
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentSpeed{ 0.0f };

	// 애니메이션 블루프린트에서 호출 가능, 이동 속도 갱신 함수
	UFUNCTION(BlueprintCallable)
	void UpdateSpeed();

	// 캐릭터 전투 상태 여부
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsInCombat{ false };

	// 캐릭터 이동 방향 (좌우 기준)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentDirection{ 0.0f };
	
public:
	// 캐릭터 방어 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBlocking = false;

	// 타겟이 변경되었을 때 호출되는 함수
	UFUNCTION(BlueprintCallable)
	void HandleUpdatedTarget(AActor* NewTargetActorRef);

	// 이동 방향 업데이트 함수
	UFUNCTION(BlueprintCallable)
	void UpdateDirection();
};
