#version 120
// Vertex Shader

varying vec3 worldNormal;
varying vec4 color;
varying vec4 worldFragPos;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	worldNormal = gl_Normal;
	worldFragPos = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	color = gl_Color;
}




