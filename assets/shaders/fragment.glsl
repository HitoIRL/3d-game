#version 460 core

in vec2 v_texCoord;

out vec4 color;

uniform sampler2D diffuse;

void main() {
	color = texture(diffuse, v_texCoord);
}
