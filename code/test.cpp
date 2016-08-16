#include <assert.h>
#define aglAssert(Condition) assert(Condition)
//efine AGL_EXTERN
#include "agl.h"
#include "agl_core3d.h"

void
InitGL()
{
	glClearColor(0.125f, 0.125f, 0.125f, 1.f);				// Black Background
	glClearDepth(1.0f);									    // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							    // Enables Depth Testing
	glDepthFunc(GL_LESS);								    // The Type Of Depth Testing To Do
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}


int
main(int argc, char **argv)
{
    char Title[128];
    b32 Paused = false;

    agl_context *Ctx = aglCreateWindow("lalala", 1024, 768, AGL_WINDOW_VSYNC | AGL_WINDOW_FULLSCREEN | AGL_WINDOW_MSAA);
    if(!Ctx) return 0;
//    aglSetVerticalSync(true);

    InitGL();
    
    agl_shader DefShader = aglInitDefaultShader();
    if(!DefShader.Success)
    {
        aglCleanup();
        return 1;
    }
    
    agl_drawable Quad = aglPrimitiveQuad();
    Quad.Material.Ambient = V3(.5f, 1.0f, .5f);
    Quad.Material.Diffuse = Quad.Material.Ambient * .5f;
    Quad.Material.Shininess = 64;
    aglGenBuffer(&Quad.Mesh);
    
    agl_drawable Grid = aglPrimitiveGrid(128);
    aglGenBuffer(&Grid.Mesh);

    r32 time_color = 0, time_pos = 0;
    v3 Pos = V3(0,0,0);
    agl_camera Camera;
    aglCameraInit(&Camera, AGL_CAMERA_MODE_TARGET, V3(-5.f, .5f, 0.0f));
    aglCameraTarget(&Camera, &Pos);
    
#if 1
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
#endif

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
    while(aglHandleEvents())
    {
        sprintf(Title, "dt: %f, fps: %i, time: %f\n", Ctx->Delta, Ctx->FPS, Ctx->Time); 
        aglSetWindowTitle(Title);
        
        if(aglKeyUp(VK_ESCAPE)) aglCloseWindow();
        if(aglKeyUp('P')) Paused = (Paused == 1 ? 0 : 1);            
        if(!Paused)
        {
            time_pos += Ctx->Delta;
            Pos.x = sinf(time_pos * 2.5f) ;
        }
        
        if(aglKeyUp(VK_F1)) aglSetVerticalSync(!Ctx->VerticalSync);
        if(aglKeyUp(VK_F5)) aglCameraTarget(&Camera, &Pos);
        if(aglKeyUp(VK_F6)) aglCameraInit(&Camera, AGL_CAMERA_MODE_FREE, V3(0, 2, 10));
        if(aglKeyUp(VK_RETURN)) aglToggleFullscreen();
        
        if(Ctx->MouseInput.Right)
        {
            time_color += Ctx->Delta;
            Quad.Material.Ambient = V3(sin(time_color * 2.0f), sin(time_color * .7f), sin(time_color * 1.3f));
            Quad.Material.Diffuse = Quad.Material.Ambient * .4f;
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        aglBeginScene3D(Ctx, &Camera);
            //glDepthMask(GL_FALSE);
            aglDraw(&Grid, IdentityMat4x4(), &DefShader);
            //glDepthMask(GL_TRUE);
            aglDraw(&Quad, TranslationMatrix(Pos.x, Pos.y, Pos.z), &DefShader);
            aglEndScene3D(Ctx, &Camera);
        glUseProgram(0);

        glFinish(); 
        aglSwapBuffers();
    }
    
    aglMeshDelete(&Quad.Mesh);
    aglMeshDelete(&Grid.Mesh);
    
    aglCleanup();
    return 0;
}
