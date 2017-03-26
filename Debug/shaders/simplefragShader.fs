#version 330 core

in vec3 position;
out vec3 color;

uniform mat4 mvp;

void main() {
    color = vec3(abs(position.x),abs(position.y),abs(position.z));
}