// Fill out your copyright notice in the Description page of Project Settings.
// EnemyActor.h
// 적이 발사하는 투사체(Projectile)를 나타내는 액터 클래스입니다.
// 플레이어와 충돌 시 피해를 주고 파티클 이펙트를 재생하며 일정 시간 후 파괴됩니다.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyActor.generated.h"

UCLASS()
class SOULDEMO_API AEnemyActor : public AActor
{
	GENERATED_BODY()

protected:
	// 충돌 시 재생될 파티클 템플릿
	UPROPERTY(EditAnywhere)
	UParticleSystem* HitTemplate;

	// 투사체가 입히는 피해량
	UPROPERTY(EditAnywhere)
	float Damage{ 10.0f };
	
public:	
	// 기본 생성자: 액터 기본 설정
	AEnemyActor();

protected:
	// 게임 시작 시 또는 액터가 스폰될 때 호출되는 초기화 함수
	virtual void BeginPlay() override;

public:	
	// 매 프레임 호출되는 함수
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 다른 액터와 충돌 시 처리 함수
	 * 플레이어와 충돌하면 피해를 주고, 파티클 재생 후 일정 시간 뒤 파괴
	 */
	UFUNCTION(BlueprintCallable)
	void HandleBeginOverlap(AActor* OtherActor);

	/**
	 * @brief 투사체 제거 함수
	 * 일정 시간 후 Destroy() 호출
	 */
	UFUNCTION()
	void DestroyProjectile();
};
