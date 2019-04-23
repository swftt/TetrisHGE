#include "stdafx.h"
#include "figure.h"
#include "field.h"
#include "world.h"
using namespace std;



template <class Ty>
Ty sign(const Ty& v) {
	return v < 0 ? -1 : v > 0 ? 1 : 0;
}
Figure::Figure()
{
	Clear();
}
Figure::Figure(Field* field, int w, int h): GameObj(w, h) 
{
	Clear();
	_field=field;
}


void Figure::SetSpeedHorizontal(const float speed)
{
	_speed.x = speed;
}

float Figure::GetSpeedHorizontal() 
{
	return _speed.x;
}

void Figure::SetSpeedVertical(const float speed)
{
	_speed.y = speed;
}

float Figure::GetSpeedVectical() 
{
	return _speed.y;
}

const hgeVector& Figure::GetPosition() 
{
	return _pos;
}

const cell_v& Figure:: GetBody()  
{
	return _body;
}

bool Figure::IsFly() 
{
	return _fly;
}
void Figure::SetPosition(const hgeVector& pos)
{
	_pos = pos;
}
void Figure::SetField(Field * otherField)
{
	_field=otherField;
}

// Rotates figure left if it`s possible
void Figure::RotateLeft()
{
	if(_pos.y+_width > _field->GetHeight()) 
		return;
	Figure newFigure(_field, _height, _width);

	for(int y=0; y<_height; ++y)
	{
		for(int x=0; x<_width; ++x)
		{
			int oldIdx = XYToIdx(x, y);
			int newIdx = newFigure.XYToIdx(y, _width-(x+1));
			newFigure._body[newIdx] = _body[oldIdx];
		}
	}

	newFigure._pos = _pos;

	//rotate figure if figure near left border
	if(newFigure._pos.x + newFigure.GetWidth() >= _field->GetWidth())
		newFigure._pos.x = _field->GetWidth() - newFigure.GetWidth();

	if(_field->Hit(&newFigure))
		return;

	_body = newFigure._body;
	_pos = newFigure._pos;
	_height = newFigure.GetHeight();
	_width = newFigure.GetWidth();
}

// Draws figure
void Figure::Draw(const hgeVector& offset)
{
	hgeVector pos(_pos.x * _field->GetWorld()->GetCellWidth(), _pos.y*_field->GetWorld()->GetCellHeight());
	pos += offset;

	for(int y=0; y<_height; ++y)
	{
		for(int x=0; x<_width; ++x)
		{
			cell_t cell = _body[XYToIdx(x, y)];

			if(!cell) 
				continue;

			World::DrawCell(_field->GetWorld(), cell, x, y, pos);
		}
	}
}

void Figure::Update(float dt)
{
	if(!_fly) 
		return;
	if(_needMove)
	{
		const float dist = destX_ - (int)_pos.x;

		if(abs(dist) ==0)
		{
			_pos.x = destX_;
			_needMove = false;
		}
		else
			_pos.x += sign(dist) * dt * GetSpeedHorizontal();
	}
	int y = _pos.y;
	float delta=dt*GetSpeedVectical();
	int nextCellCoord=0;
	for(int i=0;i<_field->GetHeight();i++)
	{
		nextCellCoord++;
		if(nextCellCoord>=_pos.y)
			break;
	}
	if(pixelCounter<=nextCellCoord)
	{
		pixelCounter+=delta;	
	}
	else
	{
		_pos.y+=1;
	}
	if(_field)
	{
		if(y != int(_pos.y) && (_field->Hit(this) || _pos.y+GetHeight()>= _field->GetHeight()))
		{ 
			_field->SetFigure(this);
			_fly = false;
			_field->KillLine();

		}
	}
}
void Figure::SetBody(cell_v body)
{
	_body=body;
}
void Figure::MoveTo(float delta)
{
	if(_needMove)
		return;
	const int x = _pos.x + delta;
	if(x < 0 || x+_width > _field->GetWidth())
		return;
	Figure temp(_field, _width, _height);
	temp._body = _body;
	temp._pos = _pos + hgeVector(delta, 0);
	if(_field->Collision(&temp)) 
		return;
	if(_field->Hit(&temp)) 
		return;
	destX_ = x;
	_needMove = true;
}

bool Figure::NeedMove()
{
	return _needMove;
}
void Figure::SetFly(bool value)
{
	_fly=value;
}
void Figure::SetPixels(float pixels)
{
	pixelCounter=pixels;
}
Figure::Figure(Field* field, cell_t cell,vector<string> figures,vector<int> figureHeight,vector<int> figureWidth)
:GameObj(0,0)
{
	srand(time(0));
	Clear();
	_field=field;

	int random=rand()% (figureWidth.size());
	SetWidth(figureWidth[random]);
	SetHeight(figureHeight[random]);
	ClearBody();
	ResizeBody();

	string figureStr=figures[random];

	for(int i=0;i<GetWidth()*GetHeight();i++)
	{
		if(figureStr[i]=='1')
		{
			_body[i]=cell;
		}
		else
		{
			_body[i]=0;
		}
	}
	figureStr.clear();
}

void Figure::WriteNextToFile()
{
	fstream fout("data/configs/figureNextInfo.txt",ios::out | ios::binary);
	if(fout.is_open())
	{
		int size= _body.size();
		fout.write(reinterpret_cast<char*>(&_fly),sizeof(bool));
		fout.write(reinterpret_cast<char*>(& _needMove),sizeof(bool));
		fout.write(reinterpret_cast<char*>(&pixelCounter),sizeof(float));
		fout.write(reinterpret_cast<char*>(& _speed.x),sizeof(float));
		fout.write(reinterpret_cast<char*>(& _speed.y) , sizeof(float));
		fout.write(reinterpret_cast<char*>(& destX_ ),sizeof(float));
		fout.write(reinterpret_cast<char*>(& _pos.y),sizeof(float));
		fout.write(reinterpret_cast<char*>(& _pos.x),sizeof(float));
		fout.write(reinterpret_cast<char*>(& _width),sizeof(int));
		fout.write(reinterpret_cast<char*>(& _height),sizeof(int));
		fout.write(reinterpret_cast<char*>(&size),sizeof(int));
		fout.write(reinterpret_cast<char*>(& _body[0]),_body.size()* sizeof(cell_t));
	}
	fout.close();
}
void Figure::WriteToFile()
{
	fstream fout("data/configs/figureInfo.txt",ios::out | ios::binary);
	if(fout.is_open())
	{
		int size= _body.size();
		fout.write(reinterpret_cast<char*>(&_fly),sizeof(bool));
		fout.write(reinterpret_cast<char*>(& _needMove),sizeof(bool));
		fout.write(reinterpret_cast<char*>(&pixelCounter),sizeof(float));
		fout.write(reinterpret_cast<char*>(& _speed.x),sizeof(float));
		fout.write(reinterpret_cast<char*>(& _speed.y) , sizeof(float));
		fout.write(reinterpret_cast<char*>(& destX_ ),sizeof(float));
		fout.write(reinterpret_cast<char*>(& _pos.y),sizeof(float));
		fout.write(reinterpret_cast<char*>(& _pos.x),sizeof(float));
		fout.write(reinterpret_cast<char*>(& _width),sizeof(int));
		fout.write(reinterpret_cast<char*>(& _height),sizeof(int));
		fout.write(reinterpret_cast<char*>(&size),sizeof(int));
		fout.write(reinterpret_cast<char*>(& _body[0]),_body.size()* sizeof(cell_t));
	}
	fout.close();
}
void Figure::Clear()
{
	pixelCounter=0;
	_fly=true;
	_speed.x=6;
	_speed.y=2;
	destX_=0;
	_needMove=false;
}

void Figure::SetDest(float destX)
{
	destX_=destX;
}
void Figure::SetMove(bool move)
{
	_needMove=move;
}