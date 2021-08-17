#include "Example.h"
#include "GameplayDebugger.h"
#include "LevelEditor.h"
#include "DebuggerCategory/CGameplayDebuggerCategory.h"
#include "ToolbarCommand/CToolbarCommand.h"

#define LOCTEXT_NAMESPACE "FExampleModule"

void FExampleModule::StartupModule()
{

	//DebuggerCategory
	{
		IGameplayDebugger& gameplayDebugger = IGameplayDebugger::Get();

		IGameplayDebugger::FOnGetCategory category =
			IGameplayDebugger::FOnGetCategory::CreateStatic(&CGameplayDebuggerCategory::MakeInstance);

		gameplayDebugger.RegisterCategory
		(
			"ExampleCategory",
			category,
			EGameplayDebuggerCategoryState::EnabledInGameAndSimulate,
			5
		);
		gameplayDebugger.NotifyCategoriesChanged();
	}

	//Toolbar
	{
		CToolbarCommand::Register();
		ToolbarExtender = MakeShareable(new FExtender());
		
		Extension = ToolbarExtender->AddToolBarExtension
		(
			"Compile",
			EExtensionHook::After,
			nullptr,
			FToolBarExtensionDelegate::CreateRaw(this, &FExampleModule::AddToolbarExtension)
		);

		FLevelEditorModule& levelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		levelEditor.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FExampleModule::ShutdownModule()
{
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger& gameplayDebugger = IGameplayDebugger::Get();
		gameplayDebugger.UnregisterCategory("ExampleCategory");
	}

	ToolbarExtender->RemoveExtension(Extension.ToSharedRef());
	ToolbarExtender.Reset();
}

void FExampleModule::AddToolbarExtension(FToolBarBuilder& InBuilder)
{
	FSlateIcon icon = FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.SelectMode");

	InBuilder.AddToolBarButton
	(
		CToolbarCommand::Get().Button,
		NAME_None,
		FText::FromString("Hello"),
		FText::FromString("Hi"),
		icon
	);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FExampleModule, Example)