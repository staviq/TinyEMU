#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D iChannel0;

uniform vec4 debug;
uniform vec2 iResolution;
uniform float iTime;

vec2 fragCoord = TexCoord * iResolution;

vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
	/*
	//FragColor = texture(iChannel0, TexCoord);
	//FragColor = mix(texture(iChannel0, TexCoord), vec4(debug,1.0), mod((TexCoord.x/4.0)+0.25,1.0));
	if( abs( TexCoord.x - debug.x ) < 0.01 && abs( TexCoord.y - debug.y ) < 0.01 )
	{
		FragColor = vec4( hsv2rgb( vec3( mod(debug.w*(iTime/100.0), 1.0), 1.0, 1.0 ) ), 1.0 );
		//FragColor = vec4( vec3( 1.0, 0.0, 0.0 ), 1.0 );
	}
	else
	{
		FragColor = vec4( texture(iChannel0, TexCoord).rgb, 1.0 );
	}
	*/
	FragColor = vec4( texture(iChannel0, TexCoord).rgb, 1.0 );
}