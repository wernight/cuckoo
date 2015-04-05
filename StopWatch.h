/////////////////////////////////////////////////////////////////////////////
// StopWatch.h  v1.0
// Copyright (C) 2000 Werner BEROUX
//

#ifndef __STOP_WATCH_H__
#define __STOP_WATCH_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct CStopWatch
{
	inline bool Init(void);
	inline void Start(void);
	inline float GetElapsed(void);

protected:
	LARGE_INTEGER nStart;
	LARGE_INTEGER nNow;
	float fRes; 
};

// Initialise timer
inline bool CStopWatch::Init(void)
{
	LARGE_INTEGER nFreq;

	if (!QueryPerformanceFrequency(&nFreq))
		return false;
	fRes = (float) (1.0f / (double) nFreq.QuadPart);
	return true;
}

// Start timer
inline void CStopWatch::Start(void)
{
	QueryPerformanceCounter(&nStart);
}

// Return elapsed time since start, in seconds.
inline float CStopWatch::GetElapsed(void)
{
	QueryPerformanceCounter(&nNow);
	return (nNow.QuadPart - nStart.QuadPart) * fRes;
}

#endif	// __STOP_WATCH_H__