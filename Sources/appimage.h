/*
 * appimage Header File
 * 
 * Many open source games commercial data files required. 
 * Usually when launching such games without commercial content, 
 * the game ends without any special warnings. Or with one line 
 * in the console. This file, when integrated into the source code, 
 * will show a beautiful window with information about the need for 
 * a legal commercial game date. And it will also show the location 
 * of the game date for the user in the home directory.
 *
 * Alexander Pavlov <t.x00100x.t@yandex.ru>
 *
 */

#ifndef INCL_GAME_H
#define INCL_GAME_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


/************************************************************
 *                       Structures                         *
 ************************************************************/

struct GLTexture {
	const char* strTexFileName;
	unsigned int id;
	int width;
	int height;
};

struct Position {
	float x;
	float y;
};

struct Color {
	unsigned char r,g,b,a;
};

struct UV {
	float u;
	float v;
};

struct Vertex2D {
	struct Color color;
	struct Position position;
	struct UV uv;
};

/************************************************************
 *                         Sprite                           *
 ************************************************************/
	extern	void 	sprExit(void);
	void 	sprDraw(void);
	void 	sprInit(float x, float y, float widht, float height);

/************************************************************
 *                     Shader Program                       *
 ************************************************************/
	void 	shpUse(void);
	void 	shpUnuse(void);
	void 	shpExit(void);
	void 	shpLinkShaders(void);
	void 	shpAddAtribute(const char* strAtrName);
	void 	shpCompileShader(const char* Source, unsigned int id);
	void 	shpCompileShaderFromSource(const char* VertexSource, const char* FragmentSource);
	unsigned int 	shpGetUniformLocation(const char* strUniform);

/************************************************************
 *                        AppImage                          *
 ************************************************************/
	void 	appCreate(int, int);
	void 	appRun(void);
	void 	appExit(void);
	void 	appInitShader(void);
	void 	appUpdate(void);
	void 	appRender(void);
	void 	appInitSDL(void);
	struct 	GLTexture sdlLoadImage(const char* strFileName);

#endif
