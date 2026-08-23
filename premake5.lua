workspace "D++ Interpreter"
	architecture "x64"
	
    configurations {

        "Debug",
        "Release",
        "Dist"

    }

    filter "system:windows"
        buildoptions { "/utf-8" }
    filter {}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "D++"
    location "D++"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "dpch.h"
    pchsource "%{prj.name}/src/dpch.cpp" 

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs{
       "%{prj.name}/src"
    }

    linker "LLD"

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

        editandcontinue "off"

        multiprocessorcompile "on"
        incrementallink "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        incrementallink "off"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"
        incrementallink "off"