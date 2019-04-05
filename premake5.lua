workspace "PrevEngine"
	startproject "Sandbox"
	architecture "x64"
	
	configurations {
        "Debug",
        "Release",
        "Distribute"
    }
	
	outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"
	
	IncludeDir = {}
	
	--[[
	Windowing API supprted  | windowingAPI
	--------------------------------------
	Win32					| PV_WINDOWING_API_WIN32 -- Dosen't support imgui viewports
	GLFW					| PV_WINDOWING_API_GLFW  -- support imgui viewports
	
	To Compile both use 	| PV_WINDOWING_API_BOTH
	]]--
	windowingAPI = "PV_WINDOWING_API_WIN32"
	
	--[[
	Rendering API supported	 | renderingAPI
	---------------------------------------
	DirectX 				 | PV_RENDERING_API_DIRECTX // 11
	OpenGL					 | PV_RENDERING_API_OPENGL
	
	To Compile both use 	 | PV_RENDERING_API_BOTH
	]]--
	renderingAPI = "PV_RENDERING_API_DIRECTX"
	
	project "PrevEngine"
		location "PrevEngine"
		kind "StaticLib"
		language "C++"
		
		targetdir ("bin/" .. outputDir .. "%{prj.name}")
		objdir ("bin-int/" .. outputDir .. "%{prj.name}")
		
		files {
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}
		
		includedirs {
			"%{prj.name}/src",
		}
		
		if (renderingAPI == "PV_RENDERING_API_OPENGL") then
			links {
				"opengl32.lib"
			}
		elseif (renderingAPI == "PV_RENDERING_API_DIRECTX") then
			links {
				"d3d11.lib",
				"d3dcompiler.lib",
				"dxgi.lib"
			}
		elseif (renderingAPI == "PV_RENDERING_API_BOTH") then
			links {
				"opengl32.lib",
				"d3d11.lib",
				"d3dcompiler.lib",
				"dxgi.lib"
			}
		else
			error("Invalid renderingAPI")
		end
		
		filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
		
        defines {
			renderingAPI,
			windowingAPI,
            "PV_BUILD_STATIC_LIB",
			"PV_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}
		
		removefiles {
			"%{prj.name}/src/platform/linux/*",
		}
		
		pchheader "pch.h"
		
		filter "action:vs*"
			pchsource "PrevEngine/src/pch.cpp"
			
		filter "configurations:Debug"
			defines {"PV_DEBUG"}
			symbols "On"
	
		filter "configurations:Release"
			defines {"PV_RELEASE"}
			optimize "On"
	
		filter "configurations:Distribute"
			defines {"PV_DIST"}
			optimize "On"
		
	project "Sandbox"
		location "Sandbox"
		kind "WindowedApp"
		language "C++"
	
		targetdir ("bin/" .. outputDir .. "%{prj.name}")
		objdir ("bin-int/" .. outputDir .. "%{prj.name}")
		
		files {
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
		}
		
		includedirs {
			"%{prj.name}/src",
			"PrevEngine"
		}
		
		defines {
			"_CRT_SECURE_NO_WARNINGS"
		}
		
		links {
			"PrevEngine"
		}
		
		filter "configurations:Debug"
			symbols "On"
	
		filter "configurations:Release"
			optimize "On"
	
		filter "configurations:Distribute"
			optimize "On"