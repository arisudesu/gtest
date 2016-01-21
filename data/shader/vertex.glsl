#version 330 core

in vec3 position;
in vec3 color;
out vec3 fcolor;
uniform mat4 viewm;
uniform mat4 modelm;

void main(void)
{
    fcolor = vec3(mod(floor(position.x), 16)/16, 1 - mod(floor(position.y), 16)/16, 0.5);
    gl_Position = viewm * modelm * vec4(position, 1.0);
}
