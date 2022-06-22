-- Get current directory path
function GetCurrentDirectoryPath()
	local currentDirectoryPath = os.getcwd()
	return currentDirectoryPath
end

function GetRootPath()
	if g_RootPath == nil then
		local currentDirectoryPath = GetCurrentDirectoryPath()
		local s, e = string.find(currentDirectoryPath, "Rasterization")
		assert(s ~= nil)
		g_RootPath = string.sub(currentDirectoryPath, 1, e)
	end

	return g_RootPath
end

-- Get build path
function GetBuildPath()
	return GetRootPath() .. "/build/"
end

-- Get _Third directory path
function GetThirdLibraryPath()
	return GetRootPath() .. "/_Third/"
end

-- Get compiled binary target path(exe)
function GetEXETargetPath(projectName)
	assert(projectName ~= nil)
	return GetRootPath() .. "/bin/" .. tostring(projectName)
end

-- Get object and other intermediate files path(exe)
function GetEXEObjPath(projectName)
	assert(projectName ~= nil)
	return GetRootPath() .. "/bin-int/" .. tostring(projectName)
end

-- Get compiled binary target path(statci library or dynamic library)
function GetLibraryTargetPath(projectName)
	assert(projectName ~= nil)
	return GetRootPath() .. "/bin/" .. tostring(projectName) .. "/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}"
end

-- Get object and other intermediate files path(statci library or dynamic library)
function GetLibraryObjPath(projectName)
	assert(projectName ~= nil)
	return GetRootPath() .. "/bin-int/" .. tostring(projectName) .. "/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}"
end

g_LibraryConfig =
{
	-- The paths of the premake5 config about Third library project:
	-- key - library name, value - premake5 config path
	projects = {},

	-- define: key - library name, value - the table that contain defines
	defines = {},
	-- Header search paths: key - library name, value - the table that contain include paths
	includes = {},
	-- Library search paths: key - library name, value - the table that contain library paths
	librarys = {},
	-- Link library name: key - library name, value - the table that contain library name
	names = {},
	-- Link options: key - library name, value - the table that contain link options
	links = {},
	-- Prebuild commands: key - library name, value - the table that contain prebuildcommands
	prebuildcommands = {},
	-- Link options: key - library name, value - the table that contain prebuildcommands
	linkoptions = {},
}

----------------------- Helper function -----------------------

local function CopyTable(dest, source)
	assert(type(dest) == "table" and type(source) == "table")
	for k, v in pairs(source) do
		if not IsInTable(dest, v) then table.insert(dest, v) end
	end
end

function InitLibrary(projectName)
	for k, v in pairs(g_LibraryConfig) do
		if k ~= "projects" then
			v[projectName] = {}
		end
	end
end

function SetupLibrary(tbl, value)
	CopyTable(tbl, value)
end

function IsInTable(tbl, value)
	assert(type(tbl) == "table")
	for k, v in pairs(tbl) do
		if value == v then return true end
	end
	return false
end

----------------------- Helper function -----------------------

----------------------- Third Library -----------------------

-- imgui library
g_LibraryConfig.projects["imgui"] = GetCurrentDirectoryPath() .. "/Projects/imgui.lua"

-- SDL library
InitLibrary("SDL")
if os.target() == "windows"		then SetupLibrary(g_LibraryConfig.includes.SDL, { GetThirdLibraryPath() .. "SDL2-2.0.20/include/" })
elseif os.target() == "macosx"	then SetupLibrary(g_LibraryConfig.includes.SDL, { GetThirdLibraryPath() .. "SDL2-2.0.20/lib/%{cfg.system}/%{cfg.architecture}/SDL2.framework/Headers" })
end
SetupLibrary(g_LibraryConfig.librarys.SDL, { GetThirdLibraryPath() .. "SDL2-2.0.20/lib/%{cfg.system}/%{cfg.architecture}" })
if os.target() == "windows"		then SetupLibrary(g_LibraryConfig.names.SDL, { "SDL2.lib", "SDL2main.lib" })
elseif os.target() == "macosx"	then SetupLibrary(g_LibraryConfig.names.SDL, { "SDL2.framework" })
end

-- spdlog library
InitLibrary("spdlog")
SetupLibrary(g_LibraryConfig.includes.spdlog, { GetThirdLibraryPath() .. "spdlog/include" })

-- stb library
InitLibrary("stb")
SetupLibrary(g_LibraryConfig.includes.stb, { GetThirdLibraryPath() .. "stb" })

----------------------- Third Library -----------------------

----------------------- Custom Library -----------------------

-- Framework library
g_LibraryConfig.projects["Framework"] = GetCurrentDirectoryPath() .. "/Projects/framework.lua"

----------------------- Custom Library -----------------------