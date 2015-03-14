// Vertex shader: Diffuse, per-vertex shading with a single point light source
// using the following locations:
// 0 VertexPosition
// 1 VertexNormal
// 2 VertexTexCoord
// 3 VertexTangent
// 4 VertexColour
#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

struct Light
{
	vec4 position;
	vec4 La;
	vec4 Ld;
	vec4 Ls;
};

struct Material
{
	vec4 Ka;
	vec4 Kd;
	vec4 Ks;
	float shininess;
};

uniform Light light[4];
uniform Material material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

uniform float attConst;
uniform float attLinear;
uniform float attQuadratic;

out vec3 LightIntensity;
out vec2 ex_TexCoord;

vec3 LightFunction(Light light)
{
	vec3 Kd = vec3(material.Kd);
	vec3 Ld = vec3(light.Ld);
	vec4 LightPosition = light.position;	
	// Convert normal and position to eye space
	vec3 norm = normalize( NormalMatrix * VertexNormal);
	vec4 position = ModelViewMatrix * vec4(VertexPosition, 1.0);
	float D = distance(position, LightPosition);
	vec3 s = normalize(vec3(LightPosition-position));

	// The light equation
	vec3 diffuseI = Ld * Kd * max ( dot (norm, s), 0.0);
	float attenuation = (attConst + attLinear * D + attQuadratic * D * D);
	if (attenuation == 0) attenuation = 1.0;
	return diffuseI/attenuation;
}

void main()
//		L = Kd x Ld * s * n;

//		Kd = diffuse reflectivity
//		Ld = intensity of the light source
//		 s = direction from surface to light
//		 n = normal 
{
	
	
	LightIntensity = vec3(0);
	for (int i = 0 ; i < 4 ; i++ )
	{
		LightIntensity += LightFunction(light[i]);
	}

	ex_TexCoord = VertexTexCoord;
	
	// Convert position to clip space
	gl_Position = MVP * vec4(VertexPosition, 1.0);

}