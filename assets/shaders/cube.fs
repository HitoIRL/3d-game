#version 460 core

in vec2 v_texCoords;
flat in int v_texIndex;

out vec4 color;

uniform sampler2D samplers[16];

void main() {
    vec4 texColor = texture(samplers[v_texIndex], v_texCoords);
    if (texColor.a < 0.5) {
        discard;
    }

    color = texColor;
}
