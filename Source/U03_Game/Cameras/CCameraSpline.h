#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCameraSpline.generated.h"

UCLASS()
class U03_GAME_API ACCameraSpline : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class UCurveFloat* Curve;

	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;

public:
	FORCEINLINE class UCurveFloat* GetCurve() { return Curve; }
	FORCEINLINE class USplineComponent* GetSpline() { return Spline; }

public:	
	ACCameraSpline();



};
