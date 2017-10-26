#version 330 core
layout (location = 0) in vec3 point;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture;
out vec3 midNormal;
out vec3 fragPos;
out vec2 TexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    vec4 tmpPos = model * vec4(point, 1.0f);
    fragPos = vec3(tmpPos);
    gl_Position = projection * view * tmpPos;
    midNormal = mat3(transpose(inverse(model))) * normal;
    TexCoord = texture;
}