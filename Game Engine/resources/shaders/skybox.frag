#version 140
in vec3 texCoord;
out vec4 fragmentColour;

uniform samplerCube sampler;

void main(){
	fragmentColour = vec4(texture(sampler, texCoord).rgb, 1.0f);
}