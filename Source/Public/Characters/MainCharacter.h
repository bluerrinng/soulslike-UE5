// Fill out your copyright notice in the Description page of Project Settings.
// MainCharacter.h
// 플레이어 캐릭터 클래스입니다.
// IMainPlayer, IFighter 인터페이스를 구현하며, 스탯, 전투, 락온, 공격 트레이스, 블록, 플레이어 액션 컴포넌트를 포함합니다.
// 데미지 처리, 회복, 회전, 애니메이션 등 플레이어 관련 모든 로직의 중심이 되는 클래스입니다.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"
#include "Interfaces/Fighter.h"
#include "MainCharacter.generated.h"

UCLASS()
class SOULDEMO_API AMainCharacter : public ACharacter, public IMainPlayer, public IFighter
{
	GENERATED_BODY()

protected:
	// 사망 애니메이션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathAnimMontage;

	// 피격 애니메이션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* HurtAnimMontage;

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// 플레이어 스탯 관리 컴포넌트
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStatsComponent* StatsComp;

	// 락온 타겟팅 컴포넌트
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ULockonComponent* LockonComp;

	// 전투 공격 컴포넌트
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCombatComponent* CombatComp;

	// 공격 범위 및 히트 판정 트레이스 컴포넌트
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTraceComponent* TraceComp;

	// 블록(방어) 컴포넌트
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBlockComponent* BlockComp;

	// 점프, 달리기, 구르기 등 플레이어 액션 컴포넌트
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPlayerActionsComponent* PlayerActionsComp;

protected:
	// 게임 시작 시 호출
	virtual void BeginPlay() override;

	// 애니메이션 인스턴스 참조
	UPROPERTY(BlueprintReadOnly)
	class UPlayerAnimInstance* PlayerAnim;

public:	
	// 매 프레임 호출
	virtual void Tick(float DeltaTime) override;

	// 입력 바인딩
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IFighter 인터페이스: 공격 데미지 반환
	virtual float GetDamage() override;

	// IMainPlayer 인터페이스: 스태미나 충분 여부 확인
	virtual bool HasEnoughStamina(float Cost) override;

	// 플레이어 사망 처리
	UFUNCTION(BlueprintCallable)
	void HandleDeath();

	// 특정 액터와의 락온 종료
	virtual void EndLockonWithActor(AActor* ActorRef) override;

	// IFighter 인터페이스: 데미지를 받을 수 있는지 여부
	virtual bool CanTakeDamage(AActor* Opponent) override;

	// 피격 애니메이션 재생 및 카메라 흔들기 적용
	UFUNCTION(BlueprintCallable)
	void PlayHurtAnim(TSubclassOf<class UCameraShakeBase> CameraShakeTemplate);
};
