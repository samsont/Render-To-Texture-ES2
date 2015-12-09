#include "triangleDrawStages.h"

// -----------------------------------------------------------------------------------------

int InitRedShaderAndVars(RenderStage *stage)
{
	GLbyte vShaderStr[] =
		"attribute vec4 vposition;    \n"
		"void main()                  \n"
		"{                            \n"
		"   gl_Position = vposition;  \n"
		"}                            \n";

	GLbyte fShaderStr[] =
		"precision mediump float;                            \n"
		"void main()                                         \n"
		"{                                                   \n"
		"  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);          \n"
		"}                                                   \n";

	if (!SetShader(stage, vShaderStr, fShaderStr)) return FALSE;

	// Bind vPosition to attribute 0   
	glBindAttribLocation(stage->shader_program, 0, "vposition");
	return TRUE;
}

// -----------------------------------------------------------------------------------------

int InitBlueShaderAndVars(RenderStage *stage)
{
	GLbyte vShaderStr[] =
		"attribute vec4 vposition;    \n"
		"void main()                  \n"
		"{                            \n"
		"   gl_Position = vposition;  \n"
		"}                            \n";

	GLbyte fShaderStr[] =
		"precision mediump float;                            \n"
		"void main()                                         \n"
		"{                                                   \n"
		"  gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);          \n"
		"}                                                   \n";
	if (!SetShader(stage, vShaderStr, fShaderStr)) return FALSE;

	// Bind vPosition to attribute 0   
	glBindAttribLocation(stage->shader_program, 0, "vposition");
	return TRUE;
}

// -----------------------------------------------------------------------------------------

void DrawFBOScene(RenderStage *stage, int width, int height, int is_red)
{
	//
	// Every frame
	// 
	glBindFramebuffer(GL_FRAMEBUFFER, stage->FBO.framebuffer);

	// Set the viewport -- really needed?
	glViewport(0, 0, width, height);

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the program object
	glUseProgram(stage->shader_program);

	//
	// This stuff looks like it only needs to be done once.
	//

	// Draw a triangle to FBO
	GLfloat vVertices[] = { 0.0f, 0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
	     0.5f, -0.5f, -0.5f };
	GLfloat vVerticesReverse[] =
	{ 0.0f, -0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	 0.5f, 0.5f, -0.5f };

	// Load the vertex position
	if (is_red)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	else
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVerticesReverse);

	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

// -----------------------------------------------------------------------------------------

int InitRenderToScreenShaderAndVars(RenderStages *render_stages)
{
	GLbyte vShaderStr[] =
		"attribute vec4 a_position;   \n"
		"attribute vec2 a_texCoord;   \n"
		"varying vec2 v_texCoord;     \n"
		"void main()                  \n"
		"{                            \n"
		"   gl_Position = a_position; \n"
		"   v_texCoord = a_texCoord;  \n"
		"}                            \n";

	GLbyte fShaderStr[] =
		"precision mediump float;                            \n"
		"varying vec2 v_texCoord;                            \n"
		"uniform sampler2D s_triangle_red;                   \n"
		"uniform sampler2D s_triangle_blue;                  \n"
		"void main()                                         \n"
		"{                                                   \n"
		"  vec4 redTriangle;                                 \n"
		"  vec4 blueTriangle;                                \n"
		"                                                    \n"
		"  redTriangle = texture2D( s_triangle_red, v_texCoord );\n"
		"  blueTriangle = texture2D( s_triangle_blue, v_texCoord );\n"
		"                                                    \n"
		"  gl_FragColor = redTriangle + blueTriangle;        \n"
		"  //gl_FragColor = blueTriangle;                    \n"
		"}                                                   \n";

	if (!SetFinalShader(&render_stages->final_stage, vShaderStr, fShaderStr)) return FALSE;

	GLfloat vVertices[] = { -0.5f, -0.5f, 0.0f,  // Position 0
		0.0f, 0.0f,        // TexCoord 0 
		0.5f, -0.5f, 0.0f,  // Position 1
		0.0f, 1.0f,        // TexCoord 1
		0.5f, 0.5f, 0.0f,  // Position 2
		1.0f, 1.0f,        // TexCoord 2
		-0.5f, 0.5f, 0.0f,  // Position 3
		1.0f, 0.0f         // TexCoord 3
	};

	// Get the attribute locations
	render_stages->final_stage.position_loc = glGetAttribLocation(render_stages->final_stage.shader_program, "a_position");
	render_stages->final_stage.texcoord_loc = glGetAttribLocation(render_stages->final_stage.shader_program, "a_texCoord");

	// Get the sampler location
	render_stages->red.sampler_loc = glGetUniformLocation(render_stages->final_stage.shader_program, "s_triangle_red");
	render_stages->blue.sampler_loc = glGetUniformLocation(render_stages->final_stage.shader_program, "s_triangle_blue");
	return TRUE;
}

// -----------------------------------------------------------------------------------------

//
// Draw the rendered texture back to screen
//

void RenderToScreen(RenderStages *render_stages, GLsizei width, GLsizei height)
{
	// Change back to window system provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//
	// These are required every frame.
	//

	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);

	// Set the viewport
	glViewport(0, 0, width, height);

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the program object
	glUseProgram(render_stages->final_stage.shader_program);

	//
	// This looks like a one time thing -- maybe not!
	//

	GLfloat vVertices[] = { -0.5f, -0.5f, 0.0f,  // Position 0
		0.0f, 0.0f,        // TexCoord 0 
		0.5f, -0.5f, 0.0f,  // Position 1
		0.0f, 1.0f,        // TexCoord 1
		0.5f, 0.5f, 0.0f,  // Position 2
		1.0f, 1.0f,        // TexCoord 2
		-0.5f, 0.5f, 0.0f,  // Position 3
		1.0f, 0.0f         // TexCoord 3
	};

	// Load the vertex position
	glVertexAttribPointer(render_stages->final_stage.position_loc, 3, GL_FLOAT,
		GL_FALSE, 5 * sizeof(GLfloat), vVertices);
	// Load the texture coordinate
	glVertexAttribPointer(render_stages->final_stage.texcoord_loc, 2, GL_FLOAT,
		GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);

	glEnableVertexAttribArray(render_stages->final_stage.position_loc);
	glEnableVertexAttribArray(render_stages->final_stage.texcoord_loc);

	/// FIXME Here can only with 2 triangles situation work.
	// Bind the texture
	// Set the sampler texture unit to 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, render_stages->red.FBO.texture);
	glUniform1i(render_stages->red.sampler_loc, 0);

	// Bind the texture
	// Set the sampler texture unit to 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, render_stages->blue.FBO.texture);
	glUniform1i(render_stages->blue.sampler_loc, 1);
	/// FIXEND

	// This looks like every frame.
	GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}
