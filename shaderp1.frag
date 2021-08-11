#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D iChannel0;

uniform vec3 debug;
uniform vec2 iResolution;
uniform float iTime;

float rand(float co) { return fract(sin(co*(91.3458)) * 47453.5453); }
float rand(vec2 co){ return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453); }
float rand(vec3 co){ return rand(co.xy+rand(co.z)); }

vec2 fragCoord = TexCoord * iResolution;

const float blurSizex = 2.0/512.0;
const float blurSizey = 3.0/512.0;
const float intensity = 0.75;

void main()
{
	//FragColor = texture(iChannel0, TexCoord);
	
    vec4 sum = vec4(0);
    vec2 texcoord = fragCoord.xy/iResolution.xy;

    sum += texture(iChannel0, vec2(texcoord.x, texcoord.y)) * 0.20;

    sum += texture(iChannel0, vec2(texcoord.x + blurSizex, texcoord.y)) * 0.11;
    sum += texture(iChannel0, vec2(texcoord.x - blurSizex, texcoord.y)) * 0.11;
    sum += texture(iChannel0, vec2(texcoord.x, texcoord.y + blurSizey)) * 0.11;
    sum += texture(iChannel0, vec2(texcoord.x, texcoord.y - blurSizey)) * 0.11;

    sum += texture(iChannel0, vec2(texcoord.x + blurSizex, texcoord.y + blurSizey)) * 0.07;
    sum += texture(iChannel0, vec2(texcoord.x + blurSizex, texcoord.y - blurSizey)) * 0.07;
    sum += texture(iChannel0, vec2(texcoord.x - blurSizex, texcoord.y + blurSizey)) * 0.07;
    sum += texture(iChannel0, vec2(texcoord.x - blurSizex, texcoord.y - blurSizey)) * 0.07;

    sum += texture(iChannel0, vec2(texcoord.x + 2.0*blurSizex, texcoord.y)) * 0.02;
    sum += texture(iChannel0, vec2(texcoord.x - 2.0*blurSizex, texcoord.y)) * 0.02;
    sum += texture(iChannel0, vec2(texcoord.x, texcoord.y + 2.0*blurSizey)) * 0.02;
    sum += texture(iChannel0, vec2(texcoord.x, texcoord.y - 2.0*blurSizey)) * 0.02;

    //increase blur with intensity!
    FragColor = sum*intensity + texture(iChannel0, texcoord); 
}