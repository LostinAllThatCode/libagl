#define AGL_MALLOC(x,u)  ((void)(u),VirtualAlloc(0,x,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE))
#define AGL_FREE(x,u)    ((void)(u),VirtualFree(x,u,MEM_RELEASE))
#define STB_malloc(x,u)  ((void)(u),VirtualAlloc(0,x,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE))
#define STB_free(x,u)    ((void)(u),VirtualFree(x,u,MEM_RELEASE))

#define AGL_IMPLEMENTATION
#define AGL_DEBUG
#define AGL_OPENGL_VSYNC
#define AGL_OPENGL_MSAA
#define AGL_OPENGL_MSAA_SAMPLES 2
#define AGL_INIT                InitGL
#define AGL_CLEANUP             CleanUp
#define AGL_LOOP                GameLoop
#include "agl.h"
#include "agl_math.h"


#define AGL_PREDEFINED_SHADERS
#include "agl_shaders.h"

#include "agl_core3d.h"
#include "agl_font2d.h"


agl_context *Ctx;
agl_shader *MainShader;
 
agl_drawable *Floor, *Quad, *Wall;

v3 Pos = V3(0);
b32 ShowPoints;
r32 time_pos;
r32 speed = 3.f;

char TextBuffer[1024];
agl_font FontConsola;

void
InitGL(agl_context *Context)
{
    Ctx = Context;
    aglSetWindowTitle(Context->GLInfo.Version);
   
    aglInitPredefinedShaders();
    aglSkyboxTextures("cubemap/nightsky_ft.tga", "cubemap/nightsky_bk.tga", "cubemap/nightsky_up.tga",
                      "cubemap/nightsky_dn.tga", "cubemap/nightsky_rt.tga", "cubemap/nightsky_lf.tga");

    aglShadowMapSize(4096, 4096);

    const char MainVertShader[] = {
        "#version 430\n"
        "layout(location = 0) in vec3 vertexPosition;\n"
        "layout(location = 1) in vec2 vertexUV;\n"
        "layout(location = 2) in vec3 vertexNormal;\n"
        "layout(location = 3) in vec3 vertexColor;\n"
        "uniform mat4 matrixModelViewProj;\n"
        "uniform mat4 matrixModel;\n"
        "uniform mat4 matrixView;\n"
        "uniform mat4 matrixSpace;\n"
        "out vec2 UV;\n"
        "out vec3 Normal;\n"
        "out vec3 FragPos;\n"
        "out vec4 FragPosLightSpace;\n"
        "void main(){\n"
        "    gl_Position = matrixModelViewProj * vec4(vertexPosition, 1);\n"
        "    UV = vertexUV;\n"
        "    FragPos = vec3(matrixModel * vec4(vertexPosition, 1));\n"
        "    Normal = transpose(inverse(mat3(matrixModel))) * vertexNormal;\n"
        "    FragPosLightSpace = matrixSpace * vec4(FragPos, 1.0);\n"
        "}\n"
    };
    const char MainFragShader[] = {
        "#version 430\n"
        "struct agl_material\n"
        "{\n"
        "    vec3  ambient;\n"
        "    vec3  diffuse;\n"
        "    vec3  specular;\n"
        "    float shininess;\n"
        "    sampler2D texture2D;\n"
        "};\n"        
        "struct agl_light\n"
        "{\n"
            "vec3 position;\n"
            "vec3 ambient;\n"
            "vec3 diffuse;\n"
            "vec3 specular;\n"
        "};\n"
        "uniform sampler2D shadowMap;\n"
        "uniform agl_material material;\n"
        "uniform agl_light light;\n"
        "uniform vec3 viewPos;\n"
        "in vec3 FragPos;\n"
        "in vec2 UV;\n"
        "in vec3 Normal;\n"
        "in vec4 FragPosLightSpace;\n"
        "uniform bool blinn = false;\n"
        "out vec4 FragColor;\n"
        "float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)\n"
        "{\n"
            "vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;\n"
            "projCoords = projCoords * 0.5 + 0.5;\n"
            "float closestDepth = texture(shadowMap, projCoords.xy).r;\n"
            "if(projCoords.z > 1.0) return 0.0;\n"
            "float currentDepth = projCoords.z;\n"
            "float bias = 0.000125;\n"
            "float shadow = 0.0;\n"
            "vec2 texelSize = .33 / textureSize(shadowMap, 0);\n"
            "for(int x = -1; x <= 1; ++x)\n"
            "{\n"
                "for(int y = -1; y <= 1; ++y)\n"
                "{\n"
                    "float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;\n"
                    "shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;\n"
                "}\n"
            "}\n"
            "shadow /= 24.0;\n"
            "return shadow;\n"
        "}\n"
        "void main(){\n"
            "vec3  color = material.ambient;\n"
            "vec3  normal = normalize(Normal);\n"
            "vec3  lightColor = vec3(1.0);\n"
            "vec3  ambient = 0.15 * color;\n"
            "vec3  lightDir = normalize(light.position - FragPos);\n"
            "float diff = max(dot(lightDir, normal), 0.0);\n"
            "vec3  diffuse = diff * lightColor;\n"
            "float spec;\n"
            "vec3 viewDir = normalize(viewPos - FragPos);\n"
            "if(blinn == true)\n"
            "{\n"
            "    vec3 reflectDir = reflect(-lightDir, normal);  \n"
            "    spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
            "} else {\n"
            "    vec3 halfwayDir = normalize(lightDir + viewDir);\n"
            "    spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);\n"
            "}\n"
            "vec3 specular = spec * lightColor;\n"
            "float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);\n"
            "vec3 lighting = ((ambient + (2.0 - shadow)) * (diffuse + specular)) * color;\n"
            "FragColor = vec4(lighting, 1.0);\n"
        "}\n"
    };
        
    MainShader = aglShaderCreate();
    aglShaderCompileAndAttach(MainShader, MainVertShader, GL_VERTEX_SHADER);
    aglShaderCompileAndAttach(MainShader, MainFragShader, GL_FRAGMENT_SHADER);
    aglShaderLink(MainShader);  
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Floor = aglPrimitiveCube();
    Floor->Material = aglMaterial(V3(0.1f), V3(0.1f), V3(.75f), 32);

    Quad = aglPrimitiveCube(4);
    Quad->Material = aglMaterial(V3(0.1f), V3(0.1f), V3(.5f), 8);
    
    Wall = aglPrimitiveCube();
    Wall->Material = aglMaterial(V3(.1f), V3(.1f), V3(.5f), 32);        

    aglFontCreate(&FontConsola, "c:/windows/fonts/arial.ttf", 16);
}        

void
CleanUp()
{
    aglFontDelete(&FontConsola);
    aglShaderDelete(MainShader);
    aglCleanupResources();

    aglDeInitPredefinedShaders();
    
}

void
GameLoop(float Delta)
{
    if(aglIsWindowActive())
    {
        time_pos += Delta;
        
        if(aglKeyUp(VK_ESCAPE)) aglCloseWindow();
        if(aglKeyUp(VK_F1)) aglToggleVSYNC();
        if(aglKeyUp(VK_F2)) ShowPoints = !ShowPoints; 
        if(aglKeyUp(VK_F3)) aglToggleFullscreen();

        mat4x4 lightProj = aglmOrhto(-10.f, 10.f, -10.f, 10.f, 1.f, 7.5f);
        mat4x4 lightView = aglmLookAt(V3(25.f), V3(0.f), V3(0.0f, 1.0f, 0.0f));
        mat4x4 lightMatrix = aglmMulMat4(lightView, lightProj);
        
        aglShadowStageBegin(lightMatrix);
        
        mat4x4 model = aglmMulMat4(aglmSclMat(.1f, 12.0f, 50.f), aglmTrlMat(-25.f, 0.f, 0.f));
        aglShaderSetUniformMat4fv(__aglsl_shadowmap, "model", model.E);
        glBindVertexArray(Wall->Mesh.VAO);
        glDrawArrays(Wall->GLRenderMode, 0, Wall->Mesh.VertexCount);
        glBindVertexArray(0);

        model = aglmMulMat4(aglmSclMat(.1f, 12.0f, 50.f), aglmTrlMat(25.f, 0.f, 0.f));
        aglShaderSetUniformMat4fv(__aglsl_shadowmap, "model", model.E);
        glBindVertexArray(Wall->Mesh.VAO);
        glDrawArrays(Wall->GLRenderMode, 0, Wall->Mesh.VertexCount);
        glBindVertexArray(0);

        model = aglmMulMat4(aglmSclMat(50.f, 12.0f, .1f), aglmTrlMat(0.f, 0.f, -25.f));
        aglShaderSetUniformMat4fv(__aglsl_shadowmap, "model", model.E);
        glBindVertexArray(Wall->Mesh.VAO);
        glDrawArrays(Wall->GLRenderMode, 0, Wall->Mesh.VertexCount);
        glBindVertexArray(0);

        model = aglmMulMat4(aglmSclMat(50.f, 12.0f, .1f), aglmTrlMat(0.f, 0.f, 25.f));
        aglShaderSetUniformMat4fv(__aglsl_shadowmap, "model", model.E);
        glBindVertexArray(Wall->Mesh.VAO);
        glDrawArrays(Wall->GLRenderMode, 0, Wall->Mesh.VertexCount);
        glBindVertexArray(0);

        for(int i=12; i > 0;i--){
            model = aglmMulMat4(aglmSclMat(1, fmax(fabs(sinf((time_pos+i) * speed )), 0.0f), 1),
                                aglmTrlMat(-26 + Pos.x + i * 4, Pos.y, Pos.z));
            aglShaderSetUniformMat4fv(__aglsl_shadowmap, "model", model.E);
            glBindVertexArray(Quad->Mesh.VAO);
            glDrawArrays(Quad->GLRenderMode, 0, Quad->Mesh.VertexCount);
            glBindVertexArray(0);
        }
        aglShadowStageEnd();
        // end of shadow stage

        // object rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        aglSetViewport3D(60, Ctx->Width, Ctx->Height);
        {
            glCullFace(GL_BACK);
            glFrontFace(GL_CW);

            glUseProgram(MainShader->Id);
            glEnable(GL_DEPTH_TEST);
            
            aglShaderSetUniformMat4fv(MainShader, "matrixSpace", lightMatrix.E);
            glBindTexture(GL_TEXTURE_2D, aglShadowMap());
            aglShaderSetUniform1i(MainShader, "shadowMap", 0);
            
            aglDraw(Wall, aglmMulMat4(aglmSclMat(.1f, 12.0f, 50.f), aglmTrlMat(-25.f, 0.f, 0.f)), MainShader, ShowPoints);
            aglDraw(Wall, aglmMulMat4(aglmSclMat(.1f, 12.0f, 50.f), aglmTrlMat(25.f, 0.f, 0.f)), MainShader, ShowPoints);
            aglDraw(Wall, aglmMulMat4(aglmSclMat(50.f, 12.0f, .1f), aglmTrlMat(0.f, 0.f, -25.f)), MainShader, ShowPoints);
            aglDraw(Wall, aglmMulMat4(aglmSclMat(50.f, 12.0f, .1f), aglmTrlMat(0.f, 0.f, 25.f)), MainShader, ShowPoints);
            aglDraw(Floor, aglmSclMat(50.f, .1f, 50.f), MainShader, ShowPoints);
            for(int i=1; i< 13;i++){
                aglDraw(Quad, aglmMulMat4(aglmSclMat(1, fmax(fabs(sinf((time_pos+i) * speed )), 0.0f), 1),
                                          aglmTrlMat(-26 + Pos.x + i * 4, Pos.y, Pos.z)), MainShader, ShowPoints);
            }

            aglSkyboxRender(__agl_proj, __agl_view);
        }

        aglSetViewport2D(Ctx->Width, Ctx->Height);
        glDisable(GL_DEPTH_TEST);
        
        sprintf(TextBuffer, "dt:%f, Übertragung", Delta);      
        aglDrawText(10, 20, TextBuffer, &FontConsola);
        /*
        sprintf(TextBuffer, "X:%i, Y:%i, Z:%i, FoV:%i, Yaw:%i , Pitch:%i",
                (s32) Camera.Position.x, (s32) Camera.Position.y, (s32) Camera.Position.z,
                (s32) Camera.FoV, (s32) (Camera.Yaw * (180/M_PI)), (s32) (Camera.Pitch * (180/M_PI)));
        aglRenderText2D(FontConsola, TextBuffer, 10, 24);
        */

        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, FontConsola.Texture);
        glBegin(GL_QUADS);
        aglDrawFontQuad(40, 40, 512 + 40, 512 + 40, 0, 0, 1, 1);
        glEnd();
        glDisable(GL_BLEND);
        
        aglSwapBuffers();
    }
}
