#version 330 core

layout (location = 0) in vec3 aPos;
uniform float colorOffset;
uniform float posOffset;
out vec4 vertexColor;

void main() {
    gl_Position = vec4(aPos.x + posOffset, aPos.y + posOffset, aPos.z, 1.0);
    vertexColor = vec4((aPos.x * 2 + 1 + colorOffset) / 3, (aPos.y * 2 + 1 + colorOffset) / 3, (aPos.z * 2 + 1 + colorOffset) / 3, 1.0);
}