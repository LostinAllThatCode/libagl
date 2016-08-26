#if !defined(AGL_H)

#ifndef AGL_INTERNAL_TYPES
    #include <stdint.h>
    #include <stddef.h>
    #include <limits.h>
    #include <float.h>
    
    typedef int8_t     ss8;  
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
typedef GLuint (AGLAPIP PFNGLCREATESHADERPROC) (GLenum type);
typedef void   (AGLAPIP PFNGLDELETESHADERPROC) (GLuint shader);
typedef void   (AGLAPIP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length);
typedef void   (AGLAPIP PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void   (AGLAPIP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void   (AGLAPIP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef GLuint (AGLAPIP PFNGLCREATEPROGRAMPROC) (void);
typedef void   (AGLAPIP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void   (AGLAPIP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void   (AGLAPIP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void   (AGLAPIP PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void   (AGLAPIP PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void   (AGLAPIP PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void   (AGLAPIP PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef GLint  (AGLAPIP PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void   (AGLAPIP PFNGLUNIFORMMATRIX3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void   (AGLAPIP PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void   (AGLAPIP PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void   (AGLAPIP PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void   (AGLAPIP PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void   (AGLAPIP PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void   (AGLAPIP PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void   (AGLAPIP PFNGLUNIFORM2IPROC) (GLint location, GLint v0, GLint v1);
typedef void   (AGLAPIP PFNGLUNIFORM3IPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void   (AGLAPIP PFNGLUNIFORM4IPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void   (AGLAPIP PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void   (AGLAPIP PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void   (AGLAPIP PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void   (AGLAPIP PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
typedef void   (AGLAPIP PFNGLBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
typedef void   (AGLAPIP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void   (AGLAPIP PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
typedef void   (AGLAPIP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef void   (AGLAPIP PFNGLENABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void   (AGLAPIP PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void   (AGLAPIP PFNGLVERTEXATTRIBPOINTERARBPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void   (AGLAPIP PFNGLVERTEXATTRIBDIVISORARBPROC) (GLuint index, GLuint divisor);
typedef void   (AGLAPIP PFNGLDRAWARRAYSINSTANCEDARBPROC) (GLenum mode, GLint first, GLsizei count, GLsizei primcount);
typedef void   (AGLAPIP PFNGLGENERATEMIPMAPPROC) (GLenum target);

#if !defined(AGL_NO_GLPROCS)
PFNGLCREATESHADERPROC                  glCreateShader;
PFNGLDELETESHADERPROC                  glDeleteShader;
PFNGLSHADERSOURCEPROC                  glShaderSource;
PFNGLCOMPILESHADERPROC                 glCompileShader;
PFNGLGETSHADERIVPROC                   glGetShader;
PFNGLGETSHADERINFOLOGPROC              glGetShaderInfoLog;
PFNGLCREATEPROGRAMPROC                 glCreateProgram;
PFNGLGETPROGRAMIVPROC                  glGetProgram;
PFNGLGETPROGRAMINFOLOGPROC             glGetProgramInfoLog;
PFNGLATTACHSHADERPROC                  glAttachShader;
PFNGLDETACHSHADERPROC                  glDetachShader;
PFNGLLINKPROGRAMPROC                   glLinkProgram;
PFNGLUSEPROGRAMPROC                    glUseProgram;
PFNGLACTIVETEXTUREARBPROC              glActiveTexture;
PFNGLGETUNIFORMLOCATIONPROC            glGetUniformLocation;
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
PFNGLGENBUFFERSARBPROC                 glGenBuffers;
PFNGLBINDBUFFERARBPROC                 glBindBuffer;
PFNGLBUFFERDATAARBPROC                 glBufferData;
PFNGLBUFFERSUBDATAARBPROC              glBufferSubData;
PFNGLDELETEBUFFERSARBPROC              glDeleteBuffers; 
PFNGLGENVERTEXARRAYSPROC               glGenVertexArrays;
PFNGLDELETEVERTEXARRAYSPROC            glDeleteVertexArrays;
PFNGLBINDVERTEXARRAYPROC               glBindVertexArray;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC    glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERARBPROC        glVertexAttribPointer;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC   glDisableVertexAttribArray;
PFNGLGENERATEMIPMAPPROC                glGenerateMipmap;
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
        glGetShader                        = (PFNGLGETSHADERIVPROC)                 aglGetProcAddress("glGetShaderiv");
        glGetShaderInfoLog                 = (PFNGLGETSHADERINFOLOGPROC)            aglGetProcAddress("glGetShaderInfoLog");
        glCreateProgram                    = (PFNGLCREATEPROGRAMPROC)               aglGetProcAddress("glCreateProgram");
        glGetProgram                       = (PFNGLGETPROGRAMIVPROC)                aglGetProcAddress("glGetProgramiv");
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
        glDeleteBuffers                    = (PFNGLDELETEBUFFERSARBPROC)            aglGetProcAddress("glDeleteBuffersARB");
        glBufferData                       = (PFNGLBUFFERDATAARBPROC)               aglGetProcAddress("glBufferDataARB");
        glBufferSubData                    = (PFNGLBUFFERSUBDATAARBPROC)            aglGetProcAddress("glBufferSubDataARB");
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
    
    __agl_Context.Delta = (Now - __agl_Context.TicksLastFrame) / 1000.f;;
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
    __agl_Context.Input.MouseXDelta = 0;
    __agl_Context.Input.MouseYDelta = 0;
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
            s32 X = GET_X_LPARAM(lParam);
            s32 Y = GET_Y_LPARAM(lParam);
            __agl_Context.Input.MouseXDelta = X - __agl_Context.Input.MouseX;
            __agl_Context.Input.MouseYDelta = Y - __agl_Context.Input.MouseY;
            __agl_Context.Input.MouseX = X;
            __agl_Context.Input.MouseY = Y;
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
#if 0
AGLDEF void aglPlatformBeginFrame()
{
    __agl_Context.Platform.TickBegin = __rdtsc();
    QueryPerformanceCounter(&__agl_Context.Platform.FrameBegin);
};

AGLDEF void aglPlatformEndFrame() {
    __agl_Context.Platform.TickEnd = __rdtsc();
    QueryPerformanceCounter(&__agl_Context.Platform.FrameEnd);
    
    LARGE_INTEGER ElapsedTime;
    ElapsedTime.QuadPart = __agl_Context.Platform.FrameEnd.QuadPart - __agl_Context.Platform.FrameBegin.QuadPart;
    ElapsedTime.QuadPart *= 1000000;
    ElapsedTime.QuadPart /= __agl_Context.Platform.Frequency.QuadPart;

    __agl_Context.Ticks = __agl_Context.Platform.TickEnd - __agl_Context.Platform.TickBegin;
    __agl_Context.Delta = ElapsedTime.QuadPart / 1000000.0f;
    __agl_Context.FPS = 1.0f / __agl_Context.Delta;
    __agl_Context.Time += __agl_Context.Delta;

    __agl_Context.Input.MouseXDelta = 0;
    __agl_Context.Input.dY = 0;
};
#endif
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
