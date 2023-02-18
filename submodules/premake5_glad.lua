project "glad"
	kind "StaticLib"
	language "C"
	architecture "x64"

    targetdir ("%{wks.location}/bin/%{output_dir}/%{prj.name}")
    objdir ("%{wks.location}/bin-int/%{output_dir}/%{prj.name}")

    includedirs {
	    "glad/include/"
	}

	files {
		"glad/src/glad.c"
	}

    flags {
        "MultiProcessorCompile"
    }

	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		defines {
			"_GLAD_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines { 
			"_GLAD_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"