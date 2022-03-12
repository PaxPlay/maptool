#version 400

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 COLOR;

uniform mat4 MVP;

out vec3 world_position;
out vec3 color;

void main() {
    gl_Position = MVP * vec4(POSITION, 1.0f);
    world_position = POSITION;
    color = COLOR;
}
