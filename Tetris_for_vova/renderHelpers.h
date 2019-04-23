#pragma once

class HGE;
// RenderHelpers
class RenderHelpers
{
public:
	static void DrawRect(HGE* hge, const hgeRect& rect, int color);
	static void DrawRect(HGE* hge, const float l, const float t, const float r, const float b, int color);
	static void FillRect(HGE* hge, const hgeRect& rect, int color);
	static void FillRect(HGE* hge, const float l, const float t, const float r, const float b, int color);
};