#pragma once

#include "gameObj.h"
#include "cell.h"
#include "world.h"
using namespace std;

class Field;

// Figure
class Figure : public GameObj
{
public:
	Figure();
	Figure(Field* field, int w, int h);
	Figure(Field* field, cell_t cell,vector<string> figures,vector<int> figureHeight,vector<int> figureWidth);

	void Draw(const hgeVector& offset);
	void Update(float dt);

	bool IsFly();

	const cell_v& GetBody();
	void SetBody(cell_v body);

	const hgeVector& GetPosition();

	void SetPosition(const hgeVector& pos);

	void RotateLeft();
	void SetField(Field * otherField);
	void SetPixels(float pixels);
	void SetDest(float destX);
	void WriteNextToFile();
	void SetMove(bool move);
	void SetSpeedHorizontal(const float speed);

	float GetSpeedHorizontal();
	void SetSpeedVertical(const float speed);

	float GetSpeedVectical();

	void MoveTo(float delta);
	void Clear();

	void SetFiguresFromFile();
	void SetFly(bool value);	
	bool NeedMove();

	void WriteToFile();
private:
	Field* _field;
	hgeVector _pos;
	bool _fly;
	hgeVector _speed;
	float destX_;
	bool _needMove;
	float pixelCounter;

};


