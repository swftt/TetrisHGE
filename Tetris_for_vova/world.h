#pragma once

#include "cell.h"
#include "mainMenu.h"
#include "music.h"
using namespace std;

class HGE;
class hgeFont;
class Field;
class Figure;
class hgeVector;
class MainMenu;
class Music;

// World
class World
{
public:


	World(HGE* renderer,int screenWidth,int screenHeight,int screenBPP,int fieldWidth,int fieldHeight,bool music,MainMenu * menu);
	virtual ~World();

	static void DrawCell(World* world, const cell_t cell, const int x, const int y, const hgeVector& offset);
	int GetCellWidth() const;
	int GetCellHeight() const;
	int ScoreForCell() const;

	float GetSpeedHorizontal();
	float GetNormalSpeedDown();
	float GetFastSpeedDown();

	void Draw();
	void Update(float dt);
	HGE* GetRenderer();

	const hgeVector& GetPosition();

	void GenerateNext();

	void MoveLeftFigure();
	void MoveRightFigure();
	void MoveDownFigure();
	void RotateLeftFigure();
	void PauseGame();

	void AddScore(int score);
	int GetScores();
	bool IsGameOver();
	bool IsGamePaused();
	void IncKillLine();
	void SetPause(bool value);
	void DrawBg();
	void SetState(int num);
	void Clear();
	void NullGameMusic();
	void UnNullGameMusic();

	int GetScreenWidth();
	int GetScreenHeight();
	int GetScreenBPP();
	bool IsMusic();
	MainMenu * GetMenu();
	int GetFieldWidth();
	int GetFieldHeight();
	void NewGame();
	void ClearField();
	void WriteToFile();
	World* ReadFromFile();
	Figure* ReadNextFigureFromFile();
	Field* ReadFieldFromFile();
	Figure* ReadFigureFromFile();
	void SetFigures();

	void SetFonts();

	void SetField(Field * field);
	void SetMainFigure(Figure * figure);
	void SetNextFigure(Figure * next);
	void SetPos(hgeVector vect);
	void SetScore(int score);
	void SetKillLines(int amount);
	void SetLevel(int level);
	void SetGameOver(bool gameover);
	void SetFiguresParams(vector<string> figures);
	void SetHeightParams(vector<int> heights);
	void SetWidthParams(vector<int> widths);
	void SetMusic(bool music);

private:
	HGE* _renderer;
	hgeFont* _fnt;
	hgeSprite* _spr;
	hgeFont* _fpsFnt;
	hgeFont* _nextFnt;
	hgeFont * _fntText;

	HTEXTURE bgtex;
	HEFFECT gameMusic;


	hgeVector _pos;
	MainMenu * _menu;
	Music * music;

	Field* _field;
	Figure* _figure;
	Figure* _next;
	

	int _state;
	int _screenWidth,_screenHeight,_screenBPP;
	int _fieldWidth,_fieldHeight;
	int _scores;
	int _level;
	int _killLines;
	bool _gameOver;
	bool _gamePaused;
	bool _music;
	bool _isSpinned;
	vector<string> _figures;
	vector<int> _paramWidth;
	vector<int> _paramHeight;
};