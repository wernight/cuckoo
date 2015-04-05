#pragma once

#include "Clock.h"
#include <gl/gl.h>

class CClockMickey : public CClock
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
		TEX_HR,
		TEX_MIN,
		TEX_HEAD,
		TEX_COUNT
	};

	enum
	{
		LIST_FOND = 0,
		LIST_HOURS,
		LIST_MIN,
		LIST_HEAD,
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

	void GotoState(int nState);

	GLuint	m_nTexture[TEX_COUNT];
	GLuint	m_nListBase;
	int		m_nState;
};