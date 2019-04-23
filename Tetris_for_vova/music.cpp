#include "stdafx.h"
#include "music.h"

Music::Music(HGE * renderer)
{
	_renderer=renderer;
	Clear();
}
Music::~Music()
{
	_renderer->Effect_Free(_start);
	_renderer->Effect_Free(_turn);
	_renderer->Effect_Free(_lineRemove);
	_renderer->Effect_Free(_gameMusic);
	_renderer->Effect_Free(_menuMusic);
	_renderer->Effect_Free(_pause);
	_renderer->Effect_Free(_menuSound);
}
void Music::Clear()
{
	/*_start=_renderer->Effect_Load("sounds/start.mp3");
	_turn=_renderer->Effect_Load("sounds/whoosh.mp3");
	_lineRemove= _renderer->Effect_Load("sounds/line-remove.mp3");
	_gameMusic= _renderer->Effect_Load("sounds/music.mp3");
	_menuMusic= _renderer->Effect_Load("sounds/menu_music.mp3");
	_pause= _renderer->Effect_Load("sounds/pause.mp3");
	_menuSound=_renderer->Effect_Load("sounds/select.mp3");
	_end=_renderer->Effect_Load("sounds/gameover.mp3");
	_down=_renderer->Effect_Load("sounds/down.mp3");*/
}
HEFFECT Music::PlayMenuMusic()
{
	return _menuMusic;
}
HEFFECT Music::PlayMenuSound()
{
	return _menuSound;
}
HEFFECT Music::PlayGameMusic()
{
	return _gameMusic;
}

HEFFECT Music::PlayPause()
{
	return _pause;
}

HEFFECT Music::PlayStart()
{
	return _start;
}

HEFFECT Music::PlayLineRemove()
{
	return _lineRemove;
}

HEFFECT Music::PlayTurn()
{
	return _turn;
}
HEFFECT Music::PlayGameOver()
{
	return _end;
}
HEFFECT Music::PlayDown()
{
	return _down;
}
void Music::NullGameMusic()
{
	_gameMusic=0;
}