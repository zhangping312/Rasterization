include("../_Config/Define")

-- Custom variables
local workspaceName = "Rasterization"

-- Workspace configuration:
workspace (workspaceName)
	startproject "Rasterization"
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
	include("Rasterization/premake5.lua")