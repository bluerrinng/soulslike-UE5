// Fill out your copyright notice in the Description page of Project Settings.
// BossCharacter.h
// 보스 캐릭터 클래스
// ABossCharacter는 AI 보스로, Ienemy 및 IFighter 인터페이스를 구현하며
// Behavior Tree 및 전투 시스템에서 사용됩니다.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/enemy.h"
#include "Characters/EEnemyState.h"
#include "Interfaces/Fighter.h"
#include "BossCharacter.generated.h"

UCLASS()
class SOULDEMO_API ABossCharacter : public ACharacter, public Ienemy, public IFighter
{
	GENERATED_BODY()

	// 초기 상태 (Behavior Tree 시작 시 적용)
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EEnemyState> InitialState;

	// AI용 블랙보드 컴포넌트
	class UBlackboardComponent* BlackboardComp;
	
	// 사망 애니메이션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathAnim;

	// AI Controller 참조
	class AAIController* ControllerRef;

public:
	// Sets default values for this character's properties
	ABossCharacter();
	
	// 보스의 스탯 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatsComponent* StatsComp;

	// 보스의 전투 컴포넌트
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UCombatComponent* CombatComp;

protected:
	// 게임 시작 또는 스폰 시 호출
	virtual void BeginPlay() override;

public:	
	// 매 프레임마다 호출
	virtual void Tick(float DeltaTime) override;

	// 입력 바인딩
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 특정 Pawn 감지 시 호출
	UFUNCTION(BlueprintCallable)
	void DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect);

	// IFighter 인터페이스: 공격력 반환
	virtual float GetDamage() override;

	// IFighter 인터페이스: 공격 수행
	virtual void Attack() override;

	// IFighter 인터페이스: 현재 애니메이션 길이 반환
	virtual float GetAnimDuration() override;

	// IFighter 인터페이스: 근접 공격 사거리 반환
	virtual float GetMeleeRange() override;

	// 플레이어 사망 시 처리
	UFUNCTION()
	void HandlePlayerDeath();

	// 보스 사망 처리
	UFUNCTION(BlueprintCallable)
	void HandleDeath();
	 
	// 사망 애니메이션 종료 후 처리
	UFUNCTION()
	void FinishDeathAnim();
};
