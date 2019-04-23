#pragma once

#include "gameObj.h"

class World;
class Figure;


// Field
class Field : public GameObj
{
public:

	Field(World* world, const int w, const int h);
	Field(const int w ,const int h);

	void DrawBkg();
	void DrawField();
	void DrawWell();

	void Update(float dt);

	void Clear();

	bool Hit(Figure* figure);
	bool Collision(Figure* figure);
	void SetFigure(Figure* figure);

	World* GetWorld();
	void KillLine();
	float GetCellHeight();
	void SetBody(cell_v body);

	void WriteToFile();

private:
	World* _world;
};