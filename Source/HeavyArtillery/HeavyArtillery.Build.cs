// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HeavyArtillery : ModuleRules
{
	public HeavyArtillery(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayTasks", "HeadMountedDisplay", "UMG", "HTTP", "Json" });
	}
}
