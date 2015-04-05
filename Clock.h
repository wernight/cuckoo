#pragma once

#include "resource.h"		// Include ressources

// Clock
class CClock
{
public:
	enum ESwapMode
	{
		SWAP_BACK_AND_FRONT,
		COPY_BACK_TO_FRONT
	};

// Attributes
	void EnableSounds(bool bTicTac, bool bHours, bool bHalfHours, bool bQuarters);
	void SetScreenSize(int nWidth, int nHeight);
	void RenderLogo();
	void SetSwapBufferMode(ESwapMode nMode);

// Overridables
	virtual bool Init();					// Initialise
	virtual void Render(float fTime) = 0;	// Render
	virtual void UpdateTime();				// Update time

// Implementation
protected:
	// Enabled sounds
	bool	m_bTicTac,		// Tic/Tac
			m_bHours,		// 0h00
			m_bHalfHours,	// 0h30
			m_bQuartHours;	// 0h15 & 0h45

	// Current date and time
	int		m_nHour,		// Hours after midnight (0 – 23).
			m_nMin,			// Minutes after hour (0 – 59).
			m_nSec,			// Seconds after minute (0 – 59).
			m_nMDay,		// Day of month (1 – 31).
			m_nMonth,		// Month (0 – 11; January = 0).
			m_nYear;		// Current year.

	// Screen size
	int		m_nWidth,		// Current screen width (not OpenGL scene)
			m_nHeight;		// Current screen height (not OpenGL scene)

	int		m_nSwapMode;	// SwapBuffer function swap or copy?

private:
	GLuint	m_nListBase;	// Logo texture id
};