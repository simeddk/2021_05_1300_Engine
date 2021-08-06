#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Select.generated.h"

UCLASS()
class U03_GAME_API UCUserWidget_Select : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE class UCUserWidget_SelectItem* GetItem(FString InName) { return Items[InName]; }

public:
	void Clicked(FString InName);
	void Hover(FString InName);
	void Unhover(FString InName);

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UGridPanel* Grid;

	UPROPERTY(BlueprintReadOnly)
		TMap<FString, class UCUserWidget_SelectItem*> Items;
};
