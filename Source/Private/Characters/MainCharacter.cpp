// Fill out your copyright notice in the Description page of Project Settings.
//MainCharacter.cpp
// 플레이어 캐릭터 클래스. 스텟, 전투, 행동, 락온 등 플레이어 관련 기능을 모두 관리.
#include "Characters/MainCharacter.h"
#include "Animations/PlayerAnimInstance.h"
#include "Characters/StatsComponent.h"
#include "Characters/EStat.h"
#include "Combat/LockonComponent.h"
#include "Combat/CombatComponent.h"
#include "Combat/TraceComponent.h"
#include "Combat/BlockComponent.h"
#include "Characters/PlayerActionsComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
    // 매 프레임 Tick() 호출 가능
    PrimaryActorTick.bCanEverTick = true;
    
    // 컴포넌트 생성
    StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component")); // 체력, 스태미나, 공격력 등
    LockonComp = CreateDefaultSubobject<ULockonComponent>(TEXT("Lockon Component")); // 락온 기능
    CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component")); // 공격 처리
    TraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("Trace Component")); // 공격 판정
    BlockComp = CreateDefaultSubobject<UBlockComponent>(TEXT("Block Component")); // 방어 판정
    PlayerActionsComp = CreateDefaultSubobject<UPlayerActionsComponent>(TEXT("PlayerActions Component")); // 구르기, 회피 등 행동 관리
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();

    // 애니메이션 인스턴스 가져오기
    PlayerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // 현재 Tick에서는 추가 로직 없음
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // 입력 바인딩 로직 필요 시 추가
}

// 캐릭터 공격력 반환
float AMainCharacter::GetDamage()
{
    return StatsComp->Stats[EStat::Strength];
}

// 스태미나가 충분한지 확인
bool AMainCharacter::HasEnoughStamina(float Cost)
{
    return StatsComp->Stats[EStat::Stamina] >= Cost;
}

// 캐릭터 사망 처리
void AMainCharacter::HandleDeath()
{
    PlayAnimMontage(DeathAnimMontage);
    DisableInput(GetController<APlayerController>()); // 플레이어 입력 비활성화
}

// 특정 액터와의 락온 종료
void AMainCharacter::EndLockonWithActor(AActor* ActorRef)
{
    if (LockonComp->CurrentTargetActor != ActorRef)
    {
        return;
    }
    LockonComp->EndLockon();
}

// 공격을 받을 수 있는지 판단
bool AMainCharacter::CanTakeDamage(AActor* Opponent)
{
    // 구르기 중이면 공격 무효
    if (PlayerActionsComp->bIsRollActive)
    {
        return false;
    }
    // 블록 중이면 BlockComp 검사
    if (PlayerAnim->bIsBlocking)
    {
        return BlockComp->Check(Opponent);
    }
    return true;
}

// 피해 애니메이션 재생 및 카메라 쉐이크
void AMainCharacter::PlayHurtAnim(TSubclassOf<class UCameraShakeBase> CameraShakeTemplate)
{
    PlayAnimMontage(HurtAnimMontage);

    if (CameraShakeTemplate)
    {
        GetController<APlayerController>()->ClientStartCameraShake(CameraShakeTemplate);
    }
}
