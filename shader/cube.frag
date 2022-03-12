#version 430

in vec3 world_position;

out vec4 frag_color;

void main() {
    frag_color = vec4(world_position, 1.0);
}
