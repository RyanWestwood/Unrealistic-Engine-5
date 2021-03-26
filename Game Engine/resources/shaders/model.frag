#version 140
in vec2 uv;
out vec4 fragmentColour;

uniform sampler2D sampler;

void main()
{
	fragmentColour = texture(sampler, uv).rgba;
}