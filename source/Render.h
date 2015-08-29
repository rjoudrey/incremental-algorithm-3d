#include <string>

#ifndef RENDER_H
#define RENDER_H

using namespace std;

namespace Render
{
	extern void DrawText(string text);
	extern void SetColor(float r, float g, float b, float a);
	extern int GenerateNoiseTexture(int size);

	extern int TextureForBMP(string filepath, int textureUnit);
	extern int LoadTexture(void *pixels, int width, int height, int bytesPerPixel, int textureUnit);
	extern void BindTexture(int texture, int unit);
	extern void DeleteTexture(int texture);
	extern void Initialize(int width, int height);
	extern void ResizeScene(int width, int height);
	// Applies a matrix stored in row major order to the current Modelview matrix
	extern void ApplyToModelViewMatrix(float *matrix);
	extern void Translate(float *position);
	extern void Translate(float x, float y, float z);
	// Rotates the modelview matrix by angle radians
	extern void Rotate(float angle, float x, float y, float z);
	extern void Scale(float x, float y, float z);
	extern void Scale(float s);
	extern void DrawSquare();
	extern void DrawCube();
	extern void SaveModelViewMatrix();
	extern void RestoreModelViewMatrix();
	// Returns the Modelview matrix stored in row major order
	extern void GetModelViewMatrix(float *matrix);
	extern void ClearBuffers(bool colorBuffer, bool depthBuffer, bool stencilBuffer);
	extern void ResetModelViewMatrix();
	extern void LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ);

	extern void EnableFaceCulling();
	extern void DisableFaceCulling();
	extern void EnableWireframe();
	extern void DisableWireframe();
	extern void EnableBlending();
	extern void DisableBlending();
	extern void EnableDepthTest();
	extern void DisableDepthTest();
	extern void SwapBuffers();
};


#endif