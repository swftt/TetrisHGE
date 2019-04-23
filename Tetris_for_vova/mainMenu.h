#pragma once

#include "music.h"

class hgeGUI;
class hgeFont;
class hgeSprite;
class HGE;

enum MenuState {
	sNone,
	sPlay,
	sExit,
};
struct sprObject {
    float x, y;
    float dx, dy;
    float scale, rot;
    float dscale, drot;
    DWORD color;
};



// MainMenu
class MainMenu
{
public:
	MainMenu(HGE* renderer,int screenWidth,int screenHeight,int screenBPP);
	virtual ~MainMenu();

	void Draw();
	void Update(float dt);
	int GetState();
	void SetBlend(int blend);
	void SetState(int otherState);

private:
	HGE* _renderer;
	hgeGUI* _gui;
	hgeFont* _fnt;
	hgeSprite* _spr;
	hgeFont * fnt;
	hgeFont * _fpsFnt;
	hgeQuad square;
	hgeQuad quad;
	HTEXTURE _tex;
	hgeSprite* spr;
	hgeSprite* bgspr;
	HTEXTURE tex, bgtex;

	Music * _music;
	sprObject* pObjects;

	int maxObjects;
	int	nBlend;
	int _screenWidth,_screenHeight,_screenBPP;
	int _state;
};