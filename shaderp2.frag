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

vec2 curve(vec2 uv)
{
	uv = (uv - 0.5) * 2.0;
	uv *= 1.1;	
	uv.x *= 1.0 + pow((abs(uv.y) / 8.0), 2.0);
	uv.y *= 1.0 + pow((abs(uv.x) / 7.0), 2.0);
	uv  = (uv / 2.0) + 0.5;
	uv =  uv *0.92 + 0.04;
	return uv;
}

// Emulated input resolution.

//vec2 res = 640.0*vec2(1.0,iResolution.y/iResolution.x);
#define res (iResolution.xy/floor(1.0+iResolution.xy/2048.0))

// Hardness of scanline.
//  -8.0 = soft
// -16.0 = medium
const float hardScan=-8.0;

// Hardness of pixels in scanline.
// -2.0 = soft
// -4.0 = hard
const float hardPix=-3.0;

// Display warp.
// 0.0 = none
// 1.0/8.0 = extreme
const vec2 warp=vec2(1.0/32.0,1.0/24.0); 

// Amount of shadow mask.
const float maskDark=0.3;
const float maskLight=2.0;

//------------------------------------------------------------------------

// sRGB to Linear.
// Assuing using sRGB typed textures this should not be needed.
float ToLinear1(float c){return(c<=0.04045)?c/12.92:pow((c+0.055)/1.055,2.4);}
vec3 ToLinear(vec3 c){return vec3(ToLinear1(c.r),ToLinear1(c.g),ToLinear1(c.b));}

// Linear to sRGB.
// Assuing using sRGB typed textures this should not be needed.
float ToSrgb1(float c){return(c<0.0031308?c*12.92:1.055*pow(c,0.41666)-0.055);}
vec3 ToSrgb(vec3 c){return vec3(ToSrgb1(c.r),ToSrgb1(c.g),ToSrgb1(c.b));}

// Nearest emulated sample given floating point position and texel offset.
// Also zero's off screen.
vec3 Fetch(vec2 pos,vec2 off){
  pos=floor(pos*res+off)/res;
  if(max(abs(pos.x-0.5),abs(pos.y-0.5))>0.5)return vec3(0.0,0.0,0.0);
  return ToLinear(texture(iChannel0,curve(pos.xy),-16.0).rgb);}

// Distance in emulated pixels to nearest texel.
vec2 Dist(vec2 pos){pos=pos*res;return -((pos-floor(pos))-vec2(0.5));}
    
// 1D Gaussian.
float Gaus(float pos,float scale){return exp2(scale*pos*pos);}

// 3-tap Gaussian filter along horz line.
vec3 Horz3(vec2 pos,float off){
  vec3 b=Fetch(pos,vec2(-2.0,off));
  vec3 c=Fetch(pos,vec2( 0.0,off));
  vec3 d=Fetch(pos,vec2( 2.0,off));
  float dst=Dist(pos).x;
  // Convert distance to weight.
  float scale=hardPix;
  float wb=Gaus(dst-2.0,scale);
  float wc=Gaus(dst+0.0,scale);
  float wd=Gaus(dst+2.0,scale);
  // Return filtered sample.
  return (b*wb+c*wc+d*wd)/(wb+wc+wd);}

// 5-tap Gaussian filter along horz line.
vec3 Horz5(vec2 pos,float off){
  vec3 a=Fetch(pos,vec2(-4.0,off));
  vec3 b=Fetch(pos,vec2(-2.0,off));
  vec3 c=Fetch(pos,vec2( 0.0,off));
  vec3 d=Fetch(pos,vec2( 2.0,off));
  vec3 e=Fetch(pos,vec2( 4.0,off));
  float dst=Dist(pos).x;
  // Convert distance to weight.
  float scale=hardPix;
  float wa=Gaus(dst-4.0,scale);
  float wb=Gaus(dst-2.0,scale);
  float wc=Gaus(dst+0.0,scale);
  float wd=Gaus(dst+2.0,scale);
  float we=Gaus(dst+4.0,scale);
  // Return filtered sample.
  return (a*wa+b*wb+c*wc+d*wd+e*we)/(wa+wb+wc+wd+we);}

// Return scanline weight.
float Scan(vec2 pos,float off){
  float dst=Dist(pos).y;
  return Gaus(dst+off,hardScan);}

// Allow nearest three lines to effect pixel.
vec3 Tri(vec2 pos){
  vec3 a=Horz3(pos,-2.0);
  vec3 b=Horz5(pos, 0.0);
  vec3 c=Horz3(pos, 2.0);
  float wa=Scan(pos,-2.0);
  float wb=Scan(pos, 0.0);
  float wc=Scan(pos, 2.0);
  return a*wa+b*wb+c*wc;}


// Shadow mask.
vec3 Mask(vec2 pos)
{
  pos.x+=pos.y*3.0;
  vec3 mask=vec3(maskDark,maskDark,maskDark);
  pos.x=fract(pos.x/6.0);
  if(pos.x<0.333)mask.r=maskLight;
  else if(pos.x<0.666)mask.g=maskLight;
  else mask.b=maskLight;
  return mask;}

void main()
{
	//FragColor = texture(iChannel0, TexCoord);
	
	    //------------------------
    // CRT
    //------------------------
    vec3 col = ToSrgb( Tri( fragCoord.xy/iResolution.xy )*Mask(fragCoord.xy*1.0) );
    //vec3 col = ToSrgb( 1.0*Mask(fragCoord.xy*1.0) );
    //col = texture( iChannel1, fragCoord.xy/iResolution.xy ).xyz;

    //------------------------
    // glow
    //------------------------
       
    //vec4  pacmanPos = loadValue( txPacmanPos );
    //vec3  pacmanDir = loadValue( txPacmanMovDirNex ).xyz;
    //vec4  ghostPos[4];
    //ghostPos[0]     = loadValue( txGhost0PosDir );
    //ghostPos[1]     = loadValue( txGhost1PosDir );
    //ghostPos[2]     = loadValue( txGhost2PosDir );
    //ghostPos[3]     = loadValue( txGhost3PosDir );
    //vec2  points    = loadValue( txPoints ).xy;
    //float state     = loadValue( txState ).x;
    //float lives     = loadValue( txLives ).x;
    //vec3 mode       = loadValue( txMode ) .xyz;

    // map
    //col = drawMap( col, fragCoord );

    // pacman
    //col = drawPacman( col, fragCoord, pacmanPos, pacmanDir );

    // ghosts
    //for( int i=0; i<4; i++ )
    //    col = drawGhost( col, fragCoord, ghostPos[i].xyz, ghostPos[i].w, float(i), mode );
    
    // score
    //col = drawScore( col, fragCoord, points, lives );
    
	FragColor = vec4( col.g, col.g/2.0, col.g/4.0, 1.0 );
}