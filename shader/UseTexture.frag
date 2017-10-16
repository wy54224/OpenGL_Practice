#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform vec4 uColor;
uniform sampler2D ourTexture;

void main(){
	color = texture(ourTexture, TexCoord);
}