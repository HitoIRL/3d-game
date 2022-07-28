#version 460 core

in vec2 v_texCoord;
in float v_texIndex;

out vec4 color;

uniform sampler2D samplers[16];

void main() {
	color = texture(samplers[int(v_texIndex)], v_texCoord);
}
