// Fill out your copyright notice in the Description page of Project Settings.
// TraceComponent.h
// 플레이어 캐릭터의 공격 시, 특정 소켓 위치를 기반으로 Box Trace를 수행하여 적에게 피해를 주는 컴포넌트

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/FTraceSockets.h"
#include "TraceComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDEMO_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

	// 공격 시 Trace를 수행할 SkeletalMeshComponent 참조
	USkeletalMeshComponent* SkeletalComp;

	// 공격할 때 사용할 소켓 정보 배열 (시작, 끝, 회전)
	UPROPERTY(EditAnywhere)
	TArray<FTraceSockets> Sockets;

	// Box Trace 크기
	UPROPERTY(EditAnywhere)
	double BoxCollisionLength = 30.0;

	// 디버그 모드 활성화 여부 (Trace 결과 시각화)
	UPROPERTY(EditAnywhere)
	bool bDebugMode{ false };

	// 이미 공격한 대상 Actor를 저장하여 중복 공격 방지
	TArray<AActor*> TargetsToIgnore;

	// 공격 히트 이펙트
	UPROPERTY(EditAnywhere)
	UParticleSystem* HitParticleTemplate;

public:	
	// Sets default values for this component's properties
	UTraceComponent();

	// 현재 공격 중인지 여부
	UPROPERTY(VisibleAnywhere)
	bool bIsAttacking{ false };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame, 공격 상태일 경우 Box Trace 수행
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 공격 후 TargetsToIgnore 초기화
	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();		
};
