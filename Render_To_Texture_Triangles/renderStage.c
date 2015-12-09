#include <stdio.h>
#include <stdlib.h>
#include "esUtil.h"
#include "RenderStage.h"
///
// Create Framebuffer object and a texture as its color buffer attachment
//

int InitRenderFBO(RenderFBO *renderFBO, GLint width, GLint height)
{
	GLint     maxRenderbufferSize;

	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);

	// check if GL_MAX_RENDERBUFFER_SIZE is >= texWidth and texHeight
	if ((maxRenderbufferSize <= width) || (maxRenderbufferSize <= height))
	{
		// cannot use framebuffer objects as we need to create
		// a depth buffer as a renderbuffer object
		printf("Cannot use framebuffer objects!\n");
		exit(EXIT_FAILURE);
		return FALSE;
	}
	// generate the framebuffer, renderbuffer, and texture object names
	glGenFramebuffers(1, &renderFBO->framebuffer);
	glGenRenderbuffers(1, &renderFBO->depth_render_buffer);
	glGenTextures(1, &renderFBO->texture);

	// bind texture and load the texture mip-level 0
	// texels are RGB565
	// no texels need to be specified as we are going to draw into
	// the texture
	glBindTexture(GL_TEXTURE_2D, renderFBO->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// bind renderbuffer and create a 16-bit depth buffer
	// width and height of renderbuffer = width and height of
	// the texture
	glBindRenderbuffer(GL_RENDERBUFFER, renderFBO->depth_render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

	// bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, renderFBO->framebuffer);
	// specify texture as color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderFBO->texture, 0);

	// specify depth_renderbufer as depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderFBO->depth_render_buffer);

	// check for framebuffer complete
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer object is not complete!\n");
		exit(EXIT_FAILURE);
		return FALSE;
	}
	return TRUE;
}

// -----------------------------------------------------------------------------------------

void FreeFBOTextures(RenderFBO *renderFBO)
{
	glDeleteTextures(1, & renderFBO->texture);
	glDeleteRenderbuffers(1, & renderFBO->depth_render_buffer);
	glDeleteFramebuffers(1, & renderFBO->framebuffer);
}

// -----------------------------------------------------------------------------------------

void FreeRenderStage(RenderStage *stage)
{
	FreeFBOTextures(& stage->FBO);
	glDeleteProgram(stage->shader_program);
}

// -----------------------------------------------------------------------------------------

void FreeFinalRenderStage(FinalRenderStage *stage)
{
	glDeleteProgram(stage->shader_program);
}

// -----------------------------------------------------------------------------------------

int SetShader(RenderStage *stage, GLbyte vShaderStr[], GLbyte fShaderStr[])
{
	// Load the shaders and get a linked program object
	// We don't really ever user this. Not clear we need to hold on to it.
	stage->shader_program = esLoadProgram(vShaderStr, fShaderStr);
	return stage->shader_program != 0;
}

// -----------------------------------------------------------------------------------------

int SetFinalShader(FinalRenderStage *stage, GLbyte vShaderStr[], GLbyte fShaderStr[])
{
	stage->shader_program = esLoadProgram(vShaderStr, fShaderStr);
	return stage->shader_program != 0;
}

// -----------------------------------------------------------------------------------------

