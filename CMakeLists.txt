##################################################
# Set these appropriately

option(COMPILE_WF_UI_EXE "Compile the standard UI executable" ON)
option(COMPILE_WF_CLI_EXE "Compile the standard CLI executable" ON)
option(COMPILE_WF_CLI_EXE_ADVANCED "Compile the standard CLI executable with more advanced options" ON)

##################################################
# Project stuff

cmake_minimum_required(VERSION 3.18)
project(Warframe-Exporter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# For clangd (linting)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "Setting build type to Release.")
	message(STATUS "To Debug, run `cmake -DCMAKE_BUILD_TYPE=Debug`")
	set(CMAKE_BUILD_TYPE "Release")
endif()

##################################################
# OS-based flags

# Idk Qt wants these
if (COMPILE_WF_UI_EXE AND MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /permissive-")
	add_definitions(-DUNICODE -D_UNICODE)
endif()

if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /O2 /sdl /Oi /W3 /std:c++17")
elseif(UNIX)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -Wall")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -ggdb")
endif()

##################################################
# Source files

file(GLOB WARFRAME_EXPORTER_SOURCES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/material/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/material/types/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/model/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/model/types/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/model/vertexcolor/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/model/vertexcolor/types/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/texture/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/level/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/level/types/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/audio/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/audio/types/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/audio/AudioPCM/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/audio/AudioPCM/types/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/audio/AudioOpus/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/audio/AudioOpus/types/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/shader/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/shader/types/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/landscape/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/landscape/types/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/levelstatic/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/levelstatic/types/*.cpp
)

file(GLOB WARFRAME_EXPORTER_CLI_SOURCES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/src/cli/*.cpp
)

file(GLOB WARFRAME_EXPORTER_UI_SOURCES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/preview/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/tasks/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/QtOpenGLViewer/QtOpenGLViewer.cpp
	# Resources file
	${CMAKE_CURRENT_SOURCE_DIR}/assets/resources.qrc
	# Since these objects define signals, they need the Q_OBJECT macro
	#   And since they have this macro, they need to be picked up by Moc (Auto mode)
	#   And since they need to be picked up by Moc, they need to be treated as a source file
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/UIPicker.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/UIExporter.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/UIMainWindow.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/UILoading.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/BatchIteratorExportQt.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/tasks/LoadTreeThread.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/tasks/ExporterDirectoryThread.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/tasks/ExporterFileThread.h
)

##################################################
# App Icon

if(MSVC)
	# No idea why this doesn't work for WARFRAME_EXPORTER_SOURCES
	set(WARFRAME_EXPORTER_UI_SOURCES ${WARFRAME_EXPORTER_UI_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/assets/resources.rc)
	set(WARFRAME_EXPORTER_CLI_SOURCES ${WARFRAME_EXPORTER_CLI_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/assets/resources.rc)
endif()

##################################################
# Include dirs

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/tclap-code/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/fx-gltf/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/Binary-Reader/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/ddspp"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib/lib/Oodle-2.9/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib/lib/liblzf/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib/lib/zstd"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib/lib/zstd/lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib/lib/zstd/lib/decompress"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/qt-install/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/QtOpenGLViewer"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/bcdec"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/PhysxParser/include"
)

##################################################
# Executable compile commands

# Add libraries among all targets

# LotusLib requires zstd
# It adds a library target `zstd_static` which needs to be linked to executables
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib")

set(BUILD_STATIC_LIBS ON CACHE BOOL "Build glm static libs")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/glm")

set(SPNG_SHARED OFF CACHE BOOL "Build spng shared library")
set(SPNG_STATIC ON CACHE BOOL "Build spng shared library")
set(BUILD_EXAMPLES OFF CACHE BOOL "Build spng examples")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/libspng")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/PhysxParser")

add_library(Warframe-Exporter-Lib STATIC ${WARFRAME_EXPORTER_SOURCES})
target_link_libraries(Warframe-Exporter-Lib spdlog glm::glm nlohmann_json::nlohmann_json spng_static PhysxParser)

if(COMPILE_WF_UI_EXE OR COMPILE_WF_CLI_EXE OR COMPILE_WF_CLI_EXE_ADVANCED)

	# zlib is used by libPNG
	# Both dynamic and static libraries will be built by zlib,
	#  but this instructs zlib to not look for dynamic libraries.
	set(ZLIB_USE_STATIC_LIBS ON)

	# Link Oodle static library
	if(WIN32)
		find_library(OODLE_CORE NAMES oo2core_win64 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/bin/Win64)	
	elseif(APPLE)
		find_library(OODLE_CORE NAMES oo2core64 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/bin/Mac)
	elseif(UNIX AND CMAKE_BUILD_TYPE STREQUAL "Release")
		find_library(OODLE_CORE NAMES oo2corelinux64 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux)
	elseif(UNIX AND CMAKE_BUILD_TYPE STREQUAL "Debug")
		find_library(OODLE_CORE NAMES oo2corelinux64_debug PATHS ${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux)
	endif()
	
	# For Shader decompilation (Windows only), link D3DCompiler
	# Only using 1 function: D3DDecompile
	if(WIN32)
		find_library(D3D_COMPILER REQUIRED NAMES d3dcompiler.lib)
	endif()

	if(COMPILE_WF_UI_EXE)
		# Auto-runs the Qt MOC tool
		# Needed for those extra files inside WARFRAME_EXPORTER_UI_SOURCES
		set(CMAKE_AUTOMOC ON)

		# Auto-compile .ui files into headers for C++ compilation
		set(CMAKE_AUTOUIC ON)
		set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)
		
		# Qt resources file (assets/resources.qrc)
		set(CMAKE_AUTORCC ON)

		add_executable(Warframe-Exporter ${WARFRAME_EXPORTER_UI_SOURCES})

		find_package(OpenGL REQUIRED)
		find_package(Qt6 QUIET COMPONENTS Widgets Core Gui OpenGLWidgets)

		# Qt6 is installed on the system (Probably using a *nix package manager)
		if (Qt6_FOUND)

			# ezpz
			target_link_libraries(Warframe-Exporter Warframe-Exporter-Lib spdlog Qt6::Widgets Qt6::Core Qt6::Gui Qt6::OpenGLWidgets OpenGL::GL ${D3D_COMPILER} LotusLib libzstd_static ${OODLE_CORE} spng_static ${ZLIB_LIBRARY} lzf PhysxParser)
			message(STATUS "Linked to Qt6 libraries installed on the system")

		# Not found. Probably Windows. Requires building from source.
		else()

			# This is simply to find MOC command
			set(Qt6Core_VERSION_MAJOR 6)
			set(Qt6Core_VERSION_MINOR 6)
			find_package(Qt6 NO_DEFAULT_PATH PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/qt-install" COMPONENTS Core)

			if (Qt6_FOUND)
			else()
				message(FATAL_ERROR "Qt6 was not found inside \"${CMAKE_CURRENT_SOURCE_DIR}/lib/qt-install\" Please build from source using the instructions in README.md")
			endif()
			
			# Typically these would be auto-discovered by find_package()
			find_library(Qt6Widgets NAMES Qt6Widgets PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/qt-install/lib" REQUIRED)
			find_library(Qt6Core NAMES Qt6Core PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/qt-install/lib" REQUIRED)
			find_library(Qt6Gui NAMES Qt6Gui PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/qt-install/lib" REQUIRED)
			find_library(Qt6OpenGL NAMES Qt6OpenGL PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/qt-install/lib" REQUIRED)
			find_library(Qt6OpenGLWidgets NAMES Qt6OpenGLWidgets PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/qt-install/lib" REQUIRED)

			target_link_libraries(Warframe-Exporter Warframe-Exporter-Lib spdlog LotusLib libzstd_static ${Qt6Widgets} ${Qt6Core} ${Qt6Gui} ${Qt6OpenGL} ${Qt6OpenGLWidgets} OpenGL::GL ${OODLE_CORE} ${D3D_COMPILER} spng_static ${ZLIB_LIBRARY} lzf PhysxParser)
			message(STATUS "Linked to Qt6 libraries built locally")

		endif()
		
		# Application will launch CMD window if this isn't set
		if (MSVC)
			target_link_options(Warframe-Exporter PUBLIC /SUBSYSTEM:windows /ENTRY:mainCRTStartup)
		endif()
	endif()

	if(COMPILE_WF_CLI_EXE)
		add_executable(Warframe-Exporter-CLI ${WARFRAME_EXPORTER_CLI_SOURCES})
		target_link_libraries(Warframe-Exporter-CLI Warframe-Exporter-Lib spdlog LotusLib libzstd_static ${OODLE_CORE} ${D3D_COMPILER} spng_static ${ZLIB_LIBRARY} lzf PhysxParser)
		target_compile_definitions(Warframe-Exporter-CLI PUBLIC WF_EXTRACT)
	endif()
		
	if(COMPILE_WF_CLI_EXE_ADVANCED)
		add_executable(Warframe-Exporter-CLI-Advanced ${WARFRAME_EXPORTER_CLI_SOURCES})
		target_link_libraries(Warframe-Exporter-CLI-Advanced Warframe-Exporter-Lib spdlog LotusLib libzstd_static ${OODLE_CORE} ${D3D_COMPILER} spng_static ${ZLIB_LIBRARY} lzf PhysxParser)
		target_compile_definitions(Warframe-Exporter-CLI-Advanced PUBLIC WF_EXTRACT WF_DEBUG WF_ADVANCED)
	endif()

endif()
