#version 430

layout(location = 0) in vec3 POSITION;

uniform mat4 MVP;

out vec3 world_position;

void main() {
    gl_Position = MVP * vec4(POSITION, 1.0f);
    world_position = POSITION;
}
