-- On Linux we have to query the dependencies for libsecret
if os.ishost("linux") then
	libFlags, code = os.outputof("pkg-config --cflags libnotify gtk+-3.0")
end

workspace("sr_gui")

-- Configurations
	configurations({ "Release", "Debug"})
	location("build")
	targetdir ("build/%{prj.name}/%{cfg.longname}")
	debugdir ("build/%{prj.name}/%{cfg.longname}")
	architecture("x64")

	-- Configuration specific settings.
	filter("configurations:Release")
		defines({ "NDEBUG" })
		optimize("On")

	filter("configurations:Debug")
		defines({ "DEBUG" })
		symbols("On")

	filter({})
	startproject("sr_gui")

project("sr_gui")
	kind("StaticLib")
	systemversion("latest")
	
	filter("toolset:not msc*")
		buildoptions({ "-Wall", "-Wextra" })
	filter("toolset:msc*")
		buildoptions({ "-W3"})
	filter({})

	includedirs({"include/"})
	-- common files
	files({"include/sr_gui.h", "src/sr_gui_internal.h"})
	
	-- system build filters
	filter("system:windows")
		language("C++")
		files({"src/sr_gui_win.cpp"})

	filter("system:macosx")
		language("C")
		files({"src/sr_gui_mac.m"})

	filter({"system:linux"})
		language("C")
		buildoptions(libFlags)
		files({"src/sr_gui_lin.c"})

   
	filter({})

project("sr_gui_cli")
	kind("StaticLib")
	systemversion("latest")
	language("C")
	
	filter("toolset:not msc*")
		buildoptions({ "-Wall", "-Wextra" })
	filter("toolset:msc*")
		buildoptions({ "-W3"})
	filter({})

	includedirs({"include/"})
	
	files({"include/sr_gui.h", "src/sr_gui_internal.h", "src/sr_gui_cli.c"})

include("example/premake5.lua")

-- Actions

newaction {
   trigger     = "clean",
   description = "Clean the build directory",
   execute     = function ()
      print("Cleaning...")
      os.rmdir("./build")
      print("Done.")
   end
}