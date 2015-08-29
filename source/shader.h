#include <string>
using namespace std;

#ifndef SHADER_H
#define SHADER_H

class Shader
{
	string vertexShaderName, fragmentShaderName;
	int programNum, alternateProg;
	void SetProgramAttributes(string vertexShaderName, string fragmentShaderName);
	void LoadShaderProgram();
public:
	int GetProgramNum();
	static Shader defaultShader;
	static void InitShaderSystem(string defaultVertexShaderPath, string defaultFragmentShaderPath);
	Shader() { programNum = -1; }
	Shader(string vertexShaderName, string fragmentShaderName);
	void Activate();
	enum SHADER_INPUT_DATA_TYPE { INT, BOOL, FLOAT, FLOAT2D, FLOAT3D, FLOAT4D, MATRIX_4x4_FLOAT };
	void UseDataInShader(SHADER_INPUT_DATA_TYPE typeOfVarInShader, string nameOfVarInShader, void *data);
};

#endif
