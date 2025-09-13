// Fill out your copyright notice in the Description page of Project Settings.
// EnemyProjectileComponent.h
// 적이 발사하는 투사체를 스폰하는 컴포넌트 클래스입니다.
// 지정된 소켓 위치에서 플레이어를 향해 투사체를 생성합니다.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyProjectileComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDEMO_API UEnemyProjectileComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// 기본 생성자: 컴포넌트 초기 설정
	UEnemyProjectileComponent();

protected:
	// 게임 시작 시 호출되는 초기화 함수
	virtual void BeginPlay() override;

public:	
	// 매 프레임 호출되는 함수
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief 투사체 생성 함수
	 * @param ComponentName : 투사체를 생성할 소켓 이름
	 * @param ProjectileClass : 생성할 투사체 클래스
	 * 지정한 소켓 위치에서 플레이어를 바라보며 투사체를 스폰합니다.
	 */
	UFUNCTION(BlueprintCallable)
	void SpwanProjectile(FName ComponentName, TSubclassOf<AActor> ProjectileClass);
		
};
