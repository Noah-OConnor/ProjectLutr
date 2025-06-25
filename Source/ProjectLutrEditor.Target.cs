// Copyright 2020 Dan Kestranek.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectLutrEditorTarget : TargetRules
{
	public ProjectLutrEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("ProjectLutr");
	}
}
