#pragma once
#include "StdIncludes.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL_thread.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

struct SDLContext
{	//The window we'll be rendering to
    SDL_Window* Window = nullptr;
    //The surface contained by the window
    SDL_Texture* ScreenTex = nullptr;
    SDL_Renderer* Renderer = nullptr;
    TTF_TextEngine* TtfEngine = nullptr;
    std::vector<TTF_Font*> Fonts;
};
using PContext = std::unique_ptr<SDLContext>();

//The application time based timer
class LTimer
{
public:
    //Initializes variables
    LTimer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    Uint64 getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
    //The clock time when the timer started
    Uint64 mStartTicks;

    //The ticks stored when the timer was paused
    Uint64 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;
};