#include <stdio.h>

#include "agl.h"
#include "agl_core3d.h"

void
InitGL()
{
	glClearColor(0.125f, 0.125f, 0.125f, 1.f);				// Black Background
	glClearDepth(1.0f);									    // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							    // Enables Depth Testing
	glDepthFunc(GL_LESS);								    // The Type Of Depth Testing To Do
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
    
    agl_drawable Floor = aglPrimitiveQuad();
    Floor.Material = aglMaterial(V3(.8f), V3(.4f), V3(.5f), 1);
    aglGenBuffer(&Floor.Mesh);

    agl_drawable Wall = aglPrimitiveQuad();
    Wall.Material = aglMaterial(V3i(1,1,1), V3(0.1f,0.1f,.1f), V3(.5f), 8);
    aglGenBuffer(&Wall.Mesh);

    
    agl_drawable Quad = aglPrimitiveQuad(4);
    Quad.Material.Ambient = V3(.6, .5f, .125f);
    Quad.Material.Diffuse = Quad.Material.Ambient * .5f;
    Quad.Material.Shininess = 80;
    aglGenBuffer(&Quad.Mesh);
    
    agl_drawable Grid = aglPrimitiveGrid(128);
    aglGenBuffer(&Grid.Mesh);

    r32 time_pos = 0;
    v3 Pos = V3(0);
    agl_camera Camera;
    aglCameraInit(&Camera, AGL_CAMERA_MODE_STATIC, V3i(0, 5, 5), 60.0f, M_PI, -.39f);
    Camera.Front = V3i(0,0,-7);
    
#if 1
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
#endif

    u32 CurrentMode = GL_FILL;
    while(aglHandleEvents())
    {
        sprintf(Title, "dt: %f, fps: %i, time: %.02f, Camera: %.02f/%.02f/%.02f Yaw: %.f, Pitch: %.f \n", Ctx->Delta, Ctx->FPS, Ctx->Time,
                Camera.Position.x, Camera.Position.y, Camera.Position.z, Camera.Yaw * (360/M_PI), Camera.Pitch * (360/M_PI)); 
        aglSetWindowTitle(Title);
        
        if(aglKeyUp(VK_ESCAPE)) aglCloseWindow();
        
        if(aglKeyUp(VK_F1)) aglSetVerticalSync(!Ctx->VerticalSync);
        if(aglKeyUp(VK_F2))
        {
            switch(CurrentMode)
            {
                case GL_FILL: { CurrentMode = GL_LINE; break; }
                case GL_LINE: { CurrentMode = GL_POINT; break; }
                case GL_POINT: { CurrentMode = GL_FILL; break; }
            }
            glPolygonMode(GL_FRONT_AND_BACK, CurrentMode);

        }
        if(aglKeyUp(VK_F3))
        {
            Camera.Mode = (Camera.Mode == AGL_CAMERA_MODE_FREE ? AGL_CAMERA_MODE_STATIC : AGL_CAMERA_MODE_FREE);
            if(Camera.Mode == AGL_CAMERA_MODE_STATIC) Camera.Front += Camera.Position;
        }
        if(aglKeyUp(VK_F12)) aglToggleFullscreen();   


        time_pos += Ctx->Delta;
        Quad.Material.Ambient = V3(sin(time_pos * 2.0f), sin(time_pos * .7f), sin(time_pos * 1.3f));
        Quad.Material.Diffuse = Quad.Material.Ambient * .4f;
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        aglBeginScene3D(Ctx, &Camera);
            aglDraw(&Wall, MultMat4x4(ScaleMatrix(.1f, 12.0f, 50.f), TranslationMatrix(-25.f, 0.f, 0.f)), &DefShader);
            aglDraw(&Wall, MultMat4x4(ScaleMatrix(.1f, 12.0f, 50.f), TranslationMatrix(25.f, 0.f, 0.f)), &DefShader);
            aglDraw(&Wall, MultMat4x4(ScaleMatrix(50.f, 12.0f, .1f), TranslationMatrix(0.f, 0.f, -25.f)), &DefShader);
            aglDraw(&Wall, MultMat4x4(ScaleMatrix(50.f, 12.0f, .1f), TranslationMatrix(0.f, 0.f, 25.f)), &DefShader);
            aglDraw(&Floor, ScaleMatrix(50.f, .1f, 50.f), &DefShader);
            aglDraw(&Quad, MultMat4x4(ScaleMatrix(1, fmax(fabs(sinf(time_pos)), 0.25f), 1), TranslationMatrix(Pos.x, Pos.y, Pos.z)), &DefShader);
        aglEndScene3D(Ctx, &Camera);
        glUseProgram(0);
           
        aglSwapBuffers();
    }
    
    aglDelete(&Quad);
    aglDelete(&Grid);
    aglDelete(&Floor);
    aglDelete(&Wall);
    
    aglCleanup();
    return 0;
}
