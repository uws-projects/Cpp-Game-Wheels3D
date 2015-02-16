// Phong fragment shader phong-tex.frag matched with phong-tex.vert
#version 330

// Some drivers require the following
precision highp float;

uniform samplerCube cubeMap;
smooth in vec3 cubeCoordinates;

layout(location = 0) out vec4 out_Color;
 
void main(void) 
{
	out_Color = texture(cubeMap, cubeCoordinates);
}