#include "CSetViewTarget.h"
#include "Global.h"
#include "Camera/CameraActor.h"

ACSetViewTarget::ACSetViewTarget()
{

}

void ACSetViewTarget::BeginPlay()
{
	Super::BeginPlay();
	
	UKismetSystemLibrary::K2_SetTimer(this, "Change", 2.0f, true);
}

void ACSetViewTarget::Change()
{
	//CameraManager
	{
		/*APlayerCameraManager* manager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		CheckNull(manager);

		manager->SetViewTarget(Cameras[Index]);
		Index++;
		Index %= Cameras.Num();*/
	}

	//Controller->SetViewTargetWithBlend
	CheckFalse(bChangeCamera);
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(controller);
	CheckFalse(Cameras.Num() > 0);

	controller->SetViewTargetWithBlend(Cameras[Index], 2.0f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.0f);
	Index++;
	Index %= Cameras.Num();
}

