#version 120

varying vec3 col;

//out vec4 color;

void main(void)
{
        gl_FragColor = vec4(1 - col, 1.0);
}

