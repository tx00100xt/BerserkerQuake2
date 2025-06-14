/*
 * appimage Source File
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

//#include "quakedef.h"
#include <unistd.h>
#include "appimage.h"
#include <GL/glew.h>

typedef enum { false, true } BOOL;
/************************************************************
 *                          Vars                            *
 ************************************************************/
static double	maxFPS;
static double	maxPeriod;
static int		iHeight;		// window size Height
static int		iWidth;			// window size Width
static BOOL		bAppImageLoop;
static GLuint		ulNumAttributes;
static GLuint		ulProgramID;	
static GLuint		ulVertexShadersID; 
static GLuint		ulFragmentShadersID;
static GLuint		vboID;
static SDL_Event	sdlEvent;
static SDL_Window	*sdlWindow;
static struct GLTexture appTexture;

/************************************************************
 *                        AppImage                          *
 ************************************************************/
void appCreate(int width, int height)
{
	bAppImageLoop = true;	// contains event loop trigger if true then loop continues
	iWidth = 1280;		// by default the width is 1280 and the height of the window is 720
	iHeight = 728;
	ulNumAttributes = 0;
	vboID = 0;
	ulProgramID = 0; 
	ulVertexShadersID = 0;
	ulFragmentShadersID = 0;

	if(width) iWidth	= width;		// initialize height and width
	if(height) iHeight	= height;
	bAppImageLoop = true;				// any event loop trigger
	appInitSDL();					// initialize the SDL library
	appInitShader();
	sprInit(-1.0, -1.0, 2.0, 2.0);
	
	// AppImage texture
	appTexture = sdlLoadImage("berserkerq2.jpg");

	// OpenGL frame rate limit
	maxFPS = 60.0;
	maxPeriod = 1.0 / maxFPS;
}

void appInitSDL(void) 
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL_Init Error: %s\n",SDL_GetError());
		exit(1);
	}

	Uint32 ulFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	sdlWindow = SDL_CreateWindow(" .: AppImage BerserkerQuake2 Window :. ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, iWidth, iHeight, ulFlags);
	if (!sdlWindow) {
		printf("SDL_Window Error: %s\n",SDL_GetError());
		exit(1);
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
	if (glContext == NULL) {
		printf("SDL_GL_CreateContext Error: %s\n",SDL_GetError());
		exit(1);
	}

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		printf("GLEW init error!\n");
		exit(1);
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.1,0.1,0.1,1);
}

void appInitShader(void) 
{
	// vertex shader
	const char * VShader = \
R"(
	#version 110
	attribute vec2	vertPos;
	attribute vec4	vertCol;
	attribute vec2	vertUV;
	varying vec4	fragCol;
	varying vec2	fragPos;
	varying vec2	fragUV;
	void main()
	{
		gl_Position	= vec4 (vertPos, 0.0, 1.0);
		fragPos		= vertPos;
		fragCol		= vertCol;
		fragUV		= vec2( vertUV.x, 1.0 - vertUV.y);
	}
)";

	// fragment shader
	const char * FShader = \
R"(
	#version 110
	varying vec4		fragCol;
	varying vec2		fragPos;
	varying vec2		fragUV;
	uniform float		brightness;
	uniform sampler2D	mSample;

	void main()
	{
		float bright = sin(brightness) + 1.75;
		bright = ( bright > 0.75) ? bright : 0.75;
		bright = ( bright < 2.5 ) ? bright : 2.5;
		gl_FragColor = fragCol * bright * texture2D (mSample, fragUV );
	}
)";

	shpCompileShaderFromSource(VShader, FShader);
	shpAddAtribute("vertPos");
	shpAddAtribute("vertCol");
	shpAddAtribute("vertUV");
	shpLinkShaders();
}

void appRun(void) 
{
	double lastTime = 0.0;
	while (bAppImageLoop)
	{
		double time = SDL_GetTicks();
		double deltaTime = time - lastTime;

		if( deltaTime >= maxPeriod ) {
		lastTime = time;
		// called with maxFPS (60)
			appUpdate();
			appRender();
		}
	}
}

void appUpdate(void) 
{
	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type) {
			case SDL_QUIT: {
					bAppImageLoop = false;
					break;
				}
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
					bAppImageLoop = false;
					break;
				}
			case SDL_WINDOWEVENT:
				if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE) {
					bAppImageLoop = false;
					break;
				}
				break;
		}
	}
}

void appRender(void) 
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shpUse();
	glActiveTexture(GL_TEXTURE0); 
	GLuint mSample = shpGetUniformLocation("mSample");
	glBindTexture(GL_TEXTURE_2D, appTexture.id);
	glUniform1f(mSample, 0);

	GLuint ulBrightnessLoc = shpGetUniformLocation("brightness");
	static float brightness = 0;
	brightness += 0.03f;
	glUniform1f(ulBrightnessLoc, brightness);

	sprDraw();
	shpUnuse();
	SDL_GL_SwapWindow(sdlWindow);
}

struct GLTexture sdlLoadImage(const char* strFileName) 
{
	struct GLTexture texture = {};
	texture.strTexFileName = strFileName;

	char path_with_img[4096]; 
	char *path = SDL_GetBasePath();
	strcpy(path_with_img, path);
	strncat(path_with_img, "berserkerq2.jpg", 15);
	printf("Path to Texture: %s\n", (const char*)path_with_img);

	SDL_Surface* sdlImage = IMG_Load(path_with_img);
	if (sdlImage == NULL) {
		printf("Load %s failed\n", (const char*)path_with_img);
		exit(1);
	}
	SDL_Surface* sdlConvertedImage = SDL_ConvertSurfaceFormat(sdlImage, SDL_PIXELFORMAT_ABGR8888, 0);
	
	texture.width = sdlConvertedImage->w;
	texture.height = sdlConvertedImage->h;

	glGenTextures(1, &(texture.id));
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdlConvertedImage->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(sdlConvertedImage); 
	SDL_FreeSurface(sdlImage); 
	return texture;
}

void appExit(void)
{
	if (sdlWindow) SDL_DestroyWindow(sdlWindow);	// destroy the window	
	sdlWindow = 0;					// reset the value, optional
	SDL_Quit();					// exit SDL
}


/************************************************************
 *                         Sprite                           *
 ************************************************************/
void sprInit(float x, float y, float widht, float height) 
{
	if (vboID == 0) {
		glGenBuffers(1, &vboID);
	}

	struct Vertex2D vertexData [6];
	// first triangle
	vertexData[0].position.x = x + widht;
	vertexData[0].position.y = y + height;
	vertexData[0].uv.u = 1.0f;
	vertexData[0].uv.v = 1.0f;
	vertexData[0].color.r = 255;
	vertexData[0].color.g = 255;
	vertexData[0].color.b = 255;
	vertexData[0].color.a = 255;
	vertexData[1].position.x = x;
	vertexData[1].position.y = y + height;
	vertexData[1].color.r = 255;
	vertexData[1].color.g = 255;
	vertexData[1].color.b = 255;
	vertexData[1].color.a = 255;
	vertexData[1].uv.u = 0.0f;
	vertexData[1].uv.v = 1.0f;
	vertexData[2].position.x = x;
	vertexData[2].position.y = y;
	vertexData[2].color.r = 255;
	vertexData[2].color.g = 255;
	vertexData[2].color.b = 255;
	vertexData[2].color.a = 255;
	vertexData[2].uv.u = 0.0f;
	vertexData[2].uv.v = 0.0f;

	// second triangle
	vertexData[3].position.x = x;
	vertexData[3].position.y = y;
	vertexData[3].color.r = 255;
	vertexData[3].color.g = 255;
	vertexData[3].color.b = 255;
	vertexData[3].color.a = 255;
	vertexData[3].uv.u = 0.0f;
	vertexData[3].uv.v = 0.0f;
	vertexData[4].position.x = x + widht;
	vertexData[4].position.y = y;
	vertexData[4].color.r = 255;
	vertexData[4].color.g = 255;
	vertexData[4].color.b = 255;
	vertexData[4].color.a = 255;
	vertexData[4].uv.u = 1.0f;
	vertexData[4].uv.v = 0.0f;
	vertexData[5].position.x = x + widht;
	vertexData[5].position.y = y + height;
	vertexData[5].color.r = 255;
	vertexData[5].color.g = 255;
	vertexData[5].color.b = 255;
	vertexData[5].color.a = 255;
	vertexData[5].uv.u = 1.0f;
	vertexData[5].uv.v = 1.0f;

	glBindBuffer (GL_ARRAY_BUFFER, vboID);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void sprDraw(void) 
{
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex2D), (void*) offsetof (struct Vertex2D, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(struct Vertex2D), (void*) offsetof (struct Vertex2D, color));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex2D), (void*) offsetof (struct Vertex2D, uv));

	glDrawArrays (GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void sprExit(void)
{
	if (vboID != 0) {
		glDeleteBuffers(1, &vboID);
	}
}


/************************************************************
 *                     Shader Program                       *
 ************************************************************/
void shpCompileShaderFromSource(const char* VertexSource, const char* FragmentSource) 
{
	ulProgramID = glCreateProgram();

	// Create a vertex shader object and save its ID
	ulVertexShadersID = glCreateShader (GL_VERTEX_SHADER);
	if (ulVertexShadersID == 0) {
		printf("Fatal error: Vertex create error !"); 
		exit(1);
	}

	// Create a fragment shader object and save its ID
	ulFragmentShadersID = glCreateShader (GL_FRAGMENT_SHADER);
	if (ulFragmentShadersID == 0) {
		printf("Fatal error: Fragment create error !"); 
		exit(1);
	}

	// Compile each shader
	shpCompileShader(VertexSource, ulVertexShadersID); 
	shpCompileShader(FragmentSource, ulFragmentShadersID);
}

void shpAddAtribute(const char* strAttrName) 
{	
	glBindAttribLocation(ulProgramID, ulNumAttributes++, strAttrName);
}

GLuint shpGetUniformLocation(const char* strUniform) 
{
	GLuint ulLocation = glGetUniformLocation(ulProgramID, strUniform);
	if (ulLocation == GL_INVALID_INDEX) {
		printf("Fatal error: Uniform not found in shader name: %s ", (const char*)strUniform);
		exit(1);
	}
	return ulLocation;
}

void shpLinkShaders(void) 
{
	// Vertex and fragment shaders are compiled.
	// Link them together into a program.
	glAttachShader(ulProgramID, ulVertexShadersID); 
	glAttachShader(ulProgramID, ulFragmentShadersID);

	// Attach a program for linking.
	glLinkProgram(ulProgramID);

	GLint ulIsLinked = 0;
	glGetProgramiv(ulProgramID, GL_LINK_STATUS, (int*)&ulIsLinked);

	if (ulIsLinked == GL_FALSE) {
		// Don't need the program anymore, let's delete it.
		glDeleteProgram(ulProgramID);
		// Also remove shaders.
		glDeleteShader(ulVertexShadersID);
		glDeleteShader(ulFragmentShadersID);
	}

	// Shaders must be removed after successful linking.
	// because the program is already compiled.
	glDetachShader(ulProgramID, ulVertexShadersID);
	glDetachShader(ulProgramID, ulFragmentShadersID);
	glDeleteShader(ulVertexShadersID);
	glDeleteShader(ulFragmentShadersID);
}

void shpUse(void) 
{
	glUseProgram(ulProgramID);
	ulNumAttributes = (ulNumAttributes < 3) ? ulNumAttributes : 3;
	for (GLuint i = 0; i < ulNumAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

void shpUnuse(void) 
{
	glUseProgram(0);
	ulNumAttributes = (ulNumAttributes < 3) ? ulNumAttributes : 3;
	for (GLuint i = 0; i < ulNumAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

void shpCompileShader(const char* Source, GLuint id) 
{
	// Tell opengl that we want to use source as the shader content.
	glShaderSource(id, 1, &Source, NULL);

	// Compile shader
	glCompileShader(id);

	// Check for errors
	GLint ulOk = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &ulOk);
	if (ulOk == GL_FALSE)
	{
		// Don't need the program anymore, let's delete it. 
		glDeleteShader(id);
	}
}

void shpExit(void)
{
	glUseProgram(0);
	if(ulProgramID) glDeleteProgram(ulProgramID);
}
