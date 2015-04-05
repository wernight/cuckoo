/**
 * Copyright (c) 2003 Werner BEROUX. All Rights Reserved.
 *
 * The copyright to the contents herein is the property of Werner BEROUX.
 * The contents may be used and/or copied only with the written permission of
 * Werner BEROUX, or in accordance with the terms and conditions stipulated in
 * the agreement/contract under which the contents have been supplied.
 *
 * @brief Manage game resources.
 * @author Werner BEROUX
 * @version 1.00
 * @date 2004.03.18
 */

#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#ifdef _WIN32
#pragma once
#endif

namespace Scene
{
	class ResourceManager
	{
	public:
		ResourceManager(void);
		~ResourceManager(void);

	// Operations
		static GLuint LoadTexture(int ressource, int mipmap, int trans, pngInfo *info, int wrapst, int minfilter, int magfilter);
		static GLuint LoadTexture(const char *szFile, int mipmap, int trans, pngInfo *info, int wrapst, int minfilter, int magfilter);
		static GLuint BuildFont(GLuint texture, GLuint charwidth);
	};
}

#endif