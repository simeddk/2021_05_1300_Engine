#include "CAim.h"
#include "Global.h"
#include "CHUD.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"

UCAim::UCAim()
{
	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Actions/Curve_Aim.Curve_Aim'");
}

void UCAim::BeginPlay(ACharacter* InCharacter)
{
	OwnerCharacter = InCharacter;

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

	TimelineFloat.BindUFunction(this, "Zooming");
	Timeline.AddInterpFloat(Curve, TimelineFloat);
	Timeline.SetPlayRate(200);

	//UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<ACHUD>();
	Hud = OwnerCharacter->GetWorld()->GetFirstPlayerController()->GetHUD<ACHUD>();
}

void UCAim::Tick(float DeltaTime)
{
	Timeline.TickTimeline(DeltaTime);
}

void UCAim::On()
{
	CheckFalse(IsAvaliable());
	CheckTrue(bInZoom);
	bInZoom = true;

	Hud->EnableDraw();

	SpringArm->TargetArmLength = 100.0f;
	SpringArm->SocketOffset = FVector(0, 30, 10);
	SpringArm->bEnableCameraLag = false;

	//Camera->FieldOfView = 45;
	Timeline.PlayFromStart();
}

void UCAim::Off()
{
	CheckFalse(IsAvaliable());
	CheckFalse(bInZoom);
	bInZoom = false;

	Hud->DisableDraw();

	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SocketOffset = FVector(0, 0, 0);
	SpringArm->bEnableCameraLag = true;

	//Camera->FieldOfView = 90;
	Timeline.ReverseFromEnd();
}

void UCAim::Zooming(float Output)
{
	Camera->FieldOfView = Output;
}
