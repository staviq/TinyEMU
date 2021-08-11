#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D iChannel0;

uniform vec3 debug;
uniform vec2 iResolution;
uniform float iTime;

vec2 fragCoord = TexCoord * iResolution;

void main()
{
	//FragColor = texture(iChannel0, TexCoord);
	FragColor = mix(texture(iChannel0, TexCoord), vec4(debug,1.0), mod(TexCoord.x,1.0));
}