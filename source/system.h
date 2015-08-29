#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>

using namespace std;
// Distances API-specific window intialization, key-presses, events, and timer functionality from user

namespace System
{
	extern void Initialize(string programName, int windowWidth, int windowHeight);
	extern void Destroy();
	extern void GetRelativeMousePos(int &deltaX, int &deltaY);
	extern bool IsKeyDown(string key);
	extern void UpdateState();
	extern void CenterMouse();
	extern int GetTime();
	extern void DelayFrameRate(int targetFrameRate);
	extern bool cursorVisible;
}

#endif