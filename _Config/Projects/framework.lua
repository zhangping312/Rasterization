include("../Define")

-- Custom variables
local projectName = "Framework"
local projectPath = GetRootPath() .. "/Framework/" .. projectName .. "/"

InitLibrary(projectName)
-- Setup define
local define = {}
if os.target() == "windows" 	then
	table.insert(define, "PLATFORM_WINDOWS")
	table.insert(define, "_CRT_SECURE_NO_WARNINGS")
elseif os.target() == "macosx" 	then table.insert(define, "PLATFORM_MAC")
end
SetupLibrary(g_LibraryConfig.defines.Framework, define)
-- Setup include path
SetupLibrary(g_LibraryConfig.includes.Framework, { projectPath .. "Include/" })
SetupLibrary(g_LibraryConfig.includes.Framework, g_LibraryConfig.includes.spdlog)
SetupLibrary(g_LibraryConfig.includes.Framework, g_LibraryConfig.includes.stb)
-- Setup library path
SetupLibrary(g_LibraryConfig.librarys.Framework, { GetLibraryTargetPath(projectName) })
-- Setup library name
SetupLibrary(g_LibraryConfig.names.Framework, { projectName })


include(g_LibraryConfig.projects.imgui)
SetupLibrary(g_LibraryConfig.includes.Framework, g_LibraryConfig.includes.SDL)
SetupLibrary(g_LibraryConfig.includes.Framework, g_LibraryConfig.includes.imgui)
SetupLibrary(g_LibraryConfig.librarys.Framework, g_LibraryConfig.librarys.SDL)
SetupLibrary(g_LibraryConfig.librarys.Framework, g_LibraryConfig.librarys.imgui)
SetupLibrary(g_LibraryConfig.names.Framework, g_LibraryConfig.names.SDL)
SetupLibrary(g_LibraryConfig.names.Framework, g_LibraryConfig.names.imgui)
if os.target() == "windows" then
	SetupLibrary(g_LibraryConfig.prebuildcommands.Framework, { "{COPY} \"%{g_LibraryConfig.librarys.SDL[1]}/*.dll\" \"%{cfg.buildtarget.directory}\"" })
elseif os.target() == "macosx" then
	SetupLibrary(g_LibraryConfig.linkoptions.Framework, { "-F'" .. tostring(g_LibraryConfig.librarys.SDL[1]) .. "'" })
	SetupLibrary(g_LibraryConfig.linkoptions.Framework, { "-Xlinker -rpath -Xlinker '" .. tostring(g_LibraryConfig.librarys.SDL[1]) .. "'" })
end

----------------------- Local -----------------------

-- files
local l_files = {}
SetupLibrary(l_files, { projectPath .. "Include/**.h", projectPath .. "Source/**.cpp" })

----------------------- Local -----------------------
-- Project configuration:
project (projectName)
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	characterset "MBCS"

	-- Place where build files will be generated
	location ("%{wks.location}/%{prj.name}")
	-- Place where compiled binary target
	targetdir (GetLibraryTargetPath(projectName))
	-- Place where object and other intermediate files
	objdir (GetLibraryObjPath(projectName))
	-- Set working directory(Don't support on mac, need to edit scheme)
	debugdir (projectPath)

	defines (g_LibraryConfig.defines.Framework)
	-- Specify script files for the project
	files (l_files)
	sysincludedirs (g_LibraryConfig.includes.Framework)
	libdirs (g_LibraryConfig.librarys.Framework)
	prebuildcommands (g_LibraryConfig.prebuildcommands.Framework)
	linkoptions (g_LibraryConfig.linkoptions.Framework)