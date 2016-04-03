#version 120

uniform mat4 proj;

attribute vec3 position;
varying vec3 col;

void main(void)
{
        gl_Position   = proj * vec4(position, 1.0);
        col = position;
}
