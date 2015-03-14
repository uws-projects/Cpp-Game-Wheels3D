// phong-tex.vert
// Vertex shader for use with a Phong or other reflection model fragment shader
// Calculates and passes on V, L, N vectors for use in fragment shader, phong2.frag
#version 330

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec2 VertexTexCoord;

out vec2 TexCoord;
// multiply each vertex position by the MVP matrix
void main(void) {

	// vertex into eye coordinates
	vec4 position = ModelViewMatrix * vec4(VertexPosition,1.0);
	gl_Position = ProjectionMatrix * position;

	TexCoord = VertexTexCoord;
}