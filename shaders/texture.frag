// textured.frag
#version 330

// Some drivers require the following
precision highp float;

uniform sampler2D tex;

in vec2 TexCoord;

out vec4 out_Color;
 
void main(void) {
   
	out_Color = texture(tex, TexCoord);
}