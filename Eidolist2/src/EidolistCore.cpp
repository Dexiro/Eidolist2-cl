#include "EidolistCore.h"
#include "MapLayers.h"
#include "WindowArea.h"
#include "ImguiStyle.h"
#include "meta/WikiData.h"
#include "GLMIncludes.h"
#include "SDLFun.h"

SDL_Semaphore* gSemaphore = nullptr;


EidolistCore::EidolistCore()
{
}

EidolistCore::~EidolistCore()
{
}


void EidolistCore::Init(SDLContext& context)
{
	wiki::GetWikiData(global::wikiData);

	global::visibleLayers[mlDefault] = true;
	global::visibleLayers[mlPano] = true;
	global::visibleLayers[mlLower] = true;
	global::visibleLayers[mlUpper] = true;
	global::visibleLayers[mlCharset] = true;
	global::visibleLayers[mlCollision] = false;
	global::visibleLayers[mlEvent] = false;
	global::visibleLayers[mlEventTags] = true;
	global::visibleLayers[mlTransferLines] = false;

	m_data.m_windowSize = glm::ivec2(0);
	SDL_GetWindowSizeInPixels(context.Window, &m_data.m_windowSize.x, &m_data.m_windowSize.y);


	m_data.m_spritesheet = std::make_unique<Spritesheet>();
	m_data.m_spritesheet->LoadSpritesheet(context, TEMPCONFIGS + "DebugTiles.png", TEMPCONFIGS + "DebugTiles.json");

	m_data.m_mapCursor.SetSprite(*m_data.m_spritesheet, "MapCursor");
	m_data.m_selBox.SetSprite(*m_data.m_spritesheet, "SelBoxBlue");
	m_data.m_highlight.SetSprite(*m_data.m_spritesheet, "Highlight1", false);
	m_data.m_highlightSelect.SetSprite(*m_data.m_spritesheet, "HighlightSelect", false);

	// timer used to trigger Update calls at fixed intervals
	m_data.m_targetFps = 60;
	m_data.m_targetMSpf = 1000 / m_data.m_targetFps;
	m_data.m_nextUpdateTick = m_data.m_targetMSpf;

	m_data.m_projectGui = std::make_shared<ProjectGui>(0);
	m_data.m_projectGui->m_debugSpritesheet = m_data.m_spritesheet.get();

	m_data.m_projectSelect = std::make_unique<ProjectSelectDialog>(m_data.m_projectGui.get());

	m_data.m_mainWindowBGCol = EStyleColor::CozyBlueDark;
	m_data.m_mainWindowGridCol = EStyleColor::CozyBlueDarker;

	//color::PushStyleColor(ImGuiCol_MenuBarBg, CozyMossDark);
	//color::PushStyleColor(ImGuiCol_FrameBg, CozyMossLight);
	//ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6);

	//ImVec4* colors = GetStyle().Colors;
	//colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	//colors[ImGuiCol_WindowBg] = ImVec4(0.30f, 0.43f, 0.34f, 0.89f);
	//colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.23f);


}

void EidolistCore::FixedUpdate(SDLContext& context, ImGuiIO& io)
{
	if (m_data.m_updateTimer.getTicks() >= m_data.m_nextUpdateTick)
	{
		m_data.m_nTicks++;
		double dt = m_data.m_updateTimer.getTicks() / 1000.0;
		m_data.m_nextUpdateTick = m_data.m_targetMSpf - (m_data.m_updateTimer.getTicks() % m_data.m_targetMSpf);
		m_data.m_updateTimer.start();
		for (int i = 0; i < m_data.m_projectGui->NMapsLoaded(); i++)
		{
			m_data.m_projectGui->GetMapObjByID(i)->FixedUpdate(context, m_data.m_nTicks, dt);
		}
		//if(startTree)
		//	tree.Update(dt);
	}
}

bool EidolistCore::Run(SDLContext& context)
{
	Init(context);

	ImGuiIO& io = ImGui::GetIO();

	m_data.m_updateTimer.start();
	while (!m_data.m_quit)
	{
		SDL_GetWindowSizeInPixels(context.Window, &m_data.m_windowSize.x, &m_data.m_windowSize.y);

		SDLInput(m_data, io);
		FixedUpdate(context, io);

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		if (m_data.show_imgui_demo)
			ImGui::ShowDemoWindow(&m_data.show_imgui_demo);

		ResetScreen(context, io);
		DrawGrid(m_data.m_worldPos, m_data.m_worldScale, m_data.m_windowSize, context);
		
		ShowMainMenu(context);

		bool showMapObjs = false;
		switch (m_data.m_activeTab)
		{
		case EMainMenuTab::mtLoadProject:
			m_data.m_projectSelect->ShowMainWindow(context);
			showMapObjs = true;
			break;
		case EMainMenuTab::mtMapTree:
			m_data.m_projectGui->Show(context);
			m_data.m_projectGui->LoadRequestedMaps(context, m_data.m_mapCaret);
			showMapObjs = true;
			break;
		case EMainMenuTab::mtDatabase:
			m_data.m_compareProjectData.ShowMainWindow(context, m_data.m_projectGui);
			break;
		case EMainMenuTab::mtMapComparison:
		case EMainMenuTab::mtWorldGraph:
			break;
		default:
			showMapObjs = true;
			break;
		};
		
		if (showMapObjs)
		{
			if (m_data.show_layer_window)
				ShowLayerMenu(context);

			m_data.m_selectedObjects_hover.clear();

			for (int mapIndex = 0; mapIndex < m_data.m_projectGui->NMapsLoaded(); mapIndex++)
			{
				MapObj* obj = m_data.m_projectGui->GetMapObjByID(mapIndex);
				if (obj)
				{
					obj->Draw(context, m_data.m_worldPos, m_data.m_worldScale);
					if (m_data.m_drag_select)
					{
						if (obj->RectCollision(m_data.m_selBox.m_dstRect, m_data.m_worldPos, m_data.m_worldScale))
						{
							m_data.m_selectedObjects_hover.push_back(obj);
							m_data.m_highlight.m_dstRect = obj->MapRect(m_data.m_worldPos, m_data.m_worldScale);
							m_data.m_highlight.Draw(context);
						}
					}

					for (auto& ev : obj->m_mapData->event_data)
					{
						SDL_SetRenderDrawColor(context.Renderer, 0xFF, 0xFF, 0xFF, 0xFF);

						glm::vec2 coord = { (float)ev.pos.x, (float)ev.pos.y };
						coord *= 16.0f;
						coord += obj->m_pos + glm::vec2(8.0f, 8.0f);
						mathf::worldToScreen(coord, coord, m_data.m_worldPos, m_data.m_worldScale);

						std::string strID = fmt::format("{}", ev.lcfEvent.ID);
						//SDL_FRect rect = { ev.pos.x - 4.0f, ev.pos.y - 4.0f, 8.0f, 8.0f };
						//SDL_RenderFillRect(context.Renderer, &rect);
						ShowTextAtPos(context, strID, coord, ev.lcfEvent.ID);
					}
				}
			}

			if (m_data.m_drag_select)
			{
				m_data.m_selBox.Draw(context);
			}

			DrawMapEventTags(m_data.m_worldPos, m_data.m_worldScale, context);
			ShowMapInfo(m_data.m_worldPos, m_data.m_worldScale, context);

			glm::vec2 caretPos = glm::vec2(0.0f);
			mathf::worldToScreen(caretPos, m_data.m_mapCaret, m_data.m_worldPos, m_data.m_worldScale);
			m_data.m_mapCursor.Draw(context, caretPos);
		}

		//tree.Draw(context, worldPos, worldScale);
		ImGui::Render();
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), context.Renderer);

		//Update screen
		SDL_RenderPresent(context.Renderer);
	}


	return true;
}

void EidolistCore::SDLInput(AppData& data, ImGuiIO& io)
{
	data.m_mousePress = false;
	data.m_mouseRelease = false;
	while (SDL_PollEvent(&data.m_e) != 0)
	{
		ImGui_ImplSDL3_ProcessEvent(&data.m_e);

		//User requests quit
		if (data.m_e.type == SDL_EVENT_QUIT)
		{
			data.m_quit = true;
		}
		if (data.m_e.type == SDL_EVENT_MOUSE_BUTTON_UP)
		{
			data.m_mouseRelease = true;
		}
		if (data.m_e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && !io.WantCaptureMouse)
		{
			data.m_mousePress = true;
			data.m_mouseAnchor = data.m_mousePos;
			data.m_worldPosAnchor = data.m_worldPos;
		}

		if (data.m_e.type == SDL_EVENT_MOUSE_WHEEL && !io.WantCaptureMouse)
		{
			glm::vec2 offset = glm::vec2(0.0f); //= (mousePos - worldPos) / worldScale;
			mathf::screenToWorld(offset, data.m_mousePos, data.m_worldPos, data.m_worldScale);

			if (data.m_e.wheel.y == -1) //negative means the scroll wheel has gone away from the user
			{
				// Zoom Out
				if (data.m_worldScale.x > 0.1f)
				{
					data.m_worldScale /= 1.25f;
				}
			}
			else if (data.m_e.wheel.y == 1) //vice-versa
			{
				// Zoom In
				if (data.m_worldScale.x < 4.0f)
				{
					data.m_worldScale *= 1.25f;
				}
			}

			mathf::worldToScreen(offset, offset, data.m_worldPos, data.m_worldScale);
			data.m_worldPos += (data.m_mousePos - offset) / data.m_worldScale;
		}
	}

	Uint32 state = SDL_GetMouseState(&data.m_mousePos.x, &data.m_mousePos.y);
	if (state & SDL_BUTTON_LMASK)
	{
		if (data.m_mousePress)
		{
			data.m_drag_map = true;

			//if()
		}
	}
	else
	{
		data.m_drag_map = false;
	}
	if (state & SDL_BUTTON_RMASK)
	{
		data.m_rmdown = true;
		if (data.m_mousePress)
		{
			data.m_drag_select = true;
		}
	}
	else
	{
		data.m_drag_select = false;

		if (data.m_rmdown)
		{
			data.m_selectedObjects.clear();
			for (auto& obj : data.m_selectedObjects_hover)
			{
				data.m_selectedObjects.push_back(obj);
			}
			data.m_selectedObjects_hover.clear();
			data.m_rmdown = false;
		}
	}
	if (data.m_drag_map)
	{
		if (m_dragTarget < 0 || m_dragLock)
		{
			data.m_worldPos = data.m_worldPosAnchor + ((data.m_mousePos - data.m_mouseAnchor) / data.m_worldScale);
		}
		else
		{

		}
	}
	if (data.m_drag_select)
	{
		glm::vec2 posA = { std::min(data.m_mousePos.x, data.m_mouseAnchor.x), std::min(data.m_mousePos.y, data.m_mouseAnchor.y) };
		glm::vec2 posB = { std::max(data.m_mousePos.x, data.m_mouseAnchor.x), std::max(data.m_mousePos.y, data.m_mouseAnchor.y) };
		glm::vec2 selSize = posB - posA;
		data.m_selBox.m_dstRect = { posA.x, posA.y, selSize.x, selSize.y };
	}
	else
	{

	}
}

void EidolistCore::DrawGrid(glm::vec2& worldPos, glm::vec2& worldScale, glm::ivec2& windowSize, SDLContext& context)
{
	int gridSize = 128;
	int sx1 = ((int)worldPos.x % gridSize) * worldScale.x;
	for (float sx = sx1; sx < windowSize.x; sx += gridSize * worldScale.x)
	{
		color::SetSDLRenderColor(context, EStyleColor::CozyBlueDarker);
		SDL_RenderLine(context.Renderer, sx, 0, sx, windowSize.y);
	}
	int sy1 = ((int)worldPos.y % gridSize) * worldScale.y;
	for (float sy = sy1; sy < windowSize.y; sy += gridSize * worldScale.y)
	{
		color::SetSDLRenderColor(context, EStyleColor::CozyBlueDarker);
		SDL_RenderLine(context.Renderer, 0, sy, windowSize.x, sy);
	}
	int sy = ((int)worldPos.y * worldScale.y);
	SDL_SetRenderDrawColor(context.Renderer, 0x00, 0x40, 0x50, 0xFF);
	SDL_RenderLine(context.Renderer, 0, sy, windowSize.x, sy);
	int sx = ((int)worldPos.x * worldScale.x);
	SDL_SetRenderDrawColor(context.Renderer, 0x00, 0x40, 0x50, 0xFF);
	SDL_RenderLine(context.Renderer, sx, 0, sx, windowSize.y);
}

void EidolistCore::DrawMapEventTags(const glm::vec2& worldPos, const glm::vec2& worldScale, SDLContext& context)
{
	int tagid = 0;
	auto& proj = m_data.m_projectGui;

	if (proj->NMapsLoaded() <= 0)
		return;

	PushStyleCompact();

	for (int mapIndex = 0; mapIndex < proj->NMapsLoaded(); mapIndex++)
	{
		MapObj* obj = proj->GetMapObjByID(mapIndex);
		if (obj)
		{
			if (global::visibleLayers[EMapLayer::mlEventTags])
			{
				for (auto transfer : obj->m_mapData->map_transfers)
				{
					if (transfer.mapName == "")
					{
						transfer.mapName = proj->GetMapData(transfer.mapID)->map_name;
					}

					std::string tagText = fmt::format("{},{}", transfer.dstCoord.x, transfer.dstCoord.y);

					glm::vec2 coord = { (float)transfer.srcCoord.x, (float)transfer.srcCoord.y };
					coord *= 16.0f;
					coord += obj->m_pos + glm::vec2(8.0f, 8.0f);
					mathf::worldToScreen(coord, coord, worldPos, worldScale);

					if (transfer.mapID == transfer.srcMapID)
					{
						glm::vec2 coord2 = { (float)transfer.dstCoord.x, (float)transfer.dstCoord.y };
						coord2 *= 16.0f;
						coord2 += obj->m_pos + glm::vec2(8.0f, 8.0f);
						mathf::worldToScreen(coord2, coord2, worldPos, worldScale);

						//std::string tagText = fmt::format("{}:{}", coord.x, coord.y);
						SDL_SetRenderDrawColor(context.Renderer, 0xFF, 0xFF, 0x00, 0xFF);

						if (global::visibleLayers[EMapLayer::mlTransferLines])
							sdlf::DrawArrow(context, coord, coord2, false);

						//SDL_RenderLine(context.Renderer, coord.x, coord.y, coord2.x, coord2.y);
						ShowTextAtPos(context, tagText, coord, tagid++, false);
					}
					else
					{
						SDL_SetRenderDrawColor(context.Renderer, 0xFF, 0x00, 0x00, 0xFF);

						SDL_FRect rect = { coord.x - 4.0f, coord.y - 4.0f, 8.0f, 8.0f };
						SDL_RenderFillRect(context.Renderer, &rect);
						ShowTextAtPos(context, transfer.mapName, coord, tagid++);
					}
				}
			}
		}
	}

	PopStyleCompact();
}

void EidolistCore::MenuTabToggle(EMainMenuTab tab_id, std::string button_text, EMainMenuTab& switch_tab)
{
	bool tab_is_active = (m_data.m_activeTab == tab_id);

	if (tab_is_active)
		color::PushStyleColor(ImGuiCol_Button, EStyleColor::FallOrangeLight);
	else
		color::PushStyleColor(ImGuiCol_Button, EStyleColor::CozyMossMid);

	if (ImGui::Button(button_text.c_str()))
		switch_tab = tab_is_active ? EMainMenuTab::mtNone : tab_id;

	ImGui::PopStyleColor();
}

void EidolistCore::ShowMainMenu(SDLContext& context)
{
	if (ImGui::BeginMainMenuBar())
	{
		EMainMenuTab switch_tab = EMainMenuTab::mtNULL;

		MenuTabToggle(EMainMenuTab::mtLoadProject, "projects", switch_tab);

		ImGui::BeginDisabled(!m_data.m_projectGui->Data());
		MenuTabToggle(EMainMenuTab::mtMapTree, "map tree", switch_tab);
		MenuTabToggle(EMainMenuTab::mtDatabase, "database", switch_tab);
		ImGui::EndDisabled();

		//ImGui::BeginDisabled();
		//color::PushStyleColor(ImGuiCol_Text, EStyleColor::White, 0.4f);
		//MenuTabToggle(EMainMenuTab::mtMapComparison, "map comparison", switch_tab);
		//MenuTabToggle(EMainMenuTab::mtWorldGraph, "world graph", switch_tab);
		//ImGui::PopStyleColor(1);
		//ImGui::EndDisabled();

		if (switch_tab != EMainMenuTab::mtNULL)
			m_data.m_activeTab = switch_tab;

		ImGui::Separator();

		//PushStyleCompact();
		color::PushStyleColor(ImGuiCol_Button, EStyleColor::CozyMossMid);
		color::PushStyleColor(ImGuiCol_FrameBg, EStyleColor::CozyMossMid);
		color::PushStyleColor(ImGuiCol_CheckMark, EStyleColor::CozyBlueDarker, 0.8f);

		bool hasWiki = !global::wikiData.locations.empty();

		if (m_data.m_expandMainMenu)
		{
			if (hasWiki)
			{
				color::PushStyleColor(ImGuiCol_Button, EStyleColor::DGrey);
				color::PushStyleColor(ImGuiCol_Text, EStyleColor::VLGrey);
				color::PushStyleColor(ImGuiCol_ButtonHovered, EStyleColor::Grey);
				color::PushStyleColor(ImGuiCol_ButtonActive, EStyleColor::LGrey);
			}

			if (ImGui::Button("request wiki data"))
				wiki::GetWikiData(global::wikiData, true);

			if (hasWiki)
				ImGui::PopStyleColor(4);

			ImGui::Checkbox("layer options", &m_data.show_layer_window);
			ImGui::Checkbox("gui demo (dev)", &m_data.show_imgui_demo);
		}
		ImGui::PopStyleColor(3);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		color::PushStyleColor(ImGuiCol_Button, EStyleColor::Ghost);

		bool toggleExpand = false;
		if (!m_data.m_expandMainMenu)
			toggleExpand = ImGui::Button("more");

		if (m_data.m_expandMainMenu)
			toggleExpand = ImGui::Button("less");

		if (toggleExpand)
			m_data.m_expandMainMenu = !m_data.m_expandMainMenu;

		//PopStyleCompact();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::EndMainMenuBar();
	}
}

void EidolistCore::CheckSDLInput(SDL_Event& e, bool& quit, ImGuiIO& io, bool& drag_map, glm::vec2& mouseAnchor, glm::vec2& mousePos, glm::vec2& worldPosAnchor, glm::vec2& worldPos, glm::vec2& worldScale)
{

}

void EidolistCore::ShowTextAtPos(SDLContext& context, std::string text, glm::vec2 screenPos, int uid, bool bg)
{
	glm::ivec2 windowSize;
	SDL_GetWindowSizeInPixels(context.Window, &windowSize.x, &windowSize.y);

	if (screenPos.x > 0 && screenPos.x < windowSize.x
		&& screenPos.y > 0 && screenPos.y < windowSize.y)
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs;
		if (!bg)
		{
			flags |= ImGuiWindowFlags_NoBackground;
		}
		
		ImGui::SetNextWindowBgAlpha(0.75f);
		ImGui::SetNextWindowBgAlpha(1.00f);
		ImGui::SetNextWindowPos(ImVec2(screenPos.x, screenPos.y));
		std::string windowName = fmt::format("trlb{}", uid);
		ImGui::Begin(windowName.c_str(), 0, flags);
		ImGui::TextUnformatted(text.c_str());
		ImGui::End();
	}
}

void EidolistCore::ShowLayerMenu(SDLContext& context)
{
	glm::ivec2 windowSize;
	SDL_GetWindowSizeInPixels(context.Window, &windowSize.x, &windowSize.y);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar;
	ImGui::SetNextWindowBgAlpha(0.5f);
	ImGui::SetNextWindowSize(ImVec2(128, 200), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.88f, windowSize.y * 0.05f));
	std::string windowName = fmt::format("Layers");
	if (!ImGui::Begin(windowName.c_str(), &m_data.show_layer_window, flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	for (int i = 0; i < EMapLayer::COUNT; i++)
	{
		ImGui::Checkbox(_mlNames[i].c_str(), &global::visibleLayers[i]);
	}

	ImGui::End();
}

void EidolistCore::ResetScreen(SDLContext& context, ImGuiIO& io)
{
	SDL_SetRenderScale(context.Renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);

	if (m_data.light_mode)
	{
		SDL_SetRenderDrawColor(context.Renderer, 0x7C, 0xA3, 0xF2, 0xFF);
	}
	else
	{
		color::SetSDLRenderColor(context, m_data.m_mainWindowBGCol);
	}
	SDL_RenderClear(context.Renderer);
}

void EidolistCore::ShowMapInfo(const glm::vec2& worldPos, const glm::vec2& worldScale, SDLContext& context)
{
	if (!m_data.m_selectedObjects.empty())
	{
		static rect::FRect windowArea = warea::GetArea(context, { 0.8f, 0.6f }, { 0.18f, 0.38f });
		ImGui::SetNextWindowSize({ windowArea.Size().x, windowArea.Size().y }, ImGuiCond_Appearing);
		ImGui::SetNextWindowPos({ windowArea.Pos().x, windowArea.Pos().y }, ImGuiCond_Appearing);

		if (ImGui::Begin("Map Info", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			for (auto& obj : m_data.m_selectedObjects)
			{
				if (obj)
				{
					obj->ShowInfo(context);
					m_data.m_highlightSelect.m_dstRect = obj->MapRect(m_data.m_worldPos, m_data.m_worldScale);
					m_data.m_highlightSelect.m_dstRect = m_data.m_highlightSelect.m_dstRect.Expand(2.0f);
					m_data.m_highlightSelect.Draw(context);
				}
			}
		}
		ImGui::End();
	}
}
