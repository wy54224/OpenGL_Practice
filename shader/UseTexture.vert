#version 330 core
layout (location = 0)in vec3 pos;
layout (location = 2)in vec2 texCoord;

uniform mat4 trans;
out vec2 TexCoord;

void main(){
	gl_Position = trans * vec4(pos, 1.0f);
	TexCoord = texCoord;
}