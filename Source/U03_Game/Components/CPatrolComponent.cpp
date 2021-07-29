#include "CPatrolComponent.h"
#include "Global.h"
#include "Actions/CPatrolPath.h"
#include "Components/SplineComponent.h"

UCPatrolComponent::UCPatrolComponent()
{

}

void UCPatrolComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UCPatrolComponent::GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius)
{
	OutLocation = FVector::ZeroVector;
	OutAcceptanceRadius = AcceptanceRadius;
	CheckNullResult(Path, false);

	OutLocation = Path->GetSpline()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
	return true;
}

void UCPatrolComponent::UpdateNextIndex()
{
	CheckNull(Path);

	int32 count = Path->GetSpline()->GetNumberOfSplinePoints();

	//IsReverse
	if (bReverse)
	{
		if(Index > 0)
		{ 
			Index--;
			return;
		}
		
		//IsClosedLoop
		if (Path->GetSpline()->IsClosedLoop())
		{
			Index = count - 1;
			return;
		}

		//IsNotClosedLoop
		Index = 1;
		bReverse = false;
		return;
	}

	//IsFoward
	if (Index < count - 1)
	{
		Index++;
		return;
	}

	//IsClosedLoop
	if (Path->GetSpline()->IsClosedLoop())
	{
		Index = 0;
		return;
	}

	//IsNotClosedLoop
	Index = count - 2;
	bReverse = true;

}





