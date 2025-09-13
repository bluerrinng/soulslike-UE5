// Fill out your copyright notice in the Description page of Project Settings.
// LockonComponent.h/cpp
// 이 컴포넌트는 플레이어 캐릭터가 주변 적을 자동으로 탐지하고 
// Lock-on 시스템을 통해 타겟을 추적하며 카메라와 캐릭터 회전을 제어합니다.

#include "Combat/LockonComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/enemy.h"

// Sets default values for this component's properties
ULockonComponent::ULockonComponent()
{
	// 매 프레임 Tick() 호출 가능
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	// 소유 캐릭터, 컨트롤러, 이동 컴포넌트, 스프링암 컴포넌트 참조
	OwnerRef = GetOwner<ACharacter>();
	Controller = GetWorld()->GetFirstPlayerController();
	MovementComp = OwnerRef->GetCharacterMovement();
	SpringArmComp = OwnerRef->FindComponentByClass<USpringArmComponent>();
}

// 타겟 검색 후 Lock-on 시작
void ULockonComponent::StartLockon(float Radius)
{
	FHitResult OutResult;
	FVector CurrentLocation{ OwnerRef->GetActorLocation() };

	// 지정한 반경의 구체 형태 충돌 검사
	FCollisionShape Sphere{ FCollisionShape::MakeSphere(Radius) };
	FCollisionQueryParams IgnoreParams{
		FName {TEXT("Ignore Collision Params")},
		false,
		OwnerRef
	};

	// 구체 충돌 검사 실행
	bool bHasFoundTarget{ GetWorld()->SweepSingleByChannel(
		OutResult,
		CurrentLocation,
		CurrentLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		Sphere,
		IgnoreParams
	) };

	// 타겟 없으면 종료
	if (!bHasFoundTarget) { return; }

	// 충돌한 오브젝트가 적이 아니면 종료
	if (!OutResult.GetActor()->Implements<Uenemy>()) { return; }

	// 현재 Lock-on 타겟 설정
	CurrentTargetActor = OutResult.GetActor();

	// Lock-on 시 카메라 및 캐릭터 회전 제어 설정
	Controller->SetIgnoreLookInput(true);
	MovementComp->bOrientRotationToMovement = false;
	MovementComp->bUseControllerDesiredRotation = true;

	// 카메라 높이 오프셋 적용
	SpringArmComp->TargetOffset = FVector{ 0.0,0.0,100.0 };

	// 타겟 선택 이벤트 호출
	Ienemy::Execute_OnSelect(CurrentTargetActor);
	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
}

// Lock-on 종료
void ULockonComponent::EndLockon()
{
	// 타겟 해제 이벤트 호출
	Ienemy::Execute_OnDeselect(CurrentTargetActor);

	// 타겟 초기화
	CurrentTargetActor = nullptr;

	// 캐릭터 이동/회전 원래대로 복원
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->bUseControllerDesiredRotation = false;
	SpringArmComp->TargetOffset = FVector::ZeroVector;

	// 카메라 입력 다시 활성화
	Controller->ResetIgnoreLookInput();

	// 이벤트 브로드캐스트
	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
}

// Lock-on 토글
void ULockonComponent::ToggleLockon(float Radius)
{
	if (IsValid(CurrentTargetActor))
	{
		// 이미 타겟이 있으면 종료
		EndLockon();
	}
	else
	{
		// 타겟 없으면 새로 검색
		StartLockon(Radius);
	}
}

// Called every frame
void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 타겟 없으면 아무것도 안함
	if (!IsValid(CurrentTargetActor)) { return; }

	FVector CurrentLocation{ OwnerRef->GetActorLocation() };
	FVector TargetLocation{ CurrentTargetActor->GetActorLocation() };

	// 캐릭터와 타겟 간 거리 계산
	double TargetDistance{ FVector::Distance(CurrentLocation, TargetLocation) };

	// 타겟과 거리가 일정 이상이면 Lock-on 해제
	if (TargetDistance >= BreakDistance)
	{
		EndLockon();
		return;
	}

	// 타겟 위치 약간 아래로 조정 (머리 아닌 몸 중앙 기준)
	TargetLocation.Z -= 125;

	// 캐릭터가 타겟을 바라보도록 회전 계산
	FRotator NewRotation{ UKismetMathLibrary::FindLookAtRotation(
		CurrentLocation, TargetLocation
	) };

	// 캐릭터 컨트롤러 회전 적용
	Controller->SetControlRotation(NewRotation);

	// 카메라 높이 유지
	SpringArmComp->TargetOffset = FVector{ 0.0, 0.0, 100.0 };
}
