#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class EXAMPLE_API CCustomDetailPannel : public IDetailCustomization
{
public:
	CCustomDetailPannel();
	~CCustomDetailPannel();


public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

public:
	FReply OnClicked();

private:
	TArray<TWeakObjectPtr<UObject>> Objects;

};
