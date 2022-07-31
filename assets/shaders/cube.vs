#version 460 core

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 v_normal;
out vec3 v_fragPos;
out vec2 v_texCoord;

uniform mat4 vp;
uniform mat4 model;

void main() {
	v_normal = mat3(transpose(inverse(model))) * normal;
	v_fragPos = vec3(model * vec4(position, 1.0));
	v_texCoord = texCoord;

	gl_Position = vp * vec4(v_fragPos, 1.0);
}
