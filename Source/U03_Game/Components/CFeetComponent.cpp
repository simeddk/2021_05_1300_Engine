#include "CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance;
	Trace(LeftSocket, leftDistance);

	float rightDistance;
	Trace(RightSocket, rightDistance);

	float offset = FMath::Min(leftDistance, rightDistance);

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, LerpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - offset), DeltaTime, LerpSpeed);
}

void UCFeetComponent::Trace(FName InSocket, float& OutDistance)
{
	OutDistance = 0.0f;

	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(InSocket);
	FVector start = FVector(location.X, location.Y, OwnerCharacter->GetActorLocation().Z);

	float traceZ = start.Z - CapsuleHalfHeight - TraceDistance;
	FVector end = FVector(location.X, location.Y, traceZ);

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start, end,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true,
		ignores,
		DrawDebugType,
		hitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	CheckFalse(hitResult.IsValidBlockingHit());

	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	OutDistance = AdjustDistance + length - TraceDistance;
}

