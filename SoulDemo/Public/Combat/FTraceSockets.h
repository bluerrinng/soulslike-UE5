// Fill out your copyright notice in the Description page of Project Settings.
// FTraceSockets.h
// 공격 트레이스에서 사용되는 소켓 정보를 담는 구조체
// Start, End 소켓과 회전 정보를 지정하여 공격 범위를 정의합니다.

#pragma once

#include "CoreMinimal.h"
#include "FTraceSockets.generated.h"

/**
 * 공격 시 사용되는 트레이스 소켓 정보를 담는 구조체
 * Start: 트레이스 시작 소켓 이름
 * End: 트레이스 종료 소켓 이름
 * Rotation: 트레이스 박스의 회전 소켓 이름
 */
USTRUCT(BlueprintType)
struct SOULDEMO_API FTraceSockets
{
	GENERATED_BODY();

	// 트레이스 시작 위치 소켓 이름
	UPROPERTY(EditAnywhere)
	FName Start;

	// 트레이스 종료 위치 소켓 이름
	UPROPERTY(EditAnywhere)
	FName End;

	// 트레이스 박스 회전 정보를 가져올 소켓 이름
	UPROPERTY(EditAnywhere)
	FName Rotation;
};
