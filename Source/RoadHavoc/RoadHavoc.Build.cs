using UnrealBuildTool;
public class RoadHavoc:ModuleRules{
 public RoadHavoc(ReadOnlyTargetRules Target):base(Target){
 PCHUsage=PCHUsageMode.UseExplicitOrSharedPCHs;
 PublicDependencyModuleNames.AddRange(new string[]{"Core","CoreUObject","Engine","InputCore","EnhancedInput","ChaosVehicles","UMG","Niagara"});
 }}