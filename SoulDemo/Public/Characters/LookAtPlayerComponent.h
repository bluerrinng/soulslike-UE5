// Fill out your copyright notice in the Description page of Project Settings.
// LookAtPlayerComponent.h
// 캐릭터가 플레이어를 바라보도록 회전시키는 컴포넌트입니다.
// Tick 함수에서 매 프레임마다 플레이어 위치를 계산하여 회전을 업데이트합니다.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LookAtPlayerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOULDEMO_API ULookAtPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// 회전 속도 (도/초)
	UPROPERTY(EditAnywhere)
	float Speed{ 400.0f };

public:	
	// Sets default values for this component's properties
	ULookAtPlayerComponent();

	// 회전 가능 여부
	UPROPERTY(VisibleAnywhere)
	bool bCanRotate{ false };

protected:
	// 게임 시작 시 호출
	virtual void BeginPlay() override;

public:	
	// 매 프레임 호출, 플레이어 방향을 바라보도록 회전 계산
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
