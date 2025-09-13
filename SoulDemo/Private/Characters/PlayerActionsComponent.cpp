// Fill out your copyright notice in the Description page of Project Settings.
//PlayerActionsComponent.cpp
// 플레이어의 이동, 스프린트, 구르기 등 행동 관리

#include "Characters/PlayerActionsComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPlayerActionsComponent::UPlayerActionsComponent()
{
    // 게임 시작 시 초기화되고, 매 프레임 Tick() 호출 가능
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPlayerActionsComponent::BeginPlay()
{
    Super::BeginPlay();
    
    CharacterRef = GetOwner<ACharacter>(); // 소유 캐릭터 참조
    MovementComp = CharacterRef->GetCharacterMovement(); // 이동 컴포넌트 참조

    // 캐릭터가 IMainPlayer 인터페이스를 구현하는지 확인
    if (!CharacterRef->Implements<UMainPlayer>())
    {
        return;
    }
    IPlayerRef = Cast<IMainPlayer>(CharacterRef); // 인터페이스 캐스팅
}

// Called every frame
void UPlayerActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // 매 프레임 필요한 행동 업데이트 가능
}

// 스프린트 처리
void UPlayerActionsComponent::Sprint()
{
    // 스태미나 부족 시 걷기
    if (!IPlayerRef->HasEnoughStamina(SprintCost))
    {
        Walk();
        return;
    }

    // 캐릭터가 움직이지 않으면 스프린트 X
    if (MovementComp->Velocity.Equals(FVector::ZeroVector,1))
    {
        return;
    }

    // 최대 이동 속도를 스프린트 속도로 변경
    MovementComp->MaxWalkSpeed = SprintSpeed;

    // 스프린트 시 이벤트 브로드캐스트
    OnSprintDelegate.Broadcast(SprintCost);
}

// 걷기 처리
void UPlayerActionsComponent::Walk()
{
    MovementComp->MaxWalkSpeed = WalkSpeed;
}

// 구르기 처리
void UPlayerActionsComponent::Roll()
{
    // 이미 구르기 중이거나 스태미나 부족 시 취소
    if (bIsRollActive || !IPlayerRef->HasEnoughStamina(RollCost))
    {
        return;
    }

    bIsRollActive = true;

    // 구르기 이벤트 브로드캐스트
    OnRollDelegate.Broadcast(RollCost);

    // 이동 방향 결정 (정지 시 앞 방향, 이동 중이면 이동 방향)
    FVector Direction = CharacterRef->GetCharacterMovement()->Velocity.Length() < 1 ?
        CharacterRef->GetActorForwardVector() : CharacterRef->GetLastMovementInputVector();

    FRotator NewRot{ UKismetMathLibrary::MakeRotFromX(Direction) };
    CharacterRef->SetActorRotation(NewRot); // 캐릭터 회전 적용

    // 구르기 애니메이션 재생
    float Duration{ CharacterRef->PlayAnimMontage(RollAnimMontage) };

    // 애니메이션 종료 후 구르기 상태 해제 타이머 설정
    FTimerHandle RollTimerHandle;
    CharacterRef->GetWorldTimerManager().SetTimer(
        RollTimerHandle,
        this,
        &UPlayerActionsComponent::FinishRollAnim,
        Duration,
        false
    );
}

// 구르기 상태 해제
void UPlayerActionsComponent::FinishRollAnim()
{
    bIsRollActive = false;
}
