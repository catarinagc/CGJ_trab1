#version 330 core

in vec4 inPosition;
in vec4 inColor;
out vec4 exColor;
uniform int GroupID;

uniform mat4 Matrix;

float random(vec3 seed) {
    return fract(sin(dot(seed, vec3(12.9898, 78.233, 45.164))) * 43758.5453123);
}

void main(void) {
    gl_Position = Matrix * inPosition;

    exColor = vec4(
        random(GroupID + vec3(0.0)),
        random(GroupID + vec3(1.0)),
        random(GroupID + vec3(2.0)),
        1.0f
    );
}
