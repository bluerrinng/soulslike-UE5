// Fill out your copyright notice in the Description page of Project Settings.
// StatsComponent.h
// 플레이어 및 적 캐릭터의 스탯 관리 컴포넌트입니다.
// Health, Stamina 등 게임 내 스탯과 관련된 로직을 담당하며,
// 스탯 변화 시 블루프린트에서 바인딩 가능한 델리게이트를 제공합니다.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/EStat.h"
#include "StatsComponent.generated.h"

// 체력 퍼센트 업데이트 시 델리게이트
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnHealthPercentUpdateSignature,
	UStatsComponent, OnHealthPercentUpdateDelegate,
	float, Percentage
);

// 스태미나 퍼센트 업데이트 시 델리게이트
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnStaminaPercentUpdateSignature,
	UStatsComponent, OnStaminaPercentUpdateDelegate,
	float, Percentage
);

// 체력이 0이 되었을 때 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnZeroHealthSignature,
	UStatsComponent, OnZeroHealthDelegate
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDEMO_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// 스태미나 재생 속도
	UPROPERTY(EditAnywhere)
	double StaminaRegenRate{ 10.0};

	// 스태미나 회복 가능 여부
	UPROPERTY(VisibleAnywhere)
	bool bCanRegen{ true };
	
	// 스태미나 회복 지연 시간
	UPROPERTY(EditAnywhere)
	float StaminaDelayDuration{ 2.0f };

public:	
	// Sets default values for this component's properties
	UStatsComponent();

	// 스탯 데이터 (Health, Stamina, Strength 등)
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EStat>, float> Stats;

	// 체력 퍼센트 변경 시 블루프린트에서 바인딩 가능한 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnHealthPercentUpdateSignature OnHealthPercentUpdateDelegate;

	// 스태미나 퍼센트 변경 시 블루프린트에서 바인딩 가능한 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnStaminaPercentUpdateSignature OnStaminaPercentUpdateDelegate;

	// 체력이 0이 되었을 때 블루프린트에서 바인딩 가능한 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnZeroHealthSignature OnZeroHealthDelegate;

protected:
	// 게임 시작 시 초기화
	virtual void BeginPlay() override;

public:	
	// 매 프레임 호출
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 체력 감소
	UFUNCTION(BlueprintCallable)
	void ReduceHealth(float Amount , AActor* Opponent);

	// 스태미나 감소
	UFUNCTION(BlueprintCallable)
	void ReduceStamina(float Amount);

	// 스태미나 재생
	UFUNCTION(BlueprintCallable)
	void RegenStamina();

	// 스태미나 회복 활성화
	UFUNCTION()
	void EnableRegen();

	// 현재 스탯을 퍼센트로 반환 (Current / Max)
	UFUNCTION(BlueprintPure)
	float GetStatPercentage(EStat Current, EStat Max);

};
