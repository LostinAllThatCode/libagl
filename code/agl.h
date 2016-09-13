#ifndef AGL_H

#ifdef AGL_EXTERN
#ifdef(__cplusplus) 
    #define AGLDEF extern "C"
#else
    #define AGLDEF extern
#endif
#else
    #define AGLDEF static
#endif
    
#ifdef AGL_IMPLEMENTATION  // AGL_IMPLEMENTATION

#ifndef AGL_WINDOW_TITLE
#define AGL_WINDOW_TITLE "libagl opengl wrapper"
#endif

#ifndef AGL_WINDOW_WIDTH
#ifndef AGL_WINDOW_HEIGHT
#define AGL_WINDOW_WIDTH  1024
#define AGL_WINDOW_HEIGHT 768
#endif
#endif
    
#ifndef AGL_INIT
#define AGL_INIT        aglInitStub
#endif
    
#ifndef AGL_CLEANUP
#define AGL_CLEANUP     aglCleanUpStub
#endif
    
#ifndef AGL_RESIZE
#define AGL_RESIZE      aglResizeStub
#endif
    
#ifndef AGL_KEYDOWN
#define AGL_KEYDOWN     aglKeyDownStub
#endif
    
#ifndef AGL_KEYUP
#define AGL_KEYUP       aglKeyUpStub
#endif
    
#ifdef AGL_DEBUG
#define AGL_MAIN int main(int argc, char *argv[])
#endif

#ifndef AGL_LOOP        
/* 
   Simple example of how your code should atleast look like to work with this lib.

   You need to define the AGL_LOOP macro before including agl.h
   Example for a simple open gl application:
           
   #define AGL_IMPLEMENTATION
   #define AGL_OPENGL_VSYNC
   #define AGL_OPENGL_MSAA
   #define AGL_OPENGL_MSAA_SAMPLES      8
   #define AGL_WINDOW_WIDTH             480
   #define AGL_WINDOW_HEIGHT            480
   #define AGL_WINDOW_TITLE             "OpenGL context with multisampling and vsync"
   #define AGL_INIT                     init
   #define AGL_KEYUP                    keyup
   #define AGL_RESIZE                   resize
   #define AGL_LOOP                     gameloop
   #include "agl.h"

   #define _USE_MATH_DEFINES
   #include <math.h>

   #include <stdio.h>

   float color[3] = {1, 1, 1};
   b32 clear_screen = true;
   r32 time;

   void resize(int w, int h)
   {
       glViewport(0, 0, h, h);
   }

   void draw_circle(GLfloat x, GLfloat y, GLfloat radius, int tricount){
       int i;
       int triangleAmount = tricount;
       GLfloat twicePi = 2.0f * M_PI;
	
       glBegin(GL_TRIANGLE_FAN);
       glVertex2f(x, y);
       for(i = 0; i <= triangleAmount;i++)
       {
           glVertex2f(x + (radius * cos(i *  twicePi / triangleAmount)), 
           y + (radius * sin(i * twicePi / triangleAmount)));
       }
       glEnd();
   }

   void init(agl_context *c)
   {
       glClearColor(0.125f, 0.125f, 0.125f, 1.f);
   }

   void keyup(char k)
   {
       switch(k)
       {
           case VK_ESCAPE : { aglCloseWindow(); } break;
           case 'C'       : { clear_screen = !clear_screen; } break;
       }
   }

   void
   gameloop(float Delta)
   {
       time += Delta;  
       if(clear_screen) glClear(GL_COLOR_BUFFER_BIT);    
       glColor3f(sinf(time * 2.0f), sinf(time * .7f), sinf(time * 1.3f));
       draw_circle(0.f, 0.f, sinf(time) * .25f, 128);
       draw_circle(sinf(time* 2.f) * .5f, cosf(time) * .5f, .0125f, 16);
       draw_circle(sinf(time) * .5f, cosf(time) * .5f, .0125f, 16);
       draw_circle(sinf(time * 4.f) * .25f, cosf(time) * .75f, .0125f, 16);
   }
*/
#error No mainloop is defined.
#endif

#ifdef AGL_DEBUG
#include <stdio.h>
#define AGL_PRINT(s,...) printf("%s (%i): "s, __FILE__, __LINE__, __VA_ARGS__)
#ifndef AGL_ASSERT
#include <assert.h>
#define AGL_ASSERT(Condition) assert(Condition)
#endif
#else
#define AGL_PRINT
#define AGL_ASSERT(Condition)
#endif

#ifdef _WIN32 // _WIN32

#ifndef AGL_MAIN
#define AGL_MAIN int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif

#ifndef AGL_WIN32_CLASSNAME
#define AGL_WIN32_CLASSNAME L"win32_libagl_window_class"
#endif

#include <windows.h>
#include <GL\GL.h>
        
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

typedef int (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const float *pfAttribFList, unsigned int nMaxFormats, int *piFormats, unsigned int *nNumFormats);
typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef int (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef int (WINAPI * PFNWGLGETSWAPINTERVALEXTPROC) (void);

#define WGL_DRAW_TO_WINDOW_ARB                      0x2001
#define WGL_ACCELERATION_ARB                        0x2003
#define WGL_SUPPORT_OPENGL_ARB                      0x2010
#define WGL_DOUBLE_BUFFER_ARB                       0x2011
#define WGL_COLOR_BITS_ARB                          0x2014
#define WGL_ALPHA_BITS_ARB                          0x201B
#define WGL_DEPTH_BITS_ARB                          0x2022
#define WGL_STENCIL_BITS_ARB                        0x2023
#define WGL_FULL_ACCELERATION_ARB                   0x2027    
#define WGL_SAMPLE_BUFFERS_ARB                      0x2041
#define WGL_SAMPLES_ARB                             0x2042
#define WGL_SWAP_METHOD_ARB                         0x2007
#define WGL_SWAP_EXCHANGE_ARB                       0x2028
#define WGL_CONTEXT_DEBUG_BIT_ARB                   0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB      0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB               0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB               0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB                 0x2093
#define WGL_CONTEXT_FLAGS_ARB                       0x2094
#define ERROR_INVALID_VERSION_ARB                   0x2095
#define WGL_CONTEXT_PROFILE_MASK_ARB                0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB            0x00000001      

static PFNWGLCHOOSEPIXELFORMATARBPROC        wglChoosePixelFormat;
static PFNWGLSWAPINTERVALEXTPROC             wglSwapInterval;
static PFNWGLGETSWAPINTERVALEXTPROC          wglGetSwapInterval;
static PFNWGLCREATECONTEXTATTRIBSARBPROC     wglCreateContextAttribs;

typedef struct 
{
    HWND              HWnd;
    HDC               DC;
    WINDOWPLACEMENT   Placement;
    int               TimerInit;              
    LARGE_INTEGER     Init, Frequency;
    HGLRC             GLContext;
} agl_platform;

// _WIN32
#elif defined(__unix)     // __unix
#elif defined(__linux__)  // __linux__
#elif defined(__APPLE)    // __APPLE
#else
#endif // PLATFORM LAYER DECLARATIONS

#else
typedef void* agl_platform;
#endif // AGL_IMPLEMENTATION


#ifdef AGL_OPENGL_VSYNC
#define AGL_OPENGL_VSYNC_ENABLE 1
#else
#define AGL_OPENGL_VSYNC_ENABLE 0
#endif
    
#ifdef AGL_OPENGL_MSAA
#define AGL_OPENGL_MSAA_ENABLE 1
    #ifndef AGL_OPENGL_MSAA_SAMPLES
    #define AGL_OPENGL_MSAA_SAMPLES 4
    #endif
#else
#define AGL_OPENGL_MSAA_ENABLE 0
#define AGL_OPENGL_MSAA_SAMPLES 0
#endif

#ifndef AGL_OPENGL_MINOR_VERSION
#define AGL_OPENGL_MINOR_VERSION 1
#endif
        
#ifndef AGL_OPENGL_MAJOR_VERSION
#define AGL_OPENGL_MAJOR_VERSION 3
#else
#if AGL_OPENGL_MAJOR_VERSION >= 4
#pragma message ( "Warning: OpenGL immediate mode is no longer supported! Please be aware of this!")
#endif
#endif

#ifndef AGL_OPENGL_EXCLUDE_GLEXT
#include "glext.h"
#endif

#define AGL_OPENGL_EXTENSIONS_CORE                                      \
    AGL_OPENGL_FUNC_DEF(GENERATEMIPMAP           , GenerateMipmap      ) \
    AGL_OPENGL_FUNC_DEF(GENVERTEXARRAYS          , GenVertexArrays     ) \
    AGL_OPENGL_FUNC_DEF(DELETEVERTEXARRAYS       , DeleteVertexArrays  ) \
    AGL_OPENGL_FUNC_DEF(BINDVERTEXARRAY          , BindVertexArray     ) \
    AGL_OPENGL_FUNC_DEF(GENFRAMEBUFFERS          , GenFramebuffers     ) \
    AGL_OPENGL_FUNC_DEF(DELETEFRAMEBUFFERS       , DeleteFramebuffers  ) \
    AGL_OPENGL_FUNC_DEF(BINDFRAMEBUFFER          , BindFramebuffer     ) \
    AGL_OPENGL_FUNC_DEF(FRAMEBUFFERTEXTURE2D     , FramebufferTexture2D)   

#define AGL_OPENGL_EXTENSIONS_DEF_OR_ARB                                \
    AGL_OPENGL_FUNC_DEF(BINDATTRIBLOCATION       , BindAttribLocation  ) \
    AGL_OPENGL_FUNC_DEF(COMPILESHADER            , CompileShader       ) \
    AGL_OPENGL_FUNC_DEF(SHADERSOURCE             , ShaderSource        ) \
    AGL_OPENGL_FUNC_DEF(LINKPROGRAM              , LinkProgram         ) \
    AGL_OPENGL_FUNC_DEF(GETUNIFORMLOCATION       , GetUniformLocation  ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM1F                , Uniform1f           ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM2F                , Uniform2f           ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM3F                , Uniform3f           ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM4F                , Uniform4f           ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM1I                , Uniform1i           ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM2I                , Uniform2i           ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM3I                , Uniform3i           ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM4I                , Uniform4i           ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM1FV               , Uniform1fv          ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM2FV               , Uniform2fv          ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM3FV               , Uniform3fv          ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM4FV               , Uniform4fv          ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM1IV               , Uniform1iv          ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM2IV               , Uniform2iv          ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM3IV               , Uniform3iv          ) \
    AGL_OPENGL_FUNC_DEF(UNIFORM4IV               , Uniform4iv          ) \
    AGL_OPENGL_FUNC_DEF(UNIFORMMATRIX2FV         , UniformMatrix2fv    ) \
    AGL_OPENGL_FUNC_DEF(UNIFORMMATRIX3FV         , UniformMatrix3fv    ) \
    AGL_OPENGL_FUNC_DEF(UNIFORMMATRIX3FV         , UniformMatrix4fv    ) \
    AGL_OPENGL_FUNC_DEF(ENABLEVERTEXATTRIBARRAY  , EnableVertexAttribArray ) \
    AGL_OPENGL_FUNC_DEF(DISABLEVERTEXATTRIBARRAY , DisableVertexAttribArray ) \
    AGL_OPENGL_FUNC_DEF(VERTEXATTRIBPOINTER      , VertexAttribPointer ) \
    AGL_OPENGL_FUNC_DEF(GENBUFFERS               , GenBuffers          ) \
    AGL_OPENGL_FUNC_DEF(BINDBUFFER               , BindBuffer          ) \
    AGL_OPENGL_FUNC_DEF(BUFFERDATA               , BufferData          ) \
    AGL_OPENGL_FUNC_DEF(BUFFERSUBDATA            , BufferSubData       ) \
    AGL_OPENGL_FUNC_DEF(ACTIVETEXTURE            , ActiveTexture       )       

#ifdef AGL_OPENGL_EXTENSIONS_USE_ARB
#define AGL_OPENGL_EXTENSIONS_DYNAMIC                                   \
    AGL_OPENGL_FUNC_DEF(ATTACHOBJECT        , AttachObject        )     \
    AGL_OPENGL_FUNC_DEF(CREATEPROGRAMOBJECT , CreateProgramObject )     \
    AGL_OPENGL_FUNC_DEF(CREATESHADEROBJECT  , CreateShaderObject  )     \
    AGL_OPENGL_FUNC_DEF(DELETEOBJECT        , DeleteObject        )     \
    AGL_OPENGL_FUNC_DEF(DETACHOBJECT        , DetachObject        )     \
    AGL_OPENGL_FUNC_DEF(USEPROGRAMOBJECT    , UseProgramObject    )     \
    AGL_OPENGL_FUNC_DEF(GETATTACHEDOBJECTS  , GetAttachedObjects  )     \
    AGL_OPENGL_FUNC_DEF(GETOBJECTPARAMETERIV, GetObjectParameteriv)     \
    AGL_OPENGL_FUNC_DEF(GETINFOLOG          , GetInfoLog          )      
#else
#define AGL_OPENGL_EXTENSIONS_DYNAMIC                                   \
    AGL_OPENGL_FUNC_DEF(ATTACHSHADER        , AttachShader        )     \
    AGL_OPENGL_FUNC_DEF(CREATEPROGRAM       , CreateProgram       )     \
    AGL_OPENGL_FUNC_DEF(DELETEPROGRAM       , DeleteProgram       )     \
    AGL_OPENGL_FUNC_DEF(CREATESHADER        , CreateShader        )     \
    AGL_OPENGL_FUNC_DEF(DELETESHADER        , DeleteShader        )     \
    AGL_OPENGL_FUNC_DEF(DETACHSHADER        , DetachShader        )     \
    AGL_OPENGL_FUNC_DEF(USEPROGRAM          , UseProgram          )     \
    AGL_OPENGL_FUNC_DEF(GETATTACHEDSHADERS  , GetAttachedShaders  )     \
    AGL_OPENGL_FUNC_DEF(GETSHADERIV         , GetShaderiv         )     \
    AGL_OPENGL_FUNC_DEF(GETSHADERINFOLOG    , GetShaderInfoLog    )     \
    AGL_OPENGL_FUNC_DEF(GETPROGRAMIV        , GetProgramiv        )     \
    AGL_OPENGL_FUNC_DEF(GETPROGRAMINFOLOG   , GetProgramInfoLog   )            
#endif

#define AGL_OPENGL_FUNC_DEF(x,y)  static PFNGL##x##PROC __gl##y;
AGL_OPENGL_EXTENSIONS_CORE

#ifdef AGL_OPENGL_EXTENSIONS_USE_ARB
#undef AGL_OPENGL_FUNC_DEF
#define AGL_OPENGL_FUNC_DEF(x,y)  static PFNGL##x##ARBPROC __gl##y##ARB;
#endif

AGL_OPENGL_EXTENSIONS_DEF_OR_ARB
AGL_OPENGL_EXTENSIONS_DYNAMIC
#undef AGL_OPENGL_FUNC_DEF
static int __active_program;

#ifdef _WIN32
#define AGL_GET_PROC(x) wglGetProcAddress(x)
#endif



void
aglInitProcs()
{
    static int IsInitialized = 0;
    if(IsInitialized) return;
    IsInitialized = 1;
    
    #define AGL_OPENGL_FUNC_DEF(x,y) __gl##y = (PFNGL##x##PROC) AGL_GET_PROC("gl" #y);
    AGL_OPENGL_EXTENSIONS_CORE
        
    #ifdef AGL_OPENGL_EXTENSIONS_USE_ARB
    #undef AGL_OPENGL_FUNC_DEF
    #define AGL_OPENGL_FUNC_DEF(x,y) __gl##y##ARB = (PFNGL##x##ARBPROC) AGL_GET_PROC("gl" #y "ARB");
    #endif
        
    AGL_OPENGL_EXTENSIONS_DEF_OR_ARB
    AGL_OPENGL_EXTENSIONS_DYNAMIC
    #undef AGL_OPENGL_FUNC_DEF

}
#undef AGL_OPENGL_EXTENSIONS_DYNAMIC

// Remapping glXXX commands since there are different versions (ARB/EXT)
// Only the new functions for shader support and vbo/ibo/vao and some others
#define glGenerateMipmap             __glGenerateMipmap
#define glGenVertexArrays            __glGenVertexArrays
#define glDeleteVertexArrays         __glDeleteVertexArrays
#define glBindVertexArray            __glBindVertexArray
#define glGenFramebuffers            __glGenFramebuffers
#define glDeleteFramebuffers         __glDeleteFramebuffers
#define glBindFramebuffer            __glBindFramebuffer 
#define glFramebufferTexture2D       __glFramebufferTexture2D

#ifdef AGL_OPENGL_EXTENSIONS_USE_ARB
#define glCreateShader               __glCreateShaderObjectARB
#define glDeleteShader               __glDeleteObjectARB
#define glAttachShader               __glAttachObjectARB
#define glDetachShader               __glDetachObjectARB
#define glShaderSource               __glShaderSourceARB
#define glCompileShader              __glCompileShaderARB
#define glGetShaderStatus(a,b)       __glGetObjectParameterivARB(a, GL_OBJECT_COMPILE_STATUS_ARB, b)
#define glGetShaderInfoLog           __glGetInfoLogARB
#define glCreateProgram              __glCreateProgramObjectARB
#define glDeleteProgram              __glDeleteObjectARB
#define glLinkProgram                __glLinkProgramARB
#define glGetProgramStatus(a,b)      __glGetObjectParameterivARB(a, GL_OBJECT_LINK_STATUS_ARB, b)
#define glGetProgramInfoLog          __glGetInfoLogARB
#define glGetAttachedShaders         __glGetAttachedObjectsARB
#define glBindAttribLocation         __glBindAttribLocationARB
#define glGetUniformLocation         __glGetUniformLocationARB
#define glUseProgram                 __glUseProgramObjectARB
#define glUniform1f                  __glUniform1fARB
#define glUniform2f                  __glUniform2fARB
#define glUniform3f                  __glUniform3fARB
#define glUniform4f                  __glUniform4fARB
#define glUniform1i                  __glUniform1iARB
#define glUniform2i                  __glUniform2iARB
#define glUniform3i                  __glUniform3iARB
#define glUniform4i                  __glUniform4iARB
#define glUniform1fv                 __glUniform1fvARB
#define glUniform2fv                 __glUniform2fvARB
#define glUniform3fv                 __glUniform3fvARB
#define glUniform4fv                 __glUniform4fvARB
#define glUniform1iv                 __glUniform1ivARB
#define glUniform2iv                 __glUniform2ivARB
#define glUniform3iv                 __glUniform3ivARB
#define glUniform4iv                 __glUniform4ivARB
#define glUniformMatrix2fv           __glUniformMatrix2fvARB
#define glUniformMatrix3fv           __glUniformMatrix3fvARB
#define glUniformMatrix4fv           __glUniformMatrix4fvARB
#define glGenBuffers                 __glGenBuffersARB
#define glBufferData                 __glBufferDataARB
#define glBufferSubData              __glBufferSubDataARB
#define glBindBuffer                 __glBindBufferARB
#define glEnableVertexAttribArray    __glEnableVertexAttribArrayARB
#define glDisableVertexAttribArray   __glDisableVertexAttribArrayARB
#define glVertexAttribPointer        __glVertexAttribPointerARB
#define glActiveTexture              __glActiveTextureARB
#else
#define glCreateShader               __glCreateShader
#define glDeleteShader               __glDeleteShader
#define glAttachShader               __glAttachShader
#define glDetachShader               __glDetachShader
#define glShaderSource               __glShaderSource
#define glCompileShader              __glCompileShader
#define glGetShaderStatus(a,b)       __glGetShaderiv(a, GL_COMPILE_STATUS, b)
#define glGetShaderInfoLog           __glGetShaderInfoLog
#define glCreateProgram              __glCreateProgram
#define glDeleteProgram              __glDeleteProgram
#define glLinkProgram                __glLinkProgram
#define glGetProgramStatus(a,b)      __glGetProgramiv(a, GL_LINK_STATUS, b)
#define glGetProgramInfoLog          __glGetProgramInfoLog
#define glGetAttachedShaders         __glGetAttachedShaders
#define glBindAttribLocation         __glBindAttribLocation
#define glUseProgram                 __glUseProgram
#define glGetUniformLocation         __glGetUniformLocation
#define glUniform1f                  __glUniform1f
#define glUniform2f                  __glUniform2f
#define glUniform3f                  __glUniform3f
#define glUniform4f                  __glUniform4f
#define glUniform1i                  __glUniform1i
#define glUniform2i                  __glUniform2i
#define glUniform3i                  __glUniform3i
#define glUniform4i                  __glUniform4i
#define glUniform1fv                 __glUniform1fv
#define glUniform2fv                 __glUniform2fv
#define glUniform3fv                 __glUniform3fv
#define glUniform4fv                 __glUniform4fv
#define glUniform1iv                 __glUniform1iv
#define glUniform2iv                 __glUniform2iv
#define glUniform3iv                 __glUniform3iv
#define glUniform4iv                 __glUniform4iv
#define glUniformMatrix2fv           __glUniformMatrix2fv
#define glUniformMatrix3fv           __glUniformMatrix3fv
#define glUniformMatrix4fv           __glUniformMatrix4fv
#define glGenBuffers                 __glGenBuffers
#define glBufferData                 __glBufferData
#define glBufferSubData              __glBufferSubData
#define glBindBuffer                 __glBindBuffer
#define glEnableVertexAttribArray    __glEnableVertexAttribArray
#define glDisableVertexAttribArray   __glDisableVertexAttribArray
#define glVertexAttribPointer        __glVertexAttribPointer
#define glActiveTexture              __glActiveTexture
#endif

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

#define AGL_INTERNAL_TYPES
#endif

#define AGL_HELPER_GPA(p, t)    p = (t) AGL_GET_PROC( #p )  // getprocaddress wrapper to have less code
#define AGL_HELPER_GPAT(p, t) t p = (t) AGL_GET_PROC( #p )  // one time use getprocaddress wrapper to have less code

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
    char *Vendor;
    char *Renderer;
    char *Version;
    char *ShadingLanguageVersion;
} agl_info;

typedef struct
{    
    b32             Running;
    b32             Active;
    s32             Width;
    s32             Height;
    s32             FPS;    
    r32             Delta;
    u32             FrameCount;
    u32             StartTime;
    u32             TicksLastFrame;
    b32             VerticalSync;
    b32             EnableMSAA;
    b32             MultisampleSupported;
    s32             MultisampleFormat;
    agl_input       Input;
    agl_info        GLInfo;
    agl_platform    Platform;
} agl_context;
agl_context __ctx;

// These are macro definable functions
AGLDEF void  AGL_LOOP(float Delta);
AGLDEF void  AGL_INIT(agl_context *Context);
AGLDEF void  AGL_KEYDOWN(char Key);
AGLDEF void  AGL_KEYUP(char Key);
AGLDEF void  AGL_RESIZE(int Width, int Heigth);
AGLDEF void  AGL_CLEANUP();

// Actual api functions

//TODO: AGLDEF void  aglCaptureMouse(b32 Capture);
AGLDEF b32   aglCreateWindow(const char *Title);
AGLDEF void  aglCloseWindow();
AGLDEF void  aglDestroyWindow();
AGLDEF r32   aglGetDelta();
AGLDEF s32   aglGetMouseDeltaX();
AGLDEF s32   aglGetMouseDeltaY();
AGLDEF s32   aglGetMousePosX();
AGLDEF s32   aglGetMousePosY();
AGLDEF s32   aglGetMouseWheelDelta();
AGLDEF u32   aglGetTicks();
AGLDEF void  aglHandleEvents();
AGLDEF b32   aglIsWindowActive();
AGLDEF b32   aglKeyDown(u8 Key);
AGLDEF s32   aglKeyDownTransState(u8 Key);
AGLDEF b32   aglKeyUp(u8 Key);
AGLDEF b32   aglMouseUp(u8 Button);
AGLDEF b32   aglMouseDown(u8 Button);
AGLDEF void  aglSetWindowTitle(char *Title);
//TODO: AGLDEF void  aglSetWindowSize(s32 Width, s32 Heigth);
//TODO: AGLDEF void  aglSetWindowPos(s32 X, s32 Y, s32 Width = 0, s32 Heigth = 0);
AGLDEF void  aglSleep(u32 Milliseconds);
AGLDEF void  aglSwapBuffers();
AGLDEF void  aglToggleFullscreen();
AGLDEF void  aglToggleVSYNC();

#if defined(AGL_IMPLEMENTATION) // AGL_IMPLEMENTATION

// AGL_MAIN replaces the platform specific main routine
AGL_MAIN
{
    __ctx.Running      = true;
    __ctx.Width        = AGL_WINDOW_WIDTH;
    __ctx.Height       = AGL_WINDOW_HEIGHT;
    __ctx.EnableMSAA   = AGL_OPENGL_MSAA_ENABLE;
    __ctx.VerticalSync = AGL_OPENGL_VSYNC_ENABLE;
    
    if(aglCreateWindow(AGL_WINDOW_TITLE) && __ctx.Platform.GLContext)
    {   
        if(__ctx.VerticalSync) aglToggleVSYNC();
        if(__ctx.EnableMSAA)   glEnable(GL_MULTISAMPLE); else glDisable(GL_MULTISAMPLE);
        
        __ctx.GLInfo.Vendor                 = (char *) glGetString(GL_VENDOR);
        __ctx.GLInfo.Renderer               = (char *) glGetString(GL_RENDERER);
        __ctx.GLInfo.Version                = (char *) glGetString(GL_VERSION);
        __ctx.GLInfo.ShadingLanguageVersion = (char *) glGetString(GL_SHADING_LANGUAGE_VERSION);

        aglInitProcs();

        AGL_INIT(&__ctx);
        while(__ctx.Running)
        {
            aglHandleEvents();
                
            AGL_LOOP(__ctx.Delta);
            
            aglSwapBuffers();
        }
        
        AGL_CLEANUP();
        
        aglDestroyWindow();
    }
    return 0;
}

AGLDEF void
aglInitStub(agl_context *Context)
{
    // Stub function
}

AGLDEF void
aglCleanUpStub()
{
    // Stub function
}

AGLDEF void
aglLoopStub(float Delta)
{
    // Stub function
};

AGLDEF void
aglKeyDownStub(char Key)
{
    // Stub function
};

AGLDEF void
aglKeyUpStub(char Key)
{
    // Stub function
};

AGLDEF void
aglResizeStub(int Width, int Height)
{
    // Stub function
    // Automatically updates the viewport after a resize event was triggered.
    glViewport(0, 0, Width, Height);
};

AGLDEF r32
aglGetDelta()
{
    return __ctx.Delta;
};

AGLDEF s32
aglKeyDownTransition(char Key)
{
    return __ctx.Input.Keys[Key].Count;
}

AGLDEF b32
aglKeyDown(u8 Key)
{
    return (!__ctx.Input.Keys[Key].EndedDown && __ctx.Input.Keys[Key].Count > 0);
}

AGLDEF b32
aglKeyUp(u8 Key)
{
    return __ctx.Input.Keys[Key].EndedDown;
}

AGLDEF s32
aglGetMouseX()
{
    return __ctx.Input.MouseX;
}

AGLDEF s32
aglGetMouseY()
{
    return __ctx.Input.MouseY;
}

AGLDEF s32
aglGetMouseDeltaX()
{
    return __ctx.Input.MouseXDelta;
}

AGLDEF s32
aglGetMouseDeltaY()
{
    return __ctx.Input.MouseYDelta;
}

AGLDEF b32
aglMouseDown(u8 MouseButton)
{
    return (!__ctx.Input.Buttons[MouseButton].EndedDown && __ctx.Input.Buttons[MouseButton].Count > 0);
}

AGLDEF b32
aglMouseUp(u8 MouseButton)
{
    return __ctx.Input.Buttons[MouseButton].EndedDown;
}

AGLDEF s32
aglMouseWheelDelta()
{
    return __ctx.Input.MouseWheelDelta;
}

AGLDEF b32
aglIsWindowActive()
{
    return __ctx.Active;
}

// WIN32 platform layer for libagl

#if defined(_WIN32) // _WIN32

AGLDEF void
aglToggleFullscreen()
{
    DWORD Style = GetWindowLong(__ctx.Platform.HWnd, GWL_STYLE);
    if(Style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO MonitorInfo = {sizeof(MonitorInfo)};
        if(GetWindowPlacement(__ctx.Platform.HWnd, &__ctx.Platform.Placement) &&
           GetMonitorInfo(MonitorFromWindow(__ctx.Platform.HWnd, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
        {
            SetWindowLong(__ctx.Platform.HWnd, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(__ctx.Platform.HWnd, HWND_TOP,
                         MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
                         MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
                         MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }

    }
    else
    {
        SetWindowLong(__ctx.Platform.HWnd, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(__ctx.Platform.HWnd, &__ctx.Platform.Placement);
        SetWindowPos(__ctx.Platform.HWnd, 0, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}

AGLDEF void
aglHandleEvents()
{
    __ctx.FrameCount++;

    u32 Now = aglGetTicks();
    // Calculate time spend per loop
    r32 dTime = (Now - __ctx.StartTime) / 1000.f;
    if(dTime > 0.25f && __ctx.FrameCount > 10)
    {
        // Calculate frames per second
        __ctx.FPS = (s32) (__ctx.FrameCount / (dTime));
        __ctx.FrameCount = 0;
        __ctx.StartTime = Now;
    }
    // Calculate delta time
    __ctx.Delta = (Now - __ctx.TicksLastFrame) / 1000.f;
    __ctx.TicksLastFrame = Now;

    // Capture mouse position and calculate delta positions.
    // This is done this way because of windows inaccurate mouse position received via message queue.
    POINT MousePos;
    GetCursorPos(&MousePos);
    __ctx.Input.MouseXDelta  = MousePos.x - __ctx.Input.MouseXScreen;
    __ctx.Input.MouseYDelta  = MousePos.y - __ctx.Input.MouseYScreen;
    __ctx.Input.MouseXScreen = MousePos.x;
    __ctx.Input.MouseYScreen = MousePos.y;
    __ctx.Input.MouseWheelDelta = 0;

    // Clear key states for pressed keys
    for(s32 i=0; i < 256; i++) {
        agl_key_state *State = __ctx.Input.Keys + i;
        if(State->EndedDown) {
            State->Count = 0;
            State->EndedDown = false;
        }
    }
    for(s32 i=0; i < 3; i++) {
        agl_key_state *State = __ctx.Input.Buttons + i;
        if(State->EndedDown) {
            State->Count = 0;
            State->EndedDown = false;
        }
    }
    
    // Loop through windows messages
    MSG msg;
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE) && __ctx.Running)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    if(!IsWindow(__ctx.Platform.HWnd)) __ctx.Running = false;
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)									// Check For Windows Messages
    {
        case WM_ACTIVATE:							// Watch For Window Activate Message
        {
            if(LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
            {
                __ctx.Active = true;
            } else if(LOWORD(wParam) == WA_INACTIVE)
            {
                __ctx.Active = false;
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
                    return 0;						// Prevent From Happening
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
            agl_key_state *State = __ctx.Input.Keys + Key;
            State->Count++;
            State->EndedDown = false;
            AGL_KEYDOWN(Key);
            return 0;
        }break;
        case WM_KEYUP:								// Has A Key Been Released?
        {
            u8 Key = (u8) wParam;
            agl_key_state *State = __ctx.Input.Keys + Key;
            State->EndedDown = true;
            AGL_KEYUP(Key);
            return 0;
        }break;
        case WM_MOUSEWHEEL:
        {
            __ctx.Input.MouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            return 0;
        }break;
        case WM_MOUSEMOVE:
        {                        
            __ctx.Input.MouseX = GET_X_LPARAM(lParam);
            __ctx.Input.MouseY = GET_Y_LPARAM(lParam);
            return 0;
        }break;
        case WM_MBUTTONUP:   { __ctx.Input.Buttons[2].EndedDown = true; return 0;}
        case WM_RBUTTONUP:   { __ctx.Input.Buttons[1].EndedDown = true; return 0;}
        case WM_LBUTTONUP:   { __ctx.Input.Buttons[0].EndedDown = true; return 0;}
        case WM_MBUTTONDOWN: { __ctx.Input.Buttons[2].Count++; return 0;}
        case WM_RBUTTONDOWN: { __ctx.Input.Buttons[1].Count++; return 0;}
        case WM_LBUTTONDOWN: { __ctx.Input.Buttons[0].Count++; return 0;}
        case WM_SIZE:
        {
            __ctx.Width = LOWORD(lParam);
            __ctx.Height = HIWORD(lParam);
            AGL_RESIZE(LOWORD(lParam), HIWORD(lParam));
            return 0;
        }break;
    }
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

AGLDEF void
aglSleep(u32 Milliseconds)
{
    Sleep(Milliseconds);
} 

AGLDEF u32
aglGetTicks()
{
    if (!__ctx.Platform.TimerInit) {
        QueryPerformanceFrequency(&__ctx.Platform.Frequency);
        QueryPerformanceCounter(&__ctx.Platform.Init);
        __ctx.Platform.TimerInit = true;
    }
    
    LARGE_INTEGER Now;
    QueryPerformanceCounter(&Now);
    Now.QuadPart -= __ctx.Platform.Init.QuadPart;
    Now.QuadPart *= 1000;
    Now.QuadPart /= __ctx.Platform.Frequency.QuadPart;
    return (u32) Now.QuadPart;
}

AGLDEF void
aglShowCursor(b32 Show)
{
    CURSORINFO CursorInfo;
    CursorInfo.cbSize = sizeof(CURSORINFO);
    GetCursorInfo(&CursorInfo);
    
    if(CursorInfo.flags == 0 && Show) ShowCursor(true);
    else if(CursorInfo.flags == CURSOR_SHOWING && !Show) ShowCursor(false);
}

AGLDEF void
aglDestroyWindow()
{
    if(IsWindow(__ctx.Platform.HWnd))
    {
        DestroyWindow(__ctx.Platform.HWnd);

        const wchar_t Clazzname[] = AGL_WIN32_CLASSNAME;
        if(!UnregisterClass( Clazzname , GetModuleHandle(0)))
        {
            MessageBoxA(NULL,"Unregister failed..","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);  
        }   
    
        if (__ctx.Platform.GLContext)											// Do We Have A Rendering Context?
        {
            if (!wglMakeCurrent(__ctx.Platform.DC, NULL))					// Are We Able To Release The DC And RC Contexts?
            {
                MessageBoxA(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);            
            }
        
            if (!wglDeleteContext(__ctx.Platform.GLContext))						// Are We Able To Delete The RC?
            {
                MessageBoxA(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
            }
            __ctx.Platform.GLContext = 0;    
        }
    }
}

// WGLisExtensionSupported: This Is A Form Of The Extension For WGL
AGLDEF b32
aglIsExtensionSupported(const char *extension)
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

		if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
			return true;															// Match
	}
}

// InitMultisample: Used To Query The Multisample Frequencies
AGLDEF b32
aglInitMultisample(HINSTANCE hInstance,HWND hWnd,PIXELFORMATDESCRIPTOR pfd)
{  
    // See If The String Exists In WGL!
	if (!aglIsExtensionSupported("WGL_ARB_multisample"))
	{
		__ctx.MultisampleSupported = false;
		return false;
	}

    wglChoosePixelFormat = (PFNWGLCHOOSEPIXELFORMATARBPROC) AGL_GET_PROC("wglChoosePixelFormatARB");
    if (!wglChoosePixelFormat) 
	{
		__ctx.MultisampleSupported = false;
		return false;
	}

	HDC hDC = GetDC(hWnd);

	int		pixelFormat;
	int		valid;
	UINT	numFormats;
	float	fAttributes[] = {0,0};

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
            WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
            WGL_SAMPLES_ARB, AGL_OPENGL_MSAA_SAMPLES,
            0,0
        };

	valid = wglChoosePixelFormat(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
 
	if (valid && numFormats >= 1)
	{
		__ctx.MultisampleSupported = true;
		__ctx.MultisampleFormat = pixelFormat;	
		return __ctx.MultisampleSupported;
	}
    
	return __ctx.MultisampleSupported;
}

AGLDEF b32
aglCreateWindow(const char *Title)
{
    WNDCLASS	wc;
    u32 		PixelFormat;			// Holds The Results After Searching For A Match

	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Value
    
    const wchar_t Clazzname[] = AGL_WIN32_CLASSNAME;
    size_t Ret=0;
    wchar_t WTitle[128] = {};
    mbstowcs_s(&Ret, WTitle, 128, Title, strlen(Title));
        
	WindowRect.left     = (long) 0;
	WindowRect.right    = (long) __ctx.Width;
	WindowRect.top      = (long) 0;
	WindowRect.bottom   = (long) __ctx.Height;

    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(0);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = Clazzname;
    
	if (!RegisterClass(&wc))
	{
        AGL_ASSERT(!"Failed To Register The Window Class.");
        return false;
    }

    dwStyle=WS_OVERLAPPEDWINDOW;
    dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	if (!(__ctx.Platform.HWnd = CreateWindowEx( 0,
                                                Clazzname,
                                                WTitle,
                                                dwStyle | WS_CLIPSIBLINGS |	WS_CLIPCHILDREN,
                                                0, 0,
                                                WindowRect.right-WindowRect.left,
                                                WindowRect.bottom-WindowRect.top,
                                                NULL,
                                                NULL,
                                                wc.hInstance,
                                                0)))
	{
        aglDestroyWindow();	
        AGL_ASSERT(!"Can't create window.");
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0,
            24,
            8,
            0,
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };
    PIXELFORMATDESCRIPTOR *ppfd = &pfd;
    
	if (!(__ctx.Platform.DC = GetDC(__ctx.Platform.HWnd)))
	{
        aglDestroyWindow();	
		AGL_ASSERT(!"Can't create a device context.");
		return false;
	}

    if(!__ctx.MultisampleSupported)
    {
        if (!(PixelFormat=ChoosePixelFormat(__ctx.Platform.DC, ppfd)))
        {
            aglDestroyWindow();	
            AGL_ASSERT(!"Can't find a fitting pixelformat.");
            return false;
        }
    }
    else
    {
        PixelFormat = __ctx.MultisampleFormat;
    }
    
	if(!SetPixelFormat(__ctx.Platform.DC, PixelFormat, ppfd))
	{
        aglDestroyWindow();	
        AGL_ASSERT(!"Can't set the desired pixelformat.");
        return false;
	}

    if(!__ctx.Platform.GLContext)
    {

        if (!(__ctx.Platform.GLContext = wglCreateContext(__ctx.Platform.DC)))
        {
            aglDestroyWindow();	
            AGL_ASSERT(!"Can't create an opengl rendering context.");
            return false;
        }  
        
        if(!wglMakeCurrent(__ctx.Platform.DC, __ctx.Platform.GLContext))
        {
            aglDestroyWindow();	
            AGL_ASSERT(!"Can't activate the opengl rendering context");
            return false;
        }
        
        if(aglIsExtensionSupported("WGL_ARB_create_context"))
        {
            int attribs[] =
                {
                    WGL_CONTEXT_MAJOR_VERSION_ARB, AGL_OPENGL_MAJOR_VERSION,
                    WGL_CONTEXT_MINOR_VERSION_ARB, AGL_OPENGL_MINOR_VERSION,
                    WGL_CONTEXT_FLAGS_ARB, 0,
                    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                    0
                };
            
            if(!wglCreateContextAttribs) wglCreateContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC) AGL_GET_PROC("wglCreateContextAttribsARB");
            HGLRC ModernContext = wglCreateContextAttribs(__ctx.Platform.DC, 0, attribs);
            if(ModernContext)
            {
                wglMakeCurrent(0, 0);
                wglDeleteContext(__ctx.Platform.GLContext);
                wglMakeCurrent(__ctx.Platform.DC, ModernContext);
                __ctx.Platform.GLContext = ModernContext;                
            }

        }
    }
    
    if(__ctx.EnableMSAA && !__ctx.MultisampleSupported)
    {
        if(aglInitMultisample(GetModuleHandle(0), __ctx.Platform.HWnd, pfd))
        {
            aglDestroyWindow();
            return aglCreateWindow(Title);
        }
    }
	ShowWindow(__ctx.Platform.HWnd, SW_SHOW);
	SetForegroundWindow(__ctx.Platform.HWnd);
    SetFocus(__ctx.Platform.HWnd);
    return true;
}

AGLDEF void
aglToggleVSYNC()
{
    if(!wglGetSwapInterval || !wglGetSwapInterval)
    {
        wglGetSwapInterval = (PFNWGLGETSWAPINTERVALEXTPROC) AGL_GET_PROC("wglGetSwapIntervalEXT");
        wglSwapInterval    = (PFNWGLSWAPINTERVALEXTPROC)    AGL_GET_PROC("wglSwapIntervalEXT");
    }
    
    if(wglGetSwapInterval && wglSwapInterval)
    {
        __ctx.VerticalSync = wglGetSwapInterval();
        wglSwapInterval(!__ctx.VerticalSync);
    }
}

AGLDEF void
aglSwapBuffers()
{
    SwapBuffers(__ctx.Platform.DC); 
}

AGLDEF void
aglCloseWindow()
{
    DestroyWindow(__ctx.Platform.HWnd);
}

AGLDEF void
aglSetWindowTitle(char *Title)
{
    SetWindowTextA(__ctx.Platform.HWnd, Title);
}

AGLDEF b32
aglIsActive()
{
    return __ctx.Active;
}

AGLDEF void
aglCaptureMouse(b32 Capture)
{
    if(Capture) SetCapture(__ctx.Platform.HWnd);
    else ReleaseCapture();
};

#elif defined(__unix) // UNIX

#elif defined(__linux__) // UNIX

#elif defined(__APPLE) // UNIX

#endif // PLATFORM LAYER IMPLEMENTATION

// Opengl utility functions. Make opengl stuff easier to use.

AGLDEF void
aglShaderUse(u32 Program)
{
    __active_program = Program;
    glUseProgram(Program);
}

AGLDEF b32
aglShaderCompileAndAttach(u32 Program, const char *ShaderSource, GLenum Type)
{
    b32 Result = false;
    s32 ShaderID = glCreateShader(Type)
		;
    if(glGetError() != GL_INVALID_ENUM)
    {
        glShaderSource(ShaderID, 1, &ShaderSource, 0);
        glCompileShader(ShaderID);
        

        s32 Status;
        glGetShaderStatus(ShaderID, &Status);
        if(Status == GL_FALSE)
        {
            s32 Length = 0;
            char ErrorMessage[1024];
            glGetShaderInfoLog(ShaderID, 1024, &Length, ErrorMessage);
            AGL_PRINT("%s\n", ErrorMessage);
            AGL_ASSERT(!"Shader compile error");
            Result = false;
        } else {
            glAttachShader(Program, ShaderID);
            Result = true;
        }
    }
    else
    {
        AGL_PRINT("%s\n", ShaderSource);
        AGL_ASSERT(!"Shader compile error");
    }
    return Result;
}

AGLDEF b32
aglShaderLink(u32 Program)
{
    b32 Result = false;
    glLinkProgram(Program);
    if(glGetError() != GL_INVALID_VALUE && glGetError() != GL_INVALID_OPERATION)
    {
        s32 Status;
        glGetProgramStatus(Program, &Status);
        if(Status == GL_FALSE)
        {
            s32 Length = 0;
            char ErrorMessage[1024];
            glGetProgramInfoLog(Program, 1024, &Length, ErrorMessage);
            AGL_PRINT("%s\n", ErrorMessage);
            AGL_ASSERT(!"Shader link error");
            Result = false;
        } else {
            u32 ShaderIds[12] = {};
            s32 AttachedShaders = 0;
            glGetAttachedShaders(Program, sizeof(ShaderIds) / sizeof(ShaderIds[0]), &AttachedShaders, ShaderIds);
            for(s32 i=0; i < 12; i++)
            {
                if(ShaderIds[i] != 0) {
                    glDetachShader(Program, ShaderIds[i]);
                    glDeleteShader(ShaderIds[i]);
                }
            }
            glUseProgram(Program);
            Result = true;
        }
    } else AGL_ASSERT(!"Shader link error");
    return Result;
}

inline AGLDEF void
aglShaderSetUniform1i( char *Uniform, s32 A)
{
    glUniform1i(glGetUniformLocation(__active_program, Uniform), A);
}

inline AGLDEF void
aglShaderSetUniform2i(char *Uniform, s32 A, s32 B)
{
    glUniform2i(glGetUniformLocation(__active_program, Uniform), A, B);
}

inline AGLDEF void
aglShaderSetUniform3i(char *Uniform, s32 A, s32 B, s32 C)
{
    glUniform3i(glGetUniformLocation(__active_program, Uniform), A, B, C);
}

inline AGLDEF void
aglShaderSetUniform4i(char *Uniform, s32 A, s32 B, s32 C, s32 D)
{
    glUniform4i(glGetUniformLocation(__active_program, Uniform), A, B, C, D);
}

inline AGLDEF void
aglShaderSetUniform1f(char *Uniform, r32 A)
{
    glUniform1f(glGetUniformLocation(__active_program, Uniform), A);
}

inline AGLDEF void
aglShaderSetUniform2f(char *Uniform, r32 A, r32 B)
{
    glUniform2f(glGetUniformLocation(__active_program, Uniform), A, B);
}

inline AGLDEF void
aglShaderSetUniform3f(char *Uniform, r32 A, r32 B, r32 C)
{
    glUniform3f(glGetUniformLocation(__active_program, Uniform), A, B, C);
}

inline AGLDEF void
aglShaderSetUniform4f(char *Uniform, r32 A, r32 B, r32 C, r32 D)
{
    glUniform4f(glGetUniformLocation(__active_program, Uniform), A, B, C, D);
}

inline AGLDEF void
aglShaderSetUniformMat3fv(char *Uniform, r32 *Matrix)
{
    glUniformMatrix3fv(glGetUniformLocation(__active_program, Uniform), 1, GL_FALSE, Matrix);
}

inline AGLDEF void
aglShaderSetUniformMat4fv(char *Uniform, r32 *Matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(__active_program, Uniform), 1, GL_FALSE, Matrix);
}


#endif // AGL_IMPLEMENTATION

#define AGL_H
#endif
