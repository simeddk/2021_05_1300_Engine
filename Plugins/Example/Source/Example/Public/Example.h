#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"

class FExampleModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void AddToolbarExtension(class FToolBarBuilder& InBuilder);

private:
	TSharedPtr<FExtender> ToolbarExtender;
	TSharedPtr<FSlateStyleSet> StyleSet;
	TSharedPtr<const FExtensionBase> Extension;
};
