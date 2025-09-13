// Fill out your copyright notice in the Description page of Project Settings.
// LockonComponent.h
// 플레이어 캐릭터가 적을 대상으로 잠금(Lock-on) 기능을 구현하는 컴포넌트

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockonComponent.generated.h"

// 잠금 대상이 변경될 때 브로드캐스트되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnUpdatedTargetSignature, 
	ULockonComponent, OnUpdatedTargetDelegate,
	AActor*, NewTargetActorRef
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDEMO_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

	// 이 컴포넌트를 소유한 캐릭터
	ACharacter* OwnerRef;

	// 플레이어 컨트롤러
	APlayerController* Controller;

	// 캐릭터 이동 컴포넌트
	class UCharacterMovementComponent* MovementComp;

	// 카메라 스프링암 컴포넌트
	class USpringArmComponent* SpringArmComp;

public:	
	// Sets default values for this component's properties
	ULockonComponent();

	// 현재 잠금 대상 액터
	AActor* CurrentTargetActor;

	// 잠금 대상이 변경될 때 실행되는 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnUpdatedTargetSignature OnUpdatedTargetDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// 지정한 반경 내에서 잠금 대상 탐색 후 잠금 시작
	UFUNCTION(BlueprintCallable)
	void StartLockon(float Radius = 750.05f);

	// 잠금 토글: 잠금 상태이면 해제, 잠금 상태가 아니면 StartLockon 호출
	UFUNCTION(BlueprintCallable)
	void ToggleLockon(float Radius = 750.05f);

	// 잠금이 해제되는 최대 거리
	UPROPERTY(EditAnywhere)
	double BreakDistance{ 1000.0 };

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 현재 잠금 상태 종료
	void EndLockon();
};
