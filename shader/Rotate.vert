#version 330 core
layout (location = 0)in vec3 pos;

uniform float angle;
uniform vec3 axis;

void main(){
    mat4 trans = mat4(1.0f);
	float l = sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
	float x = axis.x / l, y = axis.y / l, z = axis.z / l;
    trans[0][0] = cos(angle) + x * x * (1.0f - cos(angle));
    trans[1][0] = x * y * (1.0f - cos(angle)) - z * sin(angle);
    trans[2][0] = x * z * (1.0f - cos(angle)) + y * sin(angle);
    trans[0][1] = y * x * (1.0f - cos(angle)) + z * sin(angle);
    trans[1][1] = cos(angle) + y * y * (1.0f - cos(angle));
    trans[2][1] = y * z * (1.0f - cos(angle)) - x * sin(angle);
    trans[0][2] = z * x * (1.0f - cos(angle)) - y * sin(angle);
    trans[1][2] = z * y * (1.0f - cos(angle)) + x * sin(angle);
    trans[2][2] = cos(angle) + z * z * (1.0f - cos(angle));
    gl_Position = trans * vec4(pos, 1.0f);
}