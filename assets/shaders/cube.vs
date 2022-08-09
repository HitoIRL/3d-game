#version 460 core

in vec3 position;
in vec2 texCoords;
in int texIndex;

out vec2 v_texCoords;
flat out int v_texIndex;

uniform mat4 vp;
uniform mat4 model;

void main() {
    v_texCoords = texCoords;
    v_texIndex = texIndex;

	gl_Position = vp * model * vec4(position, 1.0);
}
