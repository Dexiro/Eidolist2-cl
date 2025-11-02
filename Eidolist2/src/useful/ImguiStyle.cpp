#include "ImguiStyle.h"

void color::hexToRGB(int hexValue, float& r, float& g, float& b)
{
	r = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
	g = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
	b = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte
}

ImColor color::GetImColor(EStyleColor color, float alpha)
{
	float r, g, b;
	color::hexToRGB((int)color, r, g, b);
	return ImColor(r, g, b, alpha);
}

void color::PushStyleColor(ImGuiCol idx, EStyleColor color, float alpha)
{
	ImVec4 vec = GetImColor(color, alpha);
	ImGui::PushStyleColor(idx, vec);
}

void color::SetSDLRenderColor(SDLContext& context, EStyleColor color, float alpha)
{
	ImVec4 vec = GetImColor(color, alpha);
	SDL_SetRenderDrawColorFloat(context.Renderer, vec.x, vec.y, vec.z, vec.w);
}
