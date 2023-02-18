project "test_project"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
	architecture "x64"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/%{output_dir}/%{prj.name}")
    objdir ("%{wks.location}/bin-int/%{output_dir}/%{prj.name}")

    includedirs {
        "include/",
        "../boreal/src",
        "../common/src",
        "../glfw_vulkan/src",
        "../imgui_glfw_vulkan/src",
        "../../submodules/glfw/include/",
        "../../submodules/glm/",
        "../../submodules/imgui/",
        "../../submodules/imgui/backends",
        "%{vulkan_sdk}/Include"
    }

    files {
        "src/**.h",
        "src/**.cpp"
    }

    links {
        "imgui_glfw_vulkan"
    }

    ignoredefaultlibraries {
        "LIBCMTD",
        "LIBCMT"
    }

    flags {
        "MultiProcessorCompile",
    }

    filter "system:linux"
        links {
            "dl",
            "pthread"
        }
        defines {
            "_X11"
        }

    filter "system:windows"
        systemversion "latest"
        defines {
            "_WINDOWS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        defines {
            "DEBUG",
            "DEBUG_SHADER"
        }
        
    filter "configurations:Release"
        runtime "Release"
        optimize "Full"
        defines {
            "RELEASE"
        }
        flags {
            "LinkTimeOptimization",
        }