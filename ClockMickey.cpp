#include "stdafx.h"
#include "ClockMickey.h"
#include "ResourceManager.h"

bool CClockMickey::Init()
{
	if (!CClock::Init())
		return false;

	m_nTexture[TEX_FOND]	= Scene::ResourceManager::LoadTexture(IDR_PNG_MM_FOND,	PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	m_nTexture[TEX_HR]		= Scene::ResourceManager::LoadTexture(IDR_PNG_MM_HR,	PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	m_nTexture[TEX_MIN]		= Scene::ResourceManager::LoadTexture(IDR_PNG_MM_MIN,	PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	m_nTexture[TEX_HEAD]	= Scene::ResourceManager::LoadTexture(IDR_PNG_MM_HEAD,	PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	m_nListBase = glGenLists(LIST_COUNT);
	glNewList(m_nListBase+LIST_FOND, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_FOND]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		512.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( 512.0f,	512.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( 512.0f,	0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
		glEnd();
	glEndList();
	glNewList(m_nListBase+LIST_HOURS, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_HR]);
		glTranslatef(-24.0f, -24.0f, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		64.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( 128.0f,	64.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( 128.0f,	0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
		glEnd();
	glEndList();
	glNewList(m_nListBase+LIST_MIN, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_MIN]);
		glTranslatef(-80.0f, -31.0f, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		64.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( 256.0f,	64.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( 256.0f,	0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
		glEnd();
	glEndList();
	glNewList(m_nListBase+LIST_HEAD, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_HEAD]);
		glTranslatef(-64.0f, -64.0f, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		128.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( 128.0f,	128.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( 128.0f,	0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
		glEnd();
	glEndList();

	if (m_bTicTac)
		PlaySound(MAKEINTRESOURCE(IDR_MM_TICTAC), NULL, SND_RESOURCE | SND_LOOP | SND_ASYNC);

	m_nState = STATE_DRAW_FRONT_BUFFER;
	return true;
}

void CClockMickey::Render(float t)
{	
	float	fMili = t - (int)t;

	// Clear The Screen
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
		glTranslatef(64.0f, -16.0f, 0.0f);

		// Fond
		glCallList(m_nListBase+LIST_FOND);

		// Tête de Mickey
		glPushMatrix();
			glTranslatef(188.0f+64.0f, 139.0f+64.0f, 0.0f);
			if (fMili < 0.25f || (fMili > 0.5f && fMili < 0.75f))
				glRotated(10.0f, 0, 0, 1);
			else
				glRotated(-10.0f, 0, 0, 1);
			glCallList(m_nListBase+LIST_HEAD);
		glPopMatrix();

		// Heures
		glPushMatrix();
			glTranslatef(225.0f+24.0f, 255.0f+24.0f, 0.0f);
			glRotated(-90 + m_nHour*360.0f/12.0f + m_nMin/60.0f*360.0f/12.0f, 0, 0, 1);
			glCallList(m_nListBase+LIST_HOURS);
		glPopMatrix();

		// Minutes
		glPushMatrix();
			glTranslatef(169.0f+80.0f, 249.0f+31.0f, 0.0f);
			glRotated(-90 + m_nMin*360.0f/60.0f + m_nSec/60.0f*360.0f/60.0f, 0, 0, 1);
			glCallList(m_nListBase+LIST_MIN);
		glPopMatrix();

	glPopMatrix();

	// Go to next rendering state
	switch (m_nState)
	{
	// First draw
	case STATE_DRAW_FRONT_BUFFER:
		if (m_nSwapMode == SWAP_BACK_AND_FRONT)
			GotoState(STATE_DRAW_BACK_BUFFER);
		else
			GotoState(STATE_UPDATE_PENDULUM);	// Skip the drawing in the other buffer
		break;
	case STATE_DRAW_BACK_BUFFER:
		GotoState(STATE_UPDATE_PENDULUM);
		break;

	// Update time
	case STATE_UPDATE_FRONT_BUFFER_TIME:
		if (m_nSwapMode == SWAP_BACK_AND_FRONT)
			GotoState(STATE_UPDATE_BACK_BUFFER_TIME);
		else
			GotoState(STATE_UPDATE_PENDULUM);	// Skip the drawing in the other buffer
		break;
	case STATE_UPDATE_BACK_BUFFER_TIME:
		GotoState(STATE_UPDATE_PENDULUM);
		break;
	}
}


void CClockMickey::UpdateTime()
{
	CClock::UpdateTime();

	// Redraw the clock with new time
	switch (m_nState)
	{
	case STATE_UPDATE_BACK_BUFFER_TIME:
	case STATE_UPDATE_PENDULUM:
		GotoState(STATE_UPDATE_FRONT_BUFFER_TIME);
		break;
	}
}

void CClockMickey::GotoState(int nState)
{
	// Every so and then, redraw the full screen in both buffers
	static unsigned nFrames = 0;
	if (++nFrames > 100 && nState == STATE_UPDATE_FRONT_BUFFER_TIME)
	{
		nFrames = 0;
		nState = STATE_DRAW_FRONT_BUFFER;
	}

	// On exit

	// Change state
	m_nState = nState;

	// On entry
	switch (m_nState)
	{
	case STATE_DRAW_FRONT_BUFFER:
		glDisable(GL_SCISSOR_TEST);
		break;
	case STATE_UPDATE_FRONT_BUFFER_TIME:
		glEnable(GL_SCISSOR_TEST);
		glScissor(180*m_nWidth/640, 80*m_nHeight/480, 270*m_nWidth/640, 270*m_nHeight/480);
		break;
	case STATE_UPDATE_PENDULUM:
		glEnable(GL_SCISSOR_TEST);
		glScissor(252*m_nWidth/640, 231*m_nHeight/480, 124*m_nWidth/640, 100*m_nHeight/480);
		break;
	}
}
