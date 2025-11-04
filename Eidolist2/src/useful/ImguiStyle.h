#pragma once
#include "imgui.h"
#include "SDLTypes.h"

// enjoy my random made up colour names :]
enum EStyleColor
{
	Null = 0,
	CozyBlueDark =		0x282e3b,
	CozyBlueDarker =	0x1f2029,
	CozyMossLight =		0x739a70,
	CozyMossMid =		0x4d6659,
	CozyMossDark =		0x343f41,

	FallYellow =		0xefac28,
	FallOrangeLight =	0xef692f,
	FallOrange =		0xef3a0c,
	FallRed =			0x9b1a0a,
	FallRedDark =		0x550f0a,
	FallBrownLight =	0x773421,
	FallBrownDark =		0x550f0a,

	SuperSalmon =		0xfe483c,
	SuperSand =			0xf6ba66,
	SuperSeaGreen =		0x41936e,

	PaleYellow =		0xece0cf,
	PastelYellow =		0xf8d098,
	PastelGreen =		0x69aa8d,
	Lilac =				0xbfa5c9,
	Ghost =				0x393754,
	VLGrey = 0xc2c2c2,
	LGrey = 0x8c8c8c,
	Grey = 0x575757,
	DGrey = 0x454545,

	White = 0xffffff
};

namespace color
{
	void hexToRGB(int hexValue, float& r, float& g, float& b);

	ImColor GetImColor(EStyleColor color, float alpha = 1.0f);

	void PushStyleColor(ImGuiCol idx, EStyleColor color, float alpha = 1.0f);

	void SetSDLRenderColor(SDLContext& context, EStyleColor color, float alpha = 1.0f);
}

static void PushStyleCompact()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, (float)(int)(style.FramePadding.y * 0.60f));
	ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, (float)(int)(style.ItemSpacing.y * 0.60f));
}

static void PopStyleCompact()
{
	ImGui::PopStyleVar(2);
}

static std::vector<int> _sampleColorPalette = {
0x2e222f,
0x3e3546,
0x625565,
0x966c6c,
0xab947a,
0x694f62,
0x7f708a,
0x9babb2,
0xc7dcd0,
0xffffff,
0x6e2727,
0xb33831,
0xea4f36,
0xf57d4a,
0xae2334,
0xe83b3b,
0xfb6b1d,
0xf79617,
0xf9c22b,
0x7a3045,
0x9e4539,
0xcd683d,
0xe6904e,
0xfbb954,
0x4c3e24,
0x676633,
0xa2a947,
0xd5e04b,
0xfbff86,
0x165a4c,
0x239063,
0x1ebc73,
0x91db69,
0xcddf6c,
0x313638,
0x374e4a,
0x547e64,
0x92a984,
0xb2ba90,
0x0b5e65,
0x0b8a8f,
0x0eaf9b,
0x30e1b9,
0x8ff8e2,
0x323353,
0x484a77,
0x4d65b4,
0x4d9be6,
0x8fd3ff,
0x45293f,
0x6b3e75,
0x905ea9,
0xa884f3,
0xeaaded,
0x753c54,
0xa24b6f,
0xcf657f,
0xed8099,
0x831c5d,
0xc32454,
0xf04f78,
0xf68181,
0xfca790,
0xfdcbb0,
};