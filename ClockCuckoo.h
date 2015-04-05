#pragma once

#include "Clock.h"
#include <gl/gl.h>

class CClockCuckoo : public CClock
{
public:
// Operations
	virtual bool Init();
	virtual void Render(float t);
	virtual void UpdateTime();

// Implementation
private:
	enum
	{
		TEX_BACKGROUND = 0,
		TEX_BACKGROUND_ALPHA,
		TEX_BIRD_DOOR_OPEN,
		TEX_PENDULUM,
		TEX_MINUTES,
		TEX_HOURS,
		TEX_DOT,
		TEX_COUNT
	};

	enum
	{
		LIST_FOND = 0,
		LIST_PENDULE,
		LIST_DOOR,
		LIST_HOURS,
		LIST_MIN,
		LIST_CACHE,
		LIST_COUNT
	};

	enum
	{
		STATE_DRAW_FRONT_BUFFER,
		STATE_DRAW_BACK_BUFFER, // (optional)
		STATE_UPDATE_FRONT_BUFFER_TIME,
		STATE_UPDATE_BACK_BUFFER_TIME, // (optional)
		STATE_UPDATE_PENDULUM
	};

    static void PlayBell(LPVOID lpParam);
	void GotoState(int nState);

	GLuint	m_nTexture[TEX_COUNT];
	GLuint	m_nListBase;
	bool	m_bShowCuckoo;
	int		m_nState;
};