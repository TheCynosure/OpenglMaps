#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

out vec3 position;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(vertexPosition_modelspace,1);
    position = vertexPosition_modelspace.xyz;
}