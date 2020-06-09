cmake_minimum_required(VERSION 2.8.9)
project(MyProj)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")

option(BUILD_IMGUI "Build ImGUI" OFF)
option(BUILD_NANOVG "Build nanovg" OFF)

add_definitions(
	-D__STDC_LIMIT_MACROS
	-D__STDC_FORMAT_MACROS
	-D__STDC_CONSTANT_MACROS
	-D_HAS_EXCEPTIONS=0
	-D_SCL_SECURE=0
	-D_SECURE_SCL=0
	-D_SCL_SECURE_NO_WARNINGS
	-D_CRT_SECURE_NO_WARNINGS
	-D_CRT_SECURE_NO_DEPRECATE
	-DENTRY_CONFIG_IMPLEMENT_MAIN=1)

if(MSVC)
	set(PROJECT_LINK_LIBS psapi.lib Kernel32 bgfxDebug.lib bxDebug.lib bimg_decodeDebug.lib bimgDebug.lib)
	link_directories(..)
endif()

include_directories(./src)
include_directories(./3rdparty/bgfx_common)
include_directories(./3rdparty/raudio)
include_directories(./3rdparty/raymath)

include_directories(../bgfx/include)
include_directories(../bgfx/src)

include_directories(../bgfx/3rdparty/)
include_directories(../bgfx/3rdparty/khronos/)

include_directories(../bimg/include)
include_directories(../bx/include)
if(MSVC)
	include_directories(../bx/include/compat/msvc)
endif()

# Can manually add the sources using the set command as follows:
#set(SOURCES src/main.cpp src/file1.cpp)

# However, the file(GLOB...) allows for wildcard additions:
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
file(GLOB_RECURSE BGFXCOMMON "3rdparty/bgfx_common/*.cpp" "3rdparty/bgfx_common/*.h")
file(GLOB_RECURSE PARTICLES "3rdparty/bgfx_common/ps/*.cpp" "3rdparty/bgfx_common/ps/*.h")

file(GLOB_RECURSE DATA "Data/*.sc") # shaders

#list(REMOVE_ITEM COMMON "file_to_exclude.cpp")

if(BUILD_IMGUI)
	add_definitions( -DBUILD_IMGUI )
	file(GLOB_RECURSE IMGUI "../bgfx/3rdparty/dear-imgui/*.cpp" "../bgfx/3rdparty/dear-imgui/*.h" )
endif()

if(BUILD_NANOVG)
	add_definitions( -DBUILD_NANOVG )
endif()


add_executable(MyProj ${SOURCES} ${BGFXCOMMON} ${PARTICLES} ${IMGUI} ${DATA} )

target_link_libraries(MyProj ${PROJECT_LINK_LIBS} )

source_group("src" FILES ${SOURCES})
source_group("common" FILES ${BGFXCOMMON})
source_group("particles" FILES ${PARTICLES})
source_group("imgui" FILES ${IMGUI})

source_group("DATA" FILES ${DATA})
