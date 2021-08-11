#include "CCameraActor.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Camera/CameraComponent.h"
#include "CCameraSpline.h"

ACCameraActor::ACCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Camera, "Camera");
}



void ACCameraActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACCameraSpline::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		if (actor->IsA<ACCameraSpline>() && actor->GetName().Contains("BP_CCameraSpline"))
			Spline = Cast<ACCameraSpline>(actor);
	}

}

void ACCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Timeline.IsPlaying())
		Timeline.TickTimeline(DeltaTime);

}


void ACCameraActor::StartTimeline()
{
	FOnTimelineFloat process;
	process.BindUFunction(this, "OnProcess");

	FOnTimelineEvent finish;
	finish.BindUFunction(this, "OnEndProcess");

	Timeline = FTimeline();
	Timeline.AddInterpFloat(Spline->GetCurve(), process);
	Timeline.SetTimelineFinishedFunc(finish);
	Timeline.SetPlayRate(0.25f);
	Timeline.PlayFromStart();

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(controller);
	controller->SetViewTarget(this);
}

void ACCameraActor::OnProcess(float Output)
{
	USplineComponent* spline = Spline->GetSpline();
	float length = spline->GetSplineLength();

	FVector location = spline->GetLocationAtDistanceAlongSpline(Output  * length, ESplineCoordinateSpace::World);
	FRotator rotator = spline->GetRotationAtDistanceAlongSpline(Output  * length, ESplineCoordinateSpace::World);

	SetActorLocation(location);
	SetActorRotation(rotator);
}

void ACCameraActor::OnEndProcess()
{
	Timeline.Stop();

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(controller);

	controller->SetViewTarget(controller->GetPawn());
}



