// Fill out your copyright notice in the Description page of Project Settings.
//enemyProjectile.cpp
//보스/적 캐릭터가 사용하는 투사체 스폰 전용 컴포넌트
// 특정 위치에서 플레이어를 향해 투사체를 발사
#include "Combat/EnemyProjectileComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UEnemyProjectileComponent::UEnemyProjectileComponent()
{
    // 매 프레임 Tick() 호출 가능
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UEnemyProjectileComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void UEnemyProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// 지정된 위치에서 투사체 생성
void UEnemyProjectileComponent::SpwanProjectile(FName ComponentName, TSubclassOf<AActor> ProjectileClass)
{
    // 스폰 위치를 결정할 컴포넌트 가져오기
    USceneComponent* SpawnPointComp{ Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(ComponentName)) };

    FVector SpawnLocation{ SpawnPointComp->GetComponentLocation() };

    // 플레이어 위치 가져오기
    FVector PlayerLocation{ GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() };

    // 플레이어를 바라보는 회전 계산
    FRotator SpawnRotation{
        UKismetMathLibrary::FindLookAtRotation(SpawnLocation, PlayerLocation)
    };

    // 월드에 투사체 스폰
    GetWorld()->SpawnActor(ProjectileClass, &SpawnLocation, &SpawnRotation);
}
