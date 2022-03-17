#version 430

in vec3 world_position;
in vec3 world_normal;

uniform vec3 camera;

out vec4 frag_color;

void main() {
    float cosTheta = max(dot(normalize(world_normal), normalize(camera - world_position)), 0.0f);
    vec3 color = vec3(0.5) + pow(cosTheta, 4) * 0.5;

    frag_color = vec4(color, 1.0);
}
