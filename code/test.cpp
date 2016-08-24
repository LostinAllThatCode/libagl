#include <stdio.h>

#include "agl.h"
#define AGL_MALLOC(SIZE) VirtualAlloc(0, SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)
#define AGL_FREE(ADDRESS)   VirtualFree(ADDRESS, 0, MEM_RELEASE)
#define AGL_USE_STB_TRUETYPE
#include "agl_core3d.h"


void
InitGL()
{
#if 0 
	glClearColor(0.125f, 0.125f, 0.125f, 1.f);				// Black Background
	glClearDepth(1.0f);									    // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							    // Enables Depth Testing
	glDepthFunc(GL_LESS);								    // The Type Of Depth Testing To Do
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#else
    glClearColor(0.f, 191.f / 255.f, 1.f, 1.0f);
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
#endif
}

int
main(int argc, char **argv)
{
    char Title[128];
    b32 Paused = false;

    u32 Mode = AGL_WINDOW_VSYNC | AGL_WINDOW_MSAA;
    agl_context *Ctx = aglCreateWindow("lalala", 1024, 768, Mode);
    if(!Ctx) return 0;
    InitGL();


    agl_shader DefShader = aglInitDefaultShader();
    if(!DefShader.Success)
    {
        aglCleanup();
        return 1;
    }    
    
    agl_drawable Floor = aglPrimitiveCube();
    Floor.Material = aglMaterial(V3(.8f), V3(.4f), V3(.5f), 1);
    aglGenBuffer(&Floor.Mesh);

    agl_drawable Wall = aglPrimitiveCube();
    Wall.Material = aglMaterial(V3i(1,1,1), V3(0.1f,0.1f,.1f), V3(.5f), 8);
    aglGenBuffer(&Wall.Mesh);
    
    agl_drawable Quad = aglPrimitiveCube(4);
    Quad.Material.Ambient = V3(.6, .5f, .125f);
    Quad.Material.Diffuse = Quad.Material.Ambient * .5f;
    Quad.Material.Shininess = 80;
    aglGenBuffer(&Quad.Mesh);
    
    agl_drawable Grid = aglPrimitiveGrid(128);
    aglGenBuffer(&Grid.Mesh);

    r32 time_pos = 0;
    v3 Pos = V3(0);
    agl_camera Camera;
    aglCameraInit(&Camera, AGL_CAMERA_MODE_FIRSTPERSON, V3i(0, 5, 5), 65.0f, -M_PI/2);

    agl_font *FontConsola = aglInitFont("c:/windows/fonts/consola.ttf", 12,  4096, 4096);

    char TextBuffer[512];
    u32 CurrentMode = GL_FILL;
    while(aglHandleEvents())
    {       
        if(aglKeyUp(VK_ESCAPE)) aglCloseWindow();
        if(aglKeyUp(VK_F1)) aglSetVerticalSync(!Ctx->VerticalSync);
        if(aglKeyUp(VK_F2))
        {
            switch(CurrentMode)
            {
                case GL_FILL: { CurrentMode = GL_LINE; glPointSize(1); break; }
                case GL_LINE: { CurrentMode = GL_POINT; glPointSize(10); break; }
                case GL_POINT: { CurrentMode = GL_FILL; glPointSize(1);break; }
            }
            glPolygonMode(GL_FRONT_AND_BACK, CurrentMode);

        }
        if(aglKeyUp(VK_F3)) aglToggleFullscreen(); 

        time_pos += Ctx->Delta;
        Quad.Material.Ambient = V3(sin(time_pos * 2.0f), sin(time_pos * .7f), sin(time_pos * 1.3f));
        Quad.Material.Diffuse = Quad.Material.Ambient * .4f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        aglBeginScene3D(Ctx, &Camera);
            glEnable(GL_CULL_FACE);
            aglDraw(&Wall, MultMat4x4(ScaleMatrix(.1f, 12.0f, 50.f), TranslationMatrix(-25.f, 0.f, 0.f)), &DefShader);
            aglDraw(&Wall, MultMat4x4(ScaleMatrix(.1f, 12.0f, 50.f), TranslationMatrix(25.f, 0.f, 0.f)), &DefShader);
            aglDraw(&Wall, MultMat4x4(ScaleMatrix(50.f, 12.0f, .1f), TranslationMatrix(0.f, 0.f, -25.f)), &DefShader);
            aglDraw(&Wall, MultMat4x4(ScaleMatrix(50.f, 12.0f, .1f), TranslationMatrix(0.f, 0.f, 25.f)), &DefShader);
            aglDraw(&Floor, ScaleMatrix(50.f, .1f, 50.f), &DefShader);
            aglDraw(&Quad, MultMat4x4(ScaleMatrix(1, fmax(fabs(sinf(time_pos)), 0.25f), 1), TranslationMatrix(Pos.x, Pos.y, Pos.z)), &DefShader);
            glDisable(GL_CULL_FACE);
        aglEndScene3D(Ctx, &Camera);
        glUseProgram(0);

        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        sprintf(TextBuffer, "[Cursor] X:%i, Y:%i [Camera] X:%i, Y:%i, Z:%i, FoV:%i, Yaw:%i, Pitch:%i",
                Ctx->MouseInput.X, Ctx->MouseInput.Y,
                (s32) Camera.Position.x, (s32) Camera.Position.y, (s32) Camera.Position.z,
                (s32) Camera.FoV, (s32) (Camera.Yaw * (180/M_PI)), (s32) (Camera.Pitch * (180/M_PI)));
        aglRenderText2D(FontConsola, TextBuffer, 10, 10, V3i(0,0,0));
        glPolygonMode(GL_FRONT_AND_BACK, CurrentMode);
        glEnable(GL_CULL_FACE);
        
        aglSwapBuffers();
    }

    aglDeleteFont(FontConsola);
    aglDelete(&Quad);
    aglDelete(&Grid);
    aglDelete(&Floor);
    aglDelete(&Wall);
    
    aglCleanup();
    return 0;
}
