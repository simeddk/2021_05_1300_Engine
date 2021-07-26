using UnrealBuildTool;

public class U03_Game : ModuleRules
{
	public U03_Game(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});

		PublicIncludePaths.Add(ModuleDirectory);
		//PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
