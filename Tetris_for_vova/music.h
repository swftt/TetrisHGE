#pragma once

class Music
{
public:
	Music(HGE * renderer);
	virtual ~Music();
	HEFFECT PlayPause();
	HEFFECT PlayTurn();
	HEFFECT PlayLineRemove();
	HEFFECT PlayStart();
	HEFFECT PlayMenuSound();
	HEFFECT PlayGameMusic();
	HEFFECT PlayMenuMusic();
	HEFFECT PlayGameOver();
	HEFFECT PlayDown();
	void NullGameMusic();
	void Clear();
private:
	HGE * _renderer;
	HEFFECT _down;
	HEFFECT _pause;
	HEFFECT _start;
	HEFFECT _turn;
	HEFFECT _lineRemove;
	HEFFECT _gameMusic;
	HEFFECT _menuSound;
	HEFFECT _menuMusic;
	HEFFECT _end;
};
