#version 460 core

in vec3 position;
in vec2 texCoord;

out vec2 v_texCoord;

uniform mat4 _transformation;
uniform mat4 _projection;
uniform mat4 _view;

void main() {
	gl_Position = _projection * _view * _transformation * vec4(position, 1.0);
	v_texCoord = texCoord;
}
