project "imgui"
	kind "StaticLib"
	language "C"
	architecture "x64"

    targetdir ("%{wks.location}/bin/%{output_dir}/%{prj.name}")
    objdir ("%{wks.location}/bin-int/%{output_dir}/%{prj.name}")
	
	includedirs {
		"imgui/",
		"imgui/backends/",
		"glad/include",
		"glfw/include/",
		"%{vulkan_sdk}/Include"
	}

	files {
		"imgui/*.cpp",
		"imgui/*.h",
		"imgui/backends/imgui_impl_glfw.cpp",
		"imgui/backends/imgui_impl_opengl3.cpp",
		"imgui/backends/imgui_impl_vulkan.cpp"
	}

	defines {
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}

    flags {
        "MultiProcessorCompile"
    }
    
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		defines {
			"_IMGUI_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines { 
			"_IMGUI_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"