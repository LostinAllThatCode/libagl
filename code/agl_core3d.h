#if !defined(AGL_CORE3D_H)

#ifndef AGL_TYPES
    #include <stdio.h>
    #include <stdint.h>
    #include <stddef.h>
    #include <limits.h>
    #include <float.h>
    
    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;
    typedef int32 bool32;
    
    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;
    
    typedef intptr_t intptr;
    typedef uintptr_t uintptr;
    
    typedef size_t memory_index;
        
    typedef float real32;
    typedef double real64;
        
    typedef int8 s8;
    typedef int8 s08;
    typedef int16 s16;
    typedef int32 s32;
    typedef int64 s64;
    typedef bool32 b32;
    
    typedef uint8 u8;
    typedef uint8 u08;
    typedef uint16 u16;
    typedef uint32 u32;
    typedef uint64 u64;
    
    typedef real32 r32;
    typedef real64 r64;
    #define AGL_TYPES
#endif

#ifndef AGL_MATH_H
    #include "agl_math.h"
#endif

#ifndef aglMalloc
    #define aglMalloc(Size) malloc(Size);
    #define aglFree(Size) free(Size);
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
    enum
    {
        AGL_CAMERA_STATIC      = 0,
        AGL_CAMERA_FREE        = 1,
        AGL_CAMERA_FIRSTPERSON = 2,
    };
    
    typedef struct
    {
        u32 Mode;
        v3 Position, Front, Right, Up;
        r32 Speed, Sensitivity, Yaw, Pitch, FoV;
    } agl_camera;

    typedef struct 
    {
        u32 VAO;
        u32 VBO[8];
        u32 VertexCount;
        u32 TriangleCount;
        v3  *Vertices;
        v2  *TextureCoords;
        v3  *Normals;
    } agl_mesh;

    typedef struct
    {
        v3   Ambient, Diffuse, Specular;
        r32  Shininess;
        u32  Texture;
        char Name[64];
    } agl_material;

    typedef struct
    {
        u32          GLRenderMode; // GL_TRIANGLES, GL_QUADS, GL_POINTS, ...
        agl_mesh     Mesh;
        agl_material Material;
    } agl_drawable;

    #include "agl_shaders.h"
    typedef struct
    {
        b32 IsStandard;
        u32 Id;
        u32 Camera;
        u32 Matrix[4];
        u32 Material[4];
        u32 Light[4];
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

mat4x4 CurrentProjectionMatrix, CurrentViewMatrix;
agl_camera *ActiveCamera;

extern agl_shader *
aglInitDefaultShader()
{
    s32 Shaders[] = {
        aglCompileShader(AGL_SHADERS_VERT_4, GL_VERTEX_SHADER),
        aglCompileShader(AGL_SHADERS_FRAG_3, GL_FRAGMENT_SHADER)
    };
    agl_shader *Result = (agl_shader *) aglMalloc( sizeof(agl_shader) );
    Result->Id = glCreateProgram();
    Result->IsStandard = true;
    if(aglLinkProgram(Result->Id, Shaders, 2))
    {
        Result->Matrix[0] = glGetUniformLocation(Result->Id, "matrixModelViewProj");
        Result->Matrix[1] = glGetUniformLocation(Result->Id, "matrixModel");
        Result->Matrix[2] = glGetUniformLocation(Result->Id, "matrixView");
        Result->Matrix[3] = glGetUniformLocation(Result->Id, "matrixInverse");

        Result->Material[0] = glGetUniformLocation(Result->Id, "material.ambient");
        Result->Material[1] = glGetUniformLocation(Result->Id, "material.diffuse");
        Result->Material[2] = glGetUniformLocation(Result->Id, "material.specular");
        Result->Material[3] = glGetUniformLocation(Result->Id, "material.shininess");
        
        Result->Light[0] = glGetUniformLocation(Result->Id, "light.position");
        Result->Light[1] = glGetUniformLocation(Result->Id, "light.ambient");
        Result->Light[2] = glGetUniformLocation(Result->Id, "light.diffuse");
        Result->Light[3] = glGetUniformLocation(Result->Id, "light.specular");

        Result->Camera = glGetUniformLocation(Result->Id, "viewPos");
    } else aglFree(Result);
    return Result;
}

extern void
aglCameraInit(agl_camera *Camera, v3 Position = V3(0, 0, 0),
              r32 FoV = 45.0f, r32 Speed = 9.0f, r32 Sensitivity = 0.0025f,
              r32 Yaw = M_PI, r32 Pitch = 0.0f, u32 Mode = 0)
{
    Camera->Mode = Mode;
    Camera->Position = Position;  
    Camera->Speed = Speed;
    Camera->Sensitivity = Sensitivity;
    Camera->Yaw = Yaw;
    Camera->Pitch = Pitch;
    Camera->FoV = FoV;
    Camera->Front = V3(cos(Camera->Pitch) * sin(Camera->Yaw), sin(Camera->Pitch), cos(Camera->Pitch) * cos(Camera->Yaw));
    Camera->Right = V3(sin(Camera->Yaw - M_PI/2.0f), 0, cos(Camera->Yaw - M_PI/2.0f));
    Camera->Up = CrossV3(Camera->Right, Camera->Front);
}

inline void
aglCameraUpdate(agl_camera *Camera, agl_context *Context)
{
    r32 Speed = Context->Delta * Camera->Speed;
    switch(Camera->Mode)
    {
        case AGL_CAMERA_FREE:
        {
            if(aglKeyDown(Context, 'W')) Camera->Position += Camera->Front * Speed;
            if(aglKeyDown(Context, 'S')) Camera->Position -= Camera->Front * Speed;
            if(aglKeyDown(Context, 'D')) Camera->Position += Camera->Right * Speed;
            if(aglKeyDown(Context, 'A')) Camera->Position -= Camera->Right * Speed;
            if(Context->MouseIO.Left)
            {
                Camera->Yaw -= Context->MouseIO.dX * Camera->Sensitivity;
                Camera->Pitch -= Context->MouseIO.dY * Camera->Sensitivity;
                Camera->Front = V3(cos(Camera->Pitch) * sin(Camera->Yaw), sin(Camera->Pitch), cos(Camera->Pitch) * cos(Camera->Yaw));
                Camera->Right = V3(sin(Camera->Yaw - M_PI/2.0f), 0, cos(Camera->Yaw - M_PI/2.0f));
                Camera->Up = CrossV3(Camera->Right, Camera->Front);
            }

        }break;
        case AGL_CAMERA_STATIC:
        {
            // Nothing to update
        }break;
        default:
        {
            aglAssert(!"Not implemented yet");
        }
    }
}

inline agl_material
aglMaterialDefault(v3 Ambient = V3(1.0f, 0.5f, 0.31f),
                   v3 Diffuse = V3(1.0f, 0.5f, 0.31f),
                   v3 Specular = V3(0.5f, 0.5f, 0.5f),
                   r32 Shininess = 32.0f)
{
    agl_material Material = {};
    Material.Ambient  = Ambient;
    Material.Specular = Specular;
    Material.Diffuse  = Diffuse;
    Material.Shininess = Shininess;
    Material.Texture = 0;
    sprintf(Material.Name, "DefaultMaterial");
    return Material;
}

extern void
aglMaterialDelete(agl_material *Material)
{
    aglFree(Material);
}

inline mat4x4
aglCameraView(agl_camera *Camera)
{
    mat4x4 Result;
    switch(Camera->Mode)
    {
        case AGL_CAMERA_FREE:
        {
            Result = LookAtMatrix(Camera->Position, Camera->Position + Camera->Front, Camera->Up);
        }break;
        case AGL_CAMERA_STATIC:
        {
            Result = LookAtMatrix(Camera->Position, Camera->Front, Camera->Up);
        }break;
        default:
        {
            aglAssert(!"Not implemented yet");
        }break;
    }
    return Result;
}

extern void
aglGenBuffer(agl_mesh *Mesh, b32 Dynamic = false)
{
    if(Mesh)
    {
        u32 Mode = (Dynamic ? GL_STREAM_DRAW : GL_STATIC_DRAW);
        if(Mesh->VertexCount > 0)
        {
            if(Mesh->Vertices)
            {
                glGenBuffers(1, Mesh->VBO);
                glBindBuffer(GL_ARRAY_BUFFER, Mesh->VBO[0]);
                glBufferData(GL_ARRAY_BUFFER, Mesh->VertexCount * sizeof(v3), Mesh->Vertices, Mode);
            }
            
            if(Mesh->TextureCoords)
            {
                glGenBuffers(1, Mesh->VBO + 1);
                glBindBuffer(GL_ARRAY_BUFFER, Mesh->VBO[1]);
                glBufferData(GL_ARRAY_BUFFER, Mesh->VertexCount * sizeof(v2), Mesh->TextureCoords, Mode);
            }
            
            if(Mesh->Normals)
            {
                glGenBuffers(1, Mesh->VBO + 2);
                glBindBuffer(GL_ARRAY_BUFFER, Mesh->VBO[2]);
                glBufferData(GL_ARRAY_BUFFER, Mesh->VertexCount * sizeof(v3), Mesh->Normals, Mode);
            }
        }
    }
}

extern void
aglMeshDelete(agl_mesh *Mesh)
{
    if(Mesh->Vertices)
    {
        glDeleteBuffers(1, Mesh->VBO);
        aglFree(Mesh->Vertices);
    }
    if(Mesh->TextureCoords)
    {
        glDeleteBuffers(1, Mesh->VBO + 1);
        aglFree(Mesh->TextureCoords);
    }
    if(Mesh->Normals)
    {
        glDeleteBuffers(1, Mesh->VBO + 2);
        aglFree(Mesh->Normals);
    }
}


extern void
aglGenQuad(agl_mesh *Mesh, u32 Size)
{
    u32 VertexCount = 36;
    u32 TriangleCount = VertexCount / 3;
    r32 XZ = (r32) Size / 2.0f;
    r32 Y = Size;
   
    Mesh->VertexCount = VertexCount;
    Mesh->TriangleCount = TriangleCount;
    Mesh->Vertices = (v3 *) aglMalloc( sizeof(v3) * VertexCount );
    Mesh->Normals = (v3 *) aglMalloc( sizeof(v3) * VertexCount );

    v3 *v = Mesh->Vertices, *n = Mesh->Normals;
    *v++ = {-XZ, 0.0f, XZ}; *v++ = {XZ, 0.0f, XZ}; *v++ = {XZ, Y, XZ};
    *v++ = {-XZ, 0.0f, XZ}; *v++ = {XZ, Y,    XZ}; *v++ = {-XZ, Y, XZ};
    for(s32 i=0; i<6; i++) *n++ = {0,0,1};
    *v++ = {-XZ, 0.0f, -XZ}; *v++ = {XZ, 0.0f, -XZ}; *v++ = {XZ, Y, -XZ};
    *v++ = {-XZ, 0.0f, -XZ}; *v++ = {XZ, Y,    -XZ}; *v++ = {-XZ, Y, -XZ};
    for(s32 i=0; i<6; i++) *n++ = {0,0,-1};
    *v++ = {-XZ, 0.0f, XZ}; *v++ = {XZ, 0.0f, XZ}; *v++ = {XZ, 0, -XZ};
    *v++ = {-XZ, 0.0f, XZ}; *v++ = {XZ, 0.0f, -XZ}; *v++ = {-XZ, 0, -XZ};
    for(s32 i=0; i<6; i++) *n++ = {0,-1,0};
    *v++ = {-XZ, Y, XZ}; *v++ = {XZ, Y, XZ}; *v++ = {XZ, Y, -XZ};
    *v++ = {-XZ, Y, XZ}; *v++ = {XZ, Y, -XZ}; *v++ = {-XZ, Y, -XZ};
    for(s32 i=0; i<6; i++) *n++ = {0,1,0};
    *v++ = {-XZ, 0, XZ}; *v++ = {-XZ, Y, XZ}; *v++ = {-XZ, Y, -XZ};
    *v++ = {-XZ, 0, XZ}; *v++ = {-XZ, 0, -XZ}; *v++ = {-XZ, Y, -XZ};
    for(s32 i=0; i<6; i++) *n++ = {-1,0,0};
    *v++ = {XZ, 0, XZ}; *v++ = {XZ, Y, XZ}; *v++ = {XZ, Y, -XZ};
    *v++ = {XZ, 0, XZ}; *v++ = {XZ, 0, -XZ}; *v++ = {XZ, Y, -XZ};
    for(s32 i=0; i<6; i++) *n++ = {1,0,0};
}

inline void
aglBeginScene3D(agl_context *Context, agl_camera *Camera)
{
    if(Camera) aglCameraUpdate(Camera, Context);
    glViewport(0, 0, Context->Width, Context->Height);

    CurrentProjectionMatrix = PerspectiveMatrix(Camera->FoV, (r32)Context->Width / (r32)Context->Height, .1f, 100.0f);
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
aglDraw(agl_drawable *Drawable, agl_shader *Shader, mat4x4 ModelMatrix = IdentityMat4x4())
{
    if(Drawable)
    {
        if(Shader) glUseProgram(Shader->Id);
        if(Drawable->Mesh.Vertices)
        {
            glBindBuffer(GL_ARRAY_BUFFER, Drawable->Mesh.VBO[0]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*Drawable->Mesh.Vertices), 0);
            glEnableVertexAttribArray(0);
        }
        if(Drawable->Mesh.TextureCoords)
        {
            glBindBuffer(GL_ARRAY_BUFFER, Drawable->Mesh.VBO[1]);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(*Drawable->Mesh.TextureCoords), 0);
            glEnableVertexAttribArray(1);
        }
        if(Drawable->Mesh.Normals)
        {
            glBindBuffer(GL_ARRAY_BUFFER, Drawable->Mesh.VBO[2]);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(*Drawable->Mesh.Normals), 0);
            glEnableVertexAttribArray(2);
        }
        
        mat4x4 Result = MultMat4x4(ModelMatrix, CurrentViewMatrix);
        Result = MultMat4x4(Result, CurrentProjectionMatrix);

        glUniformMatrix4f(Shader->Matrix[0], 1, GL_FALSE, (const float *) Result.E);
        glUniformMatrix4f(Shader->Matrix[1], 1, GL_FALSE, (const float *) ModelMatrix.E);
        glUniformMatrix4f(Shader->Matrix[2], 1, GL_FALSE, (const float *) CurrentViewMatrix.E);

        glUniform3f(Shader->Material[0], Drawable->Material.Ambient.r, Drawable->Material.Ambient.g, Drawable->Material.Ambient.b);
        glUniform3f(Shader->Material[1], Drawable->Material.Diffuse.r, Drawable->Material.Diffuse.g, Drawable->Material.Diffuse.b);
        glUniform3f(Shader->Material[2], Drawable->Material.Specular.r, Drawable->Material.Specular.g, Drawable->Material.Specular.b);
        glUniform1f(Shader->Material[3], Drawable->Material.Shininess);

        glUniform3f(Shader->Camera, ActiveCamera->Position.x, ActiveCamera->Position.y, ActiveCamera->Position.z);

        glUniform3f(Shader->Light[0], 5.0f, 5+sinf(time/2)*5.0f, -5.0f);
        glUniform3f(Shader->Light[1], 0.2f, 0.2f, 0.2f);
        glUniform3f(Shader->Light[2], 0.5f, 0.5f, 0.5f);
        glUniform3f(Shader->Light[3], 1.0f, 1.0f, 1.0f);
        
        glDrawArrays(Drawable->GLRenderMode, 0, Drawable->Mesh.VertexCount); 

        // Cleanup
        if(Drawable->Mesh.Vertices)      glDisableVertexAttribArray(0);
        if(Drawable->Mesh.TextureCoords) glDisableVertexAttribArray(1);
        if(Drawable->Mesh.Normals)       glDisableVertexAttribArray(2);
    }
}



#define AGL_CORE3D_H
#endif
