#include "stdafx.h"
#include "world.h"
#include "mainMenu.h"
#include "music.h"
using namespace std;


int counter=0;

HGE* hge = NULL;
World* _world = 0;
MainMenu* _menu = 0;
bool FrameFunc()
{
	// Get the time elapsed since last call of FrameFunc().
	// This will help us to synchronize on different
	// machines and video modes.
	float dt =hge->Timer_GetDelta();

	// Process keys
	if (_menu->GetState()==sNone&&hge->Input_GetKeyState(HGEK_ESCAPE))
	{
		return true;
	}
	if(!_world->IsGameOver() && _menu->GetState() == sPlay )
	{
		_world->Update(dt);
	}
	/*if(!_world->IsGameOver() && _world->IsGamePaused() )
	{
		_menu->Update(dt);
	}*/
	 if(_world->IsGameOver() )
	{
		World * tmpWorld=_world;
		_world=new World(_world->GetRenderer(),_world->GetScreenWidth(),_world->GetScreenHeight(),_world->GetScreenBPP()
			,_world->GetFieldWidth(),_world->GetFieldHeight(),_world->IsMusic(), _world->GetMenu());
		_menu->SetState(0);
		delete tmpWorld;
	}
	else
	{	
		_menu->Update(dt);
		if(_menu->GetState() == sExit) return true;
	}
	


	// Continue execution
	return false;

}

// This function will be called by HGE when
// the application window should be redrawn.
// Put your rendering code here.
bool RenderFunc()
{
	// Begin rendering quads.
	// This function must be called
	// before any actual rendering.
	hge->Gfx_BeginScene();

	// Clear screen with black color
	hge->Gfx_Clear(0);
	if(!_world->IsGameOver() && _menu->GetState() == sPlay)
	{
		_world->Draw();
	}
	else
	{
		_menu->Draw();
	}
	// End rendering and update the screen
	hge->Gfx_EndScene();
	// RenderFunc should always return false
	return false;
}




int GetSettingFromFile(int choice)
{
	string str;
	fstream f("data/configs/config.txt", ios::in);
	int width=0, height=0, screenBPP=0,fieldWidth=0,fieldHeight=0;
	bool isMusic;
	if (f.is_open())
	{
		counter=0;
		while (!f.eof())
		{
			getline(f,str);
			if (!str.empty())
			{
				if (str[0]>=48 && str[0]<=57)
				{
					for(int i=0;i<str.length();i++)
					{
						if (counter == 0)
						{
							width = width * 10 + (str[i] - 48);
						}
						if (counter == 1)
						{
							height = height * 10 + (str[i] - 48);
						}
						if (counter == 2)
						{
							screenBPP = screenBPP * 10 + (str[i] - 48);
						}
						if(counter==3)
						{
							fieldWidth = fieldWidth * 10 + (str[i] - 48);	
						}
						if(counter==4)
						{
							fieldHeight = fieldHeight * 10 + (str[i] - 48);	
						}
					}
					counter++;
				}
				if (isalpha(str[0]) && (str.compare("false") == 0 || str.compare("true") == 0))
				{
					if (str.compare("false") == 0)
					{
						isMusic = false;
					}
					else
					{
						isMusic = true;
					}

				}


			}
		}
		f.close();
	}

	switch(choice)
	{
	case 1:
		return width;
		break;
	case 2:
		return height;
		break;
	case 3:
		return screenBPP;
		break;
	case 4:
		return fieldWidth;
		break;
	case 5:
		return fieldHeight;
		break;
	case 6:
		return isMusic;
		break;
	}
}


const int screenBPP=GetSettingFromFile(3);
const int screenHeight=GetSettingFromFile(2);
const int screenWidth=GetSettingFromFile(1);
const int fieldWidth=GetSettingFromFile(4);
const int fieldHeight=GetSettingFromFile(5);
const bool music=GetSettingFromFile(6);

int _tmain(int argc, _TCHAR* argv[])
{
	AllocConsole();
	HWND console = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(console,0);

	// Get HGE interface
	hge = hgeCreate(HGE_VERSION);

	// Set up log file, frame function, render function and window title
	hge->System_SetState(HGE_LOGFILE, "tetris.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "Tetris");

	// Set up video mode
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, screenWidth);
	hge->System_SetState(HGE_SCREENHEIGHT, screenHeight);
	hge->System_SetState(HGE_SCREENBPP, screenBPP);


	if(hge->System_Initiate())
	{
		_menu = new MainMenu(hge,screenWidth,screenHeight,screenBPP);
		_world=new World(hge,screenWidth,screenHeight,screenBPP,fieldWidth,fieldHeight,music,_menu);
		hge->System_Start();
		if(_menu);
		delete _menu;
		if(_world)
			delete _world;

	}

	// Clean up and shutdown
	hge->System_Shutdown();
	hge->Release();
	return 0;
}