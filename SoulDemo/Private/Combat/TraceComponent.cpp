// Fill out your copyright notice in the Description page of Project Settings.
// TraceComponent.h/cpp
// 이 컴포넌트는 캐릭터의 공격 시 소켓 위치를 기반으로 BoxTrace를 수행하여
// 공격 범위 내 적에게 데미지를 적용하고, 디버그 시 시각화합니다.

#include "Combat/TraceComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"
#include "Interfaces/Fighter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// 매 프레임 Tick() 호출 가능
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	// 소유 캐릭터의 SkeletalMeshComponent 참조
	SkeletalComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}

// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 공격 중이 아니면 Trace 실행 안함
	if (!bIsAttacking)
	{
		return;
	}

	TArray<FHitResult> AllResults; // 이번 프레임에 맞은 대상 저장

	// 각 소켓마다 Trace 수행
	for (const FTraceSockets Socket : Sockets)
	{
		FVector StartSocketLocation = SkeletalComp->GetSocketLocation(Socket.Start);
		FVector EndSocketLocation = SkeletalComp->GetSocketLocation(Socket.End);
		FQuat ShapeRotation = SkeletalComp->GetSocketQuaternion(Socket.Rotation);

		TArray<FHitResult> OutResults;

		// Box 크기 계산
		double WeaponDistance{ FVector::Distance(StartSocketLocation, EndSocketLocation) };
		FVector BoxHalfExtent{ BoxCollisionLength, BoxCollisionLength, WeaponDistance };
		BoxHalfExtent /= 2;

		FCollisionShape Box{ FCollisionShape::MakeBox(BoxHalfExtent) };

		FCollisionQueryParams IgnoreParams{
			FName {TEXT("Ignore Params")},
			false,
			GetOwner()
		};

		// BoxTrace 실행
		bool bHasFoundTargets{ GetWorld()->SweepMultiByChannel(
			OutResults,
			StartSocketLocation,
			EndSocketLocation,
			ShapeRotation,
			ECollisionChannel::ECC_GameTraceChannel1,
			Box,
			IgnoreParams
		) };

		// 맞은 결과 저장
		for (FHitResult Hit : OutResults)
		{
			AllResults.Add(Hit);
		}

		// 디버그 모드 시 Box 시각화
		if (bDebugMode)
		{
			FVector CenterPoint{
				UKismetMathLibrary::VLerp(StartSocketLocation, EndSocketLocation, 0.5f)
			};

			UKismetSystemLibrary::DrawDebugBox(
				GetWorld(),
				CenterPoint,
				Box.GetExtent(),
				bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
				ShapeRotation.Rotator(),
				1.0f, // 지속시간
				2.0f  // 두께
			);
		}
	}

	// 맞은 대상 없으면 종료
	if (AllResults.Num() == 0)
	{
		return;
	}

	// 공격자 데미지 계산
	float CharacterDamage{ 0.0f };
	IFighter* FighterRef{ Cast<IFighter>(GetOwner()) };
	if (FighterRef)
	{
		CharacterDamage = FighterRef->GetDamage();
	}

	UE_LOG(LogTemp, Warning, TEXT("Damage :%f"), CharacterDamage);

	FDamageEvent TargetAttackedEvent;

	// 맞은 대상에게 데미지 적용
	for (const FHitResult& Hit : AllResults)
	{
		AActor* TargetActor{ Hit.GetActor() };

		// 이미 맞은 대상이면 무시
		if (TargetsToIgnore.Contains(TargetActor))
		{
			continue;
		}

		TargetActor->TakeDamage(
			CharacterDamage,
			TargetAttackedEvent,
			GetOwner()->GetInstigatorController(),
			GetOwner()
		);

		TargetsToIgnore.AddUnique(TargetActor);

		// 히트 이펙트 생성
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticleTemplate,
			Hit.ImpactPoint
		);
	}
}

// 공격 종료 시 맞은 대상 초기화
void UTraceComponent::HandleResetAttack()
{
	TargetsToIgnore.Empty();
}
