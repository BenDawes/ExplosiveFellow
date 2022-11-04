// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ExplosiveFellow : ModuleRules
{
	public ExplosiveFellow(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "GameplayAbilities", "GameplayTags", "GameplayTasks" });
    }
}
