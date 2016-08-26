#define AGL_MALLOC(x,u)  ((void)(u),VirtualAlloc(0,x,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE))
#define AGL_FREE(x,u)    ((void)(u),VirtualFree(x,u,MEM_RELEASE))
#define STB_TRUETYPE_IMPLEMENTATION

#include <stdio.h>

#include "stb_truetype.h"

#include "agl.h"
#include "agl_core3d.h"

void
InitGL()
{
    glClearColor(0.f, 191.f / 255.f, 1.f, 1.0f);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
}

typedef struct
{
    agl_drawable Drawable;
    mat4x4       Position;
} object;

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

    mat4x4 mattest = IdentityMat4x4();
    agl_shader_ext Test = {};
    aglShaderSetBinding(&Test, "bla", AGL_SHADER_TYPE_MATRIX4, &mattest);

    mattest.E[2] = 1337.f;
    
    agl_drawable *Floor = aglPrimitiveCube();
    Floor->Material = aglMaterial(V3(.8f), V3(.4f), V3(.5f), 1);

    agl_drawable *Wall = aglPrimitiveCube();
    Wall->Material = aglMaterial(V3i(1,1,1), V3(0.1f,0.1f,.1f), V3(.5f), 8);
    
    agl_drawable *Quad = aglPrimitiveCube(4);
    Quad->Material.Ambient = V3(1.0f);
    Quad->Material.Diffuse = Quad->Material.Ambient * .5f;
    Quad->Material.Shininess = 160;
    
    agl_drawable *Grid = aglPrimitiveGrid(128);
    
    r32 time_pos = 0;
    v3 Pos = V3(0);
    agl_camera Camera;
    aglCameraInit(&Camera, AGL_CAMERA_MODE_FIRSTPERSON, V3i(-22, 5, 21), 65.0f, -M_PI/4);

    agl_ttf_font *FontConsola = aglInitFont("c:/windows/fonts/consola.ttf", 12,  4096, 4096);

    char TextBuffer[512];
    u32 CurrentMode = GL_FILL;

    r32 speed = 3.f;
    
    while(aglHandleEvents())
    {
        u32 Ticks = aglPlatformGetTicks();
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
        if(aglKeyUp(VK_F3))     aglToggleFullscreen();
        
        time_pos += Ctx->Delta;
        Wall->Material.Ambient = V3(sin(time_pos * 2.0f), sin(time_pos * .7f), sin(time_pos * 1.3f));
        Wall->Material.Diffuse = Quad->Material.Ambient * .4f;
     
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        aglBeginScene3D(Ctx, &Camera);
            glEnable(GL_CULL_FACE);
            aglDraw(Wall, MultMat4x4(ScaleMatrix(.1f, 12.0f, 50.f), TranslationMatrix(-25.f, 0.f, 0.f)), &DefShader);
            aglDraw(Wall, MultMat4x4(ScaleMatrix(.1f, 12.0f, 50.f), TranslationMatrix(25.f, 0.f, 0.f)), &DefShader);
            aglDraw(Wall, MultMat4x4(ScaleMatrix(50.f, 12.0f, .1f), TranslationMatrix(0.f, 0.f, -25.f)), &DefShader);
            aglDraw(Wall, MultMat4x4(ScaleMatrix(50.f, 12.0f, .1f), TranslationMatrix(0.f, 0.f, 25.f)), &DefShader);
            aglDraw(Floor, ScaleMatrix(50.f, .1f, 50.f), &DefShader);
            for(int i=1; i< 13;i++){
                aglDraw(Quad, MultMat4x4(ScaleMatrix(1, fmax(fabs(sinf((time_pos+i) * speed )), 0.0f), 1),
                                          TranslationMatrix(-26 + Pos.x + i * 4, Pos.y, Pos.z)), &DefShader);
            }
            glDisable(GL_CULL_FACE);
        aglEndScene3D(Ctx, &Camera);
        glUseProgram(0);

        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        sprintf(TextBuffer, "[ SYSTEM ] FPS:%i, dt:%f, Frame:%u ms", Ctx->FPS, speed, aglPlatformGetTicks() - Ticks);      
        aglRenderText2D(FontConsola, TextBuffer, 10, 12, V3i(0,0,0));
        sprintf(TextBuffer, "[ CAMERA ] X:%i, Y:%i, Z:%i, FoV:%i, Yaw:%i, Pitch:%i",
                (s32) Camera.Position.x, (s32) Camera.Position.y, (s32) Camera.Position.z,
                (s32) Camera.FoV, (s32) (Camera.Yaw * (180/M_PI)), (s32) (Camera.Pitch * (180/M_PI)));
        aglRenderText2D(FontConsola, TextBuffer, 10, 24, V3i(0,0,0));        
        sprintf(TextBuffer, "[ CURSOR ] X:%i, Y:%i", Ctx->Input.MouseX, Ctx->Input.MouseY);
        aglRenderText2D(FontConsola, TextBuffer, 10, 36, V3i(0,0,0));        

        glPolygonMode(GL_FRONT_AND_BACK, CurrentMode);
        glEnable(GL_CULL_FACE);
     
        aglSwapBuffers();
    }

    aglDeleteFont(FontConsola);
    aglCleanupResources();
    
    aglCleanup();
    return 0;
}
