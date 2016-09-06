#if !defined(AGL_H)

#ifndef AGL_INTERNAL_TYPES
    #include <stdint.h>
    #include <stddef.h>
    #include <limits.h>
    #include <float.h>
    
    typedef int8_t     s8;  
    typedef int16_t    s16;
    typedef int32_t    s32;
    typedef int64_t    s64;
    typedef uint8_t    u8;
    typedef uint16_t   u16;
    typedef uint32_t   u32;
    typedef uint64_t   u64;
    typedef s32        b32;
                        
    typedef float      r32;
    typedef double     r64;

    typedef intptr_t   intptr;
    typedef uintptr_t  uintptr;

    typedef size_t memory_index;
    #define AGL_INTERNAL_TYPES
#endif

#if defined(AGL_DEBUG)
    #include <stdio.h>
    #define AGL_DEBUG_PRINT(s,...) printf(s, __VA_ARGS__)
#endif

#if !defined(AGL_ASSERT)
    #include <assert.h>
    #define AGL_ASSERT(Condition) assert(Condition)
#endif

#if defined(AGL_EXTERN)
    #if defined(__cplusplus)
        #define AGLDEF extern "C"
    #else
        #define AGLDEF extern
    #endif
#else
    #if !defined(AGL_WND_CLASS_NAME)
        #define AGL_WND_CLASS_NAME "libagl_window_class"
    #endif
    #define AGL_IMPLEMENTATION
    #define AGLDEF static
#endif
    
#if _WIN32
    #define AGLAPI WINAPI
    #define AGLAPIP AGLAPI *

    #include <windows.h>
    #include <windowsx.h>

    typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
    typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
    typedef int  (WINAPI * PFNWGLGETSWAPINTERVALEXTPROC) (void);

    #define WGL_DRAW_TO_WINDOW_ARB            0x2001
    #define WGL_ACCELERATION_ARB              0x2003
    #define WGL_SUPPORT_OPENGL_ARB            0x2010
    #define WGL_DOUBLE_BUFFER_ARB             0x2011
    #define WGL_COLOR_BITS_ARB                0x2014
    #define WGL_ALPHA_BITS_ARB                0x201B
    #define WGL_DEPTH_BITS_ARB                0x2022
    #define WGL_STENCIL_BITS_ARB              0x2023
    #define WGL_FULL_ACCELERATION_ARB         0x2027    
    #define WGL_SAMPLE_BUFFERS_ARB            0x2041
    #define WGL_SAMPLES_ARB                   0x2042

    typedef struct 
    {
        HWND              HWnd;
        HDC               DC;
        WINDOWPLACEMENT   Placement;
        BOOL              TimerInit;              
        LARGE_INTEGER     Init, Frequency;
    } agl_platform_context;
#else
    // Define this for other platform support
    typedef struct 
    {
        void *Window;
    } agl_platform_context;
#endif

// OpenGL initialization 
#if !defined(AGLAPIP)
    #define AGLAPIP
#endif

#include <GL\GL.h>

#ifndef GL_VERSION_1_2
#define GL_UNSIGNED_BYTE_3_3_2            0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_INT_8_8_8_8           0x8035
#define GL_UNSIGNED_INT_10_10_10_2        0x8036
#define GL_TEXTURE_BINDING_3D             0x806A
#define GL_PACK_SKIP_IMAGES               0x806B
#define GL_PACK_IMAGE_HEIGHT              0x806C
#define GL_UNPACK_SKIP_IMAGES             0x806D
#define GL_UNPACK_IMAGE_HEIGHT            0x806E
#define GL_TEXTURE_3D                     0x806F
#define GL_PROXY_TEXTURE_3D               0x8070
#define GL_TEXTURE_DEPTH                  0x8071
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_MAX_3D_TEXTURE_SIZE            0x8073
#define GL_UNSIGNED_BYTE_2_3_3_REV        0x8362
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV       0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV     0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV     0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV       0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368
#define GL_BGR                            0x80E0
#define GL_BGRA                           0x80E1
#define GL_MAX_ELEMENTS_VERTICES          0x80E8
#define GL_MAX_ELEMENTS_INDICES           0x80E9
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_TEXTURE_MIN_LOD                0x813A
#define GL_TEXTURE_MAX_LOD                0x813B
#define GL_TEXTURE_BASE_LEVEL             0x813C
#define GL_TEXTURE_MAX_LEVEL              0x813D
#define GL_SMOOTH_POINT_SIZE_RANGE        0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY  0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY  0x0B23
#define GL_ALIASED_LINE_WIDTH_RANGE       0x846E
#define GL_RESCALE_NORMAL                 0x803A
#define GL_LIGHT_MODEL_COLOR_CONTROL      0x81F8
#define GL_SINGLE_COLOR                   0x81F9
#define GL_SEPARATE_SPECULAR_COLOR        0x81FA
#define GL_ALIASED_POINT_SIZE_RANGE       0x846D
#endif

#ifndef GL_ARB_texture_border_clamp
#define GL_CLAMP_TO_BORDER_ARB            0x812D
#endif

#define GL_SHADING_LANGUAGE_VERSION                 0x8B8C
#define GL_MULTISAMPLE_ARB                          0x809D
#define GL_COMPILE_STATUS                           0x8B81
#define GL_LINK_STATUS                              0x8B82
#define GL_FRAGMENT_SHADER                          0x8B30
#define GL_VERTEX_SHADER                            0x8B31
                                                    
#define GL_GENERATE_MIPMAP_HINT                     0x8192
                                                    
#ifndef GL_ARB_vertex_buffer_object                 
#define GL_BUFFER_SIZE_ARB                          0x8764
#define GL_BUFFER_USAGE_ARB                         0x8765
#define GL_ARRAY_BUFFER_ARB                         0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB                 0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB                 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB         0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB          0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB          0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB           0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB           0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB   0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB       0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB  0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB          0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB   0x889F
#define GL_READ_ONLY_ARB                            0x88B8
#define GL_WRITE_ONLY_ARB                           0x88B9
#define GL_READ_WRITE_ARB                           0x88BA
#define GL_BUFFER_ACCESS_ARB                        0x88BB
#define GL_BUFFER_MAPPED_ARB                        0x88BC
#define GL_BUFFER_MAP_POINTER_ARB                   0x88BD
#define GL_STREAM_DRAW_ARB                          0x88E0
#define GL_STREAM_READ_ARB                          0x88E1
#define GL_STREAM_COPY_ARB                          0x88E2
#define GL_STATIC_DRAW_ARB                          0x88E4
#define GL_STATIC_READ_ARB                          0x88E5
#define GL_STATIC_COPY_ARB                          0x88E6
#define GL_DYNAMIC_DRAW_ARB                         0x88E8
#define GL_DYNAMIC_READ_ARB                         0x88E9
#define GL_DYNAMIC_COPY_ARB                         0x88EA
#endif

#ifndef GL_ARB_multitexture
#define GL_TEXTURE0_ARB                   0x84C0
#define GL_TEXTURE1_ARB                   0x84C1
#define GL_TEXTURE2_ARB                   0x84C2
#define GL_TEXTURE3_ARB                   0x84C3
#define GL_TEXTURE4_ARB                   0x84C4
#define GL_TEXTURE5_ARB                   0x84C5
#define GL_TEXTURE6_ARB                   0x84C6
#define GL_TEXTURE7_ARB                   0x84C7
#define GL_TEXTURE8_ARB                   0x84C8
#define GL_TEXTURE9_ARB                   0x84C9
#define GL_TEXTURE10_ARB                  0x84CA
#define GL_TEXTURE11_ARB                  0x84CB
#define GL_TEXTURE12_ARB                  0x84CC
#define GL_TEXTURE13_ARB                  0x84CD
#define GL_TEXTURE14_ARB                  0x84CE
#define GL_TEXTURE15_ARB                  0x84CF
#define GL_TEXTURE16_ARB                  0x84D0
#define GL_TEXTURE17_ARB                  0x84D1
#define GL_TEXTURE18_ARB                  0x84D2
#define GL_TEXTURE19_ARB                  0x84D3
#define GL_TEXTURE20_ARB                  0x84D4
#define GL_TEXTURE21_ARB                  0x84D5
#define GL_TEXTURE22_ARB                  0x84D6
#define GL_TEXTURE23_ARB                  0x84D7
#define GL_TEXTURE24_ARB                  0x84D8
#define GL_TEXTURE25_ARB                  0x84D9
#define GL_TEXTURE26_ARB                  0x84DA
#define GL_TEXTURE27_ARB                  0x84DB
#define GL_TEXTURE28_ARB                  0x84DC
#define GL_TEXTURE29_ARB                  0x84DD
#define GL_TEXTURE30_ARB                  0x84DE
#define GL_TEXTURE31_ARB                  0x84DF
#define GL_ACTIVE_TEXTURE_ARB             0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB      0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB          0x84E2
#endif

#ifndef GL_ARB_texture_cube_map
#define GL_NORMAL_MAP_ARB                 0x8511
#define GL_REFLECTION_MAP_ARB             0x8512
#define GL_TEXTURE_CUBE_MAP_ARB           0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB   0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB     0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB  0x851C
#endif

#ifndef GL_ARB_framebuffer_object
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT            0x8218
#define GL_FRAMEBUFFER_UNDEFINED          0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_MAX_RENDERBUFFER_SIZE          0x84E8
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_TEXTURE_STENCIL_SIZE           0x88F1
#define GL_TEXTURE_RED_TYPE               0x8C10
#define GL_TEXTURE_GREEN_TYPE             0x8C11
#define GL_TEXTURE_BLUE_TYPE              0x8C12
#define GL_TEXTURE_ALPHA_TYPE             0x8C13
#define GL_TEXTURE_DEPTH_TYPE             0x8C16
#define GL_UNSIGNED_NORMALIZED            0x8C17
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_DRAW_FRAMEBUFFER_BINDING       0x8CA6
#define GL_RENDERBUFFER_BINDING           0x8CA7
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING       0x8CAA
#define GL_RENDERBUFFER_SAMPLES           0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS          0x8CDF
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COLOR_ATTACHMENT1              0x8CE1
#define GL_COLOR_ATTACHMENT2              0x8CE2
#define GL_COLOR_ATTACHMENT3              0x8CE3
#define GL_COLOR_ATTACHMENT4              0x8CE4
#define GL_COLOR_ATTACHMENT5              0x8CE5
#define GL_COLOR_ATTACHMENT6              0x8CE6
#define GL_COLOR_ATTACHMENT7              0x8CE7
#define GL_COLOR_ATTACHMENT8              0x8CE8
#define GL_COLOR_ATTACHMENT9              0x8CE9
#define GL_COLOR_ATTACHMENT10             0x8CEA
#define GL_COLOR_ATTACHMENT11             0x8CEB
#define GL_COLOR_ATTACHMENT12             0x8CEC
#define GL_COLOR_ATTACHMENT13             0x8CED
#define GL_COLOR_ATTACHMENT14             0x8CEE
#define GL_COLOR_ATTACHMENT15             0x8CEF
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_RENDERBUFFER_WIDTH             0x8D42
#define GL_RENDERBUFFER_HEIGHT            0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT   0x8D44
#define GL_STENCIL_INDEX1                 0x8D46
#define GL_STENCIL_INDEX4                 0x8D47
#define GL_STENCIL_INDEX8                 0x8D48
#define GL_STENCIL_INDEX16                0x8D49
#define GL_RENDERBUFFER_RED_SIZE          0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE        0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE         0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE        0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE        0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE      0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES                    0x8D57
#define GL_INDEX                          0x8222
#define GL_TEXTURE_LUMINANCE_TYPE         0x8C14
#define GL_TEXTURE_INTENSITY_TYPE         0x8C15
#endif

#ifndef GL_ARB_shadow
#define GL_TEXTURE_COMPARE_MODE_ARB       0x884C
#define GL_TEXTURE_COMPARE_FUNC_ARB       0x884D
#define GL_COMPARE_R_TO_TEXTURE_ARB       0x884E
#endif

#ifndef GL_ARB_depth_texture
#define GL_DEPTH_COMPONENT16_ARB          0x81A5
#define GL_DEPTH_COMPONENT24_ARB          0x81A6
#define GL_DEPTH_COMPONENT32_ARB          0x81A7
#define GL_TEXTURE_DEPTH_SIZE_ARB         0x884A
#define GL_DEPTH_TEXTURE_MODE_ARB         0x884B
#endif



#include <stddef.h>
#ifndef GL_VERSION_2_0
/* GL type for program/shader text */
typedef char GLchar;
#endif

#ifndef GL_VERSION_1_5
/* GL types for handling large vertex buffer objects */
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
#endif

#ifndef GL_ARB_vertex_buffer_object
/* GL types for handling large vertex buffer objects */
typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;
#endif

#ifndef GL_ARB_shader_objects
/* GL types for program/shader text and shader object handles */
typedef char GLcharARB;
typedef unsigned int GLhandleARB;
#endif

/* Modern GL functions for shader, vbo/ibo, mipmapping ... support */
typedef GLuint    (AGLAPIP PFNGLCREATESHADERPROC) (GLenum type);
typedef void      (AGLAPIP PFNGLDELETESHADERPROC) (GLuint shader);
typedef void      (AGLAPIP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length);
typedef void      (AGLAPIP PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void      (AGLAPIP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void      (AGLAPIP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef GLuint    (AGLAPIP PFNGLCREATEPROGRAMPROC) (void);
typedef void      (AGLAPIP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void      (AGLAPIP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void      (AGLAPIP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void      (AGLAPIP PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void      (AGLAPIP PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void      (AGLAPIP PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void      (AGLAPIP PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef GLint     (AGLAPIP PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void      (AGLAPIP PFNGLUNIFORMMATRIX3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void      (AGLAPIP PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void      (AGLAPIP PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void      (AGLAPIP PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void      (AGLAPIP PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void      (AGLAPIP PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void      (AGLAPIP PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void      (AGLAPIP PFNGLUNIFORM2IPROC) (GLint location, GLint v0, GLint v1);
typedef void      (AGLAPIP PFNGLUNIFORM3IPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void      (AGLAPIP PFNGLUNIFORM4IPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void      (AGLAPIP PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void      (AGLAPIP PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef GLvoid*   (AGLAPIP PFNGLMAPBUFFERARBPROC) (GLenum target, GLenum access);
typedef GLboolean (AGLAPIP PFNGLUNMAPBUFFERARBPROC) (GLenum target);
typedef void      (AGLAPIP PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void      (AGLAPIP PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
typedef void      (AGLAPIP PFNGLBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);

typedef GLboolean (AGLAPIP PFNGLISRENDERBUFFERPROC) (GLuint renderbuffer);
typedef void      (AGLAPIP PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void      (AGLAPIP PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint *renderbuffers);
typedef void      (AGLAPIP PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint *renderbuffers);
typedef void      (AGLAPIP PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void      (AGLAPIP PFNGLGETRENDERBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef GLboolean (AGLAPIP PFNGLISFRAMEBUFFERPROC) (GLuint framebuffer);
typedef void      (AGLAPIP PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void      (AGLAPIP PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint *framebuffers);
typedef void      (AGLAPIP PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint *framebuffers);
typedef GLenum    (AGLAPIP PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void      (AGLAPIP PFNGLFRAMEBUFFERTEXTURE1DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void      (AGLAPIP PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void      (AGLAPIP PFNGLFRAMEBUFFERTEXTURE3DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void      (AGLAPIP PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void      (AGLAPIP PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint *params);
typedef void      (AGLAPIP PFNGLGENERATEMIPMAPPROC) (GLenum target);
typedef void      (AGLAPIP PFNGLBLITFRAMEBUFFERPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void      (AGLAPIP PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void      (AGLAPIP PFNGLFRAMEBUFFERTEXTURELAYERPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);


typedef void      (AGLAPIP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void      (AGLAPIP PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
typedef void      (AGLAPIP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef void      (AGLAPIP PFNGLENABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void      (AGLAPIP PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void      (AGLAPIP PFNGLVERTEXATTRIBPOINTERARBPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void      (AGLAPIP PFNGLVERTEXATTRIBDIVISORARBPROC) (GLuint index, GLuint divisor);
typedef void      (AGLAPIP PFNGLDRAWARRAYSINSTANCEDARBPROC) (GLenum mode, GLint first, GLsizei count, GLsizei primcount);

#if !defined(AGL_NO_GLPROCS)
PFNGLCREATESHADERPROC                  glCreateShader;
PFNGLDELETESHADERPROC                  glDeleteShader;
PFNGLSHADERSOURCEPROC                  glShaderSource;
PFNGLCOMPILESHADERPROC                 glCompileShader;
PFNGLGETSHADERIVPROC                   glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC              glGetShaderInfoLog;
PFNGLCREATEPROGRAMPROC                 glCreateProgram;
PFNGLGETPROGRAMIVPROC                  glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC             glGetProgramInfoLog;
PFNGLATTACHSHADERPROC                  glAttachShader;
PFNGLDETACHSHADERPROC                  glDetachShader;
PFNGLLINKPROGRAMPROC                   glLinkProgram;
PFNGLUSEPROGRAMPROC                    glUseProgram;

PFNGLACTIVETEXTUREARBPROC              glActiveTexture;

PFNGLUNIFORMMATRIX3FVPROC              glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC              glUniformMatrix4fv;
PFNGLUNIFORM4FPROC                     glUniform4f;
PFNGLUNIFORM3FPROC                     glUniform3f;
PFNGLUNIFORM2FPROC                     glUniform2f;
PFNGLUNIFORM1FPROC                     glUniform1f;
PFNGLUNIFORM4IPROC                     glUniform4i;
PFNGLUNIFORM3IPROC                     glUniform3i;
PFNGLUNIFORM2IPROC                     glUniform2i;
PFNGLUNIFORM1IPROC                     glUniform1i;
PFNGLGETUNIFORMLOCATIONPROC            glGetUniformLocation;

PFNGLGENBUFFERSARBPROC                 glGenBuffers;
PFNGLBINDBUFFERARBPROC                 glBindBuffer;
PFNGLMAPBUFFERARBPROC                  glMapBuffer;
PFNGLUNMAPBUFFERARBPROC                glUnmapBuffer;
PFNGLBUFFERDATAARBPROC                 glBufferData;
PFNGLBUFFERSUBDATAARBPROC              glBufferSubData;
PFNGLDELETEBUFFERSARBPROC              glDeleteBuffers;

PFNGLISRENDERBUFFERPROC                      glIsRenderBuffer;
PFNGLBINDRENDERBUFFERPROC                    glBindRenderBuffer;
PFNGLDELETERENDERBUFFERSPROC                 glDeleteRenderbuffers;
PFNGLGENRENDERBUFFERSPROC                    glGenRenderbuffers;
PFNGLRENDERBUFFERSTORAGEPROC                 glRenderbufferStorage;
PFNGLGETRENDERBUFFERPARAMETERIVPROC          glGetRenderbufferParameteriv;
PFNGLISFRAMEBUFFERPROC                       glIsFramebuffer;
PFNGLBINDFRAMEBUFFERPROC                     glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSPROC                  glDeleteFramebuffers;
PFNGLGENFRAMEBUFFERSPROC                     glGenFramebuffers;
PFNGLCHECKFRAMEBUFFERSTATUSPROC              glCheckFramebufferStatus;
PFNGLFRAMEBUFFERTEXTURE1DPROC                glFramebufferTexture1D;
PFNGLFRAMEBUFFERTEXTURE2DPROC                glFramebufferTexture2D;
PFNGLFRAMEBUFFERTEXTURE3DPROC                glFramebufferTexture3D;
PFNGLFRAMEBUFFERRENDERBUFFERPROC             glFramebufferRenderbuffer;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
PFNGLGENERATEMIPMAPPROC                      glGenerateMipmap;
PFNGLBLITFRAMEBUFFERPROC                     glBlitFramebuffer;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC      glRenderbufferStorageMultisample;
PFNGLFRAMEBUFFERTEXTURELAYERPROC             glFramebufferTextureLayer;              

PFNGLGENVERTEXARRAYSPROC               glGenVertexArrays;
PFNGLDELETEVERTEXARRAYSPROC            glDeleteVertexArrays;
PFNGLBINDVERTEXARRAYPROC               glBindVertexArray;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC    glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERARBPROC        glVertexAttribPointer;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC   glDisableVertexAttribArray;

// maybe not really needed?
PFNGLVERTEXATTRIBDIVISORARBPROC        glVertexAttribDivisor;
PFNGLDRAWARRAYSINSTANCEDARBPROC        glDrawArraysInstanced;
#endif

// libAGL initialization


typedef struct
{
    s32 Count;
    b32 EndedDown;
} agl_key_state;

enum { AGL_MOUSE_LEFT = 0, AGL_MOUSE_RIGHT = 1, AGL_MOUSE_MIDDLE = 2 };
typedef struct 
{
    agl_key_state Keys[256], Buttons[3];
    s32 MouseX, MouseY, MouseXDelta, MouseYDelta, MouseWheelDelta;
    s32 MouseXScreen, MouseYScreen;
} agl_input;

typedef struct 
{
    u8 *Vendor;
    u8 *Renderer;
    u8 *Version;
    u8 *ShadingLanguageVersion;
    u8 *Extensions;
} agl_opengl_info;

enum
{
    AGL_WINDOW_VSYNC      = 1,
    AGL_WINDOW_MSAA       = 2,
    AGL_WINDOW_FULLSCREEN = 4,
};

typedef struct
{    
    b32                  Running;
    b32                  Active;
    s32                  Width;
    s32                  Height;
    s32                  FPS;
    
    r32                  Delta;
    u32                  FrameCount;
    u32                  StartTime;
    u32                  TicksLastFrame;

    b32                  VerticalSync;
    b32                  EnableMSAA;
    b32                  MultisampleSupported;
    s32                  MultisampleFormat;
    HGLRC                GLContext;
    agl_opengl_info      GLInfo;
    agl_input            Input;
//    agl_mouse_input      MouseInput;
    agl_platform_context Platform;
} agl_context;
agl_context __agl_Context; // global context to get access to static functions like win32 messageproc


// NOTE: Define functions in your code base to get callback messages for the following callback functions.
//
//       Example: void MyKeyDownFunc(char Key) { //Your custom code here };
//       In the mainloop before aglCreateWindow() call use aglCallbackKeyDown(MyKeyDownFunc);
//       Now you will get key down messages send to your custom function.

typedef void (AGLAPI agl_callback_resize_proc) (int width, int height);
agl_callback_resize_proc *aglResizeCallback;
#define aglCallbackResize(f) aglResizeCallback = f

typedef void (AGLAPI agl_callback_keydown_proc) (char Key);
agl_callback_keydown_proc *aglKeyDownCallback;
#define aglCallbackKeyDown(f) aglKeyDownCallback = f

typedef void (AGLAPI agl_callback_keyup_proc) (char Key);
agl_callback_keyup_proc *aglKeyUpCallback;
#define aglCallbackKeyUp(f) aglKeyUpCallback = f

// NOTE: Platform specific functions. Implement these yourself if you want to. 
//       Define AGL_EXTERN beforce including this file. Ex.: #define AGL_EXTERN
//       When implementing your own platform function make sure you always apply changes
//       to __agl_Context, otherwise this api won't do anything.

// $DOC$
AGLDEF void  aglPlatformCaptureMouse(b32 Capture);
// $DOC$
AGLDEF void  aglPlatformCloseWindow();
// $DOC$
AGLDEF b32   aglPlatformCreateWindow(char *Title);
// $DOC$
AGLDEF void  aglPlatformDestroyWindow();
// $DOC$
AGLDEF void* aglPlatformGetProcAddress(char *Function);
// $DOC$
AGLDEF void  aglPlatformHandleEvents();
// $DOC$
AGLDEF b32   aglPlatformIsActive();
// $DOC$
AGLDEF b32   aglPlatformIsExtensionSupported(const char *Extension);
// $DOC$
AGLDEF void  aglPlatformSetCursor(b32 Show);
// $DOC$
AGLDEF void  aglPlatformSetVerticalSync(b32 State);
// $DOC$
AGLDEF void  aglPlatformSetWindowTitle(char *Title);
// $DOC$
AGLDEF void  aglPlatformSwapBuffers();
// $DOC$
AGLDEF void  aglPlatformToggleFullscreen();

AGLDEF u32   aglPlatformGetTicks();

// NOTE: General api code for handling the window and opengl context.
static void *
aglGetProcAddress(char *Function)
{
    void *Result = aglPlatformGetProcAddress(Function);
    if(!Result)
        int a=1;
    AGL_ASSERT(Result);
    return Result;
}

static b32
aglInitModernGLImpl()
{
#if defined(AGL_NO_GLPROCS)
    return true;
#else
    b32 Result = false;
    if(*aglGetProcAddress)
    {
        glCreateShader                     = (PFNGLCREATESHADERPROC)                aglGetProcAddress("glCreateShader");
        glDeleteShader                     = (PFNGLDELETESHADERPROC)                aglGetProcAddress("glDeleteShader");
        glShaderSource                     = (PFNGLSHADERSOURCEPROC)                aglGetProcAddress("glShaderSource");
        glCompileShader                    = (PFNGLCOMPILESHADERPROC)               aglGetProcAddress("glCompileShader");
        glGetShaderiv                      = (PFNGLGETSHADERIVPROC)                 aglGetProcAddress("glGetShaderiv");
        glGetShaderInfoLog                 = (PFNGLGETSHADERINFOLOGPROC)            aglGetProcAddress("glGetShaderInfoLog");
        glCreateProgram                    = (PFNGLCREATEPROGRAMPROC)               aglGetProcAddress("glCreateProgram");
        glGetProgramiv                     = (PFNGLGETPROGRAMIVPROC)                aglGetProcAddress("glGetProgramiv");
        glGetProgramInfoLog                = (PFNGLGETPROGRAMINFOLOGPROC)           aglGetProcAddress("glGetProgramInfoLog");
        glAttachShader                     = (PFNGLATTACHSHADERPROC)                aglGetProcAddress("glAttachShader");
        glDetachShader                     = (PFNGLDETACHSHADERPROC)                aglGetProcAddress("glDetachShader");
        glLinkProgram                      = (PFNGLLINKPROGRAMPROC)                 aglGetProcAddress("glLinkProgram");
        glUseProgram                       = (PFNGLUSEPROGRAMPROC)                  aglGetProcAddress("glUseProgram");
                                                                                    
        glActiveTexture                    = (PFNGLACTIVETEXTUREARBPROC)            aglGetProcAddress("glActiveTexture");
                                                                                    
        glGetUniformLocation               = (PFNGLGETUNIFORMLOCATIONPROC)          aglGetProcAddress("glGetUniformLocation");
        
        glUniformMatrix4fv                 = (PFNGLUNIFORMMATRIX4FVPROC)            aglGetProcAddress("glUniformMatrix4fv");
        glUniformMatrix3fv                 = (PFNGLUNIFORMMATRIX3FVPROC)            aglGetProcAddress("glUniformMatrix3fv");
        
        glUniform4f                        = (PFNGLUNIFORM4FPROC)                   aglGetProcAddress("glUniform4f");
        glUniform3f                        = (PFNGLUNIFORM3FPROC)                   aglGetProcAddress("glUniform3f");
        glUniform2f                        = (PFNGLUNIFORM2FPROC)                   aglGetProcAddress("glUniform2f");
        glUniform1f                        = (PFNGLUNIFORM1FPROC)                   aglGetProcAddress("glUniform1f");
        
        glUniform4i                        = (PFNGLUNIFORM4IPROC)                   aglGetProcAddress("glUniform4i");
        glUniform3i                        = (PFNGLUNIFORM3IPROC)                   aglGetProcAddress("glUniform3i");
        glUniform2i                        = (PFNGLUNIFORM2IPROC)                   aglGetProcAddress("glUniform2i");
        glUniform1i                        = (PFNGLUNIFORM1IPROC)                   aglGetProcAddress("glUniform1i");
        
        glGenBuffers                       = (PFNGLGENBUFFERSARBPROC)               aglGetProcAddress("glGenBuffersARB");
        glBindBuffer                       = (PFNGLBINDBUFFERARBPROC)               aglGetProcAddress("glBindBufferARB");
        glMapBuffer                        = (PFNGLMAPBUFFERARBPROC)                aglGetProcAddress("glMapBufferARB");
        glUnmapBuffer                      = (PFNGLUNMAPBUFFERARBPROC)              aglGetProcAddress("glUnmapBufferARB");
        
        glDeleteBuffers                    = (PFNGLDELETEBUFFERSARBPROC)            aglGetProcAddress("glDeleteBuffersARB");
        glBufferData                       = (PFNGLBUFFERDATAARBPROC)               aglGetProcAddress("glBufferDataARB");
        glBufferSubData                    = (PFNGLBUFFERSUBDATAARBPROC)            aglGetProcAddress("glBufferSubDataARB");

        //glIsRenderBuffer                   = (PFNGLISRENDERBUFFERPROC)              aglGetProcAddress("glIsRenderBuffer");
        //glBindRenderBuffer                 = (PFNGLBINDRENDERBUFFERPROC)            aglGetProcAddress("glBindRenderBuffer");
        glDeleteRenderbuffers              = (PFNGLDELETERENDERBUFFERSPROC)         aglGetProcAddress("glDeleteRenderbuffers");
        glGenRenderbuffers                 = (PFNGLGENRENDERBUFFERSPROC)            aglGetProcAddress("glGenRenderbuffers");
        glRenderbufferStorage              = (PFNGLRENDERBUFFERSTORAGEPROC)         aglGetProcAddress("glRenderbufferStorage");
        glGetRenderbufferParameteriv       = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)  aglGetProcAddress("glGetRenderbufferParameteriv");
        glIsFramebuffer                    = (PFNGLISFRAMEBUFFERPROC)               aglGetProcAddress("glIsFramebuffer");
        glBindFramebuffer                  = (PFNGLBINDFRAMEBUFFERPROC)             aglGetProcAddress("glBindFramebuffer");
        glDeleteFramebuffers               = (PFNGLDELETEFRAMEBUFFERSPROC)          aglGetProcAddress("glDeleteFramebuffers");
        glGenFramebuffers                  = (PFNGLGENFRAMEBUFFERSPROC)             aglGetProcAddress("glGenFramebuffers");
        glCheckFramebufferStatus           = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)      aglGetProcAddress("glCheckFramebufferStatus");
        glFramebufferTexture1D             = (PFNGLFRAMEBUFFERTEXTURE1DPROC)        aglGetProcAddress("glFramebufferTexture1D");
        glFramebufferTexture2D             = (PFNGLFRAMEBUFFERTEXTURE2DPROC)        aglGetProcAddress("glFramebufferTexture2D");
        glFramebufferTexture3D             = (PFNGLFRAMEBUFFERTEXTURE3DPROC)        aglGetProcAddress("glFramebufferTexture3D");
        glFramebufferRenderbuffer          = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)     aglGetProcAddress("glFramebufferRenderbuffer");
        glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) aglGetProcAddress("glGetFramebufferAttachmentParameteriv");
        glGenerateMipmap                   = (PFNGLGENERATEMIPMAPPROC)              aglGetProcAddress("glGenerateMipmap");
        glBlitFramebuffer                  = (PFNGLBLITFRAMEBUFFERPROC)             aglGetProcAddress("glBlitFramebuffer");
        glRenderbufferStorageMultisample   = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) aglGetProcAddress("glRenderbufferStorageMultisample");
        glFramebufferTextureLayer          = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)     aglGetProcAddress("glFramebufferTextureLayer");       
      
        glGenVertexArrays                  = (PFNGLGENVERTEXARRAYSPROC)             aglGetProcAddress("glGenVertexArrays");
        glDeleteVertexArrays               = (PFNGLDELETEVERTEXARRAYSPROC)          aglGetProcAddress("glDeleteVertexArrays");
        glBindVertexArray                  = (PFNGLBINDVERTEXARRAYPROC)             aglGetProcAddress("glBindVertexArray");
        glEnableVertexAttribArray          = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)  aglGetProcAddress("glEnableVertexAttribArrayARB");
        glVertexAttribPointer              = (PFNGLVERTEXATTRIBPOINTERARBPROC)      aglGetProcAddress("glVertexAttribPointerARB");
        glDisableVertexAttribArray         = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) aglGetProcAddress("glDisableVertexAttribArrayARB");

        glVertexAttribDivisor              = (PFNGLVERTEXATTRIBDIVISORARBPROC)      aglGetProcAddress("glVertexAttribDivisorARB");
        glDrawArraysInstanced              = (PFNGLDRAWARRAYSINSTANCEDARBPROC)      aglGetProcAddress("glDrawArraysInstancedARB");
        
        glGenerateMipmap                   = (PFNGLGENERATEMIPMAPPROC)              aglGetProcAddress("glGenerateMipmap");
        
        Result = true;
    }
    return Result;
#endif
}

static agl_context *
aglCreateWindow(char *Title = "agl default window", s32 Width = 1024, s32 Height = 768, u32 Settings = 0)
{
    __agl_Context.Width = Width;
    __agl_Context.Height = Height;
    __agl_Context.EnableMSAA = ((Settings & AGL_WINDOW_MSAA) != 0 ? true : false );
    __agl_Context.VerticalSync = ((Settings & AGL_WINDOW_VSYNC) != 0 ? true : false );
    
    if(aglPlatformCreateWindow(Title) && __agl_Context.GLContext)
    {
        __agl_Context.GLInfo.Vendor                 = (u8 *) glGetString(GL_VENDOR);
        __agl_Context.GLInfo.Renderer               = (u8 *) glGetString(GL_RENDERER);
        __agl_Context.GLInfo.Version                = (u8 *) glGetString(GL_VERSION);
        __agl_Context.GLInfo.ShadingLanguageVersion = (u8 *) glGetString(GL_SHADING_LANGUAGE_VERSION);
        __agl_Context.GLInfo.Extensions             = (u8 *) glGetString(GL_EXTENSIONS);
        
        __agl_Context.Running                       = aglInitModernGLImpl();
        if(Settings & AGL_WINDOW_FULLSCREEN) aglPlatformToggleFullscreen();
        if(__agl_Context.EnableMSAA) glEnable(GL_MULTISAMPLE_ARB); else glDisable(GL_MULTISAMPLE_ARB);
        
        return &__agl_Context;
    }
    return 0;
}

static void
aglCloseWindow()
{
    aglPlatformCloseWindow();
};

static r32
aglGetDelta()
{
    return __agl_Context.Delta;
};

static b32
aglHandleEvents()
{
    __agl_Context.FrameCount++;
    u32 Now = aglPlatformGetTicks();
    r32 dTime = (Now - __agl_Context.StartTime) / 1000.f;
    if(__agl_Context.FrameCount > 29)
    {
        __agl_Context.FPS = __agl_Context.FrameCount / (dTime);
        __agl_Context.FrameCount = 0;
        __agl_Context.StartTime = Now;
    }
    
    __agl_Context.Delta = (Now - __agl_Context.TicksLastFrame) / 1000.f;
    __agl_Context.TicksLastFrame = Now;
    
    for(s32 i=0; i < 256; i++) {
        agl_key_state *State = __agl_Context.Input.Keys + i;
        if(State->EndedDown) {
            State->Count = 0;
            State->EndedDown = false;
        }
    }
    
    for(s32 i=0; i < 3; i++) {
        agl_key_state *State = __agl_Context.Input.Buttons + i;
        if(State->EndedDown) {
            State->Count = 0;
            State->EndedDown = false;
        }
    }
    
    aglPlatformHandleEvents();
    return __agl_Context.Running;
}

static r32
aglKeyDownTransition(char Key)
{
    return (r32) __agl_Context.Input.Keys[Key].Count * __agl_Context.Delta;
}

static b32
aglKeyDown(u8 Key)
{
    return (!__agl_Context.Input.Keys[Key].EndedDown && __agl_Context.Input.Keys[Key].Count > 0);
}

static b32
aglKeyUp(u8 Key)
{
    return __agl_Context.Input.Keys[Key].EndedDown;
}

static s32
aglGetMouseX()
{
    return __agl_Context.Input.MouseX;
}

static s32
aglGetMouseY()
{
    return __agl_Context.Input.MouseY;
}


static b32
aglMouseDown(u32 MouseButton)
{
    return (!__agl_Context.Input.Buttons[MouseButton].EndedDown && __agl_Context.Input.Buttons[MouseButton].Count > 0);
}

static b32
aglMouseUp(u32 MouseButton)
{
    return __agl_Context.Input.Buttons[MouseButton].EndedDown;
}

static s32
aglMouseWheelDelta()
{
    return __agl_Context.Input.MouseWheelDelta;
}

static void
aglCaptureMouse(b32 Capture)
{
    aglPlatformCaptureMouse(Capture);
}

static b32
aglIsActive()
{
    return __agl_Context.Active;
}

static void
aglSwapBuffers()
{
    aglPlatformSwapBuffers();
}

static void
aglSetVerticalSync(b32 State)
{
    aglPlatformSetVerticalSync(State);
}
static void
aglToggleFullscreen()
{
    aglPlatformToggleFullscreen();
}

static void
aglSetWindowTitle(char * Title)
{
    aglPlatformSetWindowTitle(Title);
}

static void
aglCleanup()
{
    aglPlatformDestroyWindow();
}

#if defined(_WIN32) && defined(AGL_IMPLEMENTATION)

AGLDEF void * aglPlatformGetProcAddress(char *Function) { return wglGetProcAddress(Function); }

AGLDEF void
aglPlatformToggleFullscreen()
{
    DWORD Style = GetWindowLong(__agl_Context.Platform.HWnd, GWL_STYLE);
    if(Style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO MonitorInfo = {sizeof(MonitorInfo)};
        if(GetWindowPlacement(__agl_Context.Platform.HWnd, &__agl_Context.Platform.Placement) &&
           GetMonitorInfo(MonitorFromWindow(__agl_Context.Platform.HWnd, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
        {
            SetWindowLong(__agl_Context.Platform.HWnd, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(__agl_Context.Platform.HWnd, HWND_TOP,
                         MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
                         MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
                         MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }

    }
    else
    {
        SetWindowLong(__agl_Context.Platform.HWnd, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(__agl_Context.Platform.HWnd, &__agl_Context.Platform.Placement);
        SetWindowPos(__agl_Context.Platform.HWnd, 0, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}

AGLDEF void
aglPlatformHandleEvents()
{
    POINT MousePos;
    GetCursorPos(&MousePos);
    __agl_Context.Input.MouseXDelta  = MousePos.x - __agl_Context.Input.MouseXScreen;
    __agl_Context.Input.MouseYDelta  = MousePos.y - __agl_Context.Input.MouseYScreen;
    __agl_Context.Input.MouseXScreen = MousePos.x;
    __agl_Context.Input.MouseYScreen = MousePos.y;
    __agl_Context.Input.MouseWheelDelta = 0;
    
    MSG msg;
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE) && __agl_Context.Running)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }    
    if(!IsWindow(__agl_Context.Platform.HWnd)) __agl_Context.Running = false;
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)									// Check For Windows Messages
    {
        case WM_ACTIVATE:							// Watch For Window Activate Message
        {
            // Kind of confused by this. I don't know why its swapped.
            if(LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
            {
                __agl_Context.Active = false;
            } else if(LOWORD(wParam) == WA_INACTIVE)
            {
                __agl_Context.Active = true;
            }            
            return 0;								// Return To The Message Loop
        }
        case WM_SYSCOMMAND:							// Intercept System Commands
        {
            switch (wParam)							// Check System Calls
            {
                case SC_KEYMENU:
                case SC_SCREENSAVE:					// Screensaver Trying To Start?
                case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
                    return 0;							// Prevent From Happening
            }
            break;									// Exit
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }break;
        case WM_KEYDOWN:
        {
            u8 Key = (u8) wParam;
            agl_key_state *State = __agl_Context.Input.Keys + Key;
            State->Count++;
            State->EndedDown = false;
            if(aglKeyDownCallback) aglKeyDownCallback(Key);
            return 0;
        }break;
        case WM_KEYUP:								// Has A Key Been Released?
        {
            u8 Key = (u8) wParam;
            agl_key_state *State = __agl_Context.Input.Keys + Key;
            State->EndedDown = true;
            if(aglKeyUpCallback) aglKeyUpCallback(Key);
            return 0;
        }break;
        case WM_MOUSEWHEEL:
        {
            __agl_Context.Input.MouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            return 0;
        }break;
        case WM_MOUSEMOVE:
        {                        
            __agl_Context.Input.MouseX = GET_X_LPARAM(lParam);
            __agl_Context.Input.MouseY = GET_Y_LPARAM(lParam);
            return 0;
        }break;
        case WM_MBUTTONUP: { __agl_Context.Input.Buttons[2].EndedDown = true; return 0;}
        case WM_RBUTTONUP: { __agl_Context.Input.Buttons[1].EndedDown = true; return 0;}
        case WM_LBUTTONUP: { __agl_Context.Input.Buttons[0].EndedDown = true; return 0;}
        case WM_MBUTTONDOWN: { __agl_Context.Input.Buttons[2].Count++; return 0;}
        case WM_RBUTTONDOWN: { __agl_Context.Input.Buttons[1].Count++; return 0;}
        case WM_LBUTTONDOWN: { __agl_Context.Input.Buttons[0].Count++; return 0;}
            /*
        {
            if(wParam & MK_LBUTTON) 
            if(wParam & MK_RBUTTON) __agl_Context.Input.Buttons[1].Count++;
            if(wParam & MK_MBUTTON) __agl_Context.Input.Buttons[2].Count++;          


            __agl_Context.Input.Buttons[1].EndedDown = !(wParam & MK_RBUTTON);
            if(!__agl_Context.Input.Buttons[1].EndedDown) __agl_Context.Input.Buttons[1].Count++;

            __agl_Context.Input.Buttons[2].EndedDown = !(wParam & MK_MBUTTON);
            if(!__agl_Context.Input.Buttons[2].EndedDown) __agl_Context.Input.Buttons[2].Count++;
            
            if(wParam & MK_LBUTTON || wParam & MK_MBUTTON || wParam & MK_RBUTTON) SetCapture(__agl_Context.Platform.HWnd);
            if(!(wParam & MK_LBUTTON) && !(wParam & MK_MBUTTON) && !(wParam & MK_RBUTTON)) ReleaseCapture();

            return 0;
        }break;
            */
        case WM_SIZE:
        {
            __agl_Context.Width = LOWORD(lParam);
            __agl_Context.Height = HIWORD(lParam);
            if(aglResizeCallback) aglResizeCallback(LOWORD(lParam), HIWORD(lParam));
            return 0;
        }break;
    }
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

AGLDEF u32
aglPlatformGetTicks()
{
    if (!__agl_Context.Platform.TimerInit) {
        QueryPerformanceFrequency(&__agl_Context.Platform.Frequency);
        QueryPerformanceCounter(&__agl_Context.Platform.Init);
        __agl_Context.Platform.TimerInit = true;
    }
    
    LARGE_INTEGER Now;
    QueryPerformanceCounter(&Now);
    Now.QuadPart -= __agl_Context.Platform.Init.QuadPart;
    Now.QuadPart *= 1000;
    Now.QuadPart /= __agl_Context.Platform.Frequency.QuadPart;
    return (u32) Now.QuadPart;
}

AGLDEF void
aglPlatformSetCursor(b32 Show)
{
    CURSORINFO CursorInfo;
    CursorInfo.cbSize = sizeof(CURSORINFO);
    GetCursorInfo(&CursorInfo);
    
    if(CursorInfo.flags == 0 && Show) ShowCursor(true);
    else if(CursorInfo.flags == CURSOR_SHOWING && !Show) ShowCursor(false);
}

AGLDEF void
aglPlatformDestroyWindow()
{
    DestroyWindow(__agl_Context.Platform.HWnd);
    
    if(!UnregisterClass(AGL_WND_CLASS_NAME, GetModuleHandle(0)))
    {
        MessageBox(NULL,"Unregister failed..","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);  
    }   
    
    if (__agl_Context.GLContext)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(__agl_Context.Platform.DC, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);            
		}
        
		if (!wglDeleteContext(__agl_Context.GLContext))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
        __agl_Context.GLContext = 0;    
    }
    
}

// WGLisExtensionSupported: This Is A Form Of The Extension For WGL
AGLDEF b32
aglPlatformIsExtensionSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = NULL;

	// Try To Use wglGetExtensionStringARB On Current DC, If Possible
	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

	if (wglGetExtString)
		supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

	// If That Failed, Try Standard Opengl Extensions String
	if (supported == NULL)
		supported = (char*)glGetString(GL_EXTENSIONS);

	// If That Failed Too, Must Be No Extensions Supported
	if (supported == NULL)
		return false;

	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (const char* p = supported; ; p++)
	{
		// Advance p Up To The Next Possible Match
		p = strstr(p, extension);

		if (p == NULL)
			return false;															// No Match

		// Make Sure That Match Is At The Start Of The String Or That
		// The Previous Char Is A Space, Or Else We Could Accidentally
		// Match "wglFunkywglExtension" With "wglExtension"

		// Also, Make Sure That The Following Character Is Space Or NULL
		// Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
			return true;															// Match
	}
}

// InitMultisample: Used To Query The Multisample Frequencies
AGLDEF b32
aglPlatformInitMultisample(HINSTANCE hInstance,HWND hWnd,PIXELFORMATDESCRIPTOR pfd)
{  
    // See If The String Exists In WGL!
	if (!aglPlatformIsExtensionSupported("WGL_ARB_multisample"))
	{
		__agl_Context.MultisampleSupported = false;
		return false;
	}

	// Get Our Pixel Format
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");	
	if (!wglChoosePixelFormatARB) 
	{
		__agl_Context.MultisampleSupported = false;
		return false;
	}

	// Get Our Current Device Context
	HDC hDC = GetDC(hWnd);

	int		pixelFormat;
	int		valid;
	UINT	numFormats;
	float	fAttributes[] = {0,0};

	// These Attributes Are The Bits We Want To Test For In Our Sample
	// Everything Is Pretty Standard, The Only One We Want To 
	// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
	// These Two Are Going To Do The Main Testing For Whether Or Not
	// We Support Multisampling On This Hardware.
	int iAttributes[] =
        {
            WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
            WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB,24,
            WGL_ALPHA_BITS_ARB,8,
            WGL_DEPTH_BITS_ARB,16,
            WGL_STENCIL_BITS_ARB,0,
            WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
            WGL_SAMPLES_ARB, 4,
            0,0
        };

	// First We Check To See If We Can Get A Pixel Format For 4 Samples
	valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
 
	// If We Returned True, And Our Format Count Is Greater Than 1
	if (valid && numFormats >= 1)
	{
		__agl_Context.MultisampleSupported = true;
		__agl_Context.MultisampleFormat = pixelFormat;	
		return __agl_Context.MultisampleSupported;
	}

	// Our Pixel Format With 4 Samples Failed, Test For 2 Samples
	iAttributes[19] = 2;
	valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
	if (valid && numFormats >= 1)
	{
		__agl_Context.MultisampleSupported = true;
		__agl_Context.MultisampleFormat = pixelFormat;	 
		return __agl_Context.MultisampleSupported;
	}
	  
	// Return The Valid Format
	return __agl_Context.MultisampleSupported;
}

AGLDEF b32
aglPlatformCreateWindow(char *Title)
{    
    u32 		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Value
    
	WindowRect.left     = (long) 0;
	WindowRect.right    = (long) __agl_Context.Width;
	WindowRect.top      = (long) 0;
	WindowRect.bottom   = (long) __agl_Context.Height;

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= GetModuleHandle(0);					// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= AGL_WND_CLASS_NAME;					    // Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
	}

    dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
    dwStyle=WS_OVERLAPPEDWINDOW;
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	if (!(__agl_Context.Platform.HWnd = CreateWindowEx(	0,							// Extended Style For The Window
                                            AGL_WND_CLASS_NAME,							// Class Name
                                            Title,								// Window Title
                                            dwStyle |							// Defined Window Style
                                            WS_CLIPSIBLINGS |					// Required Window Style
                                            WS_CLIPCHILDREN,					// Required Window Style
                                            0, 0,								// Window Position
                                            WindowRect.right-WindowRect.left,	// Calculate Window Width
                                            WindowRect.bottom-WindowRect.top,	// Calculate Window Height
                                            NULL,								// No Parent Window
                                            NULL,								// No Menu
                                            wc.hInstance,							// Instance
                                            0)))								// Dont Pass Anything To WM_CREATE
	{
        aglPlatformDestroyWindow();	
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
        {
            sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
            1,											// Version Number
            PFD_DRAW_TO_WINDOW |						// Format Must Support Window
            PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
            PFD_DOUBLEBUFFER,							// Must Support Double Buffering
            PFD_TYPE_RGBA,								// Request An RGBA Format
            32,										// Select Our Color Depth
            0, 0, 0, 0, 0, 0,							// Color Bits Ignored
            0,											// No Alpha Buffer
            0,											// Shift Bit Ignored
            0,											// No Accumulation Buffer
            0, 0, 0, 0,									// Accumulation Bits Ignored
            24,											// 16Bit Z-Buffer (Depth Buffer)  
            8,											// No Stencil Buffer
            0,											// No Auxiliary Buffer
            PFD_MAIN_PLANE,								// Main Drawing Layer
            0,											// Reserved
            0, 0, 0										// Layer Masks Ignored
        };
    PIXELFORMATDESCRIPTOR *ppfd = &pfd;
    
	if (!(__agl_Context.Platform.DC = GetDC(__agl_Context.Platform.HWnd)))
	{
        aglPlatformDestroyWindow();	
		MessageBox(NULL,"Can't Create A GL Device __agl_Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

    if(!__agl_Context.MultisampleSupported)
    {
        if (!(PixelFormat=ChoosePixelFormat(__agl_Context.Platform.DC, ppfd)))
        {
            aglPlatformDestroyWindow();	
            MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
            return false;
        }
    }
    else
    {
        PixelFormat = __agl_Context.MultisampleFormat;
    }
    
	if(!SetPixelFormat(__agl_Context.Platform.DC, PixelFormat, ppfd))
	{
        aglPlatformDestroyWindow();	
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
	}

    if(!__agl_Context.GLContext)
    {

        if (!(__agl_Context.GLContext = wglCreateContext(__agl_Context.Platform.DC)))
        {
            aglPlatformDestroyWindow();	
            MessageBox(NULL,"Can't Create A GL Rendering __agl_Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);        
        }  
        
        if(!wglMakeCurrent(__agl_Context.Platform.DC, __agl_Context.GLContext))
        {
            aglPlatformDestroyWindow();	
            MessageBox(NULL,"Can't Activate The GL Rendering __agl_Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        }       
        
    }
    
    if(__agl_Context.EnableMSAA && !__agl_Context.MultisampleSupported)
    {
        if(aglPlatformInitMultisample(GetModuleHandle(0), __agl_Context.Platform.HWnd, pfd))
        {
            aglPlatformDestroyWindow();
            return aglPlatformCreateWindow(Title);
        }
    }

    aglPlatformSetVerticalSync(__agl_Context.VerticalSync);
    
	ShowWindow(__agl_Context.Platform.HWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(__agl_Context.Platform.HWnd);						// Slightly Higher Priority
    SetFocus(__agl_Context.Platform.HWnd);									// Sets Keyboard Focus To The Window
    return true;
}

AGLDEF void
aglPlatformSetVerticalSync(b32 State)
{
    PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapInterval = (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress("wglGetSwapIntervalEXT");
    PFNWGLSWAPINTERVALEXTPROC wglSwapInterval = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
    if(__agl_Context.GLContext && wglSwapInterval) wglSwapInterval(State);
    if(wglGetSwapInterval) __agl_Context.VerticalSync = wglGetSwapInterval();
}

AGLDEF void aglPlatformSwapBuffers()
{
    glFinish();
    SwapBuffers(__agl_Context.Platform.DC);
}

AGLDEF void aglPlatformCloseWindow() { DestroyWindow(__agl_Context.Platform.HWnd); }
AGLDEF void aglPlatformSetWindowTitle(char *Title) { SetWindowText(__agl_Context.Platform.HWnd, Title); }

AGLDEF b32 aglPlatformIsActive()
{
    return __agl_Context.Active;
}

AGLDEF void
aglPlatformCaptureMouse(b32 Capture)
{
    if(Capture) SetCapture(__agl_Context.Platform.HWnd); else ReleaseCapture();
};

#else // any other platform

#endif

#define AGL_H
#endif
