-- Custom variables
local projectName = "Rasterization"
local projectPath = GetCurrentDirectoryPath() .. "/"

----------------------- Local -----------------------

-- files
local l_files = {}
SetupLibrary(l_files, {  projectPath .. "Include/**.h", projectPath .. "Source/**.cpp" })

----------------------- Local -----------------------

-- Project configuration:
project (projectName)
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	-- Place where build files will be generated
	location ("%{wks.location}/%{prj.name}")
	-- Place where compiled binary target
	targetdir (GetEXETargetPath(projectName))
	-- Place where object and other intermediate files
	objdir (GetEXEObjPath(projectName))
	-- Set working directory(Don't support on mac, need to edit scheme)
	debugdir (projectPath)

	defines (g_LibraryConfig.defines.Framework)
	-- Specify script files for the project
	files (l_files)
	includedirs { projectPath .. "Include/" } -- Xcode: User Header Search Paths
	sysincludedirs (g_LibraryConfig.includes.Framework) --Xcode: Header Search Paths
	libdirs (g_LibraryConfig.librarys.Framework)
	links (g_LibraryConfig.names.Framework)
	prebuildcommands (g_LibraryConfig.prebuildcommands.Framework)
	linkoptions (g_LibraryConfig.linkoptions.Framework)

	-- Copy assets to EXE target path
	postbuildcommands
	{
		"{COPY} \"" .. projectPath .. "Assets\" \"%{cfg.buildtarget.directory}Assets\""
	}