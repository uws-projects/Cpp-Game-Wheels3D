// phongTexMap.vert
// Vertex shader for use with a Phong or other reflection model fragment shader
// Calculates and passes on V, L, N vectors for use in fragment shader, phong2.frag
#version 330

const int numberOfLights = 4;

struct Light
{
	vec4 La;
	vec4 Ld;
	vec4 Ls;
	vec4 position;
	float constant;
	float linear;
	float quadratic;
};

uniform Light light[numberOfLights];

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

uniform vec3 cameraPos;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

out vec2 originalCoordinates;
out vec3 cameraTangent;
out vec3 lightTan[numberOfLights];
out float lightRayLength[numberOfLights];

// multiply each vertex position by the MVP matrix
// and find V, L, N vectors for the fragment shader
void main(void) {

	vec4 vertexPosition = ModelViewMatrix * vec4(VertexPosition,1.0);

	// Vector from vertex to viewer source
	vec3 vertexToCamera = normalize(-vertexPosition).xyz;

	vec3 normal = normalize(NormalMatrix * VertexNormal);
	vec3 tangent = (NormalMatrix * VertexTangent.xyz);
	vec3 binormal = cross(normal,tangent) * VertexTangent.w;
	
	mat3 TBN = transpose(mat3(tangent, binormal, normal));
	
	cameraTangent = TBN * vertexToCamera;

	for (int i = 0; i < numberOfLights; i++)
	{
	vec3 vertexToLight = normalize(light[i].position.xyz - vertexPosition.xyz);
	lightRayLength[i] = length(vertexToLight);
	lightTan[i]	= TBN * vertexToLight;
	}
	
	originalCoordinates = VertexTexCoord;

    gl_Position = ProjectionMatrix * vertexPosition;
}