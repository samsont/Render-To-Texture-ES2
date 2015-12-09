#include <stdio.h>
#include <stdlib.h>
#include "esUtil.h"

#include "triangleDrawStages.h"

///
// Init shaders of drawing triangles. Order might be important since later stages reference earlier FBOs
//
int InitAfterWeHaveAContext ( RenderStages *render_stages )
{
    return InitRedShaderAndVars ( &render_stages->red ) &&
		   InitRenderFBO(&render_stages->red.FBO, 256, 256) &&
		   InitBlueShaderAndVars( &render_stages->blue) &&
		   InitRenderFBO(&render_stages->blue.FBO, 256, 256) &&
		   InitRenderToScreenShaderAndVars(render_stages);
}

///
// Draw a triangle using the shader pair created in Init()
//
void DrawAllStages ( ESContext *esContext )
{
   RenderStages *render_stages = esContext->userData;
   DrawFBOScene(&render_stages->red, esContext->width, esContext->height,  1);
   DrawFBOScene(&render_stages->blue, esContext->width, esContext->height, 0);
   RenderToScreen(render_stages,esContext->width, esContext->height);
   // display it
   eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

///
// Cleanup. Doesn't look like it is called.
//
void ShutDown ( ESContext *esContext )
{
   RenderStages *render_stages = esContext->userData;

   FreeRenderStage(& render_stages->blue);
   FreeRenderStage(& render_stages->red);
   FreeFinalRenderStage(& render_stages->final_stage);
}

int main() 
{
   ESContext esContext;
   esInitContext(&esContext);

   RenderStages  render_stages;
   esContext.userData = &render_stages;

   esCreateWindow ( &esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB );

   if (!InitAfterWeHaveAContext(&render_stages))
      return 0;

   // Now done every frame.
   //DrawFBOScene(&render_stages.red, esContext.width, esContext.height, 1);
   //DrawFBOScepne(&render_stages.blue, esContext.width, esContext.height, 0);

   esRegisterDrawFunc(&esContext, DrawAllStages);

   esMainLoop ( &esContext );

   return 0;
}