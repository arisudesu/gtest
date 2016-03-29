#version 120

uniform mat4 transform;

attribute vec3 position;
attribute vec2 texcoord;

varying vec2 texcoord_;

void main(void)
{
        gl_Position   = transform * vec4(position, 1.0);
        texcoord_ = texcoord;
}
