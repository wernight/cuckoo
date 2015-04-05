#include "StdAfx.h"
#include "ResourceManager.h"

Scene::ResourceManager::ResourceManager(void)
{
}

Scene::ResourceManager::~ResourceManager(void)
{
}

GLuint Scene::ResourceManager::LoadTexture(int ressource, int mipmap, int trans, pngInfo *info, int wrapst, int minfilter, int magfilter)
{
	unsigned char *lpData;
	int			length;
	HRSRC		rec;
	LPCTSTR		res;
	GLuint		nTex;

	pngSetStandardOrientation( 1 );

	res = MAKEINTRESOURCE(ressource);
	
	// Load ressource and get a pointer to the starting point
	rec = FindResource(NULL, res, _T("PNG"));
	if ((lpData = (unsigned char*) LockResource( LoadResource(NULL,rec) )) == NULL)
		return 0;
	length = SizeofResource(NULL, rec);

	// Create a temporary file to store data
	FILE		*file;
#ifdef _WIN32
	{
		TCHAR szTmpDir[MAX_PATH+1];
		if (GetTempPath(sizeof(szTmpDir)/sizeof(szTmpDir[0]), szTmpDir) == 0)
			_tcscpy_s(szTmpDir, _T("."));

		TCHAR szTmpPath[MAX_PATH+1];
		if (GetTempFileName(szTmpDir, _T("bst"), 0, szTmpPath) == 0)
		{
			_tcscpy_s(szTmpPath, szTmpDir);
			if (szTmpPath[_tcslen(szTmpPath) - 1] != '\\')
				_tcscat_s(szTmpPath, _T("\\"));
 			_tcscat_s(szTmpPath, _T("BerouxShellTerminal.tmp"));
		}

		if (_tfopen_s(&file, szTmpPath, _T("w+bTD")) != 0)
		{
			// Last try to get a temp file.
			if (fopen_s(&file, "CuResMan.tmp", "w+bTD") != 0)
				return 0;
		}
	}
#else
	file = tmpfile();
	if (file == NULL)
		return 0;
#endif
	fwrite(lpData, sizeof(char), length, file);
	rewind( file );

	nTex = pngBindF(file, mipmap, trans, info, wrapst, minfilter, magfilter);

	fclose( file );
	_rmtmp();												// Delete temporary files
	return nTex;
}

GLuint Scene::ResourceManager::LoadTexture(const char *szFile, int mipmap, int trans, pngInfo *info, int wrapst, int minfilter, int magfilter)
{
	FILE	*file;
	if (fopen_s(&file, szFile, "rb") != 0)
		return 0;

	pngSetStandardOrientation( 1 );

	GLuint	nTex = pngBindF(file, mipmap, trans, info, wrapst, minfilter, magfilter);
	fclose(file);
	return nTex;
}

/** Give texture ID, the width of each character (must be square).
 * @return A number used to display later. Return 0 if an error occured.
 */
GLuint Scene::ResourceManager::BuildFont(GLuint texture, GLuint charwidth)
{
	int loop;
	float	cx;											// Holds Our X Character Coord
	float	cy;											// Holds Our Y Character Coord
	const float	csize = 1.0f/16.0f;
	GLuint	base;										// Holds The GenList Return Value

	base = glGenLists(256);								// Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, texture);				// Select Our Font Texture
	for (loop=0; loop<256; loop++)						// Loop Through All 256 Lists
	{
		cx=float(loop%16)/16.0f;						// X Position Of Current Character
		cy=float(loop/16)/16.0f;						// Y Position Of Current Character

		glNewList(base+loop,GL_COMPILE);				// Start Building A List
			glBegin(GL_QUADS);							// Use A Quad For Each Character
				glTexCoord2f(cx,1.0f-cy-csize);			// Texture Coord (Bottom Left)
				glVertex2i(0,0);						// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+csize,1.0f-cy-csize);	// Texture Coord (Bottom Right)
				glVertex2i(charwidth,0);				// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+csize,1.0f-cy);			// Texture Coord (Top Right)
				glVertex2i(charwidth,charwidth);		// Vertex Coord (Top Right)
				glTexCoord2f(cx,1.0f-cy);				// Texture Coord (Top Left)
				glVertex2i(0,charwidth);				// Vertex Coord (Top Left)
			glEnd();									// Done Building Our Quad (Character)
			glTranslated(charwidth/1.5f,0,0);			// Move To The Right Of The Character
		glEndList();									// Done Building The Display List
	}													// Loop Until All 256 Are Built

	return base;
}