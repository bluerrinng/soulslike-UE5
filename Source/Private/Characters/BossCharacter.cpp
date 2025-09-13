// ABossCharacter.cpp
// 보스 캐릭터 클래스
// - 스텟 및 전투 컴포넌트 관리
// - Behavior Tree 기반 AI 상태 관리 (Idle, Range, Melee, Charge 등)
// - 플레이어 감지 및 공격 실행
// - 플레이어 사망 및 보스 사망 처리

#include "Characters/BossCharacter.h"
#include "Characters/StatsComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/CombatComponent.h"
#include "Characters/MainCharacter.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/MainPlayer.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // 스텟 컴포넌트 생성 (체력, 공격력, 근접 범위 등)
    StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));

    // 전투 컴포넌트 생성 (공격 처리, 애니메이션 등)
    CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
}

// 게임 시작 시 초기화
void ABossCharacter::BeginPlay()
{
    Super::BeginPlay();

    // AI Controller 가져오기
    ControllerRef = GetController<AAIController>();
    if (!ControllerRef)
    {
        UE_LOG(LogTemp, Error, TEXT("ControllerRef is null"));
        return;
    }

    // Blackboard 컴포넌트 가져오기
    BlackboardComp = ControllerRef->GetBlackboardComponent();
    if (!BlackboardComp)
    {
        UE_LOG(LogTemp, Error, TEXT("BlackboardComp is null"));
        return;
    }

    // 초기 상태 설정 (Idle 등)
    BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), InitialState);

    // 플레이어 참조 가져오기 및 체력 0 이벤트 바인딩
    AMainCharacter* Player = GetWorld()->GetFirstPlayerController()->GetPawn<AMainCharacter>();
    if (!Player || !Player->StatsComp)
    {
        UE_LOG(LogTemp, Error, TEXT("Player or StatsComp is null"));
        return;
    }

    // 플레이어 체력 0일 때 HandlePlayerDeath 호출
    Player->StatsComp->OnZeroHealthDelegate.AddDynamic(this, &ABossCharacter::HandlePlayerDeath);
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // 현재 Tick에서는 추가 로직 없음
}

// 보스는 플레이어 입력 필요 없음
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// 감지된 Pawn 처리
void ABossCharacter::DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect)
{
    EEnemyState CurrentState{
        static_cast<EEnemyState>(BlackboardComp->GetValueAsEnum(TEXT("CurrentState")))
    };

    // 감지한 Pawn이 플레이어가 아니거나 이미 공격 상태가 아니면 무시
    if (DetectedPawn != PawnToDetect || CurrentState != EEnemyState::Idle)
    {
        return;
    }

    // Idle 상태에서 플레이어 감지 시 원거리 공격 상태로 전환
    BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Range);
}

// 스텟 기반 공격력 반환
float ABossCharacter::GetDamage()
{
    return StatsComp->Stats[EStat::Strength];
}

// 전투 컴포넌트를 통해 랜덤 공격 실행
void ABossCharacter::Attack()
{
    CombatComp->RandomAttack();
}

// 공격 애니메이션 지속 시간 반환
float ABossCharacter::GetAnimDuration()
{
    return CombatComp->AnimDuration;
}

// 근접 공격 범위 반환
float ABossCharacter::GetMeleeRange()
{
    return StatsComp->Stats[EStat::MeleeRange];
}

// 플레이어 사망 시 처리
void ABossCharacter::HandlePlayerDeath()
{
    BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::GameOver);
}

// 보스 사망 처리
void ABossCharacter::HandleDeath()
{
    // 사망 애니메이션 재생
    float Duration = PlayAnimMontage(DeathAnim);

    // AI 동작 중지
    ControllerRef->GetBrainComponent()->StopLogic("defeated");

    // 충돌 제거
    FindComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    FTimerHandle DestroyTimerHandle;

    // 애니메이션 끝난 후 Destroy 호출
    GetWorld()->GetTimerManager().SetTimer(
        DestroyTimerHandle,
        this,
        &ABossCharacter::FinishDeathAnim,
        Duration,
        false
    );

    // 플레이어 락온 해제
    IMainPlayer* PlayerRef{
        GetWorld()->GetFirstPlayerController()->GetPawn<IMainPlayer>()
    };

    if (!PlayerRef)
    {
        return;
    }

    PlayerRef->EndLockonWithActor(this);
}

// 사망 애니메이션 종료 후 액터 제거
void ABossCharacter::FinishDeathAnim()
{
    Destroy();
}
