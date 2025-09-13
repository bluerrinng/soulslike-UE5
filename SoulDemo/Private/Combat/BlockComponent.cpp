// Fill out your copyright notice in the Description page of Project Settings.
// BlockComponent.cpp
// 플레이어 캐릭터의 방어(블록) 기능을 담당하는 컴포넌트
// 공격을 막을 수 있는지 판단하고, 블록 애니메이션 재생 및 스태미나 소모 처리
// ============================
#include "Combat/BlockComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"

// Sets default values for this component's properties
UBlockComponent::UBlockComponent()
{
    // 매 프레임 Tick() 호출 가능
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UBlockComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void UBlockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // 매 프레임 필요한 상태 갱신 가능
}

// 블록 가능 여부 체크
bool UBlockComponent::Check(AActor* Opponent)
{
    ACharacter* CharacterRef = GetOwner<ACharacter>();

    // MainPlayer 인터페이스가 없으면 블록 불가로 true 반환
    if (!CharacterRef->Implements<UMainPlayer>())
    {
        return true;
    }

    IMainPlayer* PlayerRef = Cast<IMainPlayer>(CharacterRef);

    // 공격자와 플레이어의 전방 벡터 계산
    FVector OpponentForward = Opponent->GetActorForwardVector();
    FVector PlayerForward = CharacterRef->GetActorForwardVector();

    // 두 벡터의 내적 계산 (정면 충돌 여부 판단)
    double Result = FVector::DotProduct(OpponentForward, PlayerForward);

    // 공격이 뒤에서 들어오거나 스태미나 부족 시 블록 실패(true 반환)
    if (Result > 0 || !PlayerRef->HasEnoughStamina(StaminaCost))
    {
        return true;
    }

    // 블록 애니메이션 재생
    CharacterRef->PlayAnimMontage(BlockAnimMontage);

    // 블록 사용 시 스태미나 소모 이벤트 브로드캐스트
    OnBlockDelegate.Broadcast(StaminaCost);

    // 블록 성공(false 반환)
    return false;
}
