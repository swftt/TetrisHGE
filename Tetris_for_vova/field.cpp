#include "stdafx.h"
#include "field.h"
#include "world.h"
#include "figure.h"
#include "renderHelpers.h"
#include "fstream"
#include "vector"
using namespace std;
class World;
class Field;
class Figure;
class RenderHelpers;

// Field
Field::Field(World* world, const int w, const int h): GameObj(w, h), _world(world)
{
	Clear();
}
Field::Field(const int w,const int h):GameObj(w,h)
{
	Clear();
}

// Fiils and draws background of gaming field
void Field::DrawBkg()
{
	hgeVector pos = _world->GetPosition();

	const int color = 0x64646464;
	const float border = 8;
	float l = pos.x - border;
	float t = pos.y - border;
	float r = l + _world->GetCellWidth()*_width + border*2;
	float b = t + _world->GetCellHeight()*_height+ border*2;

	RenderHelpers::FillRect(_world->GetRenderer(), l, t, r, b, color);
}
void Field::SetBody(cell_v body)
{
	_body=body;
}
World* Field:: GetWorld()
{
	return _world;
}

float Field::GetCellHeight()
{
	return _world->GetCellHeight();
}
//Draws field with filled blocks
void Field::DrawField()
{
	HGE* hge = _world->GetRenderer();
	hgeVector pos =_world->GetPosition();

	for(int y=0; y<_height; ++y)
	{
		for(int x=0; x<_width; ++x)
		{
			cell_t cell = _body[XYToIdx(x, y)];
			if(!cell)
			{
				continue;
			}
			World::DrawCell(_world, cell, x, y, pos);
		}
	}
}


// Draws "Glass"
void Field::DrawWell()
{
	HGE* hge = _world->GetRenderer();
	hgeVector pos = _world->GetPosition();

	int color = 0xFFFFFFFF;
	int cellColor=0x64646480;

	float l = pos.x;
	float t = pos.y;
	float r = l+_width*_world->GetCellWidth();
	float b = t+_height*_world->GetCellHeight();
	int incrementor=0;
	for(int i=0;i<_height;i++)
	{
		hge->Gfx_RenderLine(l,incrementor,r,incrementor,cellColor);
		incrementor+=_world->GetCellHeight();
	}
	incrementor=0;
	for(int i=0;i<_width;i++)
	{
			hge->Gfx_RenderLine(l+incrementor,b,l+incrementor,t,cellColor);
			incrementor+=_world->GetCellHeight();
	}
	hge->Gfx_RenderLine(l, t, l, b, color);
	hge->Gfx_RenderLine(l, b, r, b, color);
	hge->Gfx_RenderLine(r, b, r, t, color);
}

void Field::Update(float dt)
{
}

// Does figure hit he field
bool Field::Hit(Figure* figure)
{
	hgeVector pos = figure->GetPosition();
	for(int y=0; y<figure->GetHeight(); ++y)
	{
		for(int x=0; x<figure->GetWidth(); ++x)
		{
			if(!figure->IsCellEmpty(x, y) && !IsCellEmpty(pos.x+x,pos.y+y+1)) 
			{
				return true;
			}
		}
	}
	return false;
}


// Does figure collise with field
bool Field::Collision(Figure* figure)
{
	hgeVector pos = figure->GetPosition();
	for(int y=0; y<figure->GetHeight(); ++y)
	{
		for(int x=0; x<figure->GetWidth(); ++x)
		{
			if(!figure->IsCellEmpty(x, y) && !IsCellEmpty(pos.x+x, pos.y+y))
				return true;
		}
	}

	return false;
}
// Fills figure in field
void Field::SetFigure(Figure* figure)
{
	hgeVector pos = figure->GetPosition();

	const cell_v& body = figure->GetBody();

	for(int y=0; y<figure->GetHeight(); ++y)
	{
		for(int x=0; x<figure->GetWidth(); ++x)
		{
			if(figure->IsCellEmpty(x, y)) 
				continue;

			int toIdx = figure->XYToIdx(x, y);
			int fromIdx = XYToIdx(pos.x+x, pos.y+y);

			_body[fromIdx] = body[toIdx];
		}
	}
}

// Kills line when it is possible 
void Field::KillLine()
{
	int score = 0;

	for(int y=GetHeight()-1; y>=0;)
	{
		bool kill = true;

		for(int x=0; x<GetWidth(); ++x)
		{
			if(IsCellEmpty(x, y))
			{
				kill = false;
				break;
			}
		}

		if(kill)
		{
			for(int x=0; x<GetWidth(); ++x)
			{
				_body[XYToIdx(x, y)] = 0;
				score += _world->ScoreForCell();
			}

			for(int x=0; x<GetWidth(); ++x)
			{
				for(int yy=y; yy>0; --yy)
				{
					_body[XYToIdx(x, yy)] = _body[XYToIdx(x, yy-1)];
				}
			}

			_world->IncKillLine();
		}
		else
			--y;
	}

	_world->AddScore(score);
}
// Clears field
void Field::Clear()
{
	for(cell_v::iterator it=_body.begin(); it!=_body.end(); ++it)
	{
		(*it) = 0;
	}
}
void Field::WriteToFile()
{
	fstream fout("data/configs/fieldInfo.txt",ios::out | ios::binary);
	if(fout.is_open())
	{
		int size= _body.size();
		fout.write(reinterpret_cast<char*>(& _width),sizeof(int));
		fout.write(reinterpret_cast<char*>(& _height),sizeof(int));
		fout.write(reinterpret_cast<char*>(&size),sizeof(int));
		fout.write(reinterpret_cast<char*>(& _body[0]),_body.size()* sizeof(cell_t));
	}
	fout.close();
}