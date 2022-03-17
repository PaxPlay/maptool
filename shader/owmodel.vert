#version 430

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;

uniform mat4 MVP;
uniform mat4 M;
uniform mat3 NM;

out vec3 world_position;
out vec3 world_normal;

void main() {
    vec4 position = vec4(POSITION, 1.0f);
    vec4 posH = M * position;
    world_position = posH.xyz / posH.w;
    world_normal = -NM * NORMAL;
    gl_Position = MVP * position;
}
