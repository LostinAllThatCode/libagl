#if !defined(AGL_CORE3D_H)

#if !defined(AGL_H) || !defined(AGL_MATH_H)
#error "agl.h or agl_math.h missing. Please include them before including this file!"
#endif

#if !defined(AGL_MALLOC) || !defined(AGL_FREE)
    #include <stdlib.h>
    #define AGL_MALLOC(x,u) calloc(1,x)
    #define AGL_FREE(x,u)   free(x)
#endif

typedef struct
{
    s32 Mode;
    v3  Position, Front, Right, Up;
    r32 Speed, Sensitivity, Yaw, Pitch, FoV;
    v3 *Target;
} agl_camera3D;

mat4x4       __agl_proj, __agl_view;
agl_camera3D __agl_cam;

void
aglSetViewport3D(r32 FoV, s32 Width, s32 Height, r32 Near, r32 Far)
{
    static b32       CameraInitialized;
    agl_camera3D     *Camera    = &__agl_cam;
        
    glViewport(0, 0, Width, Height);

    if(!CameraInitialized)
    {
        Camera->Up          = V3i(0,1,0);
        Camera->Position    = V3i(0, 10, -15);
        Camera->Speed       = 10.f;
        Camera->Sensitivity = .002f;
        Camera->Yaw         = M_PI / 2;
        Camera->Pitch       = -M_PI / 4;
        Camera->FoV         = FoV;

        CameraInitialized = true;
    }

    r32 Speed = __ctx.Delta * Camera->Speed;
    switch(Camera->Mode)
    {
        case 0:
        {
            if(aglKeyDown('W')) Camera->Position += Camera->Front * Speed;
            if(aglKeyDown('S')) Camera->Position -= Camera->Front * Speed;
            if(aglKeyDown('D')) Camera->Position += Camera->Right * Speed;
            if(aglKeyDown('A')) Camera->Position -= Camera->Right * Speed;

            if(aglMouseDown(AGL_MOUSE_LEFT))
            {
                aglCaptureMouse(true);
                aglShowCursor(false);
                Camera->Yaw   += aglGetMouseDeltaX() * Camera->Sensitivity;
                Camera->Pitch -= aglGetMouseDeltaY() * Camera->Sensitivity;
            } else {
                aglCaptureMouse(false);
                aglShowCursor(true);
            }
        }
    }
     
    switch(Camera->Mode)
    {
        case 0:
        {
            Camera->Front = V3(cosf(Camera->Pitch) * cosf(Camera->Yaw), sinf(Camera->Pitch), cosf(Camera->Pitch) * sinf(Camera->Yaw));
            Camera->Front = aglmNormalizeV3(Camera->Front);
                
            Camera->Right = V3(cosf(Camera->Yaw + M_PI/2.0f), 0, sinf(Camera->Yaw + M_PI/2.0f));
            Camera->Right = aglmNormalizeV3(Camera->Right);   
        }break;
        default:
        {
            AGL_ASSERT(!"Not implemented yet");
        }break;
    }

    __agl_proj = aglmPerspective(Camera->FoV, (r32)Width / (r32)Height, .1f, 1000.0f);
    __agl_view = aglmLookAt(Camera->Position, Camera->Position + Camera->Front, Camera->Up);
}

#ifdef AGL_PREDEFINED_SHADERS

static u32 __aglsl_skybox, __aglsl_skybox_vao, __aglsl_skybox_tex;
static u32 __aglsl_shadowmap, __aglsl_shadowmap_fbo, __aglsl_shadowmap_tex;
static v2  __aglsl_shadowmap_size = V2i(4096, 4096);

void
aglInitPredefinedShaders()
{
    float __aglsl_skybox_verts[] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
    };
    const char __aglsl_skybox_vs[] = {
        "#version 330\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "out vec3 _texcoord;\n"
        "void main() {\n"
        "    vec4 pos = projection * view * vec4(position, 1.0);\n"
        "    gl_Position = pos.xyww;\n"
        "    _texcoord = position;\n"
        "}"
    };
    const char __aglsl_skybox_fs[] = {
        "#version 330\n"
        "in vec3 _texcoord;\n"
        "uniform samplerCube skybox;\n"
        "out vec4 _color;\n"
        "void main() {"    
        "    _color = texture(skybox, _texcoord);\n"
        "}"
    };
    const char __aglsl_shadowmap_vs[] = {
        "#version 330\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 lightview;\n"
        "uniform mat4 model;\n"
        "void main(){\n"
        "    gl_Position = lightview * model * vec4(position, 1.0f);\n"
        "}"   
    };
    const char __aglsl_shadowmap_fs[] = {
        "#version 330\n"
        "void main() {}\n"
    };
    const char __aglsl_fonts_vs[] = {
        "#version 330\n"
        "layout (location = 0) in vec4 combined;\n"
        "out vec2 _texcoord;\n"
        "void main() {\n"
        "    gl_Position = vec4(combined.xy, 0, 1);\n"
        "    _texcoord = combined.zw;\n"
        "};"
    };
    const char __aglsl_fonts_fs[] = {
        "#version 330\n"
        "in vec2 _texcoord;\n"
        "uniform sampler2dD texture;\n"
        "uniform vec4 color;\n"
        "void main() {\n"
        "    gl_FragColor = vec4(1, 1, 1, texture2D(texture, _texcoord).r) * color;\n"
        "}\n"
    };
    
    // Skybox shader init
    u32 _ign;
    static b32 __Initialized = false;
    if(!__Initialized)
    {
        __aglsl_skybox = glCreateProgram();
        aglShaderCompileAndAttach(__aglsl_skybox, __aglsl_skybox_vs, GL_VERTEX_SHADER);
        aglShaderCompileAndAttach(__aglsl_skybox, __aglsl_skybox_fs, GL_FRAGMENT_SHADER);
        aglShaderLink(__aglsl_skybox);
    
        glGenTextures(1, &__aglsl_skybox_tex);
        glGenVertexArrays(1, &__aglsl_skybox_vao);
        glBindVertexArray(__aglsl_skybox_vao);
        glGenBuffers(1, &_ign);
        glBindBuffer(GL_ARRAY_BUFFER, _ign);
        glBufferData(GL_ARRAY_BUFFER, sizeof(__aglsl_skybox_verts), __aglsl_skybox_verts, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexArray(0);

        // Shadowmap shader init
        __aglsl_shadowmap = glCreateProgram();
        aglShaderCompileAndAttach(__aglsl_shadowmap, __aglsl_shadowmap_vs, GL_VERTEX_SHADER);
        aglShaderCompileAndAttach(__aglsl_shadowmap, __aglsl_shadowmap_fs, GL_FRAGMENT_SHADER);
        aglShaderLink(__aglsl_shadowmap);    

        glGenTextures(1, &__aglsl_shadowmap_tex);
        glBindTexture(GL_TEXTURE_2D, __aglsl_shadowmap_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, __aglsl_shadowmap_size.w, __aglsl_shadowmap_size.h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
        glGenFramebuffers(1, &__aglsl_shadowmap_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, __aglsl_shadowmap_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, __aglsl_shadowmap_tex, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        __Initialized = true;
    }
}

void
aglDeInitPredefinedShaders()
{
    glDeleteProgram(__aglsl_skybox);
    glDeleteTextures(1, &__aglsl_skybox_tex);
    glDeleteVertexArrays(1, &__aglsl_skybox_vao);

    glDeleteProgram(__aglsl_shadowmap);
    glDeleteTextures(1, &__aglsl_shadowmap_tex);
    glDeleteFramebuffers(1, &__aglsl_shadowmap_fbo);
}

void
aglSkyboxTextures(char *Textures[])
{
    glActiveTexture(GL_TEXTURE0);

    s32 Width, Height, Components;
    u8 *image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, __aglsl_skybox_tex);
    for( s32 i=0; i < 6; i++)
    {
        image = stbi_load(Textures[i], &Width, &Height, &Components, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, Width, Height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, image);
        
        stbi_image_free(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void aglSkyboxTextures(char *Front, char *Back, char *Up, char *Down, char *Right, char *Left)
{
    char *CubemapTextures[] = { Front, Back, Up, Down , Right, Left };
    aglSkyboxTextures(CubemapTextures);
}

void aglSkyboxRender(mat4x4 Projection = __agl_proj, mat4x4 View = __agl_view)
{
    aglShaderUse(__aglsl_skybox);
    
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
    
    glBindVertexArray(__aglsl_skybox_vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, __aglsl_skybox_tex);            

    mat4x4 p = Projection;
    mat4x4 v = View;

    v.m3 = 0.f; v.m7 = 0.f; v.m11 = 0.f; v.m12 = 0.f; v.m13 = 0.f; v.m14 = 0.f;
    
    aglShaderSetUniformMat4fv("projection", p.E);
    aglShaderSetUniformMat4fv("view", v.E);
    aglShaderSetUniform1i("mainTex", 0);
            
    glDrawArrays(GL_TRIANGLES, 0, 36);            

    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

void aglShadowMapSize(s32 Width, s32 Height)
{
    __aglsl_shadowmap_size = V2i(Width, Height);
    if(__aglsl_shadowmap_tex != 0)
    {
        glBindTexture(GL_TEXTURE_2D, __aglsl_shadowmap_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, __aglsl_shadowmap_size.w, __aglsl_shadowmap_size.h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    }
}

void aglShadowStageBegin(mat4x4 LightView)
{
    aglShaderUse(__aglsl_shadowmap);
    
    glViewport(0, 0, __aglsl_shadowmap_size.w, __aglsl_shadowmap_size.h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    aglShaderSetUniformMat4fv("lightview", LightView.E);

    glBindFramebuffer(GL_FRAMEBUFFER, __aglsl_shadowmap_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void aglShadowStageEnd()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

u32 aglShadowMap()
{
    return __aglsl_shadowmap_tex;
}
#endif

#if 1

#if defined(STB_TRUETYPE_IMPLEMENTATION)
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
} agl_font_stbttf;
#endif

#define AGL_CORE3D_OBJECTS_MAX 1024

#ifdef __cplusplus
extern "C" {
#endif

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
        u32 IBO;
        u32 VBO[4];
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
        s32          ID;
        agl_mesh     Mesh;
        agl_material Material;
        s32          GLRenderMode; // GL_TRIANGLES, GL_QUADS, GL_POINTS, ...
    } agl_drawable;
    agl_drawable *__agl_Objects[AGL_CORE3D_OBJECTS_MAX];
    s32 __agl_ObjectCounter;

   #define GLSL(core, source) "#version "#core"\n" #source
    #include "agl_shaders.h"
    enum { AGL_SHADER_TYPE_CUSTOM = 0,
           AGL_SHADER_TYPE_SHADOWMAP = 1,
           AGL_SHADER_TYPE_LIGHTING = 2,
           AGL_SHADER_TYPE_FONTS = 4,
    };
    typedef struct
    {
        u32 Type;
        u32 Program;
        u32 Bindings[32];
        s32 BindingCount;
    } agl_shader_ext;
    agl_shader_ext *FontRenderingShader;
    
    typedef struct
    {
        b32 Success;
        b32 IsDefault;
        s32 Id;
        s32 Camera;
        s32 Matrix[4];
        s32 Material[4];
        s32 Light[4];
        s32 ShadowMap;
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

    typedef struct
    {
        s32 VAO;

        agl_material Material[12];
        
    } agl_object_3d;
    
#ifdef __cplusplus
}
#endif

r32 LastUpdateTime;
s32 MaterialIndexCount;
mat4x4 CurrentProjectionMatrix, CurrentViewMatrix;
agl_camera *ActiveCamera;

inline agl_shader
aglInitDefaultShader()
{
    agl_shader Result = {}; 
    
    s32 Results[] = {
        aglShaderCompile(AGL_SHADERS_VERT_4, GL_VERTEX_SHADER),
        aglShaderCompile(AGL_SHADERS_FRAG_3, GL_FRAGMENT_SHADER)
    };
    Result.Id = glCreateProgram();
    Result.IsDefault = true;
    if(aglShaderLink(Result.Id, Results, 2))
    {
        Result.Matrix[0] = glGetUniformLocation(Result.Id, "matrixModelViewProj");
        Result.Matrix[1] = glGetUniformLocation(Result.Id, "matrixModel");
        Result.Matrix[2] = glGetUniformLocation(Result.Id, "matrixView");
        Result.Matrix[3] = glGetUniformLocation(Result.Id, "matrixSpace");

        Result.Material[0] = glGetUniformLocation(Result.Id, "material.ambient");
        Result.Material[1] = glGetUniformLocation(Result.Id, "material.diffuse");
        Result.Material[2] = glGetUniformLocation(Result.Id, "material.specular");
        Result.Material[3] = glGetUniformLocation(Result.Id, "material.shininess");
        
        Result.Light[0] = glGetUniformLocation(Result.Id, "light.position");
        Result.Light[1] = glGetUniformLocation(Result.Id, "light.ambient");
        Result.Light[2] = glGetUniformLocation(Result.Id, "light.diffuse");
        Result.Light[3] = glGetUniformLocation(Result.Id, "light.specular");

        Result.Camera = glGetUniformLocation(Result.Id, "viewPos");
        Result.ShadowMap = glGetUniformLocation(Result.Id, "shadowMap");
        
        Result.Success = true;
    }
    AGL_ASSERT(Result.Success);
    return Result;
}

static b32
aglShaderSetBinding(agl_shader_ext *Shader, char *Uniform)
{
    b32 Result = true;

    if(!Shader) return false;
    s32 UniformID = glGetUniformLocation(Shader->Program, Uniform);
    if(glGetError() == GL_INVALID_VALUE || glGetError() == GL_INVALID_OPERATION)
        Result = false;
    else        
        Shader->Bindings[Shader->BindingCount++] = UniformID;

    return Result;
}
    
    
extern void
aglCameraInit(agl_camera *Camera, s32 Mode = 0, v3 Position = V3(0, 0, 1),
              r32 FoV = 45.0f, r32 Yaw = -M_PI/2, r32 Pitch = 0.0f,
              r32 Speed = 6.0f, r32 Sensitivity = 0.0025f)
{
    Camera->Mode = Mode;
    Camera->Position = Position;  
    Camera->Speed = Speed;
    Camera->Sensitivity = Sensitivity;
    Camera->Yaw = Yaw;
    Camera->Pitch = Pitch;
    Camera->FoV = FoV;
    Camera->Up    = V3i(0,1,0);
}

inline void
aglCameraUpdate(agl_camera *Camera, agl_context *Context)
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
                aglShowCursor(false);
                Camera->Yaw += Context->Input.MouseXDelta * Camera->Sensitivity;
                Camera->Pitch -= Context->Input.MouseYDelta * Camera->Sensitivity;
            } else aglShowCursor(true);
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
                                
                Camera->Yaw += Context->Input.MouseXDelta * Camera->Sensitivity;
                Camera->Pitch -= Context->Input.MouseYDelta * Camera->Sensitivity;
                    
                if(Camera->Pitch < -((M_PI-0.03f)/2)) Camera->Pitch = -((M_PI-0.03f)/2);
                if(Camera->Pitch > ((M_PI-0.03f)/2))  Camera->Pitch = (M_PI-0.03f)/2;
                                
            } else aglCaptureMouse(false);
            
            v3 Forward = aglmNormalizeV3(V3(Camera->Front.x, 0, Camera->Front.z));
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
}

inline agl_material
aglMaterial(v3 Ambient = V3(1.0f), v3 Diffuse = V3(1.0f), v3 Specular = V3(0.5f), r32 Shininess = 32.0f)
{
    agl_material Result = {};
    Result.Ambient  = Ambient;
    Result.Diffuse  = Diffuse;
    Result.Specular = Specular;
    Result.Shininess = Shininess;
    Result.ID = ++MaterialIndexCount;
    return Result;
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
            Camera->Front = aglmNormalizeV3(Camera->Front);
                
            Camera->Right = V3(cosf(Camera->Yaw + M_PI/2.0f), 0, sinf(Camera->Yaw + M_PI/2.0f));
            Camera->Right = aglmNormalizeV3(Camera->Right);
            
            Result = aglmLookAt(Camera->Position, Camera->Position + Camera->Front, Camera->Up);
        }break;
        case AGL_CAMERA_MODE_STATIC:
        {
            Result = aglmLookAt(Camera->Position, Camera->Front, Camera->Up);
        }break;
        case AGL_CAMERA_MODE_TARGET:
        {
            Result = aglmLookAt(Camera->Position, *Camera->Target, Camera->Up);
        }break;
        case AGL_CAMERA_MODE_FIRSTPERSON:
        {
            Camera->Front = V3(cosf(Camera->Pitch) * cosf(Camera->Yaw), sinf(Camera->Pitch), cosf(Camera->Pitch) * sinf(Camera->Yaw));
            Camera->Front = aglmNormalizeV3(Camera->Front);
                
            Camera->Right = V3(cosf(Camera->Yaw + M_PI/2.0f), 0, sinf(Camera->Yaw + M_PI/2.0f));
            Camera->Right = aglmNormalizeV3(Camera->Right);
            
            Result = aglmLookAt(Camera->Position, Camera->Position + Camera->Front, Camera->Up);
        }break;
        default:
        {
            AGL_ASSERT(!"Not implemented yet");
        }break;
    }
    return Result;
}

static b32
aglGenBuffer(agl_mesh *Mesh, b32 Dynamic = false)
{
    b32 Result = false;
    s32 Errors = 0;
    if(Mesh)
    {
        s32 Mode = (Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        if(Mesh->VertexCount > 0)
        {
            glGenVertexArrays(1, &Mesh->VAO);
            if (glGetError()) Errors++;
            
            glBindVertexArray(Mesh->VAO);
            if (glGetError()) Errors++;
            
            if(Mesh->Vertices)
            {
                glGenBuffers(1, Mesh->VBO);
                if (glGetError()) Errors++;
                glBindBuffer(GL_ARRAY_BUFFER, Mesh->VBO[0]);
                glBufferData(GL_ARRAY_BUFFER, Mesh->VertexCount * sizeof(v3), Mesh->Vertices, Mode);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            }
            
            if(Mesh->TextureCoords)
            {
                glGenBuffers(1, Mesh->VBO + 1);
                if (glGetError()) Errors++;
                glBindBuffer(GL_ARRAY_BUFFER, Mesh->VBO[1]);
                glBufferData(GL_ARRAY_BUFFER, Mesh->VertexCount * sizeof(v2), Mesh->TextureCoords, Mode);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            }
            
            if(Mesh->Normals)
            {
                glGenBuffers(1, Mesh->VBO + 2);
                if (glGetError()) Errors++;
                glBindBuffer(GL_ARRAY_BUFFER, Mesh->VBO[2]);
                glBufferData(GL_ARRAY_BUFFER, Mesh->VertexCount * sizeof(v3), Mesh->Normals, Mode);
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            }
            glBindVertexArray(0);
            
            Result = (!Errors);
        }
    }
    return Result;
}

static void
aglDelete(agl_drawable *Drawable)
{   
    if(Drawable->Mesh.Vertices)       AGL_FREE(Drawable->Mesh.Vertices, 0);
    if(Drawable->Mesh.TextureCoords)  AGL_FREE(Drawable->Mesh.TextureCoords, 0);
    if(Drawable->Mesh.Normals)        AGL_FREE(Drawable->Mesh.Normals, 0);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Drawable->Mesh.VAO);
    AGL_ASSERT(glGetError() != GL_INVALID_VALUE);

    __agl_Objects[Drawable->ID] = 0;
    AGL_FREE(Drawable, 0);
    AGL_ASSERT(Drawable);
}

static void
aglCleanupResources()
{
    printf("ASDASDSD");
    for(s32 i=0; i < __agl_ObjectCounter; i++)
    {
        agl_drawable *o = __agl_Objects[i];
        if(o) aglDelete(o);
    }
};

static b32
aglGenCube(agl_mesh *Mesh, r32 Size)
{    
    s32 VertexCount = 36;
    s32 TriangleCount = VertexCount / 3;
    r32 XZ = Size / 2.0f;
    r32 Y  = Size;

    Mesh->TextureCoords = 0;
    Mesh->VertexCount = VertexCount;
    Mesh->Vertices = (v3 *) AGL_MALLOC(sizeof(v3) * VertexCount, 0);
    Mesh->Normals  = (v3 *) AGL_MALLOC(sizeof(v3) * VertexCount, 0);

    if(Mesh->Vertices && Mesh->Normals)
    {
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
        return true;
    }
    return false;
}

static b32
aglGenGridFlat(agl_mesh *Mesh, r32 Size, r32 GridWidth, r32 GridHeight)
{
    s32 VertexCount = (Size * 4) + 4;
    s32 TriangleCount = 0;

    Mesh->Normals       = 0;
    Mesh->TextureCoords = 0;
    
    Mesh->VertexCount   = VertexCount;
    
    Mesh->Vertices      = (v3 *) AGL_MALLOC(sizeof(v3) * VertexCount, 0);

    if(Mesh->Vertices)
    {
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
        return true;
    }
    return false;
}

static agl_drawable *
aglPrimitiveGrid(r32 Size = 16.0f, r32 GridWidth = 1.0f, r32 GridHeight = 1.0f)
{
    agl_drawable *Result = (agl_drawable *) AGL_MALLOC(sizeof(agl_drawable), 0);
    if(Result)
    {
        Result->GLRenderMode = GL_LINES;
        Result->Material = aglMaterial();
        Result->Material.Specular = V3(0, 0, 0);
        Result->Material.Shininess = 0;
        if(aglGenGridFlat(&Result->Mesh, Size, GridWidth, GridHeight) && aglGenBuffer(&Result->Mesh))
        {
            Result->ID = __agl_ObjectCounter;
            __agl_Objects[__agl_ObjectCounter++] = Result;
        } else aglDelete(Result);
        
    }
    return Result;
}

static agl_drawable *
aglPlane(r32 Width = 1.0f, r32 Height = 1.0f)
{
    
    r32 halfX = Width * .5f;
    r32 halfZ = Height * .5f;
    r32 vData[] = {
        -halfX, 0.f, halfZ, -halfX, 0.f, -halfZ, halfX, 0.f, -halfZ,
        -halfX, 0.f, halfZ,  halfX, 0.f, -halfZ, halfX, 0.f,  halfZ,
    };
    r32 nData[] = {
        0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
        0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
    };
  
    
}


static agl_drawable *
aglPrimitiveCube(r32 Size = 1.0f)
{    
    agl_drawable *Result = (agl_drawable *) AGL_MALLOC(sizeof(agl_drawable), 0);
    if(Result)
    {
        Result->GLRenderMode = GL_TRIANGLES;
        Result->Material = aglMaterial();
        if(aglGenCube(&Result->Mesh, Size) && aglGenBuffer(&Result->Mesh))
        {
            Result->ID = __agl_ObjectCounter;
            __agl_Objects[__agl_ObjectCounter++] = Result;
        } else aglDelete(Result);
    }
    AGL_ASSERT(Result);
    return Result;
}

inline void
aglBeginScene3D(agl_context *Context, agl_camera *Camera)
{
    if(Camera) aglCameraUpdate(Camera, Context);
    
    glViewport(0, 0, Context->Width, Context->Height);

    CurrentProjectionMatrix = aglmPerspective(Camera->FoV, (r32)Context->Width / (r32)Context->Height, .1f, 1000.0f);
    CurrentViewMatrix       = aglCameraView(Camera);

    ActiveCamera = Camera;
}

inline void
aglEndScene3D(agl_context *Context, agl_camera *Camera)
{
    CurrentProjectionMatrix = Mat4(1);
    CurrentViewMatrix       = Mat4(1);
}


inline void
aglDraw(agl_drawable *Drawable, mat4x4 ModelMatrix = Mat4(1), agl_shader *Shader = 0, b32 ShowPoints = false)
{
    if(Drawable)
    {
        if(Shader && Shader->IsDefault)
        {
            //glUseProgram(Shader->Id);

            v3 lightColor = V3(1, 1, 1);
            v3 diffuse = lightColor * .5f;
            v3 ambient = lightColor * .2f;
            v3 specular = lightColor * .5f;
        
            glUniform3f(Shader->Light[0], 25.f, 25.f, 25.f);
            glUniform3f(Shader->Light[1], ambient.r, ambient.g, ambient.b);
            glUniform3f(Shader->Light[2], diffuse.r, diffuse.g, diffuse.b);
            glUniform3f(Shader->Light[3], specular.r, specular.g, specular.b);

            mat4x4 Result = aglmMulMat4(ModelMatrix, CurrentViewMatrix);
            Result = aglmMulMat4(Result, CurrentProjectionMatrix);

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
            if(ShowPoints)
            {
                glDisable(GL_DEPTH_TEST);
                glPointSize(10);
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                glDrawArrays(Drawable->GLRenderMode, 0, Drawable->Mesh.VertexCount);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glEnable(GL_DEPTH_TEST);
                glPointSize(1);
            }
            
            glBindVertexArray(0);
            
        } else glDrawArrays(Drawable->GLRenderMode, 0, Drawable->Mesh.VertexCount); 
    }
}



#if defined(STB_TRUETYPE_IMPLEMENTATION)

static char *
aglReadFile(const char *File)
{
    char *Result;
    FILE *fp;
    size_t memsize;
    fp = fopen(File, "rb");
    fseek(fp, 0, SEEK_END);
    memsize = ftell(fp);
    rewind(fp);

    Result = (char *) AGL_MALLOC(sizeof(char) * memsize, 0);
    fread(Result, 1, memsize, fp);
    fclose(fp);
    return Result;
}

static agl_font_stbttf *
aglInitFont(const char *Filename,
            s32 FontSize = AGL_TRUETYPE_DEFAULT_TEXT_SIZE,
            s32 Width = AGL_TRUETYPE_DEFAULT_TEXTURE_SIZE,
            s32 Height = AGL_TRUETYPE_DEFAULT_TEXTURE_SIZE)
{
    agl_font_stbttf *Result = (agl_font_stbttf *) AGL_MALLOC(sizeof(agl_font_stbttf), 0);
    Result->FontSize     = FontSize;
    Result->Width        = Width;
    Result->Height       = Height;
    Result->OverSampleX  = AGL_TRUETYPE_DEFAULT_OVERSAMPLES;
    Result->OverSampleY  = AGL_TRUETYPE_DEFAULT_OVERSAMPLES;
    Result->Texture = 0;

    char *FontData = aglReadFile(Filename);
    
    u8 *AtlasData = (u8*) AGL_MALLOC(Result->Width * Result->Height, 0);
    AGL_ASSERT(AtlasData);

    s32 PackSuccess = false;
    stbtt_pack_context Context;
    if(stbtt_PackBegin(&Context, AtlasData, Result->Width, Result->Height, 0, 1, 0))
    {
        stbtt_PackSetOversampling(&Context, Result->OverSampleX,  Result->OverSampleY);
        if(stbtt_PackFontRange(&Context, (u8 *) FontData, 0, Result->FontSize,
                               AGL_TRUETYPE_DEFAULT_FIRST_CHARACTER, AGL_TRUETYPE_DEFAULT_CHARACTERS, Result->CharacterInfo))
        {
            stbtt_PackEnd(&Context);
            PackSuccess = true;
        }
    } else AGL_FREE(Result, 0);

    if(!PackSuccess)
    {
        AGL_FREE(Result, 0);
        AGL_ASSERT(!"Failed to initialize font");
        return 0;
    }
    
    glGenTextures(1, &Result->Texture);
    glBindTexture(GL_TEXTURE_2D, Result->Texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Result->Width, Result->Height, 0, GL_RED, GL_UNSIGNED_BYTE, AtlasData);
    glGenerateMipmap(GL_TEXTURE_2D);

    AGL_FREE(FontData, 0);
    AGL_FREE(AtlasData, 0);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    return Result;
}

static void
aglDeleteFont(agl_font_stbttf *Font)
{
    AGL_FREE(Font, 0);
}

static agl_render_text *
aglRenderableText(char *Text, agl_font_stbttf *Font)
{
    agl_render_text * Result = (agl_render_text *) AGL_MALLOC(sizeof(agl_render_text), 0);
    
    s32 TextLength = strlen(Text);
    s32 VertexCount = 4 * TextLength;
    s32 IndexCount = 6 * TextLength;
    
    v3 *vdata      = (v3 *)  AGL_MALLOC(sizeof(v3) * VertexCount, 0);
    v2 *texdata    = (v2 *)  AGL_MALLOC(sizeof(v2) * VertexCount, 0);
    u16 *idata     = (u16 *) AGL_MALLOC(sizeof(u16) * IndexCount, 0);

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
    glBindBuffer(GL_ARRAY_BUFFER, _tmp);
    glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(v3), vdata, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &_tmp);
    glBindBuffer(GL_ARRAY_BUFFER, _tmp);
    glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(v2), texdata, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &_tmp);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _tmp);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(u16), idata, GL_STATIC_DRAW);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    glBindVertexArray(0);

    AGL_FREE(vdata, 0);
    AGL_FREE(texdata, 0);
    AGL_FREE(idata, 0);

    Result->IndexCount = IndexCount;
    return Result;
}

agl_shader_ext *
aglInitFontShader()
{
#if 0
    agl_shader_text *Shader = (agl_shader_text *) AGL_MALLOC(sizeof(agl_shader_text), 0);
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
            in vec2 uv0;
            in vec4 color;
            
            uniform sampler2D mainTex;
            
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
    glBindBuffer(GL_ARRAY_BUFFER, Shader->VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v3), 0, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
    glGenBuffers(1, Shader->VBO + 1);
    glBindBuffer(GL_ARRAY_BUFFER, Shader->VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v2), 0, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &Shader->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Shader->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16), 0, GL_DYNAMIC_DRAW);
#else
    agl_shader_ext *Shader = (agl_shader_ext *) AGL_MALLOC(sizeof(agl_shader_ext), 0);
    const char * VertexShader = GLSL
        ( 430,
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
        ( 430,
          in vec2 uv0;
          in vec4 color;
          out vec4 fragColor;
          uniform sampler2D mainTex;
            
        
          void main()
          {
              vec4 c = texture(mainTex, uv0);
              if(c.r != 0)
                  fragColor = vec4(color.r * c.r, color.g * c.r, color.b * c.r, c.r - color.a);
              else
                  fragColor = vec4(c.r,c.r,c.r,0);
          }
          );

    
    Shader->Type = AGL_SHADER_TYPE_FONTS;
    Shader->Program = glCreateProgram();
    if(aglShaderCompileAndAttach(Shader->Program, VertexShader, GL_VERTEX_SHADER) &&
       aglShaderCompileAndAttach(Shader->Program, FragmentShader, GL_FRAGMENT_SHADER))
    {
        if(aglShaderLink(Shader->Program))
        {
            aglShaderSetBinding(Shader, "viewProjMatrix");
            aglShaderSetBinding(Shader, "worldMatrix");
            aglShaderSetBinding(Shader, "mainTex");
            aglShaderSetBinding(Shader, "fontColor");
            aglShaderSetBinding(Shader, "render2D");
            aglShaderSetBinding(Shader, "renderInfo");

            glGenBuffers(1, Shader->Bindings + 20);
            glBindBuffer(GL_ARRAY_BUFFER, Shader->Bindings[20]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(v3), 0, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
            glGenBuffers(1, Shader->Bindings + 21);
            glBindBuffer(GL_ARRAY_BUFFER, Shader->Bindings[21]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(v2), 0, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

            glGenBuffers(1, Shader->Bindings + 22);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Shader->Bindings[22]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16), 0, GL_DYNAMIC_DRAW);
            return Shader;
        }
    }
    AGL_FREE(Shader, 0);
#endif
    return Shader;
}

void
aglRenderText3D(agl_font_stbttf *Font, char *Text, r32 Scale = 1.0f,
                r32 X = 0.f, r32 Y = 0.f, r32 Z = 0.f,
                v3 Color = V3(1.0f, 1.0f, 1.0f), r32 Alpha = 0.f)
{
    if(!FontRenderingShader) FontRenderingShader = aglInitFontShader();

    s32 TextLength = strlen(Text);
    s32 VertexCount = 4 * TextLength;
    s32 IndexCount = 6 * TextLength;
    
    v3 *vdata      = (v3 *)  AGL_MALLOC(sizeof(v3) * VertexCount, 0);
    v2 *texdata    = (v2 *)  AGL_MALLOC(sizeof(v2) * VertexCount, 0);
    u16 *idata     = (u16 *) AGL_MALLOC(sizeof(u16) * IndexCount, 0);
    
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

    glUseProgram(FontRenderingShader->Program);
    
    mat4x4 World = aglmSclMat((1.0f / Font->Width) * Scale, (1.0f / Font->Height) * Scale, Z);
    World = aglmMulMat4(World, aglmTrlMat(X, Y, Z));
    mat4x4 ViewProj = aglmMulMat4(CurrentViewMatrix, CurrentProjectionMatrix);

    glBindTexture(GL_TEXTURE_2D, Font->Texture);
    glUniformMatrix4fv(FontRenderingShader->Bindings[0], 1, GL_FALSE, (const float *) ViewProj.E);
    glUniformMatrix4fv(FontRenderingShader->Bindings[1], 1, GL_FALSE, (const float *) World.E);
    glUniform1i(FontRenderingShader->Bindings[2], 0);
    glUniform4f(FontRenderingShader->Bindings[3], Color.r, Color.g, Color.b, Alpha);
    glUniform1i(FontRenderingShader->Bindings[4], 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, FontRenderingShader->Bindings[20]);
    glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(v3), 0, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, VertexCount * sizeof(v3), vdata);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, FontRenderingShader->Bindings[21]);
    glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(v2), 0, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, VertexCount * sizeof(v2), texdata);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FontRenderingShader->Bindings[22]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(u16), 0, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, IndexCount * sizeof(u16), idata);
    
    glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    AGL_FREE(vdata, 0);
    AGL_FREE(texdata, 0);
    AGL_FREE(idata, 0);    
}

void
aglRenderText2D(agl_font_stbttf *Font, char *Text,
                r32 X = 0.f, r32 Y = 0.f,
                v3 Color = V3(1.0f), r32 Alpha = 0.f)
{
    if(!FontRenderingShader) FontRenderingShader = aglInitFontShader();

    s32 TextLength = strlen(Text);
    s32 VertexCount = 4 * TextLength;
    s32 IndexCount = 6 * TextLength;
   
    v3 *vdata      = (v3 *)  AGL_MALLOC(sizeof(v3) * VertexCount, 0);
    v2 *texdata    = (v2 *)  AGL_MALLOC(sizeof(v2) * VertexCount, 0);
    u16 *idata     = (u16 *) AGL_MALLOC(sizeof(u16) * IndexCount, 0);

    r32 OffsetX = 0, OffsetY = 0;
    s32 LastIndex = 0;

    s32 FirstChar = AGL_TRUETYPE_DEFAULT_FIRST_CHARACTER;
    s32 LastChar = FirstChar + AGL_TRUETYPE_DEFAULT_CHARACTERS;

    v3 *v = vdata; v2 *t = texdata; u16 *i = idata;
    for(char *c = Text; *c; c++)
    {
        stbtt_aligned_quad Quad;
        if(*c >= FirstChar && *c <= LastChar)
        {
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
    }

    glUseProgram(FontRenderingShader->Program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Font->Texture);
    glUniform1i(FontRenderingShader->Bindings[2], 0);
    glUniform1i(FontRenderingShader->Bindings[4], 1);
    
    glBindBuffer(GL_ARRAY_BUFFER, FontRenderingShader->Bindings[20]);
    glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(v3), 0, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, VertexCount * sizeof(v3), vdata);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, FontRenderingShader->Bindings[21]);
    glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(v2), 0, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, VertexCount * sizeof(v2), texdata);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FontRenderingShader->Bindings[22]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(u16), 0, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, IndexCount * sizeof(u16), idata);

    glUniform4f(FontRenderingShader->Bindings[5],__ctx.Width, __ctx.Height, X, Y);
    glUniform4f(FontRenderingShader->Bindings[3], Color.r, Color.g, Color.b, Alpha);
    
    glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_SHORT, 0);
    
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    AGL_FREE(vdata, 0);
    AGL_FREE(texdata, 0);
    AGL_FREE(idata, 0);    
}

#endif

#endif

#define AGL_CORE3D_H
#endif
