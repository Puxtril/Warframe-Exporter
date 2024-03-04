##################################################
# Set these appropriately

option(COMPILE_WF_UI_EXE "Compile the standard UI executable" ON)
option(COMPILE_WF_CLI_EXE "Compile the standard CLI executable" OFF)
option(COMPILE_WF_CLI_EXE_ADVANCED "Compile the standard CLI executable with more advanced options" OFF)
option(COMPILE_WF_LIB "Compile a static library" OFF)

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
# Dumb Qt stuff

if(COMPILE_WF_UI_EXE)
	set(CMAKE_AUTOMOC ON)
endif()

##################################################
# OS-based flags

if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /O2 /sdl /Oi /W3 /std:c++17")
	add_definitions(-DUNICODE -D_UNICODE)
elseif(UNIX)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -Wall")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -ggdb")
endif()

##################################################
# Source files

file(GLOB WARFRAME_EXPORTER_SOURCES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/material/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/model/*.cpp
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
)

file(GLOB WARFRAME_EXPORTER_CLI_SOURCES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/src/cli/*.cpp
)

file(GLOB WARFRAME_EXPORTER_UI_SOURCES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.cpp
	# Since these objects define signals, they need the Q_OBJECT macro
	#   And since they have this macro, they need to be picked up by Moc (Auto mode)
	#   And since they need to be picked up by Moc, they need to be treated as a source file
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/UIPicker.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/BatchIteratorExportQt.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/ExporterDirectoryThread.h
)

##################################################
# Include dirs

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/include" 
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/fx-gltf/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/glm"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/spdlog/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/json/single_include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/Binary-Reader/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/ddspp"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/tclap-code/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib/lib/Oodle-2.9/include"
)

##################################################
# Executable compile commands

if(COMPILE_WF_UI_EXE OR COMPILE_WF_CLI_EXE OR COMPILE_WF_CLI_EXE_ADVANCED)

	# Compile subprojects
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib")
	
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

	if(COMPILE_WF_UI_EXE)
		find_package(OpenGL REQUIRED)
		find_package(Qt6 COMPONENTS Widgets Core Gui OpenGLWidgets REQUIRED)

		add_executable(Warframe-Exporter ${WARFRAME_EXPORTER_SOURCES} ${WARFRAME_EXPORTER_UI_SOURCES})
		target_link_libraries(Warframe-Exporter Qt6::Widgets Qt6::Core Qt6::Gui Qt6::OpenGLWidgets LotusLib ${OODLE_CORE})
	endif()

	if(COMPILE_WF_CLI_EXE)
		add_executable(Warframe-Exporter-CLI ${WARFRAME_EXPORTER_SOURCES} ${WARFRAME_EXPORTER_CLI_SOURCES})
		target_link_libraries(Warframe-Exporter-CLI LotusLib ${OODLE_CORE})
		target_compile_definitions(Warframe-Exporter-CLI PUBLIC WF_EXTRACT)
	endif()
		
	if(COMPILE_WF_CLI_EXE_ADVANCED)
		add_executable(Warframe-Exporter-CLI-Advanced ${WARFRAME_EXPORTER_SOURCES} ${WARFRAME_EXPORTER_CLI_SOURCES})
		target_link_libraries(Warframe-Exporter-CLI-Advanced LotusLib ${OODLE_CORE})
		target_compile_definitions(Warframe-Exporter-CLI-Advanced PUBLIC WF_EXTRACT WF_DEBUG WF_ADVANCED)
	endif()

endif()

##################################################
# Library target

if (COMPILE_WF_LIB)
	add_library(Warframe-Exporter-Lib STATIC ${WARFRAME_EXPORTER_SOURCES})
endif()
