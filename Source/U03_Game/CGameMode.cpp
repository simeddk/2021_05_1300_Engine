#include "CGameMode.h"
#include "Global.h"
#include "GameFramework/HUD.h"

ACGameMode::ACGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");
	CHelpers::GetClass<AHUD>(&HUDClass, "Blueprint'/Game/Player/BP_CHUD.BP_CHUD_C'");
}

void ACGameMode::Tick(float DeltaSeconds)
{
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	if (controller->IsInputKeyDown(EKeys::R))
		GetWorld()->ServerTravel("/Game/Levels/TestMap");
	
}