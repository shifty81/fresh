#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 modelViewProj;

void main() {
    gl_Position = modelViewProj * vec4(inPosition, 1.0);
    fragNormal = inNormal;
    fragPos = inPosition;
}
