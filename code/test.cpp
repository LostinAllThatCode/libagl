#define AGL_MALLOC(x,u)  ((void)(u),VirtualAlloc(0,x,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE))
#define AGL_FREE(x,u)    ((void)(u),VirtualFree(x,u,MEM_RELEASE))
#define STB_malloc(x,u)  ((void)(u),VirtualAlloc(0,x,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE))
#define STB_free(x,u)    ((void)(u),VirtualFree(x,u,MEM_RELEASE))

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define AGL_IMPLEMENTATION
#define AGL_OPENGL_VSYNC
#define AGL_OPENGL_MSAA
#define AGL_OPENGL_MSAA_SAMPLES 8 
#define AGL_INIT                InitGL
#define AGL_CLEANUP             CleanUp
#define AGL_LOOP                GameLoop
#include "agl.h"
#include "agl_math.h"
#define AGL_PREDEFINED_SHADERS
#include "agl_core3d.h"

agl_context *Ctx;
agl_camera Camera;

agl_font_stbttf *FontConsola;

agl_shader DefShader;
 
agl_drawable *Floor, *Quad, *Wall;

v3 Pos = V3(0);
b32 ShowPoints;
r32 time_pos;
r32 speed = 3.f;

char TextBuffer[1024];

void
InitGL(agl_context *Context)
{
    Ctx = Context;
    aglSetWindowTitle(Context->GLInfo.Version);
   
    aglInitPredefinedShaders();
    aglSkyboxTextures("cubemap/nightsky_ft.tga", "cubemap/nightsky_bk.tga", "cubemap/nightsky_up.tga",
                      "cubemap/nightsky_dn.tga", "cubemap/nightsky_rt.tga", "cubemap/nightsky_lf.tga");

    aglShadowMapSize(4096, 4096);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Drawing primitives and default shader
    DefShader = aglInitDefaultShader();
    if(!DefShader.Success)
    {
        aglCloseWindow();
        return;
    }
    
    Floor = aglPrimitiveCube();
    Floor->Material = aglMaterial(V3(0.1f), V3(0.1f), V3(.75f), 32);

    Quad = aglPrimitiveCube(4);
    Quad->Material = aglMaterial(V3(0.1f), V3(0.1f), V3(.5f), 8);
    
    Wall = aglPrimitiveCube();
    Wall->Material = aglMaterial(V3(.1f), V3(.1f), V3(.5f), 32);        
    
    aglCameraInit(&Camera, AGL_CAMERA_MODE_FIRSTPERSON, V3i(-22, 5, -22), 65.0f, M_PI/4);

    FontConsola = aglInitFont("c:/windows/fonts/consolab.ttf", 13,  4096, 4096);
}        

void
CleanUp()
{
 
    glDeleteShader(DefShader.Id);
    glDeleteShader(FontRenderingShader->Program);
    
    aglDeleteFont(FontConsola);
    aglCleanupResources();

    aglDeInitPredefinedShaders();
    
}

void
GameLoop(float Delta)
{
    if(aglIsWindowActive())
    {
        time_pos += Delta;
        u32 Ticks = aglGetTicks();

        if(aglKeyUp(VK_ESCAPE)) aglCloseWindow();
        if(aglKeyUp(VK_F1)) aglToggleVSYNC();
        if(aglKeyUp(VK_F2)) ShowPoints = !ShowPoints; 
        if(aglKeyUp(VK_F3)) aglToggleFullscreen();

        mat4x4 lightProj = aglmOrhto(-10.f, 10.f, -10.f, 10.f, 1.f, 7.5f);
        mat4x4 lightView = aglmLookAt(V3(25.f), V3(0.f), V3(0.0f, 1.0f, 0.0f));
        mat4x4 lightMatrix = aglmMulMat4(lightView, lightProj);
        
        aglShadowStageBegin(lightMatrix);
        
        mat4x4 model = aglmMulMat4(aglmSclMat(.1f, 12.0f, 50.f), aglmTrlMat(-25.f, 0.f, 0.f));
        aglShaderSetUniformMat4fv("model", model.E);
        glBindVertexArray(Wall->Mesh.VAO);
        glDrawArrays(Wall->GLRenderMode, 0, Wall->Mesh.VertexCount);
        glBindVertexArray(0);

        model = aglmMulMat4(aglmSclMat(.1f, 12.0f, 50.f), aglmTrlMat(25.f, 0.f, 0.f));
        aglShaderSetUniformMat4fv("model", model.E);
        glBindVertexArray(Wall->Mesh.VAO);
        glDrawArrays(Wall->GLRenderMode, 0, Wall->Mesh.VertexCount);
        glBindVertexArray(0);

        model = aglmMulMat4(aglmSclMat(50.f, 12.0f, .1f), aglmTrlMat(0.f, 0.f, -25.f));
        aglShaderSetUniformMat4fv("model", model.E);
        glBindVertexArray(Wall->Mesh.VAO);
        glDrawArrays(Wall->GLRenderMode, 0, Wall->Mesh.VertexCount);
        glBindVertexArray(0);

        model = aglmMulMat4(aglmSclMat(50.f, 12.0f, .1f), aglmTrlMat(0.f, 0.f, 25.f));
        aglShaderSetUniformMat4fv("model", model.E);
        glBindVertexArray(Wall->Mesh.VAO);
        glDrawArrays(Wall->GLRenderMode, 0, Wall->Mesh.VertexCount);
        glBindVertexArray(0);

        for(int i=12; i > 0;i--){
            model = aglmMulMat4(aglmSclMat(1, fmax(fabs(sinf((time_pos+i) * speed )), 0.0f), 1),
                            aglmTrlMat(-26 + Pos.x + i * 4, Pos.y, Pos.z));
            aglShaderSetUniformMat4fv("model", model.E);
            glBindVertexArray(Quad->Mesh.VAO);
            glDrawArrays(Quad->GLRenderMode, 0, Quad->Mesh.VertexCount);
            glBindVertexArray(0);
        }
        aglShadowStageEnd();
        // end of shadow stage

        // object rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        aglBeginScene3D(Ctx, &Camera);
        {
            glCullFace(GL_BACK);
            glFrontFace(GL_CW);

            aglShaderUse(DefShader.Id);
            
            aglShaderSetUniformMat4fv("matrixSpace", lightMatrix.E);
            glBindTexture(GL_TEXTURE_2D, aglShadowMap());
            aglShaderSetUniform1i("shadowMap", 0);
            
            aglDraw(Wall, aglmMulMat4(aglmSclMat(.1f, 12.0f, 50.f), aglmTrlMat(-25.f, 0.f, 0.f)), &DefShader, ShowPoints);
            aglDraw(Wall, aglmMulMat4(aglmSclMat(.1f, 12.0f, 50.f), aglmTrlMat(25.f, 0.f, 0.f)), &DefShader, ShowPoints);
            aglDraw(Wall, aglmMulMat4(aglmSclMat(50.f, 12.0f, .1f), aglmTrlMat(0.f, 0.f, -25.f)), &DefShader, ShowPoints);
            aglDraw(Wall, aglmMulMat4(aglmSclMat(50.f, 12.0f, .1f), aglmTrlMat(0.f, 0.f, 25.f)), &DefShader, ShowPoints);
            aglDraw(Floor, aglmSclMat(50.f, .1f, 50.f), &DefShader, ShowPoints);
            for(int i=1; i< 13;i++){
                aglDraw(Quad, aglmMulMat4(aglmSclMat(1, fmax(fabs(sinf((time_pos+i) * speed )), 0.0f), 1),
                                      aglmTrlMat(-26 + Pos.x + i * 4, Pos.y, Pos.z)), &DefShader, ShowPoints);
            }

            aglSkyboxRender(CurrentProjectionMatrix, CurrentViewMatrix);
        }
        aglEndScene3D(Ctx, &Camera);

        glEnable(GL_BLEND);        
        sprintf(TextBuffer, "FPS:%i, dt:%f, Ticks:%u", Ctx->FPS, Ctx->Delta, Ticks);      
        aglRenderText2D(FontConsola, TextBuffer, 10, 12);
        sprintf(TextBuffer, "X:%i, Y:%i, Z:%i, FoV:%i, Yaw:%i , Pitch:%i",
                (s32) Camera.Position.x, (s32) Camera.Position.y, (s32) Camera.Position.z,
                (s32) Camera.FoV, (s32) (Camera.Yaw * (180/M_PI)), (s32) (Camera.Pitch * (180/M_PI)));
        aglRenderText2D(FontConsola, TextBuffer, 10, 24);        
        sprintf(TextBuffer, "X:%i, Y:%i", Ctx->Input.MouseX, Ctx->Input.MouseY);
        aglRenderText2D(FontConsola, TextBuffer, 10, 36);
        sprintf(TextBuffer, "OpenGL Version: %s", Ctx->GLInfo.Version);
        aglRenderText2D(FontConsola, TextBuffer, 10, 60);
        sprintf(TextBuffer, "Viewport: %ix%i", Ctx->Width, Ctx->Height);
        aglRenderText2D(FontConsola, TextBuffer, 10, 72);
        glDisable(GL_BLEND);
    }
}
