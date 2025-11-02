#pragma once
#include "StdIncludes.h"
#include "GLMIncludes.h"

#include <fmt/format.h>

#include "../external/nlohmann/json.hpp"
using namespace nlohmann;

#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_sdl3.h"
#include "../external/imgui/imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL_thread.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
//#undef main

#include "Types.h"

//#define TESTOUTPUT std::string("C:\\DevRepos\\eidolist2\\TestOutput\\")
//#define TEMPCONFIGS std::string("C:\\DevRepos\\eidolist2\\Configs\\")

#define TESTOUTPUT std::string(".\\output\\")
#define TEMPCONFIGS std::string(".\\data\\")