include("../_Config/Define")

-- Custom variables
local workspaceName = "Framework"

-- Workspace configuration:
workspace (workspaceName)
	startproject "Sample"
	-- Place where build files will be generated
	location "../build"
	-- Available configurations and platforms
	configurations { "Debug", "Release" }
	platforms { "x64" }
	systemversion "latest"
	architecture "x86_64"

	-- Setup configurations
	filter "configurations:Debug"
		symbols "On"
	filter "configurations:Release"
		optimize "On"

	-- Add projects
	include(g_LibraryConfig.projects.Framework)
	include("Sample/premake5.lua")