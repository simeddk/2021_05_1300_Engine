#include "CGameplayDebuggerCategory.h"

CGameplayDebuggerCategory::CGameplayDebuggerCategory()
{
	UE_LOG(LogTemp, Error, L"CGameplayDebuggerCategory Start")
}

CGameplayDebuggerCategory::~CGameplayDebuggerCategory()
{
	UE_LOG(LogTemp, Error, L"CGameplayDebuggerCategory End")
}

TSharedRef<FGameplayDebuggerCategory> CGameplayDebuggerCategory::MakeInstance()
{
	return MakeShareable(new CGameplayDebuggerCategory());
}

void CGameplayDebuggerCategory::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	Data.Actor = DebugActor;

	if (!!DebugActor)
		Data.Location = DebugActor->GetActorLocation();
}

void CGameplayDebuggerCategory::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{

	if (!!Data.Actor)
	{
		CanvasContext.Printf(FColor(255, 0, 0), L"Location : %s", *Data.Location.ToString());
		CanvasContext.Printf(FColor(0, 255, 0), L"Actor : %s", *Data.Actor->GetName());
		CanvasContext.Printf(FColor(0, 0, 255), L"Distance : %f", OwnerPC->GetPawn()->GetDistanceTo(Data.Actor));
	}


}
