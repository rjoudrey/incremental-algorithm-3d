#version 120
// Fragment Shader

#define M_PI 3.1415926535897932384626433832795

varying vec3 worldNormal;
varying vec4 worldFragPos;
varying vec4 color;
uniform sampler2D texture0;
uniform sampler2D texture1;
struct MaterialColor
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};
uniform MaterialColor matColor;

struct Light
{
	vec3 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
uniform Light light;

void main()
{		
	vec4 lightPosEye = gl_ModelViewMatrix * vec4(light.position.x, light.position.y, light.position.z, 1.0);
	vec4 fragPosEye = gl_ModelViewMatrix * worldFragPos;
	vec3 fragToLightEye = normalize(lightPosEye - fragPosEye).xyz;
	
	vec4 ambientColor = matColor.ambient * light.ambient;
	vec4 diffuseColor = matColor.diffuse * light.diffuse;
	vec4 specularColor = matColor.specular * light.specular;
	vec4 textureColor = texture2D(texture0, gl_TexCoord[0].st);
	// world -> eye
	vec3 normalEye = normalize(gl_NormalMatrix * worldNormal);
	
	float percentDiffuse = clamp(dot(fragToLightEye, normalEye), 0.0, 1.0);
	
	vec3 L = normalize(lightPosEye.xyz - fragPosEye.xyz); 
	vec3 V = normalize(vec3(0.0, 0.0, 0.0) - fragPosEye.xyz);
	vec3 H = normalize(L + V);
	float percentSpecular = pow(max(0.0, dot(H, normalEye)), matColor.shininess);
	
	vec4 finalDiffuseColor = diffuseColor * percentDiffuse;
	vec4 finalSpecularColor = specularColor * percentSpecular;

	gl_FragColor = clamp(ambientColor + finalDiffuseColor + finalSpecularColor, 0.0, 1.0);
	if (color.a != 0.0)	{
		gl_FragColor *= color;
	}
	// if (textureColor.a != 0.0) (this does not work in all cases)
	//	gl_FragColor *= textureColor;
}