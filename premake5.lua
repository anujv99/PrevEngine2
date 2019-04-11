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
	IncludeDir["glfw"] = "PrevEngine/vendor/glfw/include"
	IncludeDir["glad"] = "PrevEngine/vendor/glad/include"
	
	--[[
	Windowing API supprted  | windowingAPI
	--------------------------------------
	Win32					| PV_WINDOWING_API_WIN32 -- Dosen't support imgui viewports
	GLFW					| PV_WINDOWING_API_GLFW  -- support imgui viewports
	
	To Compile both use 	| PV_WINDOWING_API_BOTH
	]]--
	windowingAPI = "PV_WINDOWING_API_BOTH"
	
	if (windowingAPI == "PV_WINDOWING_API_GLFW" or windowingAPI == "PV_WINDOWING_API_BOTH") then
		include "PrevEngine/vendor/glfw"
	end
	
	--[[
	Rendering API supported	 | renderingAPI
	---------------------------------------
	DirectX 				 | PV_RENDERING_API_DIRECTX // 11
	OpenGL					 | PV_RENDERING_API_OPENGL
	
	To Compile both use 	 | PV_RENDERING_API_BOTH
	]]--
	renderingAPI = "PV_RENDERING_API_BOTH"
	
	if (renderingAPI == "PV_RENDERING_API_OPENGL" or renderingAPI == "PV_RENDERING_API_BOTH") then
		include "PrevEngine/vendor/glad"
	end
	
	project "PrevEngine"
		location "PrevEngine"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
		
		targetdir ("bin/" .. outputDir .. "%{prj.name}")
		objdir ("bin-int/" .. outputDir .. "%{prj.name}")
		
		files {
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}
		
		includedirs {
			"%{prj.name}/src",
		}
		
		if (windowingAPI == "PV_WINDOWING_API_GLFW" or windowingAPI == "PV_WINDOWING_API_BOTH") then
			includedirs {
				"%{IncludeDir.glfw}"
			}
			
			links {
				"GLFW"
			}
		end
		
		if (renderingAPI == "PV_RENDERING_API_OPENGL") then
			links {
				"opengl32.lib",
				"glad"
			}
			
			includedirs {
				"%{IncludeDir.glad}"
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
				"dxgi.lib",
				"glad"
			}
			
			includedirs {
				"%{IncludeDir.glad}"
			}
		else
			error("Invalid renderingAPI")
		end
		
		filter "system:windows"
			systemversion "latest"
		
        defines {
			renderingAPI,
			windowingAPI,
            "PV_BUILD_STATIC_LIB",
			"PV_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}
		
		pchheader "pch.h"
		
		filter "action:vs*"
			pchsource "PrevEngine/src/pch.cpp"
			
		filter "configurations:Debug"
			defines {"PV_DEBUG"}
			runtime "Debug"
			symbols "on"
			
	
		filter "configurations:Release"
			defines {"PV_RELEASE"}
			runtime "Release"
			optimize "on"
	
		filter "configurations:Distribute"
			defines {"PV_DIST"}
			runtime "Release"
			optimize "on"
		
	project "Sandbox"
		location "Sandbox"
		kind "WindowedApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
	
		targetdir ("bin/" .. outputDir .. "%{prj.name}")
		objdir ("bin-int/" .. outputDir .. "%{prj.name}")
		
		files {
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
		}
		
		includedirs {
			"%{prj.name}/src",
			"PrevEngine/src"
		}
		
		defines {
			"_CRT_SECURE_NO_WARNINGS"
		}
		
		links {
			"PrevEngine"
		}
		
		filter "configurations:Debug"
			defines {"PV_DEBUG"}
			runtime "Debug"
			symbols "on"
	
		filter "configurations:Release"
			defines {"PV_RELEASE"}
			runtime "Release"
			optimize "on"
	
		filter "configurations:Distribute"
			defines {"PV_DIST"}
			runtime "Release"
			optimize "on"