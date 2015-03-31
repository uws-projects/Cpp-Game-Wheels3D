// textured.frag
#version 330

// Some drivers require the following
precision highp float;

uniform sampler2D tex;
uniform float alfa;

in vec2 TexCoord;

out vec4 out_Color;
 
void main(void) {
   
	if (alfa == 0)
	{
		out_Color = texture(tex, TexCoord);
	}
	else
	{
		out_Color = texture(tex, TexCoord) * vec4(alfa);
	}
}