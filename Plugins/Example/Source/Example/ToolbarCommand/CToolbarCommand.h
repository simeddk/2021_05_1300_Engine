#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class EXAMPLE_API CToolbarCommand : public TCommands<CToolbarCommand>
{
public:
	CToolbarCommand();
	~CToolbarCommand();

public:
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> Button;
};
