#version 430

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main(void)
{	
	color = texture(ourTexture, TexCoord);
}