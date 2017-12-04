#version 330 core
layout (location = 0) in vec3 point;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture;
out vec3 midNormal;
out vec3 fragPos;
out vec4 fragPosLightSpace;
out vec2 TexCoord;
uniform mat4 model;
uniform vec4 view;
uniform mat4 projection;
uniform vec3 viewPos;
uniform mat4 lightSpaceMatrix;

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
    vec4 tmpPos = model * vec4(point, 1.0f);
    fragPos = vec3(tmpPos);
	fragPosLightSpace = lightSpaceMatrix * tmpPos;
    gl_Position = tmpPos - vec4(viewPos, 0);
    vec4 _view = -view;
    _view.w = -_view.w;
    gl_Position = multiQuat(multiQuat(view, gl_Position), _view);
    gl_Position = projection * gl_Position;
    midNormal = mat3(transpose(inverse(model))) * normal;
    TexCoord = texture;
}
