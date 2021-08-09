#include "CCameraSpline.h"
#include "Global.h"
#include "Components/SplineComponent.h"

ACCameraSpline::ACCameraSpline()
{
	CHelpers::CreateComponent(this, &Spline, "Spline");
	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Cameras/Curve_Spline.Curve_Spline'");
}

