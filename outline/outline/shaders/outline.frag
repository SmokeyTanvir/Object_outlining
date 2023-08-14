#version 330 core
out vec4 outlineColor;
in vec3 Normal;

void main(){
	outlineColor = vec4(0.7, 0.7, 0.7, 1.0);
}