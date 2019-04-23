#include "stdafx.h"
#include "gameObj.h"

// GameObj
GameObj::GameObj(const int w, const int h)
: _width(w)
, _height(h)
{
	_body.resize(w*h);
}
GameObj::GameObj()
{
}
int GameObj::GetWidth()
{
	return _width;
}
int GameObj::GetHeight()
{
	return _height;
}
void GameObj::SetWidth(int someWidth)
{
	_width=someWidth;
}
void GameObj::SetHeight(int someHeight)
{
	_height=someHeight;
}
void GameObj::ResizeBody()
{
	_body.resize(_width*_height);
}
void GameObj::ClearBody()
{
	_body.clear();
}
int GameObj::XYToIdx(const int x, const int y)
{
	int idx = y*_width+x;
	return idx >= _width*_height ? -1 : idx;
}
bool GameObj::IsCellEmpty(const int x, const int y)
{
	int idx = XYToIdx(x, y);
	return idx == -1 ? true : !_body[idx];
}