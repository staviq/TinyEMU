#include "openglrenderer.h"

GLuint gProgramID = 0;
//GLint gVertexPos2DLocation = -1;
unsigned int VBO, VAO, EBO;
unsigned int texture1;

void generateTex(uint32_t w, uint32_t h)
{
	glDeleteTextures(1, &texture1);
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); 
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void updateTex( uint32_t w, uint32_t h, void *data )
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);
}

void render()
{
	//Clear color buffer
	//glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	//Bind program
	glUseProgram( gProgramID );

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Unbind program
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

GLuint compileShader(const char* source, GLuint shaderType, const char* name) {
	fprintf( stderr, "Compiling shader: %s\r\n", name );
	//fprintf( stderr, "================================================================\r\n" );
	//fprintf( stderr, "%s\r\n", source );
	//fprintf( stderr, "================================================================\r\n" );
	// Create ID for shader
	GLuint result = glCreateShader(shaderType);
	// Define shader text
	glShaderSource(result, 1, &source, NULL);
	// Compile shader
	glCompileShader(result);

	//Check vertex shader for errors
	GLint shaderCompiled = GL_FALSE;
	glGetShaderiv( result, GL_COMPILE_STATUS, &shaderCompiled );
	if( shaderCompiled != GL_TRUE ) {
		fprintf( stderr, "Error compiling shader: %d istrue %d isfalse %d isshader %d !\r\n", result, shaderCompiled == GL_TRUE, shaderCompiled == GL_FALSE, glIsShader( result ) );
		GLint logLength;
		glGetShaderiv(result, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			GLchar *log = (GLchar*)malloc(logLength);
			glGetShaderInfoLog(result, logLength, &logLength, log);
			fprintf( stderr, "Shader compile log: %s\r\n", log );
			free(log);
		}
		glDeleteShader(result);
		result = 0;
	} else {
		fprintf( stderr, "Shader compilado correctamente. Id = %d\r\n", result );
	}
	return result;
}

GLuint compileProgram(const char* vtxFile, const char* fragFile) {
	GLuint programId = 0;
	GLuint vtxShaderId, fragShaderId;

	programId = glCreateProgram();

	char * vtxsource = file_get_contents( vtxFile );
	if( vtxsource != NULL )
	{
		vtxShaderId = compileShader(vtxsource, GL_VERTEX_SHADER, vtxFile);
		//free( vtxsource );
	}
	else
	{
		fprintf( stderr, "Error vtx processing shader %s\r\n", vtxFile );
		exit(-10);
	}

	char * fragsource = file_get_contents( fragFile );
	if( fragsource != NULL )
	{
		fragShaderId = compileShader(fragsource, GL_FRAGMENT_SHADER, fragFile);
		//free( fragsource );
	}
	else
	{
		fprintf( stderr, "Error frg processing shader %s\r\n", vtxFile );
		exit(-10);
	}

	if(vtxShaderId && fragShaderId) {
		// Associate shader with program
		glAttachShader(programId, vtxShaderId);
		glAttachShader(programId, fragShaderId);
		glLinkProgram(programId);
		glValidateProgram(programId);

		// Check the status of the compile/link
		GLint logLen;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen > 0) {
			char* log = (char*) malloc(logLen * sizeof(char));
			// Show any errors as appropriate
			glGetProgramInfoLog(programId, logLen, &logLen, log);
			//std::cout << "Prog Info Log: " << std::endl << log << std::endl;
			fprintf( stderr, "Prog Info Log: \r\n%s\r\n", log );
			free(log);
		}
	}
	if(vtxShaderId) {
		glDeleteShader(vtxShaderId);
	}
	if(fragShaderId) {
		glDeleteShader(fragShaderId);
	}
	return programId;
}

uint8_t initGL()
{
	//Success flag
	uint8_t success = 1;

	//Generate program
	gProgramID = glCreateProgram();

	//Create vertex shader
	GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );

	//Get vertex source
	const GLchar* vertexShaderSource = (const GLchar*)file_get_contents( "shader.vert" );/*[] =
	{
		"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};*/

	//Set vertex source
	glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );

	//Compile vertex source
	glCompileShader( vertexShader );

	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &vShaderCompiled );
	if( vShaderCompiled != GL_TRUE )
	{
		printf( "Unable to compile vertex shader %d!\n", vertexShader );
		printShaderLog( vertexShader );
        success = 0;
	}
	else
	{
		//Attach vertex shader to program
		glAttachShader( gProgramID, vertexShader );


		//Create fragment shader
		GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		//Get fragment source
		const GLchar* fragmentShaderSource = (const GLchar*)file_get_contents( "shader.frag" );/*[] =
		{
			"#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
		};*/

		//Set fragment source
		glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );

		//Compile fragment source
		glCompileShader( fragmentShader );

		//Check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled );
		if( fShaderCompiled != GL_TRUE )
		{
			printf( "Unable to compile fragment shader %d!\n", fragmentShader );
			printShaderLog( fragmentShader );
			success = 0;
		}
		else
		{
			//Attach fragment shader to program
			glAttachShader( gProgramID, fragmentShader );


			//Link program
			glLinkProgram( gProgramID );

			//Check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv( gProgramID, GL_LINK_STATUS, &programSuccess );
			if( programSuccess != GL_TRUE )
			{
				printf( "Error linking program %d!\n", gProgramID );
				printProgramLog( gProgramID );
				success = 0;
			}
			else
			{
				//Initialize clear color
				glEnable(GL_BLEND);
				glClearColor( 0.f, 0.f, 0.f, 0.f );

				static float vertices[] = {
					// positions          // colors           // texture coords
					 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
					 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
					-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
					-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
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
			}
		}
	}
	
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
