// Fill out your copyright notice in the Description page of Project Settings.
// LookAtPlayerComponent.cpp
// 이 컴포넌트는 소유 액터가 플레이어를 바라보도록 회전시키는 기능을 담당합니다.
// AnimNotifyState와 함께 사용하면 애니메이션 재생 중 특정 구간에서 플레이어를 바라보게 할 수 있습니다.
#include "Characters/LookAtPlayerComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULookAtPlayerComponent::ULookAtPlayerComponent()
{
    // 게임 시작 시 초기화되고, 매 프레임 Tick() 호출 가능
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void ULookAtPlayerComponent::BeginPlay()
{
    Super::BeginPlay();
    // 초기화 로직 필요 시 여기에 추가
}

// Called every frame
void ULookAtPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 회전 가능 상태가 아니면 종료
    if (!bCanRotate)
    {
        return;
    }

    // 컴포넌트 소유자 위치
    AActor* OwnerRef{ GetOwner() };
    FVector OwnerLocation{ OwnerRef->GetActorLocation() };

    // 플레이어 위치 가져오기
    APlayerController* PlayerController{ GetWorld()->GetFirstPlayerController() };
    APawn* PlayerPawn{ PlayerController->GetPawn() };
    FVector PlayerLocation{ PlayerPawn->GetActorLocation() };

    // 플레이어를 바라보는 회전 계산
    FRotator DesiredRotation{
        UKismetMathLibrary::FindLookAtRotation(OwnerLocation, PlayerLocation)
    };

    FRotator CurrentRotation{ OwnerRef->GetActorRotation() };

    // 부드럽게 회전 (Yaw 포함)
    FRotator NewRotation{
        UKismetMathLibrary::RInterpTo_Constant(
            CurrentRotation,
            DesiredRotation,
            DeltaTime,
            Speed
        )
    };

    // Yaw만 적용하고 Pitch, Roll은 유지
    FRotator NewYawOnlyRotation{
        CurrentRotation.Pitch,
        NewRotation.Yaw,
        CurrentRotation.Roll
    };

    // 소유자 회전 적용
    OwnerRef->SetActorRotation(NewYawOnlyRotation);
}
