#if 0
#define AGL_MALLOC(x,u)  ((void)(u),VirtualAlloc(0,x,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE))
#define AGL_FREE(x,u)    ((void)(u),VirtualFree(x,u,MEM_RELEASE))
#define STB_malloc(x,u)  ((void)(u),VirtualAlloc(0,x,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE))
#define STB_free(x,u)    ((void)(u),VirtualFree(x,u,MEM_RELEASE))
#endif
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define AGL_DEBUG
#include "agl.h"
#include "agl_math.h"
#include "agl_core3d.h"
        
void
InitGL()
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}        

u32
genCubeVao()
{
    float skyboxVertices[] = {
        // Positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    u32 Result = 0, _tmp=0;
    glGenVertexArrays(1, &Result);
    glBindVertexArray(Result);
    glGenBuffers(1, &_tmp);
    glBindBuffer(GL_ARRAY_BUFFER_ARB, _tmp);
    glBufferData(GL_ARRAY_BUFFER_ARB, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW_ARB);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    return Result;
}


u32
loadCubemap(char *Textures[])
{
    u32 TextureID;
    glGenTextures(1, &TextureID);
    glActiveTexture(GL_TEXTURE0_ARB);

    s32 Width, Height, Components;
    u8 *image;

    glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, TextureID);
    for( s32 i=0; i < 6; i++)
    {
        image = stbi_load(Textures[i], &Width, &Height, &Components, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i, 0, GL_RGB, Width, Height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, image);
        
        stbi_image_free(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, 0);
    
    return TextureID;
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

    // Skybox stuff
    char *CubemapTextures[] = {
        {"cubemap/nightsky_ft.tga"},
        {"cubemap/nightsky_bk.tga"},
        {"cubemap/nightsky_up.tga"},
        {"cubemap/nightsky_dn.tga"},
        {"cubemap/nightsky_rt.tga"},
        {"cubemap/nightsky_lf.tga"}
    };
    
    u32 SkyboxTextureID = loadCubemap(CubemapTextures);
    s32 SkyboxVAO = genCubeVao();
    
    const char *skybox_vs = GLSL
        (
            layout (location = 0) in vec3 position;
            out vec3 TexCoords;

            uniform mat4 projection;
            uniform mat4 view;


            void main()
            {
                vec4 pos = projection * view * vec4(position, 1.0);  
                gl_Position = pos.xyww;  
                TexCoords = position;
            }  
         );

    const char *skybox_fs = GLSL
        (
            in vec3 TexCoords;
            out vec4 color;

            uniform samplerCube skybox;

            void main()
            {    
                color = texture(skybox, TexCoords);
            }
         );
    
    agl_shader_ext SkyboxShader = {0};
    SkyboxShader.Program = glCreateProgram();
    if(aglShaderCompileAndAttach(&SkyboxShader, skybox_vs, GL_VERTEX_SHADER) &&
       aglShaderCompileAndAttach(&SkyboxShader, skybox_fs, GL_FRAGMENT_SHADER) )
    {
        if(aglShaderLink(&SkyboxShader))
        {
            aglShaderSetBinding(&SkyboxShader, "projection");
            aglShaderSetBinding(&SkyboxShader, "view");
            aglShaderSetBinding(&SkyboxShader, "skybox");
        }
    }
    
    // Shadow stuff
    s32 MapWidth = 1024, MapHeight = 1024;
    u32 DepthMapFBO;
    glGenFramebuffers(1, &DepthMapFBO);

    u32 DepthMap;
    glGenTextures(1, &DepthMap);
    glBindTexture(GL_TEXTURE_2D, DepthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                 MapWidth, MapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER_ARB);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
    
    glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    agl_shader_ext ShadowMapShader = {};
    {
        const char *shadowmap_vs = GLSL
            (
                layout (location = 0) in vec3 position;

                uniform mat4 lightSpaceMatrix;
                uniform mat4 model;

                void main()
                {
                    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
                }   
             );

        const char *shadowmap_fs = GLSL
            (
                void main() {}
             );   
        ShadowMapShader.Program = glCreateProgram();
        if(aglShaderCompileAndAttach(&ShadowMapShader, shadowmap_vs, GL_VERTEX_SHADER) &&
           aglShaderCompileAndAttach(&ShadowMapShader, shadowmap_fs, GL_FRAGMENT_SHADER) )
        {
            if(aglShaderLink(&ShadowMapShader))
            {
                aglShaderSetBinding(&ShadowMapShader, "lightSpaceMatrix");
                aglShaderSetBinding(&ShadowMapShader, "model");
            }
        }    
    }

    // Drawing primitives and default shader
    agl_shader DefShader = aglInitDefaultShader();
    if(!DefShader.Success)
    {
        aglCleanup();
        return 1;
    }
    
    agl_drawable *Floor = aglPrimitiveCube();
    Floor->Material = aglMaterial(V3(0.1f), V3(0.1f), V3(.75f), 32);

    agl_drawable *Quad = aglPrimitiveCube(4);
    Quad->Material = aglMaterial(V3(0.1f), V3(0.1f), V3(.5f), 8);
    
    agl_drawable *Wall = aglPrimitiveCube();
    Wall->Material = aglMaterial(V3(.1f), V3(.1f), V3(.5f), 32);    
    //agl_drawable *Grid = aglPrimitiveGrid(128);
    
    r32 time_pos = 0;
    v3 Pos = V3(0);
    agl_camera Camera;
    aglCameraInit(&Camera, AGL_CAMERA_MODE_FIRSTPERSON, V3i(-22, 5, -22), 65.0f, M_PI/4);

    agl_font_stbttf *FontConsola = aglInitFont("c:/windows/fonts/consolab.ttf", 12,  4096, 4096);

    char TextBuffer[512];
    u32 CurrentMode = GL_FILL;

    r32 speed = 3.f;
    b32 ShowPoints = false;
    while(aglHandleEvents())
    {
        u32 Ticks = aglPlatformGetTicks();
        if(aglKeyUp(VK_ESCAPE)) aglCloseWindow();
        if(aglKeyUp(VK_F1)) aglSetVerticalSync(!Ctx->VerticalSync);
        if(aglKeyUp(VK_F2)) ShowPoints = !ShowPoints; 
        if(aglKeyUp(VK_F3))     aglToggleFullscreen();
        
        time_pos += Ctx->Delta;
        

        // init shadow stage
        glUseProgram(ShadowMapShader.Program);
        glViewport(0, 0, MapWidth, MapHeight);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);
        
        mat4x4 lightProj = OrhtogonalMatrix(-10.f, 10.f, -10.f, 10.f, 1.f, 7.5f);
        mat4x4 lightView = LookAtMatrix(V3(25.f), V3(0.f), V3(0.0f, 1.0f, 0.0f));
        mat4x4 lightMatrix = MultMat4x4(lightView, lightProj);
        glUniformMatrix4fv(ShadowMapShader.Bindings[0], 1, GL_FALSE, lightMatrix.E);
        glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);            
        // begin of shadow stage

        mat4x4 model = MultMat4x4(ScaleMatrix(.1f, 12.0f, 50.f), TranslationMatrix(-25.f, 0.f, 0.f));
        glUniformMatrix4fv(ShadowMapShader.Bindings[1], 1, GL_FALSE, model.E);
        glBindVertexArray(Wall->Mesh.VAO);
        glDrawArrays(Wall->GLRenderMode, 0, Wall->Mesh.VertexCount);
        glBindVertexArray(0);

        model = MultMat4x4(ScaleMatrix(.1f, 12.0f, 50.f), TranslationMatrix(25.f, 0.f, 0.f));
        glUniformMatrix4fv(ShadowMapShader.Bindings[1], 1, GL_FALSE, model.E);
        glBindVertexArray(Wall->Mesh.VAO);
        glDrawArrays(Wall->GLRenderMode, 0, Wall->Mesh.VertexCount);
        glBindVertexArray(0);

        model = MultMat4x4(ScaleMatrix(50.f, 12.0f, .1f), TranslationMatrix(0.f, 0.f, -25.f));
        glUniformMatrix4fv(ShadowMapShader.Bindings[1], 1, GL_FALSE, model.E);
        glBindVertexArray(Wall->Mesh.VAO);
        glDrawArrays(Wall->GLRenderMode, 0, Wall->Mesh.VertexCount);
        glBindVertexArray(0);

        model = MultMat4x4(ScaleMatrix(50.f, 12.0f, .1f), TranslationMatrix(0.f, 0.f, 25.f));
        glUniformMatrix4fv(ShadowMapShader.Bindings[1], 1, GL_FALSE, model.E);
        glBindVertexArray(Wall->Mesh.VAO);
        glDrawArrays(Wall->GLRenderMode, 0, Wall->Mesh.VertexCount);
        glBindVertexArray(0);

        for(int i=12; i > 0;i--){
            model = MultMat4x4(ScaleMatrix(1, fmax(fabs(sinf((time_pos+i) * speed )), 0.0f), 1),
                               TranslationMatrix(-26 + Pos.x + i * 4, Pos.y, Pos.z));
            glUniformMatrix4fv(ShadowMapShader.Bindings[1], 1, GL_FALSE, model.E);
            glBindVertexArray(Quad->Mesh.VAO);
            glDrawArrays(Quad->GLRenderMode, 0, Quad->Mesh.VertexCount);
            glBindVertexArray(0);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // end of shadow stage

        // object rendering
        aglBeginScene3D(Ctx, &Camera);      
            glCullFace(GL_BACK);
            glFrontFace(GL_CW);

            glUseProgram(DefShader.Id);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glUniformMatrix4fv(DefShader.Matrix[3], 1, GL_FALSE, lightMatrix.E);
            glActiveTexture(GL_TEXTURE0_ARB);
            glBindTexture(GL_TEXTURE_2D, DepthMap);
            glUniform1i(DefShader.ShadowMap, 0);
            
            aglDraw(Wall, MultMat4x4(ScaleMatrix(.1f, 12.0f, 50.f), TranslationMatrix(-25.f, 0.f, 0.f)), &DefShader, ShowPoints);
            aglDraw(Wall, MultMat4x4(ScaleMatrix(.1f, 12.0f, 50.f), TranslationMatrix(25.f, 0.f, 0.f)), &DefShader, ShowPoints);
            aglDraw(Wall, MultMat4x4(ScaleMatrix(50.f, 12.0f, .1f), TranslationMatrix(0.f, 0.f, -25.f)), &DefShader, ShowPoints);
            aglDraw(Wall, MultMat4x4(ScaleMatrix(50.f, 12.0f, .1f), TranslationMatrix(0.f, 0.f, 25.f)), &DefShader, ShowPoints);
            aglDraw(Floor, ScaleMatrix(50.f, .1f, 50.f), &DefShader, ShowPoints);
            for(int i=1; i< 13;i++){
                aglDraw(Quad, MultMat4x4(ScaleMatrix(1, fmax(fabs(sinf((time_pos+i) * speed )), 0.0f), 1),
                                          TranslationMatrix(-26 + Pos.x + i * 4, Pos.y, Pos.z)), &DefShader, ShowPoints);
            }

            // Skybox rendering
            glDisable(GL_CULL_FACE);
            glDepthMask(GL_FALSE);
            glUseProgram(SkyboxShader.Program);
            
            mat4x4 view = CurrentViewMatrix;
            view.m3 = 0.f; view.m7 = 0.f; view.m11 = 0.f; view.m12 = 0.f; view.m13 = 0.f; view.m14 = 0.f;
            glUniformMatrix4fv(SkyboxShader.Bindings[0], 1, GL_FALSE, (const float *) CurrentProjectionMatrix.E);
            glUniformMatrix4fv(SkyboxShader.Bindings[1], 1, GL_FALSE, (const float *) view.E);
            
            glBindVertexArray(SkyboxVAO);
            glActiveTexture(GL_TEXTURE0_ARB);
            glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, SkyboxTextureID);
            glUniform1i(SkyboxShader.Bindings[2], 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthMask(GL_TRUE);            
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
        glDisable(GL_BLEND);

        aglSwapBuffers();
    }

    aglDeleteFont(FontConsola);
    aglCleanupResources();

    glDeleteFramebuffers(1, &DepthMapFBO);
    glDeleteTextures(1, &DepthMap);
    aglCleanup();
    return 0;
}
