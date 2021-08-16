#include "openglrenderer.h"

struct VirtMachine * openglrenderer_owner = NULL;

GLuint gProgramIDd0 = 0;
GLuint gProgramIDd1 = 0;
GLuint gProgramID0 = 0;
GLuint gProgramID1 = 0;
GLuint gProgramID2 = 0;
GLuint gProgramID3 = 0;
GLuint VBO, VAO, EBO;
GLuint iChannelA;
GLuint iChannelB;
GLuint iChannel[GL_BUFFERS];
GLint  fBuffA;
GLuint fBuff[GL_BUFFERS];
GLuint rBuff[GL_BUFFERS];

void generateTex(uint32_t w, uint32_t h)
{
	// only called if screen size changed, which currently is always only once right after gl init.
	
	glDeleteTextures(1, &iChannelA);
	glGenTextures(1, &iChannelA);
	glBindTexture(GL_TEXTURE_2D, iChannelA); 
	// set the texture wrapping parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	
	glDeleteTextures( GL_BUFFERS, iChannel );
	glGenTextures( GL_BUFFERS, iChannel );
	glDeleteFramebuffers( GL_BUFFERS, fBuff );
	glGenFramebuffers( GL_BUFFERS, fBuff );
	glDeleteRenderbuffers(GL_BUFFERS, rBuff);
	glGenRenderbuffers(GL_BUFFERS, rBuff);
	
	for( int i=0; i< GL_BUFFERS; ++i )
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fBuff[i]);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, iChannel[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glBindRenderbuffer(GL_RENDERBUFFER, rBuff[i]);
		
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w * (sdl_fullscreen?2:1), h * (sdl_fullscreen?2:1), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
		//glTexImage2D  (GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, NULL);
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, iChannel[i], 0 );
		
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rBuff[i]);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			fprintf( stderr, "ERROR: Framebuffer is not complete!\r\n" ); fflush( stderr );
		}
		//else
		//{
		//	fprintf( stderr, "FRAMEBUFFER %d RDY (%d)\r\n", i, glCheckFramebufferStatus(GL_FRAMEBUFFER) ); fflush( stderr );
		//}
	}
	
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, fBuffA );
}

extern void * openglrenderer_register_ram( uint64_t addr, uint64_t size );

void updateTex( uint32_t w, uint32_t h, void *data )
{
	fb_image * datamaptest = (fb_image*)data;
	uint8_t * imgdata = 
	
	fprintf( stderr, "DATAMAPTEST dx%04d dy%04d w%04d h%04d ptr:0x%016llX\r\n", datamaptest[0].dx, datamaptest[0].dy, datamaptest[0].width, datamaptest[0].height, (uintptr_t)(datamaptest[0].data) ); fflush( stderr );
	/*
	0xFFFFFFE001B69500
	0xFFFFFFE001B69500
	*/
	
	void * datamem = openglrenderer_register_ram( (uintptr_t)(datamaptest[0].data), 128/*lol*/);
	
	for( int i=0; i<8; ++i )
	{
		if( !(i%8) )
		{
			fprintf( stderr, "\r\n" );fflush( stderr );
		}
		fprintf( stderr, " 0x%02X", datamem+i );
	}
	fprintf( stderr, "\r\n" );fflush( stderr );
	
	glBindTexture(GL_TEXTURE_2D, iChannelA);
	if( sdl_fullscreen )
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexImage2D  (GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexImage2D  (GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void render( uint32_t w, uint32_t h )
{
	
	//generateTex(w,h);
	
	static uint8_t faketime = 0;
	
	glBindFramebuffer( GL_FRAMEBUFFER, fBuff[0] );
	//glBindFramebuffer( GL_FRAMEBUFFER, fBuffA );
	glDisable(GL_DEPTH_TEST);
	
	//Clear color buffer
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iChannelA);
	
	//Bind program
	if( sdl_debugshaders )
	{
		glUseProgram( gProgramIDd0 );
		
		glUniform4f( glGetUniformLocation(gProgramIDd0, "debug") , 0.0, 0.0, 0.0, 1.0);
		glUniform2f( glGetUniformLocation(gProgramIDd0, "iResolution") , w, h);
		glUniform1f( glGetUniformLocation(gProgramIDd0, "iTime") , faketime++);
	}
	else
	{
		glUseProgram( gProgramID0 );
		
		glUniform4f( glGetUniformLocation(gProgramID0, "debug") , 0.25, 0.5, 0.0, 1.0);
		glUniform2f( glGetUniformLocation(gProgramID0, "iResolution") , w, h);
		glUniform1f( glGetUniformLocation(gProgramID0, "iTime") , faketime++);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram( 0 );
	
	//---
	
	glBindFramebuffer( GL_FRAMEBUFFER, fBuff[1] );
	//glBindFramebuffer( GL_FRAMEBUFFER, fBuffA );
	glDisable(GL_DEPTH_TEST);
	
	//Clear color buffer
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iChannel[0]);
	
	//Bind program
	if( sdl_debugshaders )
	{
		glUseProgram( gProgramIDd0 );
		
		glUniform4f( glGetUniformLocation(gProgramIDd0, "debug") , 0.0, 0.0, 0.0, 1.0);
		glUniform2f( glGetUniformLocation(gProgramIDd0, "iResolution") , w, h);
		glUniform1f( glGetUniformLocation(gProgramIDd0, "iTime") , faketime++);
	}
	else
	{
		glUseProgram( gProgramID1 );
		
		glUniform4f( glGetUniformLocation(gProgramID1, "debug") , 0.5, 0.5, 0.0, 0.25);
		glUniform2f( glGetUniformLocation(gProgramID1, "iResolution") , w, h);
		glUniform1f( glGetUniformLocation(gProgramID1, "iTime") , faketime++);
	}
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram( 0 );
	
	//---
	
	glBindFramebuffer( GL_FRAMEBUFFER, fBuffA );
	glDisable(GL_DEPTH_TEST);
	
	//Clear color buffer
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, iChannelA);
	glBindTexture(GL_TEXTURE_2D, iChannel[1]);
	
	//Bind program
	if( sdl_debugshaders )
	{
		glUseProgram( gProgramIDd0 );
		
		glUniform4f( glGetUniformLocation(gProgramIDd0, "debug") , 0.0, 0.0, 0.0, 1.0);
		glUniform2f( glGetUniformLocation(gProgramIDd0, "iResolution") , w, h);
		glUniform1f( glGetUniformLocation(gProgramIDd0, "iTime") , faketime++);
	}
	else
	{
		glUseProgram( gProgramID2 );
		
		glUniform4f( glGetUniformLocation(gProgramID2, "debug") , 0.75, 0.5, 0.0, 0.5);
		glUniform2f( glGetUniformLocation(gProgramID2, "iResolution") , w, h);
		glUniform1f( glGetUniformLocation(gProgramID2, "iTime") , faketime++);
	}
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram( 0 );
}

char * file_get_contents( const char * filename )
{
	FILE *file = NULL;
	char * buf = NULL;
	uint64_t buf_cnt_size = 0;
	uint64_t buf_cnt_read = 0;
	const uint16_t buf_step = 16;
	
	file = fopen(filename, "r");
	if( file != NULL )
	{
		while( !feof( file ) )
		{
			if( ( buf_cnt_size - buf_cnt_read ) < buf_step )
			{
				buf = realloc( buf, buf_cnt_size + buf_step );
				if( buf == NULL )
				{
					fprintf( stderr, "wtf\r\n" );
					return NULL;
				}
				else
				{
					buf_cnt_size += buf_step;
				}
			}
			size_t read_length = fread( buf+buf_cnt_read, sizeof(char), buf_step, file );
			buf_cnt_read += read_length;
		}
		
		if( ( buf_cnt_size - buf_cnt_read ) < buf_step )
		{
			buf = realloc( buf, buf_cnt_size + buf_step );
			if( buf == NULL )
			{
				fprintf( stderr, "wtf\r\n" );
				return NULL;
			}
			else
			{
				buf_cnt_size += buf_step;
			}
		}
		
		buf[buf_cnt_read] = '\0';
	}
	else
	{
		return NULL;
	}
	
	fclose( file );
	
	return buf;
}

GLuint compileShader(const char* source, GLuint shaderType, const char* name)
{
	GLuint shader = glCreateShader( shaderType );
	glShaderSource( shader, 1, &source, NULL );
	
	glCompileShader( shader );
	
	//Check vertex shader for errors
	GLint shaderCompiled = GL_FALSE;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &shaderCompiled );
	if( shaderCompiled != GL_TRUE )
	{
		fprintf( stderr, "Unable to compile shader %s!\n", name );
		printShaderLog( shader );
		exit(-15);
	}
	
	return shader;
}

GLuint compileProgram(const char* vertFile, const char* fragFile)
{
	unsigned int program = glCreateProgram();
	
	char * vertSource = file_get_contents( vertFile );
	char * fragSource = file_get_contents( fragFile );
	
	GLuint vertexShader   = compileShader( vertSource, GL_VERTEX_SHADER,   vertFile );
	GLuint fragmentShader = compileShader( fragSource, GL_FRAGMENT_SHADER, fragFile );
	
	glAttachShader( program, vertexShader   );
	glAttachShader( program, fragmentShader );
	
	//Link program
	glLinkProgram( program );

	//Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv( program, GL_LINK_STATUS, &programSuccess );
	if( programSuccess != GL_TRUE )
	{
		fprintf( stderr, "Error linking program %d!\n", program );
		printProgramLog( program );
		exit(-15);
	}
	
	return program;
}

uint8_t initGL()
{
	fprintf( stderr, "GL INIT\r\n" ); fflush( stderr );
	
	//Success flag
	uint8_t success = 1;

	//Generate program
	gProgramIDd0 = compileProgram( "shaderpd.vert", "shaderpd.frag" );
	gProgramIDd1 = compileProgram( "shaderpd2.vert", "shaderpd2.frag" );
	
	gProgramID0 = compileProgram( "shaderp0.vert", "shaderp0.frag" );
	gProgramID1 = compileProgram( "shaderp1.vert", "shaderp1.frag" );
	gProgramID2 = compileProgram( "shaderp2.vert", "shaderp2.frag" );

	//Initialize clear color
	glEnable(GL_BLEND);
	glClearColor( 0.f, 0.f, 0.f, 0.f );

	/*
	static float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	*/
	static float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // top left 
	};
	static unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	//unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	return success;
}

void printProgramLog( GLuint program )
{
	//Make sure name is shader
	if( glIsProgram( program ) )
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		
		//Get info string length
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
		
		//Allocate string
		char* infoLog = malloc( maxLength );
		
		//Get info log
		glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			fprintf( stderr, "%s\n", infoLog );
		}
		
		//Deallocate string
		free(infoLog);
	}
	else
	{
		fprintf( stderr, "Name %d is not a program\n", program );
	}
}

void printShaderLog( GLuint shader )
{
	//Make sure name is shader
	if( glIsShader( shader ) )
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		
		//Get info string length
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
		
		//Allocate string
		char* infoLog = malloc( maxLength );
		
		//Get info log
		glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			fprintf( stderr, "%s\n", infoLog );
		}

		//Deallocate string
		free(infoLog);
	}
	else
	{
		fprintf( stderr, "Name %d is not a shader\n", shader );
	}
}
