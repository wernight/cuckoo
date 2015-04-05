#include "stdafx.h"
#include "ClockCuckoo.h"
#include "ResourceManager.h"

bool CClockCuckoo::Init()
{
	m_bShowCuckoo = false; 

	if (!CClock::Init())
		return false;

	m_nTexture[TEX_BACKGROUND]		= Scene::ResourceManager::LoadTexture(IDR_PNG_CC0, PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	m_nTexture[TEX_BIRD_DOOR_OPEN]	= Scene::ResourceManager::LoadTexture(IDR_PNG_CC1, PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	m_nTexture[TEX_PENDULUM]		= Scene::ResourceManager::LoadTexture(IDR_PNG_CC2, PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	m_nTexture[TEX_MINUTES]			= Scene::ResourceManager::LoadTexture(IDR_PNG_CC3, PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	m_nTexture[TEX_HOURS]			= Scene::ResourceManager::LoadTexture(IDR_PNG_CC4, PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	m_nTexture[TEX_DOT]				= Scene::ResourceManager::LoadTexture(IDR_PNG_CC5, PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	m_nTexture[TEX_BACKGROUND_ALPHA]= Scene::ResourceManager::LoadTexture(IDR_PNG_CC6, PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	// Lists
	m_nListBase = glGenLists(LIST_COUNT);
	glNewList(m_nListBase+LIST_FOND, GL_COMPILE);
		// Fond
		glPushMatrix();
		glPushAttrib(GL_COLOR_BUFFER_BIT);
			glTranslatef(64.5f, -46.0f, 0.0f);
			glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_BACKGROUND_ALPHA]);
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		512.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f( 512.0f,	512.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f( 512.0f,	0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_BACKGROUND]);
			glBlendFunc(GL_ONE, GL_ONE);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		512.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f( 512.0f,	512.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f( 512.0f,	0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
			glEnd();
		glPopAttrib();
		glPopMatrix();
	glEndList();
	glNewList(m_nListBase+LIST_PENDULE, GL_COMPILE);
		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_BLEND);
			glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_PENDULUM]);
			glTranslatef(-72.0f/2.0f, 0.0f, 0.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		144.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f( 72.0f,	144.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f( 72.0f,	0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
			glEnd();
		glPopAttrib();
	glEndList();
	glNewList(m_nListBase+LIST_DOOR, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_BIRD_DOOR_OPEN]);
		glPushMatrix();
			glTranslatef(64.5f+243.0f, -46.0f+183.0f, 0.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		64.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f( 64.0f,	64.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f( 64.0f,	0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
			glEnd();
		glPopMatrix();
	glEndList();
	glNewList(m_nListBase+LIST_HOURS, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_HOURS]);
		glTranslatef(-30.2f, -6.0f, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		12.7f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( 51.0f,	12.7f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( 51.0f,	0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
		glEnd();
	glEndList();
	glNewList(m_nListBase+LIST_MIN, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_MINUTES]);
		glTranslatef(-41.0f, -7.0f, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		24.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( 48.0f,	24.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( 48.0f,	0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
		glEnd();
	glEndList();
	glNewList(m_nListBase+LIST_CACHE, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_nTexture[TEX_DOT]);
		glPushMatrix();
			glTranslatef(323.0f-4.0f, 235.0f-4.0f, 0.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		8.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f( 8.0f,		8.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f( 8.0f,		0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
			glEnd();
		glPopMatrix();
	glEndList();

	m_nState = STATE_DRAW_FRONT_BUFFER;
	return true;
}

void CClockCuckoo::UpdateTime()
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

void CClockCuckoo::Render(float t)
{
	// Son Clic/Clac de la pendule
	if (m_bTicTac && !m_bShowCuckoo)				// Don't play sound when cucoo snd is played
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
		glTranslatef(320.0f, 320.0f, 0.0f);
		glRotated(sin(t*2.0f*M_PI)*12.0f, 0, 0, 1);
		glCallList(m_nListBase+LIST_PENDULE);
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
		// Cuckoo door
		if (m_bShowCuckoo)
			glCallList(m_nListBase+LIST_DOOR);

		// Hours
		glPushMatrix();
			glTranslatef(323.0f, 235.0f, 0.0f);
			glRotated(90 + m_nHour*360.0f/12.0f + m_nMin/60.0f*360.0f/12.0f, 0, 0, 1);
			glCallList(m_nListBase+LIST_HOURS);
		glPopMatrix();

		// Minutes
		glPushMatrix();
			glTranslatef(323.0f, 235.0f, 0.0f);
			glRotated(90 + m_nMin*360.0f/60.0f + m_nSec/60.0f*360.0f/60.0f, 0, 0, 1);
			glCallList(m_nListBase+LIST_MIN);
		glPopMatrix();

		// Cache aiguilles
		glCallList(m_nListBase+LIST_CACHE);
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

void CClockCuckoo::PlayBell(LPVOID lpParam)
{
	CClockCuckoo *pClock = (CClockCuckoo*) lpParam;
	int i, nRepeat;

	// Show cuckoo
	pClock->m_bShowCuckoo = true;
	Sleep(500);

	// Play sounds
	switch (pClock->m_nMin)
	{
	case 0:	// Hours
		// Repeat how many times the cucoo ?
		nRepeat = pClock->m_nHour;
		if (nRepeat > 12)
			nRepeat -= 12;

		// Play Sound
		if (pClock->m_nMDay == 0 && pClock->m_nMonth == 0 ||		// New year
			pClock->m_nMDay == 25 && pClock->m_nMonth == 0)			// Programmer's birthday
		{
			// Special MOUH sound
			PlaySound(MAKEINTRESOURCE(IDR_CC_DEBUT_WAVE), NULL, SND_RESOURCE | SND_SYNC);	// Start
			for (i=0; i<nRepeat-1; i++)
				PlaySound(MAKEINTRESOURCE(IDR_MEUH_WAVE), NULL, SND_RESOURCE | SND_SYNC);	// Midle
		}
		else
		{
			// Normal sound
			PlaySound(MAKEINTRESOURCE(IDR_CC_DEBUT_WAVE), NULL, SND_RESOURCE | SND_SYNC);	// Start
			for (i=0; i<nRepeat-1; i++)
				PlaySound(MAKEINTRESOURCE(IDR_CC_MILIEU_WAVE), NULL, SND_RESOURCE | SND_SYNC);	// Midle
			PlaySound(MAKEINTRESOURCE(IDR_CC_FIN_WAVE), NULL, SND_RESOURCE | SND_SYNC);	// End
		}
		break;

	case 30:	// Half-hours
		// Play sound
		pClock->m_bShowCuckoo = true;
		PlaySound(MAKEINTRESOURCE(IDR_CC_DEBUT_WAVE), NULL, SND_RESOURCE | SND_SYNC);	// Start
		PlaySound(MAKEINTRESOURCE(IDR_CC_FIN_WAVE), NULL, SND_RESOURCE | SND_SYNC);	// End
		break;
	}

	// Hide the Cuckoo
	pClock->m_bShowCuckoo = false;
}

void CClockCuckoo::GotoState(int nState)
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
		glScissor(270*m_nWidth/640, 10*m_nHeight/480, 100*m_nWidth/640, 340*m_nHeight/480);
		break;
	case STATE_UPDATE_PENDULUM:
		glEnable(GL_SCISSOR_TEST);
		glScissor(270*m_nWidth/640, 10*m_nHeight/480, 100*m_nWidth/640, 100*m_nHeight/480);
		break;
	}
}
