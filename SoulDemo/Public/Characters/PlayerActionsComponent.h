// Fill out your copyright notice in the Description page of Project Settings.
// PlayerActionsComponent.h
// 플레이어의 이동 및 액션 관련 컴포넌트입니다.
// 스프린트, 걷기, 구르기(Roll) 등 플레이어의 액션 로직을 담당하며,
// 각 액션 발생 시 블루프린트와 연동 가능한 델리게이트를 제공합니다.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerActionsComponent.generated.h"

// 스프린트 액션 델리게이트 (Cost 전달)
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnSprintSignature,
	UPlayerActionsComponent, OnSprintDelegate,
	float, Cost
);

// 구르기 액션 델리게이트 (Cost 전달)
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnRollSignature,
	UPlayerActionsComponent,
	OnRollDelegate,
	float,
	Cost
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDEMO_API UPlayerActionsComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// 액션을 수행할 캐릭터 참조
	ACharacter* CharacterRef;

	// IMainPlayer 인터페이스 참조
	class IMainPlayer* IPlayerRef;

	// 캐릭터 무브먼트 컴포넌트 참조
	class UCharacterMovementComponent* MovementComp;

	// 스프린트 시 소모 스태미나
	UPROPERTY(EditAnywhere)
	float SprintCost{ 0.1f };

	// 스프린트 속도
	UPROPERTY(EditAnywhere)
	float SprintSpeed{ 1000.0f };

	// 일반 걷기 속도
	UPROPERTY(EditAnywhere)
	float WalkSpeed{ 500.0f };

	// 구르기 애니메이션
	UPROPERTY(EditAnywhere)
	UAnimMontage* RollAnimMontage;

	// 구르기 시 소모 스태미나
	UPROPERTY(EditAnywhere)
	float RollCost{ 5.0f };

public:	
	// Sets default values for this component's properties
	UPlayerActionsComponent();

	// 스프린트 발생 시 블루프린트에서 바인딩 가능한 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnSprintSignature OnSprintDelegate;

	// 구르기 발생 시 블루프린트에서 바인딩 가능한 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnRollSignature OnRollDelegate;

	// 구르기 액션이 활성화 상태인지 여부
	bool bIsRollActive = false;

protected:
	// 게임 시작 시 초기화
	virtual void BeginPlay() override;

public:	
	// 매 프레임 호출
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 스프린트 시작
	UFUNCTION(BlueprintCallable)
	void Sprint();

	// 걷기 속도로 이동
	UFUNCTION(BlueprintCallable)
	void Walk();

	// 구르기 액션
	UFUNCTION(BlueprintCallable)
	void Roll();

	// 구르기 애니메이션 종료 처리
	UFUNCTION(BlueprintCallable)
	void FinishRollAnim();
};
