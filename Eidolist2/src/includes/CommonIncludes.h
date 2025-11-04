#pragma once
#include "StdIncludes.h"
#include "GLMIncludes.h"

#include <fmt/format.h>

#include "json.hpp"
using namespace nlohmann;

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL_thread.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
//#undef main

#define TESTOUTPUT std::string(".\\output\\")
#define TEMPCONFIGS std::string(".\\data\\")