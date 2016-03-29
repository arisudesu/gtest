#version 120

varying vec2 texcoord_;
uniform sampler2D texture;

void main(void)
{
    float a = texture2D(texture, texcoord_).r;
    gl_FragColor = vec4(0, 0, 0, a);
}
