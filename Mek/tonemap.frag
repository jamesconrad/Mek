#version 430

in vec2 uv;
layout(location = 0) out vec4 colour;

uniform sampler2D tex;

float l;

void main()
{
    colour = texture2D(tex, uv);
    l = colour.r * 0.3 + colour.g * 0.6 + colour.b * 0.1;
    colour = vec4(vec3((l * 2 - 1) * 2 - 1), 1) * colour;
}
