#version 330 core

uniform vec3 lightColor;
uniform float alpha;

out vec4 color;

void main() {

	color = vec4(lightColor, alpha);

}