// Fill out your copyright notice in the Description page of Project Settings.
// Fighter.h
// 전투 캐릭터용 인터페이스. 
// 공격, 피해, 애니메이션 관련 정보를 제공하며 캐릭터에서 구현 가능

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Fighter.generated.h"

// 인터페이스 정의: UFighter
// 이 클래스는 직접 수정할 필요가 없으며, 전투 캐릭터에서 구현될 인터페이스임
UINTERFACE(MinimalAPI)
class UFighter : public UInterface
{
	GENERATED_BODY()
};

/**
 * IFighter
 * 전투 캐릭터가 구현해야 하는 인터페이스 함수
 * 공격력, 공격 실행, 애니메이션 시간, 근접 공격 거리, 피해 가능 여부 제공
 */
class SOULDEMO_API IFighter
{
	GENERATED_BODY()

public:
	// 캐릭터의 공격력 반환
	virtual float GetDamage() { return 0.0f; }

	// 캐릭터 공격 실행
	virtual void Attack() { }

	// 공격 애니메이션 길이 반환
	virtual float GetAnimDuration() { return 0.0f; }

	// 근접 공격 범위 반환
	virtual float GetMeleeRange() { return 0.0f; }

	// 현재 캐릭터가 특정 상대에게 피해를 받을 수 있는지 반환
	virtual bool CanTakeDamage(AActor* Opponent) { return true; }
};
