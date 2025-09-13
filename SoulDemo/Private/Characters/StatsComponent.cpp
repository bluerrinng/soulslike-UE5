// Fill out your copyright notice in the Description page of Project Settings.
//StatsComponent.cpp
// 캐릭터 스텟 관리: Health, Stamina, Strength 등
#include "Characters/StatsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/Fighter.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
    // 매 프레임 Tick() 호출 가능
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UStatsComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // 매 프레임 필요한 상태 갱신 가능
}

// 체력 감소 처리
void UStatsComponent::ReduceHealth(float Amount, AActor* Opponent)
{
    // 이미 체력이 0이면 무시
    if (Stats[EStat::Health] <= 0) return;

    IFighter* FighterRef = GetOwner<IFighter>();

    // 공격자가 공격 가능 상태인지 확인
    if (!FighterRef->CanTakeDamage(Opponent)) return;

    // 체력 감소
    Stats[EStat::Health] -= Amount;

    // 체력 범위 제한 (0 ~ MaxHealth)
    Stats[EStat::Health] = UKismetMathLibrary::FClamp(
        Stats[EStat::Health], 
        0,
        Stats[EStat::MaxHealth]
    );

    // 체력 UI 업데이트
    OnHealthPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Health, EStat::MaxHealth));

    // 체력이 0이 되면 사망 이벤트 브로드캐스트
    if (Stats[EStat::Health] <= 0)
    {
        OnZeroHealthDelegate.Broadcast();
    }
}

// 스태미나 감소 처리
void UStatsComponent::ReduceStamina(float Amount)
{
    Stats[EStat::Stamina] -= Amount;

    // 스태미나 범위 제한 (0 ~ MaxStamina)
    Stats[EStat::Stamina] = UKismetMathLibrary::FClamp(
        Stats[EStat::Stamina],
        0,
        Stats[EStat::MaxStamina]
    );

    bCanRegen = false; // 감소 직후 재생성 금지

    // 일정 시간 후 스태미나 재생성 가능하게 설정
    FLatentActionInfo FunctionInfo{
        0,
        100,
        TEXT("EnableRegen"),
        this
    };
    UKismetSystemLibrary::RetriggerableDelay(
        GetWorld(),
        StaminaDelayDuration,
        FunctionInfo
    );

    // 스태미나 UI 업데이트
    OnStaminaPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Stamina, EStat::MaxStamina));
}

// 스태미나 재생성 처리
void UStatsComponent::RegenStamina()
{
    if (!bCanRegen) return;

    // 스태미나 점진적 증가
    Stats[EStat::Stamina] = UKismetMathLibrary::FInterpTo_Constant(
        Stats[EStat::Stamina],
        Stats[EStat::MaxStamina],
        GetWorld()->DeltaTimeSeconds,
        StaminaRegenRate
    );

    // UI 업데이트
    OnStaminaPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Stamina, EStat::MaxStamina));
}

// 재생성 가능 상태 활성화
void UStatsComponent::EnableRegen()
{
    bCanRegen = true;
}

// 현재 값의 비율 계산 (예: 체력 %)
float UStatsComponent::GetStatPercentage(EStat Current, EStat Max)
{
    return Stats[Current] / Stats[Max];
}
