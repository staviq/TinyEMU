#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

extern GLuint gProgramIDd;
extern GLuint gProgramID0;
extern GLuint gProgramID1;
extern GLuint gProgramID2;
extern GLuint gProgramID3;
extern unsigned int VBO, VAO, EBO;
extern unsigned int iChannel0;
extern unsigned int iChannel1;
extern unsigned int iChannel2;
extern unsigned int iChannel3;
extern unsigned int fBuff0;
extern unsigned int fBuff1;
extern unsigned int fBuff2;
extern unsigned int fBuff3;
extern unsigned int rBuff0;
extern unsigned int rBuff1;
extern unsigned int rBuff2;
extern unsigned int rBuff3;

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