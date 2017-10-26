#version 330 core
layout(location = 0)in vec3 pos;
layout(location = 1)in vec3 color;

out vec3 midColor;

uniform mat4 projection;
uniform vec4 view;
uniform mat4 model;
uniform vec3 viewPos;

vec4 multiQuat(vec4 v1, vec4 v2)
{
	vec4 v;
	v.w = v1.w * v2.w - v1.x * v2.x - v1.y * v2.y - v1.z * v2.z;
	v.x = v1.w * v2.x + v2.w * v1.x + v1.y * v2.z - v1.z * v2.y;
	v.y = v1.w * v2.y + v2.w * v1.y + v1.z * v2.x - v1.x * v2.z;
	v.z = v1.w * v2.z + v2.w * v1.z + v1.x * v2.y - v1.y * v2.x;
	return v;
}

void main(){
    gl_Position = model * vec4(pos, 1.0f);
    gl_Position -= vec4(viewPos, 0);
    vec4 _view = -view;
    _view.w = -_view.w;
    gl_Position = multiQuat(multiQuat(view, gl_Position), _view);
    gl_Position = projection * gl_Position;
    midColor = color;
}