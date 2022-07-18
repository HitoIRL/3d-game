#version 460 core

in vec3 position;
in vec2 texCoord;

out vec2 v_texCoord;

void main(){
	gl_Position = vec4(position, 1.0);
	v_texCoord = texCoord;
}
