include("../Define")

-- Custom variables
local projectName = "imgui"
local projectPath = GetThirdLibraryPath() .. projectName .. "/"

InitLibrary(projectName)
-- Setup include path
SetupLibrary(g_LibraryConfig.includes.imgui, { projectPath })
-- Setup library path
SetupLibrary(g_LibraryConfig.librarys.imgui, { GetLibraryTargetPath(projectName) })
-- Setup library name
SetupLibrary(g_LibraryConfig.names.imgui, { projectName })

----------------------- Local -----------------------

-- files
local l_files = {}
SetupLibrary(l_files,
{
	projectPath .. "imconfig.h",
	projectPath .. "imgui.cpp",
	projectPath .. "imgui.h",
	projectPath .. "imgui_demo.cpp",
	projectPath .. "imgui_draw.cpp",
	projectPath .. "imgui_internal.h",
	projectPath .. "imgui_tables.cpp",
	projectPath .. "imgui_widgets.cpp",
	projectPath .. "imstb_rectpack.h",
	projectPath .. "imstb_textedit.h",
	projectPath .. "imstb_truetype.h"
})
----------------------- Local -----------------------

-- Project configuration:
project (projectName)
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	-- Place where build files will be generated
	location ("%{wks.location}/%{prj.name}")
	-- Place where compiled binary target
	targetdir (GetLibraryTargetPath(projectName))
	-- Place where object and other intermediate files
	objdir (GetLibraryObjPath(projectName))
	-- Set working directory(Don't support on mac, need to edit scheme)
	debugdir (projectPath)

	-- Specify script files for the project
	files (l_files)
	sysincludedirs (g_LibraryConfig.includes.imgui)