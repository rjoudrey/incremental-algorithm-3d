#include "system.h"
#include <iostream>

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "platform.h"

namespace System
{
	Uint8 *keys;
	int mouseX, mouseY, time;
	bool cursorVisible = false;
}


void System::Initialize(string programName, int windowWidth, int windowHeight)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
	{
		char *errorMsg = SDL_GetError();
		cout << errorMsg << endl;
		exit(-1);
	}

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_WM_SetCaption(programName.c_str(), NULL);

	if (!SDL_SetVideoMode(windowWidth, windowHeight, 32, SDL_OPENGL)) 
	{
		char *errorMsg = SDL_GetError();
		cout << errorMsg << endl;
		exit(-1);
	}

	time = 0;
	SDL_EnableKeyRepeat(1, 1);
}

void System::UpdateState()
{
	SDL_PumpEvents();
	time = SDL_GetTicks();
	keys = SDL_GetKeyState(NULL);
	SDL_GetRelativeMouseState(&mouseX, &mouseY);
}

void System::GetRelativeMousePos(int &deltaX, int &deltaY)
{
	deltaX = mouseX; deltaY = mouseY;
}

bool System::IsKeyDown(string key)
{
	char ascii = key[0];

	if (key.compare("ESC") == 0)
	{
		if (keys[SDLK_ESCAPE])
			return true;
	}
	else if (ascii >= 'a' && ascii <= 'z')
	{
		Uint8 SDLKeyValue = SDLK_a + ascii - 'a';
		if (keys[SDLKeyValue])
		{
			return true;
		}
	}

	return false;
}


void System::DelayFrameRate(int targetFrameRate)
{
	if (SDL_GetTicks() - time < 1000 / targetFrameRate)
		SDL_Delay((1000 / targetFrameRate ) - (SDL_GetTicks() - time));
}

int System::GetTime() { return SDL_GetTicks(); } 

void System::CenterMouse()
{
	const SDL_VideoInfo* screenInfo = SDL_GetVideoInfo();
	if (SDL_GetAppState() != SDL_APPMOUSEFOCUS && SDL_GetAppState() != SDL_APPACTIVE)
		SDL_WarpMouse(screenInfo->current_w / 2, screenInfo->current_h / 2);
	// Dont want this movement to affect next relative mouse position
	UpdateState();
}