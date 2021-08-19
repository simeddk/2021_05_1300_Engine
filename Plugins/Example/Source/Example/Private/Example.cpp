#include "Example.h"
#include "GameplayDebugger.h"
#include "LevelEditor.h"
#include "DebuggerCategory/CGameplayDebuggerCategory.h"
#include "ToolbarCommand/CToolbarCommand.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "AssetToolsModule.h"
#include "DataAsset/CDataAssetAction.h"

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
		
		TSharedPtr<FUICommandList> commandList = MakeShareable(new FUICommandList());
		commandList->MapAction
		(
			CToolbarCommand::Get().Button,
			FExecuteAction::CreateRaw(this, &FExampleModule::ToolbarButton_Clicked),
			FCanExecuteAction()
		);

		Extension = ToolbarExtender->AddToolBarExtension
		(
			"Compile",
			EExtensionHook::After,
			commandList,
			FToolBarExtensionDelegate::CreateRaw(this, &FExampleModule::AddToolbarExtension)
		);

		FLevelEditorModule& levelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		levelEditor.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	//StyleSet
	{
		StyleSet = MakeShareable(new FSlateStyleSet("ExampleStyle"));

		FString path = IPluginManager::Get().FindPlugin("Example")->GetContentDir();
		StyleSet->SetContentRoot(path);

		FSlateImageBrush* brush = new FSlateImageBrush(path / L"Icon.png", FVector2D(48, 48));
		StyleSet->Set("Example.ToolbarIcon", brush);

		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
	}

	//AssetTool
	{
		IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		//EAssetTypeCategories::Type category = EAssetTypeCategories::Misc;
		EAssetTypeCategories::Type category = assetTools.RegisterAdvancedAssetCategory(FName(), FText::FromString("CustomCategory"));
		Action = MakeShareable(new CDataAssetAction(category));
		assetTools.RegisterAssetTypeActions(Action.ToSharedRef());
	}
}

void FExampleModule::ShutdownModule()
{
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger& gameplayDebugger = IGameplayDebugger::Get();
		gameplayDebugger.UnregisterCategory("ExampleCategory");
	}

	FSlateStyleRegistry::UnRegisterSlateStyle("ExampleStyle");
	StyleSet.Reset();

	ToolbarExtender->RemoveExtension(Extension.ToSharedRef());
	ToolbarExtender.Reset();

}

void FExampleModule::AddToolbarExtension(FToolBarBuilder& InBuilder)
{
	FSlateIcon icon = FSlateIcon("ExampleStyle", "Example.ToolbarIcon");

	InBuilder.AddToolBarButton
	(
		CToolbarCommand::Get().Button,
		NAME_None,
		FText::FromString("Hello"),
		FText::FromString("Hi"),
		icon
	);
}

void FExampleModule::ToolbarButton_Clicked()
{
	UE_LOG(LogTemp, Error, L"Hello Unreal~~~");
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FExampleModule, Example)