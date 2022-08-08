#version 460 core

in vec2 v_texCoords;

out vec4 color;

uniform sampler2D samplers[16];
uniform int texIndex;

void main() {
    color = texture(samplers[texIndex], v_texCoords);
}
