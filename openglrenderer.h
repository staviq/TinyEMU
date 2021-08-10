#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

extern GLuint gProgramID;
//extern GLint gVertexPos2DLocation;
extern unsigned int VBO, VAO, EBO;
extern unsigned int texture1;

char * file_get_contents( const char * filename );
GLuint compileShader(const char* source, GLuint shaderType, const char* name);
GLuint compileProgram(const char* vtxFile, const char* fragFile);
uint8_t initGL();
void printProgramLog( GLuint program );
void printShaderLog( GLuint shader );
void generateTex(uint32_t w, uint32_t h);
void updateTex(uint32_t w, uint32_t h, void *data);
void render( uint32_t w, uint32_t h );

#endif