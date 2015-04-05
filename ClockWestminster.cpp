#include "stdafx.h"
#include "ClockWestminster.h"
#include "ResourceManager.h"

bool CClockWestminster::Init()
{
	m_bStopTicTac = false;

	if (!CClock::Init())
		return false;

	m_nTexture[TEX_FOND] = Scene::ResourceManager::LoadTexture(IDR_PNG_WM0, PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	m_nTexture[TEX_REFLECTION] = Scene::ResourceManager::LoadTexture(IDR_PNG_WM1, PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	m_nTexture[TEX_AIGUILLES] = Scene::ResourceManager::LoadTexture(IDR_PNG_WM2, PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	m_nTexture[TEX_FOND_ALPHA] = Scene::ResourceManager::LoadTexture(IDR_PNG_WM3, PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	m_nListBase = glGenLists(LIST_COUNT);
	glNewList(m_nListBase+LIST_PENDULE_BOIS, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_FOND]);
		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
		glDisable(GL_BLEND);
		glColor3f(0.25f, 0.25f, 0.3f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.05f, 0.2f); glVertex2f(-8.0f, 175.0f);
			glTexCoord2f(0.1f, 0.2f); glVertex2f(8.0f, 175.0f);
			glTexCoord2f(0.1f, 0.5f); glVertex2f(8.0f,  0.0f);
			glTexCoord2f(0.05f, 0.5f); glVertex2f(-8.0f,  0.0f);
		glEnd();
		glPopAttrib();
	glEndList();
	glNewList(m_nListBase+LIST_PENDULE_BOULE, GL_COMPILE);
		glMatrixMode(GL_MODELVIEW);
		glPushAttrib(GL_TEXTURE_BIT);
			glTranslatef(0.0f, 195.0f, 0.0f);
			glScalef(1.0f, 1.0f, 0.001f);
			glRotated(180.0f/8.0f, 0, 0, 1);
			glRotated(90.0f, 0, 0, 1);
			glRotated(90.0f, 0, 1, 0);
			glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_REFLECTION]);
			glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj, GL_TRUE);
			gluSphere(obj, 35.0f, 4, 4);
			gluDeleteQuadric( obj );
		glPopAttrib();
		glMatrixMode(GL_TEXTURE);
	glEndList();
	glNewList(m_nListBase+LIST_FOND, GL_COMPILE);
		glPushAttrib(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
			glTranslatef(192.0f, -12.0f, 0.0f);
			glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_FOND_ALPHA]);
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		512.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f( 256.0f,	512.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f( 256.0f,	0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_FOND]);
			glBlendFunc(GL_ONE, GL_ONE);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		512.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f( 256.0f,	512.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f( 256.0f,	0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
			glEnd();
		glPopMatrix();
		glPopAttrib();
	glEndList();
	glNewList(m_nListBase+LIST_HOURS, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_AIGUILLES]);
		glTranslatef(-5.0f, -7.0f, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		14.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( 45.0f,	14.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( 45.0f,	0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
		glEnd();
	glEndList();
	glNewList(m_nListBase+LIST_MIN, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_AIGUILLES]);
		glTranslatef(-7.0f, -7.0f, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		14.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( 65.0f,	14.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( 65.0f,	0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
		glEnd();
	glEndList();

	m_nState = STATE_DRAW_FRONT_BUFFER;
	return true;
}

void CClockWestminster::UpdateTime()
{
	int	nPrevMin = m_nMin;

	CClock::UpdateTime();
	
	// Bell
	if (m_nMin != nPrevMin)
		switch(m_nMin)
		{
		case 0:
			if (m_bHours)
				_beginthread(PlayBell, 0, this);
			break;
		case 30:
			if (m_bHalfHours)
				_beginthread(PlayBell, 0, this);
			break;
		case 15:
		case 45:
			if (m_bQuartHours)
				_beginthread(PlayBell, 0, this);
			break;
		}

	// Redraw the clock with new time
	switch (m_nState)
	{
	case STATE_UPDATE_BACK_BUFFER_TIME:
	case STATE_UPDATE_PENDULUM:
		GotoState(STATE_UPDATE_FRONT_BUFFER_TIME);
		break;
	}
}

void CClockWestminster::Render(float t)
{
	// Son Clic/Clac de la pendule
	if (m_bTicTac && !m_bStopTicTac)				// Don't play sound when cucoo snd is played
	{
		float	fMilli = t - (int)t;
		static float fPrevMilli = fMilli;

		// Play Clack
		if (fPrevMilli < 0.100f && fMilli >= 0.100f)
			PlaySound(MAKEINTRESOURCE(IDR_WM_CLACK_WAVE), NULL, SND_RESOURCE | SND_ASYNC);
		// Play Click
		if (fPrevMilli < 0.600f && fMilli >= 0.600f)
			PlaySound(MAKEINTRESOURCE(IDR_WM_CLICK_WAVE), NULL, SND_RESOURCE | SND_ASYNC);

		fPrevMilli = fMilli;
	}

	// Clear The Screen
	glClear(GL_COLOR_BUFFER_BIT);

	// Pendule
	glPushMatrix();
		glTranslatef(320.0f, 165.0f, 0.0f);
		glRotated(sin(t*2.0f*3.1415927f)*4.0f, 0, 0, 1);
		glCallList(m_nListBase+LIST_PENDULE_BOIS);	

		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
			glTranslatef((float)sin(t*2.0f*3.1415927f)*0.02f, 0, 0);
			glCallList(m_nListBase+LIST_PENDULE_BOULE);	
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// Fond
	glCallList(m_nListBase+LIST_FOND);

	switch (m_nState)
	{
	case STATE_DRAW_FRONT_BUFFER:
	case STATE_DRAW_BACK_BUFFER:
		// Add the logo
		RenderLogo();
	case STATE_UPDATE_FRONT_BUFFER_TIME:
	case STATE_UPDATE_BACK_BUFFER_TIME:
		// Heures
		glPushMatrix();
			glTranslatef(319.0f, 162.0f, 0.0f);
			glRotated(-90 + m_nHour*360.0f/12.0f + m_nMin/60.0f*360.0f/12.0f, 0, 0, 1);
			glCallList(m_nListBase+LIST_HOURS);
		glPopMatrix();

		// Minutes
		glPushMatrix();
			glTranslatef(319.0f, 162.0f, 0.0f);
			glRotated(-90 + m_nMin*360.0f/60.0f + m_nSec/60.0f*360.0f/60.0f, 0, 0, 1);
			glCallList(m_nListBase+LIST_MIN);
		glPopMatrix();
	}

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

void CClockWestminster::PlayBell(LPVOID lpParam)
{
	CClockWestminster *pClock = (CClockWestminster*) lpParam;
	int i, nRepeat;

	// Play sound
	pClock->m_bStopTicTac = true;
	switch(pClock->m_nMin)
	{
	case 0:
		nRepeat = pClock->m_nHour;
		if (nRepeat > 12)
			nRepeat -= 12;

		PlaySound(MAKEINTRESOURCE(IDR_WM_H_WAVE), NULL, SND_RESOURCE | SND_SYNC);	// Start
		for (i=0; i<nRepeat; i++)
			PlaySound(MAKEINTRESOURCE(IDR_WM_HPLUS_WAVE), NULL, SND_RESOURCE | SND_SYNC);	// END
		break;

	case 15:
		PlaySound(MAKEINTRESOURCE(IDR_WM_15_WAVE), NULL, SND_RESOURCE | SND_SYNC);	// Start
		break;

	case 30:
		PlaySound(MAKEINTRESOURCE(IDR_WM_30_WAVE), NULL, SND_RESOURCE | SND_SYNC);	// Start
		break;

	case 45:
		PlaySound(MAKEINTRESOURCE(IDR_WM_45_WAVE), NULL, SND_RESOURCE | SND_SYNC);	// Start
		break;
	}
	pClock->m_bStopTicTac = false;
}

void CClockWestminster::GotoState(int nState)
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
		glScissor(260*m_nWidth/640, 80*m_nHeight/480, 120*m_nWidth/640, 300*m_nHeight/480);
		break;
	case STATE_UPDATE_PENDULUM:
		glEnable(GL_SCISSOR_TEST);
		glScissor(270*m_nWidth/640, 80*m_nHeight/480, 100*m_nWidth/640, 140*m_nHeight/480);
		break;
	}
}
