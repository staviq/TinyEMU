#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include "cutils.h"
//#include "iomem.h"
//#include "machine.h"

typedef uint8_t __u8;
typedef uint16_t __u16;
typedef uint32_t __u32;
typedef uint64_t __u64;

typedef struct fb_cmap {
	__u32 start;			/* First entry	*/
	__u32 len;			/* Number of entries */
	__u16 *red;			/* Red values	*/
	__u16 *green;
	__u16 *blue;
	__u16 *transp;			/* transparency, can be NULL */
} fb_cmap;

typedef struct fb_image {
	__u32 dx;		/* Where to place image */
	__u32 dy;
	__u32 width;		/* Size of image */
	__u32 height;
	__u32 fg_color;		/* Only used when a mono bitmap */
	__u32 bg_color;
	__u8  depth;		/* Depth of the image */
	const char *data;	/* Pointer to image data */
	struct fb_cmap cmap;	/* color map info */
} fb_image;

extern uint8_t sdl_fullscreen;
extern uint8_t sdl_debugshaders;

extern struct VirtMachine * openglrenderer_owner;

#define GL_BUFFERS (2)
extern GLuint gProgramIDd0;
extern GLuint gProgramIDd1;
extern GLuint gProgramID0;
extern GLuint gProgramID1;
extern GLuint gProgramID2;
extern GLuint gProgramID3;
extern GLuint VBO, VAO, EBO;
extern GLuint iChannelA;
extern GLuint iChannelB;
extern GLuint iChannel[GL_BUFFERS];
extern GLint  fBuffA;
extern GLuint fBuff[GL_BUFFERS];
extern GLuint rBuff[GL_BUFFERS];

char * file_get_contents( const char * filename );
GLuint compileShader(const char* source, GLuint shaderType, const char* name);
GLuint compileProgram(const char* vertFile, const char* fragFile);
uint8_t initGL();
void printProgramLog( GLuint program );
void printShaderLog( GLuint shader );
void generateTex(uint32_t w, uint32_t h);
void updateTex(uint32_t w, uint32_t h, void *data);
void render( uint32_t w, uint32_t h );

#endif
