/*
 * Copyright (c) 2015 Jeff Boody
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "gears_renderer.h"

#define LOG_TAG "gears"
#include "a3d/a3d_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

static gears_renderer_t* gears_renderer = NULL;

static SDL_Window*   gWindow = NULL;
static SDL_GLContext gContext;

static const int SCREEN_WIDTH  = 1280;
static const int SCREEN_HEIGHT = 720;

/***********************************************************
* main                                                     *
***********************************************************/

int main(int argc, const char** argv)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOGE("SDL_Init failed: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	gWindow = SDL_CreateWindow("gears2",
	                           SDL_WINDOWPOS_UNDEFINED,
	                           SDL_WINDOWPOS_UNDEFINED,
	                           SCREEN_WIDTH,
	                           SCREEN_HEIGHT,
	                           SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if(gWindow == NULL)
	{
		LOGE("SDL_CreateWindow failed: %s", SDL_GetError());
		goto fail_window;
	}

	gContext = SDL_GL_CreateContext(gWindow);
	if(gContext == NULL)
	{
		LOGE("SDL_GL_CreateContext failed: %s", SDL_GetError());
		goto fail_context;
	}

	if(SDL_GL_SetSwapInterval(1) < 0)
	{
		LOGW("SDL_GL_SetSwapInterval failed: %s", SDL_GetError());
	}

	// Initialize gears renderer
	gears_renderer = gears_renderer_new("whitrabt.texgz");
	if(gears_renderer == NULL)
	{
		goto fail_renderer;
	}
	gears_renderer_resize(gears_renderer,
	                      SCREEN_WIDTH,
	                      SCREEN_HEIGHT);

	// main loop
	int running = 1;
	while(running)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
			{
				running = 0;
			}
		}

		gears_renderer_draw(gears_renderer);
		SDL_GL_SwapWindow(gWindow);
	}

	// success
	SDL_GL_DeleteContext(gContext);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
	return EXIT_SUCCESS;

	// failure
	fail_renderer:
	SDL_GL_DeleteContext(gContext);
	fail_context:
		SDL_DestroyWindow(gWindow);
	fail_window:
		SDL_Quit();
	return EXIT_FAILURE;
}
