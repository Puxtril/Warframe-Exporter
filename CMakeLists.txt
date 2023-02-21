##################################################
# Set these appropriately!

set(ENABLE_EXE_COMPILATION ON BOOL)
set(ENABLE_CLI_EXTRACT ON BOOL)
set(ENABLE_CLI_DEBUG ON BOOL)

##################################################
# Project stuff

cmake_minimum_required(VERSION 3.18)
project(Warframe-Exporter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

##################################################
# OS-based flags

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /W4")
elseif(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")
endif()
if(ANDROID)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsigned-char")
endif()

##################################################
# Source files

file(GLOB WARFRAME_EXPORTER_SOURCES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/material/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/model/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/texture/*.cpp
)

##################################################
# Include dirs

include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/fx-gltf/include")
include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm")
include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/spdlog/include")
include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/json/single_include")
include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/Binary-Reader/include")
include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/ddspp")
include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/tclap-code/include")
include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib/include")
include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib/lib/Oodle-2.9/include")

##################################################
# Executable compile commands

if(ENABLE_EXE_COMPILATION)
	add_executable(Warframe-Exporter ${WARFRAME_EXPORTER_SOURCES})
	
	# Compile subprojects
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/Binary-Reader")
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/LotusLib")
	
	# Link Oodle static library
	if(MSVC)
		find_library(OODLE_CORE NAMES oo2core_win64 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/bin/Win64)
	elseif(APPLE)
		find_library(OODLE_CORE NAMES oo2core64 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/bin/Mac)
	elseif(ANDROID)
		find_library(OODLE_CORE NAMES oo2coreandroid64 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/bin/Android/arm64-v8a)
	elseif(UNIX)
		find_library(OODLE_CORE NAMES oo2corelinux64 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux)
	endif()
	target_link_libraries(Warframe-Exporter Binary-Reader LotusLib ${OODLE_CORE})
	
	# Set CLI options
	if(ENABLE_CLI_EXTRACT)
		target_compile_definitions(Warframe-Exporter PUBLIC WF_EXTRACT)
	endif()
	if(ENABLE_CLI_DEBUG)
		target_compile_definitions(Warframe-Exporter PUBLIC WF_DEBUG)
	endif()
endif()

##################################################
# Library target

add_library(Warframe-Exporter-Lib STATIC ${WARFRAME_EXPORTER_SOURCES})
