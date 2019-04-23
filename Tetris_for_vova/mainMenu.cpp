#include "stdafx.h"
#include "mainMenu.h"
#include "menuitem.h"
#include "music.h"

MainMenu::MainMenu(HGE* renderer,int screenWidth,int screenHeight,int screenBPP): 
_renderer(renderer), _state(sNone)
{
	_music=new Music(_renderer);
	_screenWidth=screenWidth;
	_screenBPP=screenBPP;
	_screenHeight=screenHeight;
	maxObjects=50*((_screenWidth/800)+(_screenHeight/600));
	bgtex = _renderer->Texture_Load("data/textures/tetriswallpaper.png");
	tex = _renderer->Texture_Load("data/textures/Tetris_block_cyan.png");
	_tex = _renderer->Texture_Load("data/textures/cursor.png");
	fnt=new hgeFont("data/fonts/somefnt.fnt");
	fnt->SetColor(0xFFFF80A0);
	_fpsFnt=new hgeFont("data/fonts/somefnt.fnt");
	_fpsFnt->SetColor(0xFF00FF00);
	_fnt = new hgeFont("data/fonts/somefnt.fnt");
	_spr = new hgeSprite(_tex,0,0,32,32);
	spr = new hgeSprite(tex, 0, 0, 30, 30);
	spr->SetHotSpot(32, 32);

	bgspr = new hgeSprite(bgtex, 0, 0, _screenWidth,_screenHeight);
	bgspr->SetBlendMode(BLEND_COLORADD | BLEND_ALPHABLEND | BLEND_NOZWRITE);
	bgspr->SetColor(0xFF000000, 0);
	bgspr->SetColor(0xFF000000, 1);
	bgspr->SetColor(0xFF000040, 2);
	bgspr->SetColor(0xFF000040, 3);

	pObjects = new sprObject[maxObjects];

	for (int i = 0; i < maxObjects; i++) 
	{
		pObjects[i].x = _renderer->Random_Float(0,_screenWidth);
		pObjects[i].y = _renderer->Random_Float(0,_screenHeight);
		pObjects[i].dx = _renderer->Random_Float(-200, 200);
		pObjects[i].dy = _renderer->Random_Float(-200, 200);
		pObjects[i].scale = _renderer->Random_Float(0.5f, 2.0f);
		pObjects[i].dscale = _renderer->Random_Float(-1.0f, 1.0f);
		pObjects[i].rot = _renderer->Random_Float(0,M_PI * 2);
		pObjects[i].drot = _renderer->Random_Float(-1.0f, 1.0f);
	}
	SetBlend(0);

	// Create and initialize the GUI
	_gui = new hgeGUI();
	int incrementer=0;

	_gui->AddCtrl(new hgeGUIMenuItem(1, fnt, _music->PlayMenuSound(), _screenWidth/2, _screenHeight/3+incrementer, 0.0f, "Play"));
	incrementer+=_screenHeight/12;
	_gui->AddCtrl(new hgeGUIMenuItem(2, fnt, _music->PlayMenuSound(), _screenWidth/2, _screenHeight/3+incrementer, 0.1f, "Countinue"));
	incrementer+=_screenHeight/12;
	_gui->AddCtrl(new hgeGUIMenuItem(3, fnt, _music->PlayMenuSound(), _screenWidth/2, _screenHeight/3+incrementer, 0.2f, "Options"));
	incrementer+=_screenHeight/12;
	_gui->AddCtrl(new hgeGUIMenuItem(4, fnt, _music->PlayMenuSound(), _screenWidth/2, _screenHeight/3+incrementer, 0.3f, "Exit"));

	_gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
	_gui->SetCursor(_spr);
	_gui->SetFocus(1);
	_gui->Enter();
	
}
void MainMenu::SetBlend(int blend)
{
	static int sprBlend[5]=
	{
		BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE,
		BLEND_COLORADD | BLEND_ALPHABLEND | BLEND_NOZWRITE,
		BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE,
		BLEND_COLORMUL | BLEND_ALPHAADD   | BLEND_NOZWRITE,
		BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE
	};
	static DWORD fntColor[5]=
	{
		0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF
	};
	static DWORD sprColors[5][5]=
	{
		{ 0xFFFFFFFF, 0xFFFFE080, 0xFF80A0FF, 0xFFA0FF80, 0xFFFF80A0 },
		{ 0xFF000000, 0xFF303000, 0xFF000060, 0xFF006000, 0xFF600000 },
		{ 0x80FFFFFF, 0x80FFE080, 0x8080A0FF, 0x80A0FF80, 0x80FF80A0 },
		{ 0x80FFFFFF, 0x80FFE080, 0x8080A0FF, 0x80A0FF80, 0x80FF80A0 },
		{ 0x40202020, 0x40302010, 0x40102030, 0x40203010, 0x40102030 }
	};
	if(blend>4) blend=0;
	nBlend=blend;

	spr->SetBlendMode(sprBlend[blend]);
	_fnt->SetColor(fntColor[blend]);
	for(int i=0;i<maxObjects;i++) 
	{
		pObjects[i].color=sprColors[blend][_renderer->Random_Int(0,4)];
	}
}
MainMenu::~MainMenu()
{
	_renderer->Texture_Free(_tex);
	_renderer->Texture_Free(tex);
	_renderer->Texture_Free(bgtex);

	delete _fpsFnt;
	delete spr;
	delete _spr;
	delete _gui;
	delete _fnt;
	delete fnt;

}
void MainMenu::Draw()
{
	bgspr->Render(0,0);
	for(int i=0;i<maxObjects;i++)
	{
		spr->SetColor(pObjects[i].color); 
		spr->RenderEx(pObjects[i].x, pObjects[i].y, pObjects[i].rot, pObjects[i].scale);
	}
	_gui->Render();
	_fpsFnt->printf(7, 7, HGETEXT_LEFT, "FPS: %d",_renderer->Timer_GetFPS());
}
int MainMenu::GetState()
{
	return _state;
}
void MainMenu::SetState(int otherState)
{
	_state=otherState; 
}

void MainMenu::Update(float dt)
{
	static int lastid = 0;
	int id = _gui->Update(dt);
	_renderer->Effect_Play(_music->PlayMenuMusic());

	if(id == -1)
	{
		switch(lastid)
		{
		case 1:
			_state = sPlay;
			_renderer->Effect_Free(_music->PlayMenuMusic());
		case 2:
		case 3:
			_gui->SetFocus(1);
			_gui->Enter();
			break;
		case 4:
			_state = sExit;
			break;
		}
	}
	else if(id)
	{ 
		lastid = id;
		_gui->Leave();
	}
	for (int i = 0; i < maxObjects; i++) 
	{
		pObjects[i].x += pObjects[i].dx * dt/3;
		if (pObjects[i].x > _screenWidth || pObjects[i].x < 0) pObjects[i].dx = -pObjects[i].dx;
		pObjects[i].y += pObjects[i].dy * dt/3;
		if (pObjects[i].y > _screenHeight || pObjects[i].y < 0) pObjects[i].dy = -pObjects[i].dy;
		pObjects[i].scale += pObjects[i].dscale * dt/3;
		if (pObjects[i].scale > 2 || pObjects[i].scale < 0.5) pObjects[i].dscale = -pObjects[i].
			dscale;
		pObjects[i].rot += pObjects[i].drot * dt;
	}
}
