#version 460 core

in vec2 v_texCoord;

out vec4 color;

uniform sampler2D _texture;

void main() {
	color = texture(_texture, v_texCoord);
}
