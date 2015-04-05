// Cuckoo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Clock.h"
#include "ClockMickey.h"
#include "ClockCuckoo.h"
#include "ClockWestminster.h"
#include "StopWatch.h"
#include "CuckooSettings.h"

enum EStates
{
	STATE_SWAP_TEST,
	STATE_INIT_CLOCK,
	STATE_RENDER
};

static HWND		g_hWnd=NULL;				// Holds Our Window Handle
static CClock	*g_pClock = NULL;			// Clock to render

// Saved settings
static CStopWatch	g_swTimer;					// Multemedia precise timing


GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
		height=1;										// Making Height Equal One

	glViewport(0, 0, width, height);
	g_pClock->SetScreenSize(width, height);

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	gluOrtho2D(0, 640, 480, 0);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL()											// All Setup For OpenGL Goes Here
{
	// Init GL
	glShadeModel(GL_FLAT);								// Flat Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
	glColor3f(1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	glEnable(GL_TEXTURE_2D);							// Enable texture Mapping
	glEnable(GL_BLEND);									// Enables Blending
	glDisable(GL_SCISSOR_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Type Of Bending

#ifdef _DEBUG
	// Check we are drawing right way, in debug mode.
	glEnable(GL_CULL_FACE);								// Remove non visible faces
#endif

	return TRUE;										// Initialization Went OK
}

/** Check if SwapBuffers() swap or copy back to front
 * @param hDC	OpenGL device contect.
 * @return True if the SwapBuffer function copies back to front.
 */
bool IsSwapBufferCopyingBackToFront(HDC hDC)
{
	GLbyte	bColorDraw[] = {5, 5, 0},
			bColorSeen[3];
	float fViewport[4];

	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT);

	// Swap buffers
	glFlush();
	SwapBuffers(hDC);

	// Display something
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
		glDisable(GL_TEXTURE_2D);
		glColor3bv(bColorDraw);
		glBegin(GL_TRIANGLES);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(0.0f, 5.0f);
			glVertex2f(5.0f, 0.0f);
		glEnd();
	glPopAttrib();

	// Swap buffers
	glFlush();
	SwapBuffers(hDC);

	// Check if it's still displayed
	glGetFloatv(GL_VIEWPORT, fViewport);
	glReadPixels((int)fViewport[0]+1, (int)(fViewport[1]+fViewport[3])-1, 1, 1, GL_RGB, GL_BYTE, bColorSeen);
	return (abs(bColorSeen[0]-bColorDraw[0]) <= 2 && abs(bColorSeen[1]-bColorDraw[1]) <= 2 && abs(bColorSeen[2]-bColorDraw[2]) <= 2);
}

void DrawGLScene(GLvoid)								// Here's Where We Do All The Drawing
{
#ifdef _DEBUG
	// DEBUG: FPS
	static CStopWatch w;
	static int nFrames = 0;
	if (nFrames == 0)
	{
		w.Init();
		w.Start();
	}
	if (++nFrames == 500)
	{
		FILE *f = fopen("cuckoo.debug.log", "a");
		fprintf(f, "FPS: %.2f\n", nFrames/w.GetElapsed());
		fclose(f);
		Beep(5000, 100);
	}

	// DEBUG: Gestion d'erreurs
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		FILE *f = fopen("cuckoo.debug.log", "a");
		fprintf(f, "Error: %s\n", gluErrorString(error));
		fclose(f);
		Beep(3000, 1000);
	}
#endif

	glLoadIdentity();									// Reset The View

	// Affiche l'horloge
	g_pClock->Render(g_swTimer.GetElapsed());

/*	glAccum(GL_MULT, 0.5f);
	glAccum(GL_ACCUM, 0.5f);
	glAccum(GL_RETURN, 1.0f);
	glFlush();*/

	return;
}

GLvoid KillGLWindow(HWND hWnd, HDC hDC, HGLRC hRC)			// Properly Kill The Window
{
	if (hRC)												// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))						// Are We Able To Release The DC And RC Contexts?
			MessageBox(hWnd, _T("Release Of DC And RC Failed."), _T("SHUTDOWN ERROR"), MB_OK | MB_ICONINFORMATION);

		if (!wglDeleteContext(hRC))							// Are We Able To Delete The RC?
			MessageBox(hWnd, _T("Release Rendering Context Failed."), _T("SHUTDOWN ERROR"), MB_OK | MB_ICONINFORMATION);
	}

	if (hDC && !ReleaseDC(hWnd, hDC))						// Are We Able To Release The DC
		MessageBox(hWnd, _T("Release Device Context Failed."), _T("SHUTDOWN ERROR"), MB_OK | MB_ICONINFORMATION);
}

// Save settings
void SaveSettings(HWND hDlg)
{
	CCuckooSettings	settings;

	// Save
	settings.SetClockType(
		(int)SendMessage(GetDlgItem(hDlg, IDC_CLOCK_COMBO), CB_GETCURSEL, 0, 0) );

	settings.SetDisabledOnMonitorTurnOff(
		IsDlgButtonChecked(hDlg, IDC_MONITORPOWER_CHECK) == BST_CHECKED );

	settings.SetBellsOnHours(
		IsDlgButtonChecked(hDlg, IDC_SND00_CHECK) == BST_CHECKED );

	settings.SetBellsOnHalfHours(
		IsDlgButtonChecked(hDlg, IDC_SND30_CHECK) == BST_CHECKED );

	settings.SetBellsOnQuartHours(
		IsDlgButtonChecked(hDlg, IDC_SND15_CHECK) == BST_CHECKED );

	settings.SetTicTacSound(
		IsDlgButtonChecked(hDlg, IDC_SNDTT_CHECK) == BST_CHECKED );

	settings.Save();
}

BOOL CreateGLWindow(HWND hWnd, HDC *phDC, HGLRC *phRC)
{
	GLuint		nPixelFormat;						// Holds The Results After Searching For A Match
	RECT		rect;

	PIXELFORMATDESCRIPTOR pfd =						// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW							// Format Must Support Window
		| PFD_SUPPORT_OPENGL						// Format Must Support OpenGL
		| PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		0,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		0,											// Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(*phDC = GetDC(hWnd)))						// Did We Get A Device Context?
	{
		KillGLWindow(hWnd, *phDC, *phRC);			// Reset The Display
		MessageBox(hWnd, _T("Can't Create A GL Device Context."), _T("ERROR"), MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(nPixelFormat = ChoosePixelFormat(*phDC, &pfd)))// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow(hWnd, *phDC, *phRC);			// Reset The Display
		MessageBox(hWnd, _T("Can't Find A Suitable PixelFormat."), _T("ERROR"), MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(*phDC, nPixelFormat, &pfd))	// Are We Able To Set The Pixel Format?
	{
		KillGLWindow(hWnd, *phDC, *phRC);			// Reset The Display
		MessageBox(hWnd, _T("Can't Set The PixelFormat."), _T("ERROR"), MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(*phRC = wglCreateContext(*phDC)))			// Are We Able To Get A Rendering Context?
	{
		KillGLWindow(hWnd, *phDC, *phRC);			// Reset The Display
		MessageBox(hWnd, _T("Can't Create A GL Rendering Context."), _T("ERROR"), MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(*phDC, *phRC))				// Try To Activate The Rendering Context
	{
		KillGLWindow(hWnd, *phDC, *phRC);			// Reset The Display
		MessageBox(hWnd, _T("Can't Activate The GL Rendering Context."), _T("ERROR"), MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	GetClientRect(hWnd, &rect);
	ReSizeGLScene(rect.right, rect.bottom);			// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow(hWnd, *phDC, *phRC);			// Reset The Display
		MessageBox(hWnd, _T("Initialization Failed."), _T("ERROR"), MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

EXTERN_C LONG WINAPI ScreenSaverProc(
	HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	static HDC		hDC = NULL;	// Private GDI Device Context
	static HGLRC	hRC = NULL;	// Permanent Rendering Context
	static int		nState;
	static int		nFrames;

	switch (uMsg)									// Check For Windows Messages
	{
	case WM_CREATE:
		{
		// Load settings
		CCuckooSettings	settings;
		settings.Load();

		// Load clock
		switch (settings.GetClockType())
		{
		case 2:		// Mickey
			g_pClock = new CClockMickey;
			break;
		case 1:		// Wesminster
			g_pClock = new CClockWestminster;
			break;
		case 0:		// Cuckoo
			g_pClock = new CClockCuckoo;
			break;
		default:	// Error
			MessageBox(hWnd, _T("Unknown clock: Please configure the screen saver.\n\nHorloge inconnue: Veuillez reconfigurer l'écran de veille."), _T("Cuckoo"), MB_ICONEXCLAMATION);
			return -1;
		}
		if (g_pClock == NULL)
		{
			MessageBox(hWnd, _T("Not enought memory to load the clock."), _T("Cuckoo"), MB_ICONERROR);
			return -1;
		}

		// Initialize OpenGL on hWnd.
		if (!CreateGLWindow(hWnd, &hDC, &hRC))
			return -1;

		// Starting state
		nState = STATE_SWAP_TEST;

		// Set timers
		SetTimer(hWnd, 1, 10, NULL);
		return 0;
		}

	case WM_DESTROY:
		// Timers
		KillTimer(hWnd, 1);
		// Clock
		if (g_pClock != NULL)
		{
			delete g_pClock;
			g_pClock = NULL;
		}
		// OpenGL
		KillGLWindow(hWnd, hDC, hRC);
		return 0;

	case WM_TIMER:
		switch (nState)
		{
		case STATE_SWAP_TEST:
			// Check if SwapBuffers() swap or copy back to front
			if (IsSwapBufferCopyingBackToFront(hDC))
				g_pClock->SetSwapBufferMode(CClock::COPY_BACK_TO_FRONT);
			else
				g_pClock->SetSwapBufferMode(CClock::SWAP_BACK_AND_FRONT);
			nState++;
			break;

		case STATE_INIT_CLOCK:
			{
			// Load settings
			CCuckooSettings	settings;
			settings.Load();

			// Clock
			g_pClock->EnableSounds(settings.HasTicTacSound(), settings.BellsOnHours(), settings.BellsOnHalfHours(), settings.BellsOnQuartHours());
			g_pClock->Init();

			// Precise timing
			g_swTimer.Init();
			g_swTimer.Start();

			// Frame counter
			nFrames = 0;

			nState++;
			break;
			}

		case STATE_RENDER:
			// Met à jour l'heure
			if (++nFrames > 4)
			{
				nFrames = 0;
				g_pClock->UpdateTime();
			}

			// Affiche la scène
			DrawGLScene();
			glFlush();
			SwapBuffers(hDC);
			break;
		}
		return 0;

	case WM_SYSCOMMAND:
		if (wParam == SC_MONITORPOWER)
		{
			// Load settings
			CCuckooSettings	settings;
			settings.Load();
			if (settings.IsDisabledOnMonitorTurnOff())
				PostQuitMessage(0);
		}
	}

	// Pass All Unhandled Messages To DefScreenSaverProc
	return DefScreenSaverProc(hWnd, uMsg, wParam, lParam);
}

void DrawTransparent(HDC hdc, HBITMAP hbmp, int x, int y, COLORREF crColour)
{
	COLORREF crOldBack = SetBkColor(hdc, RGB(255, 255, 255));
	COLORREF crOldText = SetTextColor(hdc, RGB(0, 0, 0));

	// Create two memory dcs for the image and the mask
	HDC hdcImage = CreateCompatibleDC(hdc);
	HDC hdcTrans = CreateCompatibleDC(hdc);

	// Select the image into the appropriate dc
	HBITMAP hbmpOldBitmapImage = (HBITMAP) SelectObject(hdcImage, hbmp);

	// Create the mask bitmap
	BITMAP bmp;
	GetObject(hbmp, sizeof(bmp), &bmp);
	int nWidth = bmp.bmWidth;
	int nHeight = bmp.bmHeight;
	HBITMAP hbmpTrans = CreateBitmap(nWidth, nHeight, 1, 1, NULL);

	// Select the mask bitmap into the appropriate dc
	HBITMAP hbmpOldBitmapTrans = (HBITMAP) SelectObject(hdcTrans, hbmpTrans);

	// Build mask based on transparent colour
	SetBkColor(hdcImage, crColour);
	BitBlt(hdcTrans, 0, 0, nWidth, nHeight, hdcImage, 0, 0, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display
	BitBlt(hdc, x, y, nWidth, nHeight, hdcImage, 0, 0, SRCINVERT);
	BitBlt(hdc, x, y, nWidth, nHeight, hdcTrans, 0, 0, SRCAND);
	BitBlt(hdc, x, y, nWidth, nHeight, hdcImage, 0, 0, SRCINVERT);

	// Restore settings
	SelectObject(hdcImage, hbmpOldBitmapImage);
	SelectObject(hdcTrans, hbmpOldBitmapTrans);
	SetBkColor(hdc, crOldBack);
	SetTextColor(hdc, crOldText);
}

INT_PTR CALLBACK AboutDialogProc(
	HWND hDlg,
	UINT uMsg, 
	WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg) 
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_PAINT:
		{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);

		// Paint the Liberty's logo
		HBITMAP hbmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LIBERTYS));

		RECT rectDlg;
		GetClientRect(hDlg, &rectDlg);

		BITMAP bmp;
		GetObject(hbmp, sizeof(bmp), &bmp);

		DrawTransparent(hdc, hbmp, 0, (rectDlg.bottom-rectDlg.top)-bmp.bmHeight, RGB(0, 128, 128));

		EndPaint(hDlg, &ps);
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;

		case IDC_BEROUX_BUTTON:
			ShellExecute(hDlg, "open", "http://www.beroux.com/", NULL, NULL, SW_SHOWNORMAL);
			break;

		case IDC_LIB_BUTTON:
			ShellExecute(hDlg, "open", "http://www.libertys.com/", NULL, NULL, SW_SHOWNORMAL);
			break;
		}
		return TRUE;
	}

	return FALSE;
}

EXTERN_C BOOL WINAPI ScreenSaverConfigureDialog(
	HWND hDlg,
	UINT uMsg, 
	WPARAM wParam,
	LPARAM lParam)
{
	InitCommonControls();  

	switch (uMsg) 
	{
	case WM_INITDIALOG:
		{
		// Load settings
		CCuckooSettings settings;
		settings.Load();

		// Show current settings
		SendMessage(GetDlgItem(hDlg, IDC_CLOCK_COMBO), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"Cuckoo");
		SendMessage(GetDlgItem(hDlg, IDC_CLOCK_COMBO), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"Westminster");
		SendMessage(GetDlgItem(hDlg, IDC_CLOCK_COMBO), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"Mickey Mouse");
		SendMessage(GetDlgItem(hDlg, IDC_CLOCK_COMBO), CB_SETCURSEL, settings.GetClockType(), 0);

		CheckDlgButton(hDlg, IDC_MONITORPOWER_CHECK, settings.IsDisabledOnMonitorTurnOff()?BST_CHECKED:BST_UNCHECKED);

		CheckDlgButton(hDlg, IDC_SND00_CHECK, settings.BellsOnHours()?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg, IDC_SND30_CHECK, settings.BellsOnHalfHours()?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg, IDC_SND15_CHECK, settings.BellsOnQuartHours()?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg, IDC_SNDTT_CHECK, settings.HasTicTacSound()?BST_CHECKED:BST_UNCHECKED);

		// Update
		SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_CLOCK_COMBO, CBN_SELCHANGE), (LPARAM) GetDlgItem(hDlg, IDC_CLOCK_COMBO));
		return TRUE;
		}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			SaveSettings(hDlg);
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case IDC_ABOUT_BUTTON:
			InitCommonControls();
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, AboutDialogProc);
			break;
		case IDC_CLOCK_COMBO:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				int n = (int)SendMessage(GetDlgItem(hDlg, IDC_CLOCK_COMBO), CB_GETCURSEL, 0, 0);
				EnableWindow(GetDlgItem(hDlg, IDC_SND00_CHECK), n!=2);
				EnableWindow(GetDlgItem(hDlg, IDC_SND30_CHECK), n!=2);
				EnableWindow(GetDlgItem(hDlg, IDC_SND15_CHECK), n==1);
			}
			break;
		}
		return TRUE;
	}

	return FALSE;
}

// Needed for SCRNSAVE.LIB
BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	return TRUE;
}
