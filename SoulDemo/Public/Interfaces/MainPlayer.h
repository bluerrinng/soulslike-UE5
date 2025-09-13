// Fill out your copyright notice in the Description page of Project Settings.
// MainPlayer.h
// 플레이어 캐릭터용 인터페이스
// 스태미나 소모 확인, 잠금 대상 해제 등 플레이어 관련 기능을 제공

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MainPlayer.generated.h"

// 인터페이스 정의: UMainPlayer
// 이 클래스는 직접 수정할 필요 없으며, 플레이어 캐릭터에서 구현될 인터페이스임
UINTERFACE(MinimalAPI)
class UMainPlayer : public UInterface
{
	GENERATED_BODY()
};

/**
 * IMainPlayer
 * 플레이어 캐릭터가 구현해야 하는 인터페이스 함수
 * 스태미나 체크, 잠금 대상 해제 기능 제공
 */
class SOULDEMO_API IMainPlayer
{
	GENERATED_BODY()

public:
	// 스태미나가 특정 비용만큼 충분한지 반환
	virtual bool HasEnoughStamina(float Cost)
	{
		return true;
	}

	// 특정 액터와의 잠금(Lockon) 해제
	virtual void EndLockonWithActor(AActor* ActorRef) {};
};
