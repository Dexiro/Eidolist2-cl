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


include(ExternalProject)


project ("eidolist2")

list(APPEND HASH_LIBRARY_SOURCES
# "external/hash-library/crc32.cpp"
# "external/hash-library/crc32.h"
# "external/hash-library/hash.h"
# "external/hash-library/hmac.h"
# "external/hash-library/keccak.cpp"
# "external/hash-library/keccak.h"
# "external/hash-library/md5.cpp"
# "external/hash-library/md5.h"
# "external/hash-library/sha1.cpp"
# "external/hash-library/sha1.h"
# "external/hash-library/sha3.cpp"
# "external/hash-library/sha3.h"
# "external/hash-library/sha256.cpp"
# "external/hash-library/sha256.h"
)

list(APPEND NLOHMANN_SOURCES
# external/nlohmann/json.hpp
)

list(APPEND IMGUI_SOURCES
# external/imgui/imstb_truetype.h
# external/imgui/imstb_textedit.h
# external/imgui/imstb_rectpack.h
# external/imgui/imgui_widgets.cpp
# external/imgui/imgui_tables.cpp
# external/imgui/imgui_internal.h
# external/imgui/imgui_draw.cpp
# external/imgui/imgui_demo.cpp
# external/imgui/imgui.h
# external/imgui/imgui.cpp
# external/imgui/imconfig.h
# external/imgui/imgui_impl_sdlrenderer3.h
# external/imgui/imgui_impl_sdlrenderer3.cpp
# external/imgui/imgui_impl_sdl3.h
# external/imgui/imgui_impl_sdl3.cpp
# external/imgui/imgui_stdlib.h
# external/imgui/imgui_stdlib.cpp
)

list(APPEND EIDOLIST2_SOURCES
# src/AssetLoader.h
# src/AssetLoader.cpp
# src/MapDiff.h
# src/MapDiff.cpp
# src/Types.h
# src/Tileset.h
# src/Tileset.cpp
# src/CommonIncludes.h
# src/SDLTypes.h
# src/SDLTypes.cpp
# src/TilesetHelper.h
# src/TilesetHelper.cpp
# src/TilesetHelper_Texture.h
# src/TilesetHelper_Texture.cpp
# src/Transform.h
# src/Transform.cpp
# "src/Rect.h" 
# "src/Rect.cpp"
# "src/system/MessageHandler.h" 
# "src/system/MessageHandler.cpp" 
# "src/gui/MainMenuGui.h" 
# "src/gui/MainMenuGui.cpp" 
# "src/obj/MapObj.h" 
# "src/obj/MapObj.cpp" 
# "src/obj/MapComparisonObj.h" 
# "src/obj/MapComparisonObj.cpp" 
# "src/gui/IGuiMenu.h" 
# "src/projectdata/Project.h"
# "src/projectdata/Project.cpp"
# "src/projectdata/ProjectData.h" 
# "src/projectdata/LoadProject.h" 
# "src/projectdata/LoadProject.cpp"
# "src/projectdata/ProjectGui.h" 
# "src/projectdata/ProjectGui.cpp" 
# "src/LCFIncludes.h" 
# "src/LCFTypes.h"
# "src/core/EidolistCore.h" 
# "src/core/EidolistCore.cpp" 
# "src/Spritesheet.h" 
# "src/Spritesheet.cpp" 
# "src/MapLayers.h" 
# "src/MapLayers.cpp" 
# "src/gui/ProjectSelectDialog.h" 
# "src/gui/ProjectSelectDialog.cpp" 
# "src/WindowArea.h" 
# "src/WindowArea.cpp"  
# "src/obj/BaseObj.h" 
# "src/obj/BaseObj.cpp" 
# "src/StdIncludes.h" 
# "src/io/FileHasher.h" 
# "src/io/FileHasher.cpp" 
# "src/gui/ImguiStyle.h" 
# "src/graph/WorldGraph.h" 
# "src/graph/WorldGraph.cpp" 
# "src/graph/WikiData.h" 
# "src/graph/WikiData.cpp" 
# "src/io/Cache.h" 
# "src/io/Cache.cpp" 
# "src/GLMIncludes.h" 
# "src/obj/CompileMap.h" 
# "src/obj/CompileMap.cpp" 
# "src/AutotileData.cpp" 
# "src/projectdata/ProjectData.cpp" 
# "src/gui/ImguiStyle.cpp" 
# "src/projectdata/CompareProjectData.h" 
# "src/projectdata/CompareProjectData.cpp"  
# "src/projectdata/ListProjectData.h" 
# "src/projectdata/ListProjectData.cpp" 
# "src/io/FileIO.h"
)

# Add source to this project's executable.
add_executable (eidolist2 "eidolist2-main.cpp" "eidolist2-main.h" ${EIDOLIST2_SOURCES} ${IMGUI_SOURCES} ${VMATH_SOURCES} ${NLOHMANN_SOURCES} ${HASH_LIBRARY_SOURCES} )

# if (${BUILD_DOCTEST})
# 	add_executable(tests "src/UnitTests/main.cpp" "src/TilesetHelper.h" "src/TilesetHelper.cpp" ${VMATH_SOURCES}  "src/GLMIncludes.h")
# 	target_compile_features(tests PRIVATE cxx_std_20)
# 	find_package(doctest CONFIG REQUIRED)
# 	target_link_libraries(tests PRIVATE doctest::doctest)
# endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET eidolist2 PROPERTY CXX_STANDARD 20)
endif()

option(BUILD_SDL3 "Build SDL3" ON)
if(BUILD_SDL3)
	set(SDL3_GIT "https://github.com/libsdl-org/SDL.git")
	set(SDL3_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL")
	if(NOT EXISTS ${SDL3_PATH})
		find_package(Git REQUIRED)
		execute_process(COMMAND ${GIT_EXECUTABLE} clone "--depth=1" "${SDL3_GIT}" "${SDL3_PATH}")
	endif()

	set(SDL3_TTF_GIT "https://github.com/libsdl-org/SDL_ttf.git")
	set(SDL3_TTF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL_ttf")
	if(NOT EXISTS ${SDL3_TTF_PATH})
		find_package(Git REQUIRED)
		execute_process(COMMAND ${GIT_EXECUTABLE} clone "--depth=1" "${SDL3_TTF_GIT}" "${SDL3_TTF_PATH}")
	endif()

	set(SDL3_IMAGE_GIT "https://github.com/libsdl-org/SDL_image.git")
	set(SDL3_IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL_image")
	if(NOT EXISTS ${SDL3_IMAGE_PATH})
		find_package(Git REQUIRED)
		execute_process(COMMAND ${GIT_EXECUTABLE} clone "--depth=1" "${SDL3_IMAGE_GIT}" "${SDL3_IMAGE_PATH}")
	endif()

	set(SDL_STATIC ON)
	add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)
	add_subdirectory(lib/SDL_ttf EXCLUDE_FROM_ALL)
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
	add_subdirectory(lib/SDL_image EXCLUDE_FROM_ALL)
	target_link_libraries(eidolist2 PRIVATE SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf SDL3::SDL3)
endif()



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
	# liblcf is built as part of this cmake file
	set(BUILD_LIBLCF_GIT "https://github.com/EasyRPG/liblcf.git")
	set(BUILD_LIBLCF_BRANCH "master")
	set(LIBLCF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/liblcf")

	if(NOT EXISTS ${LIBLCF_PATH})
		find_package(Git REQUIRED)
		execute_process(COMMAND ${GIT_EXECUTABLE} clone "--depth=1" "--branch"
			"${BUILD_LIBLCF_BRANCH}"
			"${BUILD_LIBLCF_GIT}"
			"${LIBLCF_PATH}")
	endif()

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
