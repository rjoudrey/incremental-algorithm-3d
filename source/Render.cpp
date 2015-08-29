#include "Render.h"
#include "platform.h"
#include <fstream>
#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#define PI 3.14159265f
#define DEGREE 180 / PI
#define RADIAN PI / 180

void SetSDLSurfacePixel(SDL_Surface *surface, int x, int y, int color);

void Render::ClearBuffers(bool colorBuffer, bool depthBuffer, bool stencilBuffer)
{
	glClear(GL_COLOR_BUFFER_BIT * colorBuffer | 
		    GL_DEPTH_BUFFER_BIT * depthBuffer | 
			GL_STENCIL_BUFFER_BIT * stencilBuffer);
}



void Render::EnableFaceCulling()
{
	glEnable(GL_CULL_FACE);
}

void Render::DisableFaceCulling()
{
	glDisable(GL_CULL_FACE);
}

void Render::ResetModelViewMatrix()
{
	glLoadIdentity();
}

void Render::SwapBuffers()
{
	SDL_GL_SwapBuffers();
}

void Render::Scale(float s)
{
	glScalef(s, s, s);
}

void Render::Rotate(float angle, float x, float y, float z)
{
	glRotatef(angle * DEGREE, x, y, z);
}

void Render::Translate(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void Render::Translate(float *position)
{
	glTranslatef(position[0], position[1], position[2]);
}

void Render::Scale(float x, float y, float z)
{
	glScalef(x, y, z);
}

void Render::SaveModelViewMatrix()
{
	glPushMatrix();
}

void Render::RestoreModelViewMatrix()
{
	glPopMatrix();
}

void Render::ResizeScene(int width, int height)
{                    
	glViewport(0, 0, width, height);               
	glMatrixMode(GL_PROJECTION);                 
	glLoadIdentity();               
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 900.0f);
	glMatrixMode(GL_MODELVIEW);      
	glLoadIdentity();                
}

void Render::EnableWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);;
}

void Render::DisableWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Render::EnableBlending()
{
	glEnable(GL_BLEND);
}

void Render::DisableBlending()
{
	glDisable(GL_BLEND);
}

void Render::Initialize(int width, int height)
{
	ResizeScene(width, height);
	glEnable(GL_NORMALIZE);
	glEnable(GL_MULTISAMPLE);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glFrontFace(GL_CCW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(.54f, .81f, .94f, 1.0f); 	
	glEnable(GL_DEPTH_TEST);   
	glDepthFunc(GL_LEQUAL);    
}

void Render::LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ)
{
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0);
}

void Render::DrawCube() 
{
	glFrontFace(GL_CW);
	glBegin(GL_QUADS);
	// BACK
	glNormal3f(0.0, 0.0, -1.0);  
	glTexCoord2d(1, 1);
	glVertex3f(  0.5, -0.5, -0.5 );
	glTexCoord2d(1, 0); 
	glVertex3f(  0.5,  0.5, -0.5 ); 
	glTexCoord2d(0, 0); 
	glVertex3f( -0.5,  0.5, -0.5 );
	glTexCoord2d(0, 1);
	glVertex3f( -0.5, -0.5, -0.5 ); 

	// FRONT
	glNormal3f(0.0, 0.0, 1.0); 
	glTexCoord2d(0, 1);
	glVertex3f(  0.5, -0.5, 0.5 );
	glTexCoord2d(1, 1);
	glVertex3f( -0.5, -0.5, 0.5 );
	glTexCoord2d(1, 0);
	glVertex3f( -0.5,  0.5, 0.5 );
	glTexCoord2d(0, 0);
	glVertex3f(  0.5,  0.5, 0.5 );

	// RIGHT
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2d(0, 1);
	glVertex3f( 0.5, -0.5, -0.5 );
	glTexCoord2d(1, 1);
	glVertex3f( 0.5, -0.5,  0.5 );
	glTexCoord2d(1, 0);
	glVertex3f( 0.5,  0.5,  0.5 );
	glTexCoord2d(0, 0);
	glVertex3f( 0.5,  0.5, -0.5 );

	// LEFT
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2d(0, 1);
	glVertex3f( -0.5, -0.5,  0.5 );
	glTexCoord2d(1, 1);
	glVertex3f( -0.5, -0.5, -0.5 );
	glTexCoord2d(1, 0);
	glVertex3f( -0.5,  0.5, -0.5 );
	glTexCoord2d(0, 0);
	glVertex3f( -0.5,  0.5,  0.5 );

	// TOP
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2d(0, 1);
	glVertex3f(  0.5,  0.5,  0.5 );
	glTexCoord2d(1, 1);
	glVertex3f( -0.5,  0.5,  0.5 );
	glTexCoord2d(1, 0);
	glVertex3f( -0.5,  0.5, -0.5 );
	glTexCoord2d(0, 0);
	glVertex3f(  0.5,  0.5, -0.5 );

	// BOTTOM
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2d(0, 1);
	glVertex3f(  0.5, -0.5, -0.5 );
	glTexCoord2d(1, 1);
	glVertex3f( -0.5, -0.5, -0.5 );
	glTexCoord2d(1, 0);
	glVertex3f( -0.5, -0.5,  0.5 );
	glTexCoord2d(0, 0);
	glVertex3f(  0.5, -0.5,  0.5 );

	glEnd();
	glFrontFace(GL_CCW);
}


