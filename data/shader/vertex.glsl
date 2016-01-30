#version 330 core

uniform mat4 proj;

in vec3 position;
out vec3 col;

void main(void)
{
        gl_Position   = proj * vec4(position, 1.0);
        col = position;
}
