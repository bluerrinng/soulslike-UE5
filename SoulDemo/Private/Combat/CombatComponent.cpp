// Fill out your copyright notice in the Description page of Project Settings.
//CombatComponent.cpp
// 플레이어나 AI 캐릭터의 공격 로직을 담당하는 컴포넌트
// 콤보 공격, 랜덤 공격, 스태미나 소모, 공격 쿨다운 관리
#include "Combat/CombatComponent.h"
#include "GameFrameWork/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/MainPlayer.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
    // 매 프레임 Tick() 호출 가능
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();

    // 소유 캐릭터 참조 저장
    CharacterRef = GetOwner<ACharacter>();
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// 플레이어용 콤보 공격 실행
void UCombatComponent::ComboAttack()
{
    // MainPlayer 인터페이스 체크 후 스태미나 확인
    if (CharacterRef->Implements<UMainPlayer>())
    {
        IMainPlayer* IPlayerRef{ Cast<IMainPlayer>(CharacterRef) };

        if (IPlayerRef && !IPlayerRef->HasEnoughStamina(StaminaCost))
        {
            return; // 스태미나 부족 시 공격 취소
        }
    }

    if (!bCanAttack) { return; } // 공격 쿨다운 중이면 취소
    bCanAttack = false;           // 공격 시작

    // 현재 콤보 단계에 맞는 애니메이션 재생
    CharacterRef->PlayAnimMontage(AttackAnimations[ComboCounter]);

    // 콤보 카운터 증가
    ComboCounter++;

    int MaxCombo{ AttackAnimations.Num() };

    // 콤보 순환 처리 (-1 ~ MaxCombo-1)
    ComboCounter = UKismetMathLibrary::Wrap(
        ComboCounter,
        -1,
        (MaxCombo - 1)
    );

    // 공격 수행 시 스태미나 소모 이벤트 브로드캐스트
    OnAttackPerformedDelegate.Broadcast(StaminaCost);
}

// 공격 상태 초기화 (쿨다운 끝났을 때 호출)
void UCombatComponent::HandleResetAttack()
{
    bCanAttack = true;
}

// 보스/AI용 랜덤 공격
void UCombatComponent::RandomAttack()
{
    // 랜덤 인덱스로 공격 애니메이션 선택
    int RandomIndex{
        FMath::RandRange(0, AttackAnimations.Num() - 1)
    };

    // 애니메이션 재생 후 길이를 AnimDuration에 저장
    AnimDuration = CharacterRef->PlayAnimMontage(AttackAnimations[RandomIndex]);
}
