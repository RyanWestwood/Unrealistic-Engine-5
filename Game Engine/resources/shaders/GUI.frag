#version 140

in vec2 uv;

uniform sampler2D sampler;

out vec4 fragmentColour;

void main()
{
	vec4 texColour = texture(sampler, uv);

	fragmentColour = texColour;
}