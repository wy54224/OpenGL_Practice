#version 330 core
layout (location = 0)in vec3 pos;

uniform vec3 v;

void main(){
    mat4 trans = mat4(1.0f);
	trans[0][0] = v.x;
	trans[1][1] = v.y;
	trans[2][2] = v.z;
    gl_Position = trans * vec4(pos, 1.0f);
}