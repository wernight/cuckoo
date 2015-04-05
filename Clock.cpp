#include "stdafx.h"
#include "Clock.h"
#include "ResourceManager.h"

void CClock::EnableSounds(bool bTicTac, bool bHours, bool bHalfHours, bool bQuarters)
{
	m_bTicTac = bTicTac;
	m_bHours = bHours;
	m_bHalfHours = bHalfHours;
	m_bQuartHours = bQuarters;
}

void CClock::SetScreenSize(int nWidth, int nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

void CClock::RenderLogo()
{
	glCallList(m_nListBase);
}

void CClock::SetSwapBufferMode(ESwapMode nMode)
{
	m_nSwapMode = nMode;
}

bool CClock::Init()
{
	GLuint nTexLogo;

	// Load textures
	nTexLogo = Scene::ResourceManager::LoadTexture(IDR_PNG_LIBERTYS, PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	// Logo Liberty's
	m_nListBase = glGenLists(1);
	glNewList(m_nListBase, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, nTexLogo);
		glPushMatrix();
		glTranslatef(5.0f, 420.0f, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f,		54.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( 132.0f,	54.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( 132.0f,	0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f,		0.0f);
		glEnd();
		glPopMatrix();
	glEndList();

	UpdateTime();
	return true;
}

void CClock::UpdateTime()
{
	__time64_t		lTime;
	tm				*pLocalTime;

	// Get timer
	_time64( &lTime );							// Get time as long integer.
	pLocalTime = _localtime64( &lTime );		// Convert to local time.

	m_nHour = pLocalTime->tm_hour;
	m_nMin = pLocalTime->tm_min;
	m_nSec = pLocalTime->tm_sec;

	m_nMDay = pLocalTime->tm_mday;
	m_nMonth = pLocalTime->tm_mon;
	m_nYear = pLocalTime->tm_year + 1900;
}