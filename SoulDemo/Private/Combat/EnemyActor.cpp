// Fill out your copyright notice in the Description page of Project Settings.
//EnemyActor.cpp
// 적 캐릭터가 발사하는 투사체(미사일, 화살 등)를 구현
// 플레이어와 충돌 시 피해 적용, 파티클 재생, 투사체 삭제 처리
#include "Combat/EnemyActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
    // 매 프레임 Tick() 호출 가능
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// 플레이어와 충돌했을 때 처리
void AEnemyActor::HandleBeginOverlap(AActor* OtherActor)
{
    // OtherActor가 Pawn인지 확인 후 캐스팅
    APawn* PawnRef{ Cast<APawn>(OtherActor) };

    // 플레이어가 아니면 무시
    if (!PawnRef->IsPlayerControlled())
    {
        return;
    }

    // 충돌 시 파티클 이펙트 재생
    FindComponentByClass<UParticleSystemComponent>()->SetTemplate(HitTemplate);

    // 투사체 이동 즉시 중지
    FindComponentByClass<UProjectileMovementComponent>()->StopMovementImmediately();

    // 투사체 삭제 타이머 설정 (0.5초 후)
    FTimerHandle DeathTimerHandle{};
    GetWorldTimerManager().SetTimer(
        DeathTimerHandle,
        this,
        &AEnemyActor::DestroyProjectile,
        0.5f
    );

    // 충돌 콜리전 비활성화
    FindComponentByClass<USphereComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 플레이어에게 피해 적용
    FDamageEvent ProjectileAttackEvent{};
    PawnRef->TakeDamage(
        Damage,                   // 데미지 값
        ProjectileAttackEvent,    // 데미지 이벤트
        PawnRef->GetController(), // 공격자 컨트롤러
        this                      // 데미지 근원 액터
    );
}

// 투사체 제거 함수
void AEnemyActor::DestroyProjectile()
{
    Destroy();
}
