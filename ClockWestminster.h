#pragma once

#include "Clock.h"
#include <gl/gl.h>

class CClockWestminster : public CClock
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
		TEX_FOND = 0,
		TEX_REFLECTION,
		TEX_AIGUILLES,
		TEX_FOND_ALPHA,
		TEX_COUNT
	};

	enum
	{
		LIST_PENDULE_BOIS = 0,
		LIST_PENDULE_BOULE,
		LIST_FOND,
		LIST_HOURS,
		LIST_MIN,
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
	bool	m_bStopTicTac;
	int		m_nState;
};