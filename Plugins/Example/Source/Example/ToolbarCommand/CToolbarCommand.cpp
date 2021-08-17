#include "CToolbarCommand.h"

CToolbarCommand::CToolbarCommand()
	: TCommands
	(
		"NewButton",
		FText::FromString("New"),
		NAME_None,
		FEditorStyle::GetStyleSetName()
	)
{
}

CToolbarCommand::~CToolbarCommand()
{
}

void CToolbarCommand::RegisterCommands()
{
#define LOCTEXT_NAMESPACE "CToolbarCommand"
	UI_COMMAND
	(
		Button,
		"My Toolbar Name",
		"My Toolbar Buttom Command",
		EUserInterfaceActionType::Button,
		FInputGesture()
	);
#undef LOCTEXT_NAMESPACE
}
