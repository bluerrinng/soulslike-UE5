// Fill out your copyright notice in the Description page of Project Settings.
// CombatComponent.h
// 캐릭터의 공격 기능을 담당하는 컴포넌트입니다.
// 콤보 공격, 랜덤 공격 기능을 제공하며,
// 공격 시 스태미나 소모와 델리게이트 브로드캐스트를 처리합니다.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

// 공격 이벤트 델리게이트, 공격 시 소모되는 스태미나 또는 데미지 전달
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnAttackPerformedSignature,
	UCombatComponent, OnAttackPerformedDelegate,
	float, Amount
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDEMO_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// 공격 애니메이션 몽타주 배열 (콤보용)
	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> AttackAnimations;

	// 소유 캐릭터 참조
	ACharacter* CharacterRef;

	// 현재 콤보 단계
	UPROPERTY(VisibleAnywhere)
	int ComboCounter{ 0 };

	// 공격 가능 여부 플래그
	UPROPERTY(VisibleAnywhere)
	bool bCanAttack{ true };

	// 공격 시 소모되는 스태미나
	UPROPERTY(EditAnywhere)
	float StaminaCost{ 5.0f };

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	// 공격 수행 시 브로드캐스트되는 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnAttackPerformedSignature OnAttackPerformedDelegate;

	// 현재 재생 중인 애니메이션 길이
	float AnimDuration;

protected:
	// 게임 시작 시 호출되는 초기화 함수
	virtual void BeginPlay() override;

public:	
	// 매 프레임 호출되는 함수
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief 콤보 공격 수행 함수
	 * 콤보 단계에 맞는 애니메이션을 재생하고, 스태미나 체크 후 델리게이트 브로드캐스트
	 */
	UFUNCTION(BlueprintCallable)
	void ComboAttack();

	/**
	 * @brief 공격 상태 초기화 함수
	 * 공격 후 다음 공격을 위해 bCanAttack을 true로 설정
	 */
	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();

	/**
	 * @brief 랜덤 공격 함수
	 * 배열에서 무작위 애니메이션을 재생
	 */
	void RandomAttack();
};
