#pragma once

#include "cell.h"
// GameObj
class GameObj
{
public:
	GameObj(const int w, const int h);
	GameObj();
	virtual ~GameObj() {}

	int GetWidth();
	int GetHeight();

	int XYToIdx(const int x, const int y);
	bool IsCellEmpty(const int x, const int y);

	void SetWidth(int someWidth);
	void SetHeight(int someHeight);
	void ClearBody();
	void ResizeBody();
	

protected:
	cell_v _body;
	int _width;
	int _height;
};