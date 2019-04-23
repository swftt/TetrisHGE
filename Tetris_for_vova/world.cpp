#include "stdafx.h"
#include "world.h"
#include "renderHelpers.h"
#include "figure.h"
#include "field.h"
using namespace std;

#define NORMAL__speedDOWN	1
#define FAST__speedDOWN		6

// World
World::World(HGE* renderer,int screenWidth,int screenHeight,int screenBPP,int fieldWidth,int fieldHeight,bool music,MainMenu * menu)
: _renderer(renderer)
{
	_menu=menu;
	this->music=new Music(_renderer);
	_screenWidth=screenWidth;
	_screenHeight=screenHeight;
	_screenBPP=screenBPP;
	fieldWidth>6 ? _fieldWidth=fieldWidth:_fieldWidth=9;
	fieldHeight>10 ? _fieldHeight=fieldHeight:_fieldHeight=16;
	_music=music;
	Clear();
	SetFonts();
	_field = new Field(this, _fieldWidth, _fieldHeight);
	SetFigures();
	UnNullGameMusic();
	GenerateNext();

}
World::~World()
{
	if(_field)
		delete _field;
	if(_figure)
		delete _figure;
	if(_next)
		delete _next;
	if(_fnt)
		delete _fnt;
	if(music)
		delete music;
	if(_spr)
		delete _spr;
	if(_fpsFnt)
		delete _fpsFnt;
	if(_nextFnt)
		delete _nextFnt;
	if(_fntText)
		delete _fntText;
	NullGameMusic();
	_renderer->Texture_Free(bgtex);
}
void World::NullGameMusic()
{
	_renderer->Effect_Free(gameMusic);
	gameMusic=0;
}
void World::UnNullGameMusic()
{
	gameMusic=music->PlayGameMusic();
}
void World::DrawBg()
{
	_spr->Render(0,0);
}

int World::GetCellWidth() const
{
	return _screenHeight/_fieldHeight;
}

int World::GetCellHeight() const
{
	return _screenHeight/_fieldHeight;
}

int World::ScoreForCell() const
{
	return 2*_level;
}

void World::AddScore(int score)
{
	_scores += score;
}

float World::GetSpeedHorizontal()
{
	return 10;
}

float World::GetNormalSpeedDown()
{
	return NORMAL__speedDOWN*_level;
}

float World::GetFastSpeedDown()
{
	return FAST__speedDOWN*_level;
}

void World::IncKillLine()
{
	_renderer->Effect_Play(music->PlayLineRemove());
	++_killLines;

	if((_killLines / 10)+1 > _level)
		++_level;
}

void World::Draw()
{
	DrawBg();
	_fpsFnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d", _renderer->Timer_GetDelta(), _renderer->Timer_GetFPS());
	_field->DrawBkg();
	_field->DrawField();

	if(_figure)
		_figure->Draw(_pos);

	_field->DrawWell();

	if(_next)
	{
		_nextFnt->printf(_pos.x+GetCellWidth()*_field->GetWidth()+_screenWidth/7, _pos.y+_screenHeight/9, HGETEXT_LEFT, "NEXT:");
		_next->Draw(hgeVector(_pos.x+GetCellWidth()*_field->GetWidth()+50, _pos.y+_screenHeight/6));
	}

	_fntText->printf(_pos.x+GetCellWidth()*_field->GetWidth()+_screenWidth/6, _pos.y+_screenHeight/2, HGETEXT_LEFT, "Scores: %i\n Level: %i", _scores, _level);
	_fntText->printf(_pos.x+GetCellWidth()*_field->GetWidth()+_screenWidth/6,_pos.y+_screenHeight/1.5,HGETEXT_LEFT,"Press P for \nPAUSE / UNPAUSE");
	if(IsGamePaused())
	{
		_fnt->printf(_screenWidth/2-385,_screenHeight/3, HGETEXT_LEFT, "Game is PAUSED");
	}

}

void World::SetState(int num)
{
	_state=num;
}
void World::Update(float dt)
{
	if(_gameOver)
	{
		_renderer->Effect_Free(gameMusic);
		return;
	}
	if(_figure)
	{
		if(_renderer->Input_GetKeyState(HGEK_ESCAPE))
		{
			SetPause(true);
			PauseGame();
			SetState(sNone);
		}

		if(!IsGamePaused())
		{
			if(gameMusic==music->PlayGameMusic())
				_renderer->Effect_Play(gameMusic);
			_figure->SetSpeedVertical(GetNormalSpeedDown());
			_figure->SetSpeedHorizontal(GetSpeedHorizontal());

			if (_renderer->Input_KeyUp(HGEK_UP))
			{
				_renderer->Effect_Play(music->PlayTurn());
				RotateLeftFigure();

			}
			if(!_figure->NeedMove())
			{
				if (_renderer->Input_GetKey() == HGEK_LEFT )
				{
					MoveLeftFigure();
				}

				if (_renderer->Input_GetKey() == HGEK_RIGHT)
				{
					MoveRightFigure();
				}
			}

			if (_renderer->Input_GetKeyState(HGEK_DOWN))
				MoveDownFigure();
		}
		if(_renderer->Input_GetKey() == HGEK_P)
		{
			_renderer->Effect_Play(music-> PlayPause());

			if(IsGamePaused())
			{
				UnNullGameMusic();
				SetPause(false);
				_figure->SetSpeedVertical(GetNormalSpeedDown());
				_figure->SetSpeedHorizontal(GetSpeedHorizontal());

			}
			else
			{
				NullGameMusic();
				SetPause(true);
				PauseGame();

			}
		}

	}
	_field->Update(dt);

	if(_figure)
	{
		_figure->Update(dt);
	}

	if(_figure && !_figure->IsFly())
	{
		_renderer->Effect_Play(music->PlayDown());
		_figure->Clear();
		Figure* tmp_figure = _figure;
		_figure = _next;
		_next = tmp_figure;
		_figure->SetPosition(hgeVector((_field->GetWidth() - _figure->GetWidth())/2, 0));
		_figure->SetSpeedVertical(GetNormalSpeedDown());
		_next = 0;
		GenerateNext();
		if(_field->Collision(_figure))
		{
			
			_figure->WriteToFile();
			//_figure->ReadFromFile();
			_field->WriteToFile();
			WriteToFile();
			ReadFromFile();
			_renderer->Effect_Play(music->PlayGameOver());
			_gameOver = true;
		}
	}
}

void World::MoveLeftFigure()
{
	if(_figure)
		_figure->MoveTo(-1);
}

void World::MoveRightFigure()
{
	if(_figure)
		_figure->MoveTo(1);
}
void World::MoveDownFigure()
{
	if(_figure)
		_figure->SetSpeedVertical(GetFastSpeedDown());
}
void World::PauseGame()
{
	if(_figure)
	{
		if(IsGamePaused())
		{
			_figure->SetSpeedHorizontal(0);
			_figure->SetSpeedVertical(0);
		}
	}
}
void World::RotateLeftFigure()
{
	if(_figure)
	{
		_isSpinned=false;
		_figure->RotateLeft();
		_isSpinned=true;
	}
}
void World::SetPause(bool value)
{
	_gamePaused=value;
}
void World::GenerateNext()
{
	if(_next) 
		return;

	int colors[] = {0xFF00FFFF, 0xFF0000FF,  0xFFFF00FF, 0xFF008000,0xFF000080, 0xFF800000,0xFF808000,0xFF008080,0xFFFFFF00};
	int selectColor = colors[_renderer->Random_Int(0, 8)];

	if (!_next)
	{
		_next=new Figure(_field,selectColor,_figures,_paramHeight,_paramWidth);
		_next->SetSpeedVertical(NORMAL__speedDOWN);
	}
	if (!_figure)
	{
		_figure = _next;
		_next=0;
		_next =new Figure(_field,selectColor,_figures,_paramHeight,_paramWidth);
	}
}
HGE* World::GetRenderer() 
{
	return _renderer;
}

const hgeVector& World::GetPosition()
{
	return _pos;
}

int World::GetScores()
{
	return _scores;
}



bool World::IsGameOver()
{
	return _gameOver;
}
bool World::IsGamePaused() 
{
	return _gamePaused;
}



void World::DrawCell(World* world, const cell_t cell, const int x, const int y, const hgeVector& offset)
{
	HGE* hge = world->GetRenderer();
	int cellWidth = world->GetCellWidth();
	int cellHeight = world->GetCellHeight();

	int color = 0xFF000000;
	float border = 4;

	float l = offset.x + x*cellWidth;
	float t = offset.y + y*cellHeight;
	float r = l + cellWidth;
	float b = t + cellHeight;

	RenderHelpers::FillRect(hge, l+1, t+1, r-1, b-1, cell);
	RenderHelpers::DrawRect(hge, l+border, t+border, r-border, b-border, color);
	RenderHelpers::DrawRect(hge, l+border+1, t+border+1, r-border-1, b-border-1, color);
}
int World::GetScreenWidth()
{
	return _screenWidth;
}
int World::GetScreenHeight()
{
	return _screenHeight;
}
int World::GetScreenBPP()
{
	return _screenBPP;
}
bool World::IsMusic()
{
	return _music;
}
MainMenu * World::GetMenu()
{
	return _menu;
}
int World::GetFieldWidth()
{
	return _field->GetWidth();
}
int World::GetFieldHeight()
{
	return _field->GetHeight();
}


void World::SetFonts()
{
	bgtex=_renderer->Texture_Load("data/textures/bg_game.png");
	_spr = new hgeSprite(bgtex, 0, 0, _screenWidth,_screenHeight);
	_fnt = new hgeFont("data/fonts/somefnt.fnt");
	_fnt->SetColor(0xFFFFFFFF);
	_fntText=new hgeFont("data/fonts/somefnt.fnt");
	_fntText->SetColor(0xFFFF0000);
	_fnt->SetScale(_fnt->GetScale()+3);
	_fpsFnt=new hgeFont("data/fonts/somefnt.fnt");
	_fpsFnt->SetColor(0xFF00FF00);
	_nextFnt=new hgeFont("data/fonts/somefnt.fnt");
	_nextFnt->SetColor(0xFF00FF00);
}
void World::Clear()
{
	_state=0;
	_isSpinned=true;
	_next=0;
	_pos.x=_screenWidth/6;
	_pos.y=0;
	_figure=0;
	_scores=0;
	_level=1;
	_killLines=0;
	_gameOver=false;
	_gamePaused=false;
	_paramWidth.clear();
	_paramHeight.clear();
	_figures.clear();

}

void World::SetFigures()
{
	//fix path
	fstream f1("data/configs/figures.txt",ios::in);
	if(f1.is_open())
	{
		string str;
		string figureStr;
		int width=0,height=0;
		while(!f1.eof())
		{
			getline(f1,str);
			while(!str.empty())
			{
				for(int i=0;i<str.length();i++)
				{
					figureStr.push_back(str[i]);
					if(width<i+1)
						width=i+1;
				}
				height++;
				if(!getline(f1,str))
					break;
			}
			_figures.push_back(figureStr);
			_paramWidth.push_back(width);
			_paramHeight.push_back(height);
			figureStr.clear();
			height=0,width=0;
		}
	}
	f1.close();
}
void World::WriteToFile()
{

	fstream fout("data/configs/worldInfo.txt",ios::out | ios::binary);
	if(fout.is_open())
	{
		int size= _paramHeight.size();
		fout.write(reinterpret_cast<char*>(& _state),sizeof(int));
		fout.write(reinterpret_cast<char*>(&_scores),sizeof(int));
		fout.write(reinterpret_cast<char*>(&_level),sizeof(int));
		fout.write(reinterpret_cast<char*>(&_killLines),sizeof(int));
		fout.write(reinterpret_cast<char*>(&_pos.y),sizeof(float));
		fout.write(reinterpret_cast<char*>(&_pos.x),sizeof(float));
		fout.write(reinterpret_cast<char*>(&_gameOver),sizeof(bool));
		fout.write(reinterpret_cast<char*>(&_gamePaused),sizeof(bool));
		fout.write(reinterpret_cast<char*>(&_music),sizeof(bool));
		fout.write(reinterpret_cast<char*>(&size),sizeof(int));
		fout.write(reinterpret_cast<char*>(& _paramHeight[0]),size*sizeof(int));
		fout.write(reinterpret_cast<char*>(& _paramWidth[0]),size*sizeof(int));
		for(int i=0;i<size;i++)
		{
			string str=_figures[i];
			fout.write(reinterpret_cast<char*>(&str),sizeof(str));
		}	
	}
	fout.close();
}
World * World::ReadFromFile()
{
	World * tmpWorld = NULL;
	fstream fin("data/configs/worldInfo.txt",ios::in | ios::binary);
	if(fin.is_open())
	{
		int state,score,level,killLines,size;
		float posX=0,posY=0;
		bool gameOver=false,gamePaused=false,music=false;
		vector<int> paramHeight;
		vector<int> paramWidth;
		vector<string> figures;
		fin.read(reinterpret_cast<char*>(&state),sizeof(int));
		fin.read(reinterpret_cast<char*>(&score),sizeof(int));
		fin.read(reinterpret_cast<char*>(&level),sizeof(int));
		fin.read(reinterpret_cast<char*>(&killLines),sizeof(int));
		fin.read(reinterpret_cast<char*>(&posY),sizeof(float));
		fin.read(reinterpret_cast<char*>(&posX),sizeof(float));
		fin.read(reinterpret_cast<char*>(&gameOver),sizeof(bool));
		fin.read(reinterpret_cast<char*>(&gamePaused),sizeof(bool));
		fin.read(reinterpret_cast<char*>(&music),sizeof(bool));
		fin.read(reinterpret_cast<char*>(&size),sizeof(int));
		paramHeight.resize(size);
		paramWidth.resize(size);
		figures.resize(size);
		fin.read(reinterpret_cast<char*>(& paramHeight[0]),size* sizeof(int));
		fin.read(reinterpret_cast<char*>(& paramWidth[0]),size* sizeof(int));
		for(int i=0;i<size;i++)
		{
			string str;
			str.resize(paramHeight[i]*paramWidth[i]);
			fin.read(reinterpret_cast<char*>(& str),sizeof(str));
			figures[i]=str;
		}
		
		World * tmpWorld=new World(_renderer,_screenWidth,_screenHeight,_screenBPP,_fieldWidth,_fieldHeight,_music,_menu);
		tmpWorld->SetState(state);
		tmpWorld->SetKillLines(killLines);
		tmpWorld->SetFiguresParams(figures);
		tmpWorld->SetPos(hgeVector(posX,posY));
		tmpWorld->SetHeightParams(paramHeight);
		tmpWorld->SetWidthParams(paramWidth);
		tmpWorld->SetScore(score);
		tmpWorld->SetLevel(level);
		tmpWorld->SetPause(gamePaused);
		tmpWorld->SetGameOver(gameOver);
		tmpWorld->SetMusic(music);
	}
	fin.close();
	return tmpWorld;
}
Field * World::ReadFieldFromFile()
{
	Field * tmpField; 
	fstream fin("data/configs/fieldInfo.txt",ios::in | ios::binary);
	if(fin.is_open())
	{
		int width,height,sizeOfBody;
		cell_v tmpBody;
		fin.read(reinterpret_cast<char*>(&width),sizeof(int));
		fin.read(reinterpret_cast<char*>(&height),sizeof(int));
		fin.read(reinterpret_cast<char*>(&sizeOfBody),sizeof(int));
		tmpBody.resize(sizeOfBody);
		fin.read(reinterpret_cast<char*>(&tmpBody[0]), sizeOfBody*sizeof(cell_t));
		tmpField=new Field(width,height);
		tmpField->SetBody(tmpBody);
		return tmpField;
	}
	fin.close();
}
Figure* World::ReadFigureFromFile()
{
	Figure * someFigure=new Figure();
	fstream fin("data/configs/figureInfo.txt",ios::in | ios::binary);
	if(fin.is_open())
	{
		float pxlCounter,speedX,speedY,destX,posY,posX;
		int width,height,sizeOfBody;
		bool fly,needMove;
		cell_v tmpBody;
		fin.read(reinterpret_cast<char*>(&fly),sizeof(bool));
		fin.read(reinterpret_cast<char*>(&needMove),sizeof(bool));
		fin.read(reinterpret_cast<char*>(&pxlCounter),sizeof(float));
		fin.read(reinterpret_cast<char*>(&speedX),sizeof(float));
		fin.read(reinterpret_cast<char*>(&speedY) , sizeof(float));
		fin.read(reinterpret_cast<char*>(&destX ),sizeof(float));
		fin.read(reinterpret_cast<char*>(&posY),sizeof(float));
		fin.read(reinterpret_cast<char*>(&posX),sizeof(float));
		fin.read(reinterpret_cast<char*>(&width),sizeof(int));
		fin.read(reinterpret_cast<char*>(&height),sizeof(int));
		fin.read(reinterpret_cast<char*>(&sizeOfBody),sizeof(int));
		tmpBody.resize(sizeOfBody);
		fin.read(reinterpret_cast<char*>(&tmpBody[0]), sizeOfBody*sizeof(cell_t));
		someFigure->SetBody(tmpBody);
		someFigure->SetWidth(width);
		someFigure->SetHeight(height);
		someFigure->SetPosition(hgeVector(posX,posY));
		someFigure->SetSpeedHorizontal(speedX);
		someFigure->SetSpeedVertical(speedY);
		someFigure->SetMove(needMove);
		someFigure->SetFly(fly);
		someFigure->SetPixels(pxlCounter);
		someFigure->SetDest(destX);
		return someFigure;
	}
	fin.close();
}
Figure* World::ReadNextFigureFromFile()
{
	Figure * someFigure=new Figure();
	fstream fin("data/configs/figureNextInfo.txt",ios::in | ios::binary);
	if(fin.is_open())
	{
		float pxlCounter,speedX,speedY,destX,posY,posX;
		int width,height,sizeOfBody;
		bool fly,needMove;
		cell_v tmpBody;
		fin.read(reinterpret_cast<char*>(&fly),sizeof(bool));
		fin.read(reinterpret_cast<char*>(&needMove),sizeof(bool));
		fin.read(reinterpret_cast<char*>(&pxlCounter),sizeof(float));
		fin.read(reinterpret_cast<char*>(&speedX),sizeof(float));
		fin.read(reinterpret_cast<char*>(&speedY) , sizeof(float));
		fin.read(reinterpret_cast<char*>(&destX ),sizeof(float));
		fin.read(reinterpret_cast<char*>(&posY),sizeof(float));
		fin.read(reinterpret_cast<char*>(&posX),sizeof(float));
		fin.read(reinterpret_cast<char*>(&width),sizeof(int));
		fin.read(reinterpret_cast<char*>(&height),sizeof(int));
		fin.read(reinterpret_cast<char*>(&sizeOfBody),sizeof(int));
		tmpBody.resize(sizeOfBody);
		fin.read(reinterpret_cast<char*>(&tmpBody[0]), sizeOfBody*sizeof(cell_t));
		someFigure->SetBody(tmpBody);
		someFigure->SetWidth(width);
		someFigure->SetHeight(height);
		someFigure->SetPosition(hgeVector(posX,posY));
		someFigure->SetSpeedHorizontal(speedX);
		someFigure->SetSpeedVertical(speedY);
		someFigure->SetMove(needMove);
		someFigure->SetFly(fly);
		someFigure->SetPixels(pxlCounter);
		someFigure->SetDest(destX);
		return someFigure;
	}
	fin.close();
}


void World::SetField(Field * field)
{
	_field=field;
}
void World::SetMainFigure(Figure * figure)
{
	_figure =figure;
}
void World::SetNextFigure(Figure * next)
{
	_next=next;
}
void World::SetPos(hgeVector vect)
{
	_pos=vect;
}
void World::SetScore(int score)
{
	_scores=score;
}
void World::SetKillLines(int amount)
{
	_killLines=amount;
}
void World::SetLevel(int level)
{
	_level=level;
}
void World::SetGameOver(bool gameover)
{
	_gameOver=gameover;
}
void World::SetFiguresParams(vector<string> figures)
{
	_figures=figures;
}
void World::SetHeightParams(vector<int> heights)
{

	_paramHeight=heights;
}
void World::SetWidthParams(vector<int> widths)
{
	_paramWidth=widths;
}
void World::SetMusic(bool music)
{
	_music=music;
}