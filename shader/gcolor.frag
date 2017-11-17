#version 330 core
in vec3 midColor;
out vec4 color;

void main(){
    color = vec4(midColor, 1.0f);
}