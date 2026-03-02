#version 430

layout (location=0) in vec3 position;
layout (location = 1) in vec3 aNormal;   // 法向量
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 aInstanceMatrix;   // 实例矩阵
layout (location = 7) in vec2 aInstanceColor;   // 实例颜色

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform vec2 texturezuobiao;
out vec2 TexCoord;

void main(void)
{
	gl_Position = proj_matrix * mv_matrix *aInstanceMatrix* vec4(position,1.0);
	TexCoord.x=aTexCoord.x+aInstanceColor.x;
	TexCoord.y=aTexCoord.y+aInstanceColor.y;
} 
