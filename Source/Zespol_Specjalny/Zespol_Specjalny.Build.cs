// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Zespol_Specjalny : ModuleRules
{
	public Zespol_Specjalny(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Zespol_Specjalny",
			"Zespol_Specjalny/Variant_Strategy",
			"Zespol_Specjalny/Variant_Strategy/UI",
			"Zespol_Specjalny/Variant_TwinStick",
			"Zespol_Specjalny/Variant_TwinStick/AI",
			"Zespol_Specjalny/Variant_TwinStick/Gameplay",
			"Zespol_Specjalny/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
