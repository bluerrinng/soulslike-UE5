// Fill out your copyright notice in the Description page of Project Settings.
// BlockComponent.h
// 캐릭터의 블록 기능을 담당하는 컴포넌트입니다.
// 스태미나를 소모하여 공격을 막고, 블록 애니메이션을 재생하며,
// 블록 성공 시 델리게이트를 통해 이벤트를 브로드캐스트합니다.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlockComponent.generated.h"

// 블록 이벤트 델리게이트, 블록 시 소모되는 스태미나를 전달
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnBlockSignature,
	UBlockComponent,
	OnBlockDelegate,
	float,
	Cost
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDEMO_API UBlockComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// 블록 시 소모되는 스태미나
	UPROPERTY(EditAnywhere)
	float StaminaCost = 10.0f;

	// 블록 애니메이션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* BlockAnimMontage;

public:	
	// Sets default values for this component's properties
	UBlockComponent();

	// 블록 성공 시 브로드캐스트되는 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnBlockSignature OnBlockDelegate;

protected:
	// 게임 시작 시 호출되는 초기화 함수
	virtual void BeginPlay() override;

public:	
	// 매 프레임 호출되는 함수
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief 블록 체크 함수
	 * @param Opponent 상대방 액터
	 * @return true면 블록 실패, false면 블록 성공
	 * 상대방과의 방향, 스태미나 여부를 체크하고 성공 시 애니메이션 재생 후 델리게이트 브로드캐스트
	 */
	bool Check(AActor* Opponent);
};
