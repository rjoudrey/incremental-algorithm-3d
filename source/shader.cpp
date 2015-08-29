#include "shader.h"
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "platform.h"

namespace ShaderUtility
{
	typedef GLhandleARB (APIENTRYP  PFNGLCREATESHADEROBJECTARBPROC) (GLenum shaderType);
	typedef void (APIENTRYP PFNGLSHADERSOURCEARBPROC) (GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length);
	typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
	typedef void (APIENTRYP PFNGLATTACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB obj);
	typedef void (APIENTRYP PFNGLLINKPROGRAMARBPROC) (GLhandleARB programObj);
	typedef void (APIENTRYP PFNGLUSEPROGRAMOBJECTARBPROC) (GLhandleARB programObj);
	typedef GLhandleARB (APIENTRYP PFNGLCREATEPROGRAMOBJECTARBPROC) (void);
	typedef void (APIENTRYP PFNGLGETINFOLOGARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
	typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar *name);
	typedef void (APIENTRYP PFNGLUNIFORM1IARBPROC) (GLint location, GLint v0); 
	typedef void (APIENTRYP PFNGLUNIFORM1FARBPROC) (GLint location, GLfloat v0); 
	typedef void (APIENTRYP PFNGLUNIFORM2FARBPROC) (GLint location, GLfloat v0, GLfloat v1); 
	typedef void (APIENTRYP PFNGLUNIFORM3FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2); 
	typedef void (APIENTRYP PFNGLUNIFORM4FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3); 
	typedef void (APIENTRYP PFNGGLUNIFORMMATRIX4FVARBPROC) (GLint location, GLsizei count, GLboolean tranpose, const GLfloat *value);

	PFNGLUNIFORM1IARBPROC glUniform1iARB; 
	PFNGLUNIFORM1FARBPROC glUniform1fARB; 
	PFNGLUNIFORM2FARBPROC glUniform2fARB; 
	PFNGLUNIFORM3FARBPROC glUniform3fARB; 
	PFNGLUNIFORM4FARBPROC glUniform4fARB; 
	PFNGGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB;

	PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
	PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
	PFNGLCOMPILESHADERPROC glCompileShaderARB;
	PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
	PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
	PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
	PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
	PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocationARB;

	void LoadSource(string filename, string &source);
	void LoadShaderProgram(int &program, string vertShaderName, string fragShaderName);
	int LoadShader(string filename, bool isVertexShader);

	void Initalize()
	{
		// Register the functions, whose interfaces are loaded at runtime depending on the driver/hardware of the client system
		glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC) SDL_GL_GetProcAddress("glCreateShaderObjectARB");
		glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) SDL_GL_GetProcAddress("glShaderSourceARB");
		glCompileShaderARB = (PFNGLCOMPILESHADERPROC) SDL_GL_GetProcAddress("glCompileShaderARB");
		glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC) SDL_GL_GetProcAddress("glAttachObjectARB");
		glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) SDL_GL_GetProcAddress("glLinkProgramARB");
		glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC) SDL_GL_GetProcAddress("glUseProgramObjectARB");
		glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) SDL_GL_GetProcAddress("glCreateProgramObjectARB");
		glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) SDL_GL_GetProcAddress("glGetInfoLogARB");

		glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONPROC) SDL_GL_GetProcAddress("glGetUniformLocationARB");
		glUniform1iARB = (PFNGLUNIFORM1IARBPROC) SDL_GL_GetProcAddress("glUniform1iARB");
		glUniform1fARB = (PFNGLUNIFORM1FARBPROC) SDL_GL_GetProcAddress("glUniform1fARB"); 
		glUniform2fARB = (PFNGLUNIFORM2FARBPROC) SDL_GL_GetProcAddress("glUniform2fARB"); 
		glUniform3fARB = (PFNGLUNIFORM3FARBPROC) SDL_GL_GetProcAddress("glUniform3fARB"); 
		glUniform4fARB = (PFNGLUNIFORM4FARBPROC) SDL_GL_GetProcAddress("glUniform4fARB");
		glUniformMatrix4fvARB = (PFNGGLUNIFORMMATRIX4FVARBPROC) SDL_GL_GetProcAddress("glUniformMatrix4fv");
		string shaderVersion = string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
		cout << "OpenGL Shading Language version " << shaderVersion << "\n\n";
	}

	void LoadShaderProgram(int &program, string vertShaderName, string fragShaderName)
	{
		if (vertShaderName == "" || fragShaderName == "")
			exit(-1);
#ifdef PLATFORM_OSX
		void *
#else
		int
#endif
			programPtr = glCreateProgramObjectARB();
		program = (intptr_t)programPtr;
		

		GLuint vertShaderNum = 0, fragShaderNum = 0;
		vertShaderNum = LoadShader(vertShaderName, true);
		fragShaderNum = LoadShader(fragShaderName, false);

		glAttachObjectARB(programPtr, 
#ifdef PLATFORM_OSX
		(void*)
#endif
			vertShaderNum
		);
		glAttachObjectARB(programPtr, 
#ifdef PLATFORM_OSX
		(void*)
#endif
			fragShaderNum
		);
		glLinkProgramARB(programPtr);
		
		/*
		char infoLog[1028]; for (int i = 0; i < 1028; ++i) infoLog[i] = 0;
		glGetInfoLogARB(program, 1028, NULL, infoLog);
		*/
	}

	int LoadShader(string filename, bool isVertexShader)
	{
		GLuint shaderConst = 0;
		if (isVertexShader)
			shaderConst = GL_VERTEX_SHADER_ARB;
		else
			shaderConst = GL_FRAGMENT_SHADER_ARB;
		GLuint shaderNum = (intptr_t)glCreateShaderObjectARB(shaderConst);
		string loadedSource;
		LoadSource(filename, loadedSource);
		char *front = &loadedSource.at(0);
	    
		glShaderSourceARB(
#ifdef PLATFORM_OSX
		(void*)
#endif
			shaderNum, 
			1, (const GLcharARB**)&front, NULL
		);
		glCompileShaderARB(shaderNum);
		
		char infoLog[1024]; for (int i = 0; i < 1024; ++i) infoLog[i] = 0;
		glGetInfoLogARB(
#ifdef PLATFORM_OSX
		(void*)
#endif
			shaderNum, 
			1024, NULL, infoLog
		);
		cout << "Compiling... \n" << infoLog;
		if (strstr(infoLog, "error C"))
		{
			cout << "Error detected. Halting" << endl << endl;
			exit(1);
		}
		cout << endl << endl;
		return shaderNum;
	}

	void LoadSource(string filename, string &source)
	{
		std::ostringstream contents;
		std::ifstream file(filename.c_str());
		if (!file.is_open()) {
			cout << "Failed to read shader at: " << filename << endl;
			exit(1);
		}
		else
			cout << "Loading Shader: " << filename << endl; 
		contents << file.rdbuf();
		source.append(contents.str());
	}

	void UseShader(int shaderNum)
	{
		glUseProgramObjectARB(
#ifdef PLATFORM_OSX
		(void*)
#endif
		shaderNum
	   );
	}
}

Shader Shader::defaultShader;

void Shader::InitShaderSystem(string defaultVertexShaderPath, string defaultFragmentShaderPath)
{
	ShaderUtility::Initalize();
	Shader::defaultShader = Shader(defaultVertexShaderPath, defaultFragmentShaderPath);
}

Shader::Shader(string vertexShaderName, string fragmentShaderName)
{
	SetProgramAttributes(vertexShaderName, fragmentShaderName);
	ShaderUtility::LoadShaderProgram(programNum, vertexShaderName, fragmentShaderName);
}

void Shader::SetProgramAttributes(string vertexShaderName, string fragmentShaderName)
{
	this->vertexShaderName = vertexShaderName; 
	this->fragmentShaderName = fragmentShaderName;
}
void Shader::LoadShaderProgram()
{
	ShaderUtility::LoadShaderProgram(programNum, vertexShaderName, fragmentShaderName);
}
void Shader::Activate()
{
	ShaderUtility::UseShader(programNum);
}
int Shader::GetProgramNum()
{
	return programNum;
}

void Shader::UseDataInShader(SHADER_INPUT_DATA_TYPE typeOfVarInShader, string nameOfVarInShader, void *data)
{
	Activate();
	int location = ShaderUtility::glGetUniformLocationARB(programNum, nameOfVarInShader.c_str());
	if (location == -1)
		cout << ">> Warning: Variable \"" << nameOfVarInShader << "\" in shader is unused. Has no effect.\n";
	else
	{
		switch (typeOfVarInShader)
		{
		case INT: {
			int *i = (int*)data;
			ShaderUtility::glUniform1iARB(location, *i);
		  } break;
		case FLOAT: {
			float *f = (float*)data;
			ShaderUtility::glUniform1fARB(location, *f);
		  } break;
		case BOOL: {
		bool *f = (bool*)data;
		ShaderUtility::glUniform1fARB(location, *f);
		} break;
		case FLOAT2D: {
			float *uv = (float*)data;
			ShaderUtility::glUniform2fARB(location, uv[0], uv[1]);
		  } break;
		case FLOAT3D: {
			float *xyz = (float*)data;
			ShaderUtility::glUniform3fARB(location, xyz[0], xyz[1], xyz[2]);
		  } break;
		case FLOAT4D: {
			float *xyzw = (float*)data;
			ShaderUtility::glUniform4fARB(location, xyzw[0], xyzw[1], xyzw[2], xyzw[3]);
		  } break;
		case MATRIX_4x4_FLOAT: {
			ShaderUtility::glUniformMatrix4fvARB(location, 1, false, (float*)data);
   		  } break;
		};
	}
}
