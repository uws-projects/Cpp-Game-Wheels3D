// Fragment shader: Diffuse, per-vertex shading with a single point light source
// using the following locations:
// 0 VertexPosition
// 1 VertexNormal
// 2 VertexTexCoord
// 3 VertexTangent
// 4 VertexColour
#version 330

in vec3 LightIntensity;
in vec2 ex_TexCoord;

uniform sampler2D tex;

layout(location = 0) out vec4 out_Color;

void main()
{
	out_Color = texture2D(tex, ex_TexCoord)*vec4(LightIntensity*1.13, 1.0);
}