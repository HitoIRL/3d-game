#version 460 core

in vec3 position;
in vec2 texCoords;

out vec2 v_texCoords;

uniform mat4 vp;
uniform mat4 model;

void main() {
	gl_Position = vp * model * vec4(position, 1.0);

    v_texCoords = texCoords;
}
