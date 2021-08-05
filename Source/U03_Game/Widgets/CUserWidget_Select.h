// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Select.generated.h"

UCLASS()
class U03_GAME_API UCUserWidget_Select : public UUserWidget
{
	GENERATED_BODY()

public:
	void Clicked(FString InName);
	void Hover(FString InName);
	void Unhover(FString InName);
	
};
