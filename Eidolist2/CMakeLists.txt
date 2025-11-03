# CMakeList.txt : CMake project for eidolist2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_TOOLCHAIN_FILE "C:/DevRepos/vcpkg/scripts/buildsystems/vcpkg.cmake")


include(externalProject)


project ("eidolist2")

list(APPEND HASH_LIBRARY_SOURCES
external/hash-library/md5.cpp
external/hash-library/md5.h
external/hash-library/sha1.cpp
external/hash-library/sha1.h
)

list(APPEND NLOHMANN_SOURCES
external/nlohmann/single_include/nlohmann/json.hpp
)

list(APPEND IMGUI_SOURCES
external/imgui/imstb_truetype.h
external/imgui/imstb_textedit.h
external/imgui/imstb_rectpack.h
external/imgui/imgui_widgets.cpp
external/imgui/imgui_tables.cpp
external/imgui/imgui_internal.h
external/imgui/imgui_draw.cpp
external/imgui/imgui_demo.cpp
external/imgui/imgui.h
external/imgui/imgui.cpp
external/imgui/imconfig.h
external/imgui/backends/imgui_impl_sdlrenderer3.h
external/imgui/backends/imgui_impl_sdlrenderer3.cpp
external/imgui/backends/imgui_impl_sdl3.h
external/imgui/backends/imgui_impl_sdl3.cpp
external/imgui/misc/cpp/imgui_stdlib.h
external/imgui/misc/cpp/imgui_stdlib.cpp
)

list(APPEND EIDOLIST2_SOURCES

src/EidolistCore.h
src/EidolistCore.cpp

src/includes/LCFIncludes.h
src/includes/GLMIncludes.h 
src/includes/StdIncludes.h
src/includes/CommonIncludes.h

src/useful/SDLTypes.h
src/useful/SDLTypes.cpp
src/useful/LCFTypes.h
src/useful/SDLFun.h
src/useful/SDLFun.cpp
src/useful/MathFun.h
src/useful/MathFun.cpp
src/useful/Rect.h
src/useful/Rect.cpp
src/useful/ImguiStyle.h 
src/useful/ImguiStyle.cpp
src/useful/TilesetHelper.h
src/useful/TilesetHelper.cpp
src/useful/TilesetHelper_Texture.h
src/useful/TilesetHelper_Texture.cpp
src/useful/WindowArea.h
src/useful/WindowArea.cpp

# src/MapDiff.h
# src/MapDiff.cpp

src/obj/MapObj.h
src/obj/MapObj.cpp 
src/obj/MapComparisonObj.h
src/obj/MapComparisonObj.cpp
src/obj/Spritesheet.h
src/obj/Spritesheet.cpp
src/obj/SpriteObj.h
src/obj/SpriteObj.cpp
src/obj/BaseObj.h
src/obj/BaseObj.cpp
src/obj/CompileMap.h
src/obj/CompileMap.cpp

src/projectdata/Project.h
src/projectdata/Project.cpp
src/projectdata/ProjectData.h
src/projectdata/ProjectData.cpp
src/projectdata/LoadProject.h
src/projectdata/LoadProject.cpp
src/projectdata/CompareProjectData.h
src/projectdata/CompareProjectData.cpp 
src/projectdata/ListProjectData.h
src/projectdata/ListProjectData.cpp
src/projectdata/AutotileData.h
src/projectdata/AutotileData.cpp

src/gui/MapLayers.h
src/gui/MapLayers.cpp
src/gui/ProjectGui.h
src/gui/ProjectGui.cpp
src/gui/ProjectSelectDialog.h
src/gui/ProjectSelectDialog.cpp

src/projectdata/meta/WikiData.h 
src/projectdata/meta/WikiData.cpp 

src/io/AssetLoader.h
src/io/AssetLoader.cpp
src/io/FileHasher.h 
src/io/FileHasher.cpp
"src/io/Cache.h"
"src/io/Cache.cpp"
src/io/FileIO.h

# "src/graph/WorldGraph.h" 
# "src/graph/WorldGraph.cpp" 
)

# Add source to this project's executable.
add_executable (eidolist2 "eidolist2-main.cpp" "eidolist2-main.h" ${EIDOLIST2_SOURCES} ${IMGUI_SOURCES} ${NLOHMANN_SOURCES} ${HASH_LIBRARY_SOURCES} )

# if (${BUILD_DOCTEST})
# 	add_executable(tests "src/UnitTests/main.cpp" "src/TilesetHelper.h" "src/TilesetHelper.cpp" ${VMATH_SOURCES}  "src/GLMIncludes.h")
# 	target_compile_features(tests PRIVATE cxx_std_20)
# 	find_package(doctest CONFIG REQUIRED)
# 	target_link_libraries(tests PRIVATE doctest::doctest)
# endif()


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET eidolist2 PROPERTY CXX_STANDARD 20)
endif()


target_include_directories(eidolist2 PRIVATE src src/includes src/useful src/projectdata src/obj external/imgui external/imgui/backends external/imgui/misc/cpp external/nlohmann/single_include/nlohmann external/hash-library external/liblcf/src )

set(SDL_STATIC ON)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL_ttf EXCLUDE_FROM_ALL)
set(SDLIMAGE_AVIF OFF)
set(SDLIMAGE_BMP  OFF)
set(SDLIMAGE_GIF  OFF)
set(SDLIMAGE_JPG  ON)
set(SDLIMAGE_JXL  OFF)
set(SDLIMAGE_LBM  OFF)
set(SDLIMAGE_PCX  OFF)
set(SDLIMAGE_PNG  ON)
set(SDLIMAGE_PNM  OFF)
set(SDLIMAGE_QOI  OFF)
set(SDLIMAGE_SVG  OFF)
set(SDLIMAGE_TGA  OFF)
set(SDLIMAGE_TIF  OFF)
set(SDLIMAGE_WEBP OFF)
set(SDLIMAGE_XCF  OFF)
set(SDLIMAGE_XPM  OFF)
set(SDLIMAGE_XV	  OFF)
add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)
target_link_libraries(eidolist2 PRIVATE SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf SDL3::SDL3)

find_package(unofficial-inih CONFIG REQUIRED)
target_link_libraries(eidolist2 PRIVATE unofficial::inih::inireader)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(eidolist2 PRIVATE fmt::fmt)

find_package(expat CONFIG REQUIRED)
target_link_libraries(eidolist2 PRIVATE expat::expat)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(eidolist2 PRIVATE cpr::cpr)

find_package(glm CONFIG REQUIRED)
target_link_libraries(eidolist2 PRIVATE glm::glm)

option(BUILD_LIBLCF "For developers: Clone liblcf and build it along with the app." ON)
if(BUILD_LIBLCF)
	set(LIBLCF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/liblcf")
	function(add_liblcf)
		set(BUILD_SHARED_LIBS OFF)
		set(LIBLCF_ENABLE_INSTALL OFF)
		add_subdirectory(${LIBLCF_PATH})
	endfunction()
	add_liblcf()
	target_link_libraries(${PROJECT_NAME} PRIVATE lcf)
else()
	# Use system package
	player_find_package(NAME liblcf VERSION 0.8.1 TARGET liblcf::liblcf REQUIRED)
endif()

set(BUILD_SHARED_LIBS ON)
