#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 testcolor;
uniform vec2 resolution;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	FragColor = mix(texture(texture1, TexCoord), vec4(testcolor,1.0), mod(TexCoord.x,1.0));
	//FragColor = texture(texture1, TexCoord);
}