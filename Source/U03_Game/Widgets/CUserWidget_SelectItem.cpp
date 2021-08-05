#include "CUserWidget_SelectItem.h"
#include "Global.h"
#include "Widgets/CUserWidget_Select.h"
#include "Characters/CPlayer.h"

void UCUserWidget_SelectItem::Click()
{
	GetSelectWidget()->Clicked(GetName());
}

void UCUserWidget_SelectItem::Hover()
{
	GetSelectWidget()->Hover(GetName());
}

void UCUserWidget_SelectItem::Unhover()
{
	GetSelectWidget()->Unhover(GetName());
}

UCUserWidget_Select* UCUserWidget_SelectItem::GetSelectWidget()
{
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	return player->GetSelectWidget();

}