
#include <assert.h>
#define aglAssert(Condition) assert(Condition)

#include "agl.h"
#include "agl_core3d.h"

void
InitGL()
{
	glClearColor(0.125f, 0.125f, 0.125f, 1.f);				// Black Background
	glClearDepth(1.0f);									    // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							    // Enables Depth Testing
	glDepthFunc(GL_LESS);								    // The Type Of Depth Testing To Do
    glEnable(GL_MULTISAMPLE);
}

int
main(int argc, char **argv)
{
    char Title[128];
    b32 Paused = false;
    
    agl_context *Ctx = aglCreateWindow();
    if(!Ctx) return 0;
    aglSetFixedFPS(120);
    InitGL();

    printf("%s\n", __agl_Context.GLInfo.Version);
    printf("%s\n", __agl_Context.GLInfo.Vendor);
    printf("%s\n", __agl_Context.GLInfo.Renderer);
    printf("%s\n", __agl_Context.GLInfo.ShadingLanguageVersion);
    
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
    
    agl_drawable Grid = aglPrimitiveGrid(512);
    aglGenBuffer(&Grid.Mesh);

    r32 time = 0;
    v3 Pos = V3(0,0,0);
    agl_camera Camera;
    aglCameraInit(&Camera, AGL_CAMERA_MODE_TARGET, V3(-5.f, .5f, 0.0f));
    aglCameraTarget(&Camera, &Pos);

#if 0
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
#endif
    
    while(aglHandleEvents())
    {
        sprintf(Title, "%f, %i\n", Ctx->Delta, Ctx->FPS); 
        aglSetWindowTitle(Title);
        
        if(aglKeyUp(VK_ESCAPE)) aglCloseWindow();
        if(aglKeyUp('P')) Paused = (Paused == 1 ? 0 : 1);
        if(!Paused)
        {
            if(aglKeyUp(VK_F1)) aglSetFixedFPS(30);
            if(aglKeyUp(VK_F2)) aglSetFixedFPS(60);
            if(aglKeyUp(VK_F3)) aglSetFixedFPS(120);
            if(aglKeyUp(VK_F4)) aglSetFixedFPS(299);
            if(aglKeyUp(VK_F5)) aglCameraTarget(&Camera, &Pos);
            if(aglKeyUp(VK_F6)) {
                aglCameraInit(&Camera, AGL_CAMERA_MODE_FREE, V3(0, 2, 10));
            }
            if(aglKeyUp(VK_RETURN)) aglToggleFullscreen();
            if(Ctx->MouseInput.Right)
            {
                time += Ctx->Delta;
                Quad.Material.Ambient = V3(sin(time * 2.0f), sin(time * .7f), sin(time * 1.3f));
                Quad.Material.Diffuse = Quad.Material.Ambient * .4f;
            }
            Pos.x = sin(Ctx->Time * 2.5f) ;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
            aglBeginScene3D(Ctx, &Camera);
                //glDepthMask(GL_FALSE);
                aglDraw(&Grid, IdentityMat4x4(), &DefShader);
                //glDepthMask(GL_TRUE);
                aglDraw(&Quad, TranslationMatrix(Pos.x, Pos.y, Pos.z), &DefShader);
            aglEndScene3D(Ctx, &Camera);
        
            glUseProgram(0);
            aglSwapBuffers();
        }
    }
    
    aglMeshDelete(&Quad.Mesh);
    aglMeshDelete(&Grid.Mesh);
    
    aglCleanup();
    return 0;
}
