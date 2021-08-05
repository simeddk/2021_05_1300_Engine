#include "CUserWidget_Select.h"
#include "Global.h"

void UCUserWidget_Select::Clicked(FString InName)
{
	CLog::Log(InName + FString(" is Clicked"));
}

void UCUserWidget_Select::Hover(FString InName)
{
	CLog::Log(InName + FString(" is Hovered"));
}

void UCUserWidget_Select::Unhover(FString InName)
{
	CLog::Log(InName + FString(" is Unhovered"));
}