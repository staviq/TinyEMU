#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#define GL_BUFFERS (1)
extern GLuint gProgramIDd;
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