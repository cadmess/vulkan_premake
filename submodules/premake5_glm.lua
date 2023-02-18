project "glm"
	kind "StaticLib"
	language "C"
	architecture "x64"

    targetdir ("%{wks.location}/bin/%{output_dir}/%{prj.name}")
    objdir ("%{wks.location}/bin-int/%{output_dir}/%{prj.name}")
	
	includedirs {
		"glm/"
	}

	files {
		"glm/glm/**"
	}

    flags {
        "MultiProcessorCompile"
    }
    
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		defines {
			"_GLM_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines { 
			"_GLM_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"