cmake_minimum_required(VERSION 3.18)
project(Warframe-Exporter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /W4")
elseif(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")
endif()

file(GLOB WARFRAME_EXPORTER_SOURCES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/animation/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/material/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/model/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/texture/*.cpp
)
set_source_files_properties(${WARFRAME_EXPORTER_SOURCES} PROPERTIES LANGUAGE CXX)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
add_library(Warframe-Exporter STATIC ${WARFRAME_EXPORTER_SOURCES})

target_include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/fx-gltf/include")
target_include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm")
target_include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/json/single_include")
target_include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/Binary-Reader/include")
target_include_directories(Warframe-Exporter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../Warframe-Reader/include")
