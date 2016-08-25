#if !defined(AGL_CORE3D_H)

#include "agl_math.h"

#if !defined(AGL_MALLOC) || !defined(AGL_FREE)
    #include <stdlib.h>
    #define AGL_MALLOC(s) malloc(s)
    #define AGL_FREE(v) free(v)
#endif

#if defined(AGL_USE_STB_TRUETYPE)
    #define STB_TRUETYPE_IMPLEMENTATION
    #include "stb_truetype.h"

    #define AGL_TRUETYPE_DEFAULT_TEXT_SIZE       48
    #define AGL_TRUETYPE_DEFAULT_TEXTURE_SIZE    1024
    #define AGL_TRUETYPE_DEFAULT_CHARACTERS      95
    #define AGL_TRUETYPE_DEFAULT_OVERSAMPLES     2
    #define AGL_TRUETYPE_DEFAULT_FIRST_CHARACTER 32
    typedef struct
    {
        u32 FontSize;
        u32 Width;
        u32 Height;
        u32 OverSampleX;
        u32 OverSampleY;
        stbtt_packedchar CharacterInfo[AGL_TRUETYPE_DEFAULT_CHARACTERS];
        u32 Texture;
    } agl_font;

    typedef struct
    {
        u32 Id;
        u32 Uniforms[6];
        u32 VBO[2];
        u32 IBO;
        /* 
           0 = viewProjMatrix
           1 = worldMatrix
           2 = mainTex
           3 = fontColor
           4 = render2D
           5 = renderInfo
        */  
    } agl_shader_text;

    typedef struct
    {
        u32 VAO;
        u32 VBO[3];
        s32 IndexCount;
    } agl_render_text;
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
    enum
    {
        AGL_CAMERA_MODE_STATIC      = 0,
        AGL_CAMERA_MODE_FREE        = 1,
        AGL_CAMERA_MODE_FIRSTPERSON = 2,
        AGL_CAMERA_MODE_TARGET      = 3,
    };

    typedef struct
    {
        s32 Mode;
        v3  Position, Front, Right, Up;
        r32 Speed, Sensitivity, Yaw, Pitch, FoV;
        v3 *Target;
    } agl_camera;

    typedef struct 
    {
        u32 VAO;
        u32 VBO[8];
        u32 IBO;
        s32 VertexCount;
        s32 IndexCount;
        v3  *Vertices;
        v2  *TextureCoords;
        v3  *Normals;
        u16 *Indicies;
    } agl_mesh;

    typedef struct
    {
        s32  ID;
        v3   Ambient, Diffuse, Specular;
        r32  Shininess;
        s32  Texture;
    } agl_material;

    typedef struct
    {
        agl_mesh     Mesh;
        agl_material Material;
        s32          GLRenderMode; // GL_TRIANGLES, GL_QUADS, GL_POINTS, ...
    } agl_drawable;

    #include "agl_shaders.h"
    typedef struct
    {
        b32 Success;
        b32 IsDefault;
        s32 Id;
        s32 Camera;
        s32 Matrix[4];
        s32 Material[4];
        s32 Light[4];
        /* For internal drawing operations this ordering must be used.
           Otherwise you have to bind your uniforms yourself before drawing.
           
           Matrix:
           0 = matrixModelViewProj;
           1 = matrixModelView;
           2 = matrixInverse;
           
           Material:
           0 = material.ambient;
           1 = material.diffuse;
           2 = material.specular;
           3 = material.shininess;
           
           Light:
           0 = light.ambient;
           1 = light.diffuse;
           2 = light.specular;
           3 = light.shininess;
        */
    } agl_shader;
    
    
#ifdef __cplusplus
}
#endif

r32 LastUpdateTime;
s32 MaterialIndexCount;
mat4x4 CurrentProjectionMatrix, CurrentViewMatrix;
agl_camera *ActiveCamera;
agl_shader_text *FontRenderingShader;

inline agl_shader
aglInitDefaultShader()
{
    agl_shader Result = {}; 
    
    s32 Results[] = {
        aglCompileShader(AGL_SHADERS_VERT_4, GL_VERTEX_SHADER),
        aglCompileShader(AGL_SHADERS_FRAG_3, GL_FRAGMENT_SHADER)
    };
    Result.Id = glCreateProgram();
    Result.IsDefault = true;
    if(aglLinkProgram(Result.Id, Results, 2))
    {
        Result.Matrix[0] = glGetUniformLocation(Result.Id, "matrixModelViewProj");
        Result.Matrix[1] = glGetUniformLocation(Result.Id, "matrixModel");
        Result.Matrix[2] = glGetUniformLocation(Result.Id, "matrixView");
        Result.Matrix[3] = glGetUniformLocation(Result.Id, "matrixInverse");

        Result.Material[0] = glGetUniformLocation(Result.Id, "material.ambient");
        Result.Material[1] = glGetUniformLocation(Result.Id, "material.diffuse");
        Result.Material[2] = glGetUniformLocation(Result.Id, "material.specular");
        Result.Material[3] = glGetUniformLocation(Result.Id, "material.shininess");
        
        Result.Light[0] = glGetUniformLocation(Result.Id, "light.position");
        Result.Light[1] = glGetUniformLocation(Result.Id, "light.ambient");
        Result.Light[2] = glGetUniformLocation(Result.Id, "light.diffuse");
        Result.Light[3] = glGetUniformLocation(Result.Id, "light.specular");

        Result.Camera = glGetUniformLocation(Result.Id, "viewPos");
        Result.Success = true;
    }
    AGL_ASSERT(Result.Success);
    return Result;
}

extern void
aglCameraInit(agl_camera *Camera, s32 Mode = 0, v3 Position = V3(0, 0, 1),
              r32 FoV = 45.0f, r32 Yaw = -M_PI/2, r32 Pitch = 0.0f,
              r32 Speed = 6.0f, r32 Sensitivity = 0.005f)
{
    Camera->Mode = Mode;
    Camera->Position = Position;  
    Camera->Speed = Speed;
    Camera->Sensitivity = Sensitivity;
    Camera->Yaw = Yaw;
    Camera->Pitch = Pitch;
    Camera->FoV = FoV;
    Camera->Up    = V3i(0,1,0);
    /*
      Camera->Front = V3(cosf(Camera->Pitch) * sinf(Camera->Yaw), sinf(Camera->Pitch), cosf(Camera->Pitch) * cosf(Camera->Yaw));
      Camera->Right = V3(sinf(Camera->Yaw - M_PI/2.0f), 0, cosf(Camera->Yaw - M_PI/2.0f));
      Camera->Up = CrossV3(Camera->Right, Camera->Front);
    */
}

inline void
aglCameraUpdate(agl_camera *Camera, agl_context *Context)
{
    if(LastUpdateTime != Context->Time)
    {
        r32 Speed = Context->Delta * Camera->Speed;
        switch(Camera->Mode)
        {
            case AGL_CAMERA_MODE_FREE:
            {
                if(aglKeyDown('W')) Camera->Position += Camera->Front * Speed;
                if(aglKeyDown('S')) Camera->Position -= Camera->Front * Speed;
                if(aglKeyDown('D')) Camera->Position += Camera->Right * Speed;
                if(aglKeyDown('A')) Camera->Position -= Camera->Right * Speed;

                if(aglMouseDown(AGL_MOUSE_LEFT))
                {
                    aglPlatformSetCursor(false);
                    Camera->Yaw += Context->MouseInput.dX * Camera->Sensitivity;
                    Camera->Pitch -= Context->MouseInput.dY * Camera->Sensitivity;
                } else aglPlatformSetCursor(true);
            }break;
            case AGL_CAMERA_MODE_STATIC:
            {
                // Nothing to update
            }break;
            case AGL_CAMERA_MODE_TARGET:
            {
                // 
            }break;
            case AGL_CAMERA_MODE_FIRSTPERSON:
            {
                Camera->FoV -= (aglMouseWheelDelta() * Camera->Sensitivity);
                if(aglMouseDown(AGL_MOUSE_LEFT))
                {
                    aglCaptureMouse(true);

                    Camera->Yaw += Context->MouseInput.dX * Camera->Sensitivity;
                    Camera->Pitch -= Context->MouseInput.dY * Camera->Sensitivity;

                    if(Camera->Pitch < -((M_PI-0.03f)/2)) Camera->Pitch = -((M_PI-0.03f)/2);
                    if(Camera->Pitch > ((M_PI-0.03f)/2))  Camera->Pitch = (M_PI-0.03f)/2;

                
                } else aglCaptureMouse(false);
            
                v3 Forward = NormalizeV3(V3(Camera->Front.x, 0, Camera->Front.z));
                if(aglKeyDown('W')) Camera->Position += Forward * Speed;
                if(aglKeyDown('S')) Camera->Position -= Forward * Speed;
                if(aglKeyDown('D')) Camera->Position += Camera->Right * Speed;
                if(aglKeyDown('A')) Camera->Position -= Camera->Right * Speed;
            
            }break;
            default:
            {
                AGL_ASSERT(!"Not implemented yet");
            }
        }
        LastUpdateTime = Context->Time;
    }
}

inline agl_material
aglMaterial(v3 Ambient = V3(1.0f), v3 Diffuse = V3(1.0f), v3 Specular = V3(0.5f), r32 Shininess = 32.0f)
{
    agl_material Material = {};
    Material.Ambient  = Ambient;
    Material.Diffuse  = Diffuse;
    Material.Specular = Specular;
    Material.Shininess = Shininess;
    Material.ID = ++MaterialIndexCount;
    return Material;
}

inline mat4x4
aglCameraView(agl_camera *Camera)
{
    mat4x4 Result;
    switch(Camera->Mode)
    {
        case AGL_CAMERA_MODE_FREE:
        {
            Camera->Front = V3(cosf(Camera->Pitch) * cosf(Camera->Yaw), sinf(Camera->Pitch), cosf(Camera->Pitch) * sinf(Camera->Yaw));
            Camera->Front = NormalizeV3(Camera->Front);
                
            Camera->Right = V3(cosf(Camera->Yaw + M_PI/2.0f), 0, sinf(Camera->Yaw + M_PI/2.0f));
            Camera->Right = NormalizeV3(Camera->Right);
            
            Result = LookAtMatrix(Camera->Position, Camera->Position + Camera->Front, Camera->Up);
        }break;
        case AGL_CAMERA_MODE_STATIC:
        {
            Result = LookAtMatrix(Camera->Position, Camera->Front, Camera->Up);
        }break;
        case AGL_CAMERA_MODE_TARGET:
        {
            Result = LookAtMatrix(Camera->Position, *Camera->Target, Camera->Up);
        }break;
        case AGL_CAMERA_MODE_FIRSTPERSON:
        {
            Camera->Front = V3(cosf(Camera->Pitch) * cosf(Camera->Yaw), sinf(Camera->Pitch), cosf(Camera->Pitch) * sinf(Camera->Yaw));
            Camera->Front = NormalizeV3(Camera->Front);
                
            Camera->Right = V3(cosf(Camera->Yaw + M_PI/2.0f), 0, sinf(Camera->Yaw + M_PI/2.0f));
            Camera->Right = NormalizeV3(Camera->Right);
            
            Result = LookAtMatrix(Camera->Position, Camera->Position + Camera->Front, Camera->Up);
        }break;
        default:
        {
            AGL_ASSERT(!"Not implemented yet");
        }break;
    }
    return Result;
}

extern void
aglGenBuffer(agl_mesh *Mesh, b32 Dynamic = false)
{
    if(Mesh)
    {
        s32 Mode = (Dynamic ? GL_DYNAMIC_DRAW_ARB : GL_STATIC_DRAW_ARB);
        if(Mesh->VertexCount > 0)
        {
            glGenVertexArrays(1, &Mesh->VAO);
            glBindVertexArray(Mesh->VAO);
            
            if(Mesh->Vertices)
            {
                glGenBuffers(1, Mesh->VBO);
                glBindBuffer(GL_ARRAY_BUFFER_ARB, Mesh->VBO[0]);
                glBufferData(GL_ARRAY_BUFFER_ARB, Mesh->VertexCount * sizeof(v3), Mesh->Vertices, Mode);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            }
            
            if(Mesh->TextureCoords)
            {
                glGenBuffers(1, Mesh->VBO + 1);
                glBindBuffer(GL_ARRAY_BUFFER_ARB, Mesh->VBO[1]);
                glBufferData(GL_ARRAY_BUFFER_ARB, Mesh->VertexCount * sizeof(v2), Mesh->TextureCoords, Mode);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            }
            
            if(Mesh->Normals)
            {
                glGenBuffers(1, Mesh->VBO + 2);
                glBindBuffer(GL_ARRAY_BUFFER_ARB, Mesh->VBO[2]);
                glBufferData(GL_ARRAY_BUFFER_ARB, Mesh->VertexCount * sizeof(v3), Mesh->Normals, Mode);
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            }
            glBindVertexArray(0);
        }
        
    }
}

extern void
aglDelete(agl_drawable *Drawable)
{
    if(Drawable->Mesh.Vertices)      AGL_FREE(Drawable->Mesh.Vertices);
    if(Drawable->Mesh.TextureCoords) AGL_FREE(Drawable->Mesh.TextureCoords);
    if(Drawable->Mesh.Normals)       AGL_FREE(Drawable->Mesh.Normals);

    glDeleteVertexArrays(1, &Drawable->Mesh.VAO);
    AGL_ASSERT(glGetError() != GL_INVALID_VALUE);
}

static void
aglGenCube(agl_mesh *Mesh, r32 Size)
{
    s32 VertexCount = 36;
    s32 TriangleCount = VertexCount / 3;
    r32 XZ = Size / 2.0f;
    r32 Y  = Size;

    Mesh->TextureCoords = 0;
    
    Mesh->VertexCount = VertexCount;

    Mesh->Vertices = (v3 *) AGL_MALLOC( sizeof(v3) * VertexCount );
    AGL_ASSERT(Mesh->Vertices);
    Mesh->Normals = (v3 *) AGL_MALLOC( sizeof(v3) * VertexCount );
    AGL_ASSERT(Mesh->Normals);
    
    // Clockwise vertex definition
    v3 *v = Mesh->Vertices, *n = Mesh->Normals;
    *v++ = {-XZ, 0.0f, XZ}; *v++ = {XZ, Y, XZ}; *v++ = {XZ, 0.0f, XZ};
    *v++ = {-XZ, 0.0f, XZ}; *v++ = {-XZ, Y, XZ}; *v++ = {XZ, Y, XZ};
    for(s32 i=0; i<6; i++) *n++ = {0,0,1};
    *v++ = {-XZ, 0.0f, -XZ}; *v++ = {XZ, 0.0f, -XZ}; *v++ = {XZ, Y, -XZ}; 
    *v++ = {-XZ, 0.0f, -XZ}; *v++ = {XZ, Y, -XZ}; *v++ = {-XZ, Y, -XZ}; 
    for(s32 i=0; i<6; i++) *n++ = {0,0,-1};
    *v++ = {-XZ, 0.0f, XZ}; *v++ = {XZ, 0.0f, XZ}; *v++ = {XZ, 0, -XZ};
    *v++ = {-XZ, 0.0f, XZ}; *v++ = {XZ, 0.0f, -XZ}; *v++ = {-XZ, 0, -XZ};
    for(s32 i=0; i<6; i++) *n++ = {0,-1,0};
    *v++ = {-XZ, Y, XZ}; *v++ = {XZ, Y, -XZ}; *v++ = {XZ, Y, XZ};
    *v++ = {-XZ, Y, XZ}; *v++ = {-XZ, Y, -XZ}; *v++ = {XZ, Y, -XZ};
    for(s32 i=0; i<6; i++) *n++ = {0,1,0};
    *v++ = {-XZ, 0, XZ};  *v++ = {-XZ, Y, -XZ}; *v++ = {-XZ, Y, XZ};
    *v++ = {-XZ, 0, XZ}; *v++ = {-XZ, 0, -XZ}; *v++ = {-XZ, Y, -XZ};
    for(s32 i=0; i<6; i++) *n++ = {-1,0,0};
    *v++ = {XZ, 0, XZ}; *v++ = {XZ, Y, XZ}; *v++ = {XZ, Y, -XZ};
    *v++ = {XZ, 0, XZ}; *v++ = {XZ, Y, -XZ}; *v++ = {XZ, 0, -XZ};
    for(s32 i=0; i<6; i++) *n++ = {1,0,0};
}

static void
aglGenGridFlat(agl_mesh *Mesh, r32 Size, r32 GridWidth, r32 GridHeight)
{
    s32 VertexCount = (Size * 4) + 4;
    s32 TriangleCount = 0;

    Mesh->Normals       = 0;
    Mesh->TextureCoords = 0;
    
    Mesh->VertexCount   = VertexCount;
    
    Mesh->Vertices      = (v3 *) AGL_MALLOC( sizeof(v3) * VertexCount );
    AGL_ASSERT(Mesh->Vertices);
    
    v3 *v = Mesh->Vertices;
    for(s32 i = 0; i < Size+1; i++)
    {
        r32 X = -((Size * GridWidth)  / 2.0f);
        r32 Z = -((Size * GridHeight) / 2.0f) + (i * GridWidth);
        
        *v++ = { X, 0, Z };
        *v++ = { -X, 0, Z };

        *v++ = { Z, 0, X };
        *v++ = { Z, 0, -X };
    }
}

inline agl_drawable
aglPrimitiveGrid(r32 Size = 16.0f, r32 GridWidth = 1.0f, r32 GridHeight = 1.0f)
{
    agl_drawable Result = {};
    Result.GLRenderMode = GL_LINES;
    Result.Material = aglMaterial();
    Result.Material.Specular = V3(0, 0, 0);
    Result.Material.Shininess = 0;
    aglGenGridFlat(&Result.Mesh, Size, GridWidth, GridHeight);
    return Result;    
}

inline agl_drawable
aglPrimitiveCube(r32 Size = 1.0f)
{
    agl_drawable Result = {};
    Result.GLRenderMode = GL_TRIANGLES;
    Result.Material = aglMaterial();
    aglGenCube(&Result.Mesh, Size);
    return Result;
}

inline void
aglBeginScene3D(agl_context *Context, agl_camera *Camera)
{
    if(Camera) aglCameraUpdate(Camera, Context);
    
    glViewport(0, 0, Context->Width, Context->Height);

    CurrentProjectionMatrix = PerspectiveMatrix(Camera->FoV, (r32)Context->Width / (r32)Context->Height, .1f, 1000.0f);
    CurrentViewMatrix       = aglCameraView(Camera);

    ActiveCamera = Camera;
}

inline void
aglEndScene3D(agl_context *Context, agl_camera *Camera)
{
    CurrentProjectionMatrix = IdentityMat4x4();
    CurrentViewMatrix       = IdentityMat4x4();
}


inline void
aglDraw(agl_drawable *Drawable, mat4x4 ModelMatrix = IdentityMat4x4(), agl_shader *Shader = 0)
{
    if(Drawable)
    {
        if(Shader && Shader->IsDefault)
        {
            glUseProgram(Shader->Id);

            v3 lightColor = V3( 1, 1, 1);
            v3 diffuse = lightColor;
            v3 ambient = diffuse * .5f;
        
            glUniform3f(Shader->Light[0], 0.f, 30.f, 0.f);
            glUniform3f(Shader->Light[1], ambient.r, ambient.g, ambient.b);
            glUniform3f(Shader->Light[2], diffuse.r, diffuse.g, diffuse.b);
            glUniform3f(Shader->Light[3], 1.0f, 1.0f, 1.0f);

            mat4x4 Result = MultMat4x4(ModelMatrix, CurrentViewMatrix);
            Result = MultMat4x4(Result, CurrentProjectionMatrix);

            glUniformMatrix4fv(Shader->Matrix[0], 1, GL_FALSE, (const float *) Result.E);
            glUniformMatrix4fv(Shader->Matrix[1], 1, GL_FALSE, (const float *) ModelMatrix.E);
            glUniformMatrix4fv(Shader->Matrix[2], 1, GL_FALSE, (const float *) CurrentViewMatrix.E);

            glUniform3f(Shader->Material[0], Drawable->Material.Ambient.r, Drawable->Material.Ambient.g, Drawable->Material.Ambient.b);
            glUniform3f(Shader->Material[1], Drawable->Material.Diffuse.r, Drawable->Material.Diffuse.g, Drawable->Material.Diffuse.b);
            glUniform3f(Shader->Material[2], Drawable->Material.Specular.r, Drawable->Material.Specular.g, Drawable->Material.Specular.b);
            glUniform1f(Shader->Material[3], Drawable->Material.Shininess);

            glUniform3f(Shader->Camera, ActiveCamera->Position.x, ActiveCamera->Position.y, ActiveCamera->Position.z);

            glBindVertexArray(Drawable->Mesh.VAO);
            
            glDrawArrays(Drawable->GLRenderMode, 0, Drawable->Mesh.VertexCount);
            
            glBindVertexArray(0);
            
        } else glDrawArrays(Drawable->GLRenderMode, 0, Drawable->Mesh.VertexCount); 
    }
}



#if defined(AGL_USE_STB_TRUETYPE)

char *
aglReadFile(const char *File)
{
    char *Result;
    FILE *fp;
    size_t memsize;
    fp = fopen(File, "rb");
    fseek(fp, 0, SEEK_END);
    memsize = ftell(fp);
    rewind(fp);

    Result = (char *) AGL_MALLOC( sizeof(char) * (memsize+1));
    fread(Result, 1, memsize, fp);
    fclose(fp);
    return Result;
}

agl_font *
aglInitFont(const char *Filename,
            s32 FontSize = AGL_TRUETYPE_DEFAULT_TEXT_SIZE,
            s32 Width = AGL_TRUETYPE_DEFAULT_TEXTURE_SIZE,
            s32 Height = AGL_TRUETYPE_DEFAULT_TEXTURE_SIZE)
{
    agl_font *Result = (agl_font *) AGL_MALLOC( sizeof(agl_font) );
    Result->FontSize     = FontSize;
    Result->Width        = Width;
    Result->Height       = Height;
    Result->OverSampleX  = AGL_TRUETYPE_DEFAULT_OVERSAMPLES;
    Result->OverSampleY  = AGL_TRUETYPE_DEFAULT_OVERSAMPLES;
    Result->Texture = 0;

    char *FontData = aglReadFile(Filename);
    
    u8 *AtlasData = (u8 *) AGL_MALLOC( Result->Width * Result->Height );
    AGL_ASSERT(AtlasData);
    
    stbtt_pack_context Context;
    if(stbtt_PackBegin(&Context, AtlasData, Result->Width, Result->Height, 0, 1, 0))
    {
        stbtt_PackSetOversampling(&Context, Result->OverSampleX,  Result->OverSampleY);
        if(stbtt_PackFontRange(&Context, (u8 *) FontData, 0, Result->FontSize,
                               AGL_TRUETYPE_DEFAULT_FIRST_CHARACTER, AGL_TRUETYPE_DEFAULT_CHARACTERS, Result->CharacterInfo))
        {
            stbtt_PackEnd(&Context);
        } else AGL_FREE(Result);
    } else AGL_FREE(Result);
    
    AGL_ASSERT(Result);
    
    glGenTextures(1, &Result->Texture);
    glBindTexture(GL_TEXTURE_2D, Result->Texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Result->Width, Result->Height, 0, GL_RED, GL_UNSIGNED_BYTE, AtlasData);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);

    AGL_FREE(FontData);
    AGL_FREE(AtlasData);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return Result;
}

void
aglDeleteFont(agl_font *Font)
{
    AGL_FREE(Font);
}

agl_render_text *
aglRenderableText(char *Text, agl_font *Font)
{
    agl_render_text * Result = (agl_render_text *) AGL_MALLOC(sizeof(agl_render_text));
    
    s32 TextLength = strlen(Text);
    s32 VertexCount = 4 * TextLength;
    s32 IndexCount = 6 * TextLength;
    
    v3 *vdata      = (v3 *)  AGL_MALLOC(sizeof(v3) * VertexCount);
    v2 *texdata    = (v2 *)  AGL_MALLOC(sizeof(v2) * VertexCount);
    u16 *idata     = (u16 *) AGL_MALLOC(sizeof(u16) * IndexCount);

    r32 OffsetX = 0, OffsetY = 0;
    s32 LastIndex = 0;

    v3 *v = vdata; v2 *t = texdata; u16 *i = idata;
    for(char *c = Text; *c; c++)
    {
        stbtt_aligned_quad Quad;
        stbtt_GetPackedQuad(Font->CharacterInfo, Font->Width, Font->Height,
                            *c - AGL_TRUETYPE_DEFAULT_FIRST_CHARACTER,
                            &OffsetX, &OffsetY, &Quad, 1);

        *v++ = {Quad.x0, -Quad.y1, 0}; *v++ = {Quad.x0, -Quad.y0, 0};
        *v++ = {Quad.x1, -Quad.y0, 0}; *v++ = {Quad.x1, -Quad.y1, 0};

        *t++ = {Quad.s0, Quad.t1}; *t++ = {Quad.s0, Quad.t0};
        *t++ = {Quad.s1, Quad.t0}; *t++ = {Quad.s1, Quad.t1};

        *i++ = LastIndex; *i++ = LastIndex+1; *i++ = LastIndex+2;
        *i++ = LastIndex; *i++ = LastIndex+2; *i++ = LastIndex+3;
        
        LastIndex += 4;
    }
    
    glGenVertexArrays(1, &Result->VAO);
    glBindVertexArray(Result->VAO);

    u32 _tmp;
    glGenBuffers(1, &_tmp);
    glBindBuffer(GL_ARRAY_BUFFER_ARB, _tmp);
    glBufferData(GL_ARRAY_BUFFER_ARB, VertexCount * sizeof(v3), vdata, GL_STATIC_DRAW_ARB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &_tmp);
    glBindBuffer(GL_ARRAY_BUFFER_ARB, _tmp);
    glBufferData(GL_ARRAY_BUFFER_ARB, VertexCount * sizeof(v2), texdata, GL_STATIC_DRAW_ARB);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &_tmp);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, _tmp);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, IndexCount * sizeof(u16), idata, GL_STATIC_DRAW_ARB);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    glBindVertexArray(0);

    AGL_FREE(vdata);
    AGL_FREE(texdata);
    AGL_FREE(idata);

    Result->IndexCount = IndexCount;
    return Result;
}

agl_shader_text *
aglInitFontShader()
{
    agl_shader_text *Shader = (agl_shader_text *) AGL_MALLOC( sizeof(agl_shader_text) );
    const char * VertexShader = GLSL
        (
            in vec4 position;
            in vec2 texCoord0;

            uniform bool render2D = false;
            uniform vec4 renderInfo;
            
            uniform mat4 worldMatrix;
            uniform mat4 viewProjMatrix;
            uniform vec4 fontColor = vec4(1.0, 1.0, 1.0, 1.0);
        
            out vec2 uv0;
            out vec4 color;
        
            void main()
            {
                if(render2D)
                {
                    vec2 halfdim = vec2(renderInfo.x, renderInfo.y)/2;
                    vec2 pos = vec2(position) - halfdim;
                    pos.x += renderInfo.z;
                    pos.y += renderInfo.w;
                    pos /= halfdim;
                    gl_Position = vec4(pos.x,pos.y,0,1);
                }
                else
                    gl_Position = viewProjMatrix * worldMatrix * position;
                
                uv0 = texCoord0;
                color = fontColor;
            }
         );

    const char * FragmentShader = GLSL
        (
            uniform sampler2D mainTex;
            in vec2 uv0;
            in vec4 color;
            out vec4 fragColor;
        
            void main()
            {
                vec4 c = texture(mainTex, uv0);
                if(c.r != 0)
                    fragColor = vec4(color.r * c.r, color.g * c.r, color.b * c.r, c.r - color.a);
                else
                    fragColor = vec4(c.r,c.r,c.r,0);
            }
         );

    Shader->Id = glCreateProgram();
    s32 Shaders[] = {
        aglCompileShader(VertexShader, GL_VERTEX_SHADER),
        aglCompileShader(FragmentShader, GL_FRAGMENT_SHADER)
    };
        
    if(!aglLinkProgram(Shader->Id, Shaders, 2)) AGL_ASSERT(false);

    Shader->Uniforms[0] = glGetUniformLocation(Shader->Id, "viewProjMatrix");
    Shader->Uniforms[1] = glGetUniformLocation(Shader->Id, "worldMatrix");
    Shader->Uniforms[2] = glGetUniformLocation(Shader->Id, "mainTex");
    Shader->Uniforms[3] = glGetUniformLocation(Shader->Id, "fontColor");
    Shader->Uniforms[4] = glGetUniformLocation(Shader->Id, "render2D");
    Shader->Uniforms[5] = glGetUniformLocation(Shader->Id, "renderInfo");
    
    glGenBuffers(1, Shader->VBO);
    glBindBuffer(GL_ARRAY_BUFFER_ARB, Shader->VBO[0]);
    glBufferData(GL_ARRAY_BUFFER_ARB, sizeof(v3), 0, GL_DYNAMIC_DRAW_ARB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
    glGenBuffers(1, Shader->VBO + 1);
    glBindBuffer(GL_ARRAY_BUFFER_ARB, Shader->VBO[1]);
    glBufferData(GL_ARRAY_BUFFER_ARB, sizeof(v2), 0, GL_DYNAMIC_DRAW_ARB);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &Shader->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, Shader->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(u16), 0, GL_DYNAMIC_DRAW_ARB);
    return Shader;
}

void
aglRenderText3D(agl_font *Font, char *Text, r32 Scale = 1.0f,
                r32 X = 0.f, r32 Y = 0.f, r32 Z = 0.f,
                v3 Color = V3(1.0f, 1.0f, 1.0f), r32 Alpha = 0.f)
{
    if(!FontRenderingShader) FontRenderingShader = aglInitFontShader();

    s32 TextLength = strlen(Text);
    s32 VertexCount = 4 * TextLength;
    s32 IndexCount = 6 * TextLength;
    
    v3 *vdata      = (v3 *)  AGL_MALLOC(sizeof(v3) * VertexCount);
    v2 *texdata    = (v2 *)  AGL_MALLOC(sizeof(v2) * VertexCount);
    u16 *idata     = (u16 *) AGL_MALLOC(sizeof(u16) * IndexCount);

    r32 OffsetX = 0, OffsetY = 0;
    s32 LastIndex = 0;

    v3 *v = vdata; v2 *t = texdata; u16 *i = idata;
    for(char *c = Text; *c; c++)
    {
        stbtt_aligned_quad Quad;
        stbtt_GetPackedQuad(Font->CharacterInfo, Font->Width, Font->Height,
                            *c - AGL_TRUETYPE_DEFAULT_FIRST_CHARACTER,
                            &OffsetX, &OffsetY, &Quad, 1);

        *v++ = {Quad.x0, -Quad.y1, 0}; *v++ = {Quad.x0, -Quad.y0, 0};
        *v++ = {Quad.x1, -Quad.y0, 0}; *v++ = {Quad.x1, -Quad.y1, 0};

        *t++ = {Quad.s0, Quad.t1}; *t++ = {Quad.s0, Quad.t0};
        *t++ = {Quad.s1, Quad.t0}; *t++ = {Quad.s1, Quad.t1};

        *i++ = LastIndex; *i++ = LastIndex+1; *i++ = LastIndex+2;
        *i++ = LastIndex; *i++ = LastIndex+2; *i++ = LastIndex+3;
        
        LastIndex += 4;
    }

    glUseProgram(FontRenderingShader->Id);
    
    mat4x4 World = ScaleMatrix((1.0f / Font->Width) * Scale, (1.0f / Font->Height) * Scale, Z);
    World = MultMat4x4(World, TranslationMatrix(X, Y, Z));
    mat4x4 ViewProj = MultMat4x4(CurrentViewMatrix, CurrentProjectionMatrix);

    glUniformMatrix4fv(FontRenderingShader->Uniforms[0], 1, GL_FALSE, (const float *) ViewProj.E);
    glUniformMatrix4fv(FontRenderingShader->Uniforms[1], 1, GL_FALSE, (const float *) World.E);
    glBindTexture(GL_TEXTURE_2D, Font->Texture);
    glUniform1i(FontRenderingShader->Uniforms[2], 0);
    glUniform4f(FontRenderingShader->Uniforms[3], Color.r, Color.g, Color.b, Alpha);
    glUniform1i(FontRenderingShader->Uniforms[4], 0);
    
    glBindBuffer(GL_ARRAY_BUFFER_ARB, FontRenderingShader->VBO[0]);
    glBufferData(GL_ARRAY_BUFFER_ARB, VertexCount * sizeof(v3), 0, GL_DYNAMIC_DRAW_ARB);
    glBufferSubData(GL_ARRAY_BUFFER_ARB, 0, VertexCount * sizeof(v3), vdata);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER_ARB, FontRenderingShader->VBO[1]);
    glBufferData(GL_ARRAY_BUFFER_ARB, VertexCount * sizeof(v2), 0, GL_DYNAMIC_DRAW_ARB);
    glBufferSubData(GL_ARRAY_BUFFER_ARB, 0, VertexCount * sizeof(v2), texdata);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, FontRenderingShader->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, IndexCount * sizeof(u16), 0, GL_DYNAMIC_DRAW_ARB);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER_ARB, 0, IndexCount * sizeof(u16), idata);
    
    glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    AGL_FREE(vdata);
    AGL_FREE(texdata);
    AGL_FREE(idata);    
}

void
aglRenderText2D(agl_font *Font, char *Text,
                r32 X = 0.f, r32 Y = 0.f,
                v3 Color = V3(1.0f, 1.0f, 1.0f), r32 Alpha = 0.f)
{
    if(!FontRenderingShader) FontRenderingShader = aglInitFontShader();

    s32 TextLength = strlen(Text);
    s32 VertexCount = 4 * TextLength;
    s32 IndexCount = 6 * TextLength;
    
    v3 *vdata      = (v3 *)  AGL_MALLOC(sizeof(v3) * VertexCount);
    v2 *texdata    = (v2 *)  AGL_MALLOC(sizeof(v2) * VertexCount);
    u16 *idata     = (u16 *) AGL_MALLOC(sizeof(u16) * IndexCount);

    r32 OffsetX = 0, OffsetY = 0;
    s32 LastIndex = 0;

    v3 *v = vdata; v2 *t = texdata; u16 *i = idata;
    for(char *c = Text; *c; c++)
    {
        stbtt_aligned_quad Quad;
        if(*c == '\n') {
            *c = ' ';

        }
        stbtt_GetPackedQuad(Font->CharacterInfo, Font->Width, Font->Height,
                            *c - AGL_TRUETYPE_DEFAULT_FIRST_CHARACTER,
                            &OffsetX, &OffsetY, &Quad, 1);

        *v++ = {Quad.x0, -Quad.y1, 0 }; *v++ = {Quad.x0, -Quad.y0, 0};
        *v++ = {Quad.x1,  -Quad.y0 , 0}; *v++ = {Quad.x1, -Quad.y1, 0};

        *t++ = {Quad.s0, Quad.t1}; *t++ = {Quad.s0, Quad.t0};
        *t++ = {Quad.s1, Quad.t0}; *t++ = {Quad.s1, Quad.t1};

        *i++ = LastIndex; *i++ = LastIndex+1; *i++ = LastIndex+2;
        *i++ = LastIndex; *i++ = LastIndex+2; *i++ = LastIndex+3;
        
        LastIndex += 4;
    }

    glUseProgram(FontRenderingShader->Id);

    glBindTexture(GL_TEXTURE_2D, Font->Texture);
    glUniform1i(FontRenderingShader->Uniforms[2], 0);
    glUniform1i(FontRenderingShader->Uniforms[4], 1);
    glUniform4f(FontRenderingShader->Uniforms[5], __agl_Context.Width, __agl_Context.Height, X+1, Y-1);
    
    glBindBuffer(GL_ARRAY_BUFFER_ARB, FontRenderingShader->VBO[0]);
    glBufferData(GL_ARRAY_BUFFER_ARB, VertexCount * sizeof(v3), 0, GL_DYNAMIC_DRAW_ARB);
    glBufferSubData(GL_ARRAY_BUFFER_ARB, 0, VertexCount * sizeof(v3), vdata);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER_ARB, FontRenderingShader->VBO[1]);
    glBufferData(GL_ARRAY_BUFFER_ARB, VertexCount * sizeof(v2), 0, GL_DYNAMIC_DRAW_ARB);
    glBufferSubData(GL_ARRAY_BUFFER_ARB, 0, VertexCount * sizeof(v2), texdata);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, FontRenderingShader->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, IndexCount * sizeof(u16), 0, GL_DYNAMIC_DRAW_ARB);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER_ARB, 0, IndexCount * sizeof(u16), idata);

    glUniform4f(FontRenderingShader->Uniforms[3], (u8) (255 - (u8)Color.r), (u8) (255 - (u8)Color.g), (u8) (255 - (u8)Color.b), Alpha);
    glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_SHORT, 0);
    
    glUniform4f(FontRenderingShader->Uniforms[5], __agl_Context.Width, __agl_Context.Height, X, Y);
    glUniform4f(FontRenderingShader->Uniforms[3], Color.r, Color.g, Color.b, Alpha);    
    glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_SHORT, 0);
    
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    AGL_FREE(vdata);
    AGL_FREE(texdata);
    AGL_FREE(idata);    
}

#endif

#define AGL_CORE3D_H
#endif
