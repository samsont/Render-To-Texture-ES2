//
// A Render Stage.
// Code for setting up FBO, drawing to a buffer, cleaning up, etc.
//

#pragma once

typedef struct
{
	// FBO and its buffers handle 
	GLuint    framebuffer;
	GLuint    depth_render_buffer;
	GLuint    texture;
} RenderFBO;

typedef struct
{
	// Everything that goes into a render stage
	RenderFBO FBO;
	GLuint shader_program;  // held on to for cleanup.
	GLint sampler_loc;      // needed to grab output texture for next stage.

	// Attribute locations
	GLint  position_loc;
	GLint  texcoord_loc;

} RenderStage;

// Same as above, but no FBO and no sampler_loc
typedef struct
{
	GLuint shader_program;

	// Attribute locations
	GLint  position_loc;
	GLint  texcoord_loc;

} FinalRenderStage;



int InitRenderFBO(RenderFBO *renderFBO, GLint width, GLint height);
int SetShader(RenderStage *stage, GLbyte vShaderStr[], GLbyte fShaderStr[]); //returns TRUE on success;
int SetFinalShader(FinalRenderStage *stage, GLbyte vShaderStr[], GLbyte fShaderStr[]);

void FreeFBOTextures(RenderFBO *renderFBO);
void FreeRenderStage(RenderStage *stage);
void FreeFinalRenderStage(FinalRenderStage *stage);