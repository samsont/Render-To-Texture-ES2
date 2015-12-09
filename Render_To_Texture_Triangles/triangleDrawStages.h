#ifndef TRIANGLE_DRAW_H
#define TRIANGLE_DRAW_H

#include <stdio.h>
#include <stdlib.h>
#include "esUtil.h"

#include "RenderStage.h"

typedef struct
{
	RenderStage red;
	RenderStage blue;
	FinalRenderStage final_stage;
} RenderStages;

int InitRedShaderAndVars(RenderStage *stage);
int InitBlueShaderAndVars(RenderStage *stage);
int InitRenderToScreenShaderAndVars(RenderStages *render_stages);

// one function for blue and red.
void DrawFBOScene(RenderStage *stage, int width, int height, int is_red);
// the final render.
void RenderToScreen(RenderStages *render_stages, GLsizei width, GLsizei height);

#endif
