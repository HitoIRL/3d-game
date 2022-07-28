#version 460 core

in vec3 position;
in vec2 texCoord;
in float texIndex;

out vec2 v_texCoord;
out float v_texIndex;

uniform mat4 mvp;

void main() {
	gl_Position = mvp * vec4(position, 1.0);
	v_texCoord = texCoord;
	v_texIndex = texIndex;
}
