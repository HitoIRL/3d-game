#version 460 core

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 v_normal;
out vec3 v_fragPos;
out vec2 v_texCoord;

uniform mat4 mvp;
uniform mat4 model;

void main() {
	gl_Position = mvp * vec4(position, 1.0);
	v_normal = mat3(transpose(inverse(model))) * normal; // calculate on CPU
	v_fragPos = vec3(model * vec4(position, 1.0));
	v_texCoord = texCoord;
}
