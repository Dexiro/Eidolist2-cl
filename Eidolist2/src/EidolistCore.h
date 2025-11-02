#pragma once
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "MathFun.h"
#include "SDLFun.h"

#include "SDLTypes.h"
#include "MapDiff.h"
#include "AssetLoader.h"
#include "Tileset.h"
#include "TilesetHelper.h"
#include "TilesetHelper_Texture.h"
#include "Transform.h"
#include "projectdata/ProjectGui.h"
#include "projectdata/CompareProjectData.h"
#include "gui/ProjectSelectDialog.h"
#include "obj/MapObj.h"
#include "Spritesheet.h"
#include "GLMIncludes.h"
#include "gui/ImguiStyle.h"



enum EMainMenuTab
{
	mtNone,
	mtLoadProject,
	mtMapTree,
	mtDatabase,
	mtMapComparison,
	mtWorldGraph,
	mtNULL,
};

struct AppData
{
	bool m_quit = false;

	bool light_mode = false;
	bool show_imgui_demo = false;
	bool show_layer_window = true;
	//bool show_proj_workspace = true;

	EMainMenuTab m_activeTab = EMainMenuTab::mtLoadProject;

	CompareDataGui m_compareProjectData;
	PProjectGui m_projectGui;
	std::unique_ptr<ProjectSelectDialog> m_projectSelect;

	// update timing
	LTimer m_updateTimer;
	int m_targetFps = 0;
	int m_targetMSpf = 0;
	int m_nextUpdateTick = 0;
	int m_nTicks = 0;

	glm::ivec2 m_windowSize;

	// Spritesheet / UI Objects
	std::unique_ptr<Spritesheet> m_spritesheet;
	SpriteObj m_mapCursor;
	SpriteObj m_selBox;
	SpriteObj m_highlight;
	SpriteObj m_highlightSelect;

	std::vector<MapObj*> m_selectedObjects;
	std::vector<MapObj*> m_selectedObjects_hover;

	// World State
	glm::vec2 m_mapCaret = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_mousePos = glm::vec2(0);
	glm::vec2 m_mouseAnchor = glm::vec2(0);
	glm::vec2 m_worldPosAnchor = glm::vec2(0);
	glm::vec2 m_worldPos = glm::ivec2(0);
	glm::vec2 m_worldScale = glm::vec2(1);
	glm::vec2 m_mapVelocity = glm::vec2(0);

	bool m_drag_map = false;
	bool m_drag_select = false;
	bool m_lmdown = false;
	bool m_rmdown = false;
	bool m_mousePress = false;
	bool m_mouseRelease = false;

	EStyleColor m_mainWindowBGCol = EStyleColor::Null;
	EStyleColor m_mainWindowGridCol = EStyleColor::Null;

	bool m_expandMainMenu = false;

	SDL_Event m_e = SDL_Event();
};

class EidolistCore
{
public:
	EidolistCore();
	~EidolistCore();

	void Init(SDLContext& context);
	bool Run(SDLContext& context);

	void FixedUpdate(SDLContext& context, ImGuiIO& io);

	void SDLInput(AppData& data, ImGuiIO& io);

	void DrawGrid(glm::vec2& worldPos, glm::vec2& worldScale, glm::ivec2& windowSize, SDLContext& context);

	void DrawMapEventTags(const glm::vec2& worldPos, const glm::vec2& worldScale, SDLContext& context);
	void ShowMapInfo(const glm::vec2& worldPos, const glm::vec2& worldScale, SDLContext& context);
	void ShowWorldGraph(const glm::vec2& worldPos, const glm::vec2& worldScale, SDLContext& context);

protected:
	AppData m_data;

	int m_dragTarget = -1;
	bool m_dragLock = false;

	void ShowMainMenu(SDLContext& context);
	void ShowLayerMenu(SDLContext& context);
	void ShowTextAtPos(SDLContext& context, std::string text, glm::vec2 screenPos, int uid, bool bg = true);
	void CheckSDLInput(SDL_Event& e, bool& quit, ImGuiIO& io, bool& drag_map, glm::vec2& mouseAnchor, glm::vec2& mousePos, glm::vec2& worldPosAnchor, glm::vec2& worldPos, glm::vec2& worldScale);

	void MenuTabToggle(EMainMenuTab tab_id, std::string button_text, EMainMenuTab& switch_tab);

	void ResetScreen(SDLContext& context, ImGuiIO& io);
};
