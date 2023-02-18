project "common"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    architecture "x64"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/%{output_dir}/%{prj.name}")
    objdir ("%{wks.location}/bin-int/%{output_dir}/%{prj.name}")

    includedirs {
        "include/",
        "../../submodules/glfw/include/",
    }

    files {
        "src/**.h",
        "src/**.cpp"
    }

    flags {
        "MultiProcessorCompile",
    }