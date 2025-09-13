// Fill out your copyright notice in the Description page of Project Settings.
// enemy.h
// 적 캐릭터를 위한 인터페이스. 
// Lockon 시스템에서 대상 선택 및 해제 이벤트를 구현할 수 있도록 제공

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "enemy.generated.h"

// 인터페이스 정의: Uenemy
// 이 클래스는 직접 수정할 필요가 없으며, 적 캐릭터에서 구현될 인터페이스임
UINTERFACE(MinimalAPI)
class Uenemy : public UInterface
{
	GENERATED_BODY()
};

/**
 * Ienemy
 * 적 캐릭터가 구현해야 하는 인터페이스 함수
 * Lockon 시 선택/해제 이벤트를 블루프린트에서 구현 가능
 */
class SOULDEMO_API Ienemy
{
	GENERATED_BODY()

public:
	// Lockon 대상이 선택되었을 때 호출되는 이벤트
	UFUNCTION(BlueprintImplementableEvent)
	void OnSelect();

	// Lockon 대상이 해제되었을 때 호출되는 이벤트
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeselect();
};
