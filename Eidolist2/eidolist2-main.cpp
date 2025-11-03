// eidolist2.cpp : Defines the entry point for the application.
//
#include "eidolist2-main.h"
#include "src/EidolistCore.h"
#include <SDL3/SDL_main.h>

using namespace std;

int main(int argc, char* argv[])
{
	//using namespace OIIO;

	cout << "Hello CMake." << endl;

	//Initialize SDL
	SDLContext context;
	bool bInitSDL = true;
	InitSDL(bInitSDL, context);
	TTF_Init();

	SDL_Color text_color = { 0xFF, 0xFF, 0xFF, 0xFF };
	//std::string fontPath = TEMPCONFIGS + "Fonts\\Arvo\\Arvo-Regular.ttf";
	//std::string fontPath = TEMPCONFIGS + "Fonts\\Roboto\\static\\Roboto-Regular.ttf";
	//std::string fontPath = TEMPCONFIGS + "Fonts\\Roboto_Mono\\RobotoMono-VariableFont_wght.ttf";
	//std::string fontPath = TEMPCONFIGS + "Fonts\\Roboto_Slab\\RobotoSlab-VariableFont_wght.ttf";
	//std::string fontPath = TEMPCONFIGS + "Fonts\\Slabo_27px\\Slabo27px-Regular.ttf";
	//TTF_Font* ttf_font = TTF_OpenFont(fontPath.c_str(), 14);
	//context.Fonts.push_back(ttf_font);
	//ttf_font = TTF_OpenFont(fontPath.c_str(), 22);
	//context.Fonts.push_back(ttf_font);
	//context.TtfEngine = TTF_CreateRendererTextEngine(context.Renderer);

	std::unique_ptr<EidolistCore> eidolistCore = std::make_unique<EidolistCore>();

	if (bInitSDL)
	{
		SDL_SetRenderDrawBlendMode(context.Renderer, SDL_BLENDMODE_BLEND);

		//eidolistCore->Run(context);
	}

	eidolistCore.reset();

	//Free loaded image
	SDL_DestroyTexture(context.ScreenTex);
	context.ScreenTex = NULL;


	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	for (auto font : context.Fonts)
	{
		TTF_CloseFont(font);
	}

	TTF_DestroyRendererTextEngine(context.TtfEngine);

	//Destroy window    
	TTF_Quit();
	SDL_DestroyRenderer(context.Renderer);
	SDL_DestroyWindow(context.Window);
	context.Window = NULL;
	context.Renderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}

void InitSDL(bool& bInitSDL, SDLContext& context)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		bInitSDL = false;
	}
	else
	{
		//SDL_WINDOW_RESIZABLE
		if (SDL_CreateWindowAndRenderer("Map Viewer", 1280, 720, SDL_WINDOW_TRANSPARENT, &context.Window, &context.Renderer) == false)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			bInitSDL = false;
		}
		else
		{
			//SDL_SetRenderDrawColor(context.Renderer, 0x026, 0x026, 0x26, 0xFF);
		}
		// SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE
	}

	float main_scale = 1.0f;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup scaling
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(context.Window, context.Renderer);
	ImGui_ImplSDLRenderer3_Init(context.Renderer);
}
