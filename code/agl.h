#if !defined(AGL_H)
#include <stdio.h>

#if defined (_WIN32)
    #if defined (AGL_EXPORT)
        #define AGLAPI __declspec( dllexport )
    #else
        #define AGLAPI __declspec( dllimport )
    #endif
#elif

#endif

#if _WIN32
    #ifndef AGL_WND_CLASS_NAME
        #define AGL_WND_CLASS_NAME "win32_agl_window_class"
    #endif

    #include <windows.h>		// Header File For Windows

    #define WGL_CONTEXT_MAJOR_VERSION_ARB              0x2091
    #define WGL_CONTEXT_MINOR_VERSION_ARB              0x2092
    #define WGL_CONTEXT_LAYER_PLANE_ARB                0x2093
    #define WGL_CONTEXT_FLAGS_ARB                      0x2094
    #define WGL_CONTEXT_PROFILE_MASK_ARB               0x9126
    #define WGL_CONTEXT_DEBUG_BIT_ARB                  0x0001
    #define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB     0x0002
    #define WGL_CONTEXT_CORE_PROFILE_BIT_ARB           0x00000001
    #define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB  0x00000002

    typedef HGLRC wgl_create_context_attribts_arb(HDC hDC, HGLRC hshareContext, const int *attribList);
    struct win32_context
    {
        HINSTANCE Instance;
        HWND Hwnd;
        HDC DC;
    };
    AGLAPI win32_context * aglGetWin32Context(void);
#else

#endif

#define ASSERT(Expression) if(!(Expression)) {*(int *)0 = 0;}

#include <GL\GL.h>
#include "glcorearb.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    struct agl_mouse_input
    {
        GLint X;
        GLint Y;
        GLint dX;
        GLint dY;
        GLint Wheel;
        GLint dWheel;
        GLboolean Left;
        GLboolean Middle;
        GLboolean Right;
    };
    AGLAPI agl_mouse_input aglMouseInput;
    //INFO(APIUSER): This global variable is accessable for you at anytime

    struct agl_opengl_info
    {
        char *Vendor;
        char *Renderer;
        char *Version;
        char *ShadingLanguageVersion;
        char *Extensions;
    };
    AGLAPI agl_opengl_info aglOpenGLInfo(void); 

    AGLAPI PFNGLDEBUGMESSAGECALLBACKPROC       glDebugMessageCallback;
    
    AGLAPI PFNGLCREATESHADERPROC               glCreateShader;
    AGLAPI PFNGLDELETESHADERPROC               glDeleteShader;
    AGLAPI PFNGLSHADERSOURCEPROC               glShaderSource;
    AGLAPI PFNGLCOMPILESHADERPROC              glCompileShader;
    AGLAPI PFNGLGETSHADERIVPROC                glGetShader;
    AGLAPI PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog;
    AGLAPI PFNGLCREATEPROGRAMPROC              glCreateProgram;
    AGLAPI PFNGLGETPROGRAMIVPROC               glGetProgram;
    AGLAPI PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog;
    AGLAPI PFNGLATTACHSHADERPROC               glAttachShader;
    AGLAPI PFNGLDETACHSHADERPROC               glDetachShader;
    AGLAPI PFNGLLINKPROGRAMPROC                glLinkProgram;
    AGLAPI PFNGLUSEPROGRAMPROC                 glUseProgram;

    AGLAPI PFNGLACTIVETEXTUREPROC              glActiveTexture;

    AGLAPI PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation;
    AGLAPI PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv;
    
    AGLAPI PFNGLGENBUFFERSPROC                 glGenBuffers;
    AGLAPI PFNGLBINDBUFFERPROC                 glBindBuffer;
    AGLAPI PFNGLDELETEBUFFERSPROC              glDeleteBuffers;
    AGLAPI PFNGLBUFFERDATAPROC                 glBufferData;
    AGLAPI PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays;
    AGLAPI PFNGLBINDVERTEXARRAYPROC            glBindVertexArray;
    AGLAPI PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray;
    AGLAPI PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer;
    AGLAPI PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray;
    
    #define aglAssignFunc(FN, P) if (!FN) FN = P
    
    typedef void* (APIENTRYP PFNAGLGETPROCADDRESS) (char* procname);
    AGLAPI PFNAGLGETPROCADDRESS aglGetProcAddress;
    #define aglAssignGetProcAddress(P) aglAssignFunc(aglGetProcAddress, P);

    typedef void  (APIENTRYP PFNAGLINITGLPROC) (void);
    AGLAPI PFNAGLINITGLPROC aglInitGL;
    #define aglAssignInitGL(P) aglAssignFunc(aglInitGL, P);
    
    typedef void  (APIENTRYP PFNAGLRESIZEPROC) (int width, int height);
    AGLAPI PFNAGLRESIZEPROC aglResizeScene;
    #define aglAssignResizeScene(P) aglAssignFunc(aglResizeScene, P);

    typedef void  (APIENTRYP PFNAGLKEYDOWNPROC) (unsigned int Key);
    AGLAPI PFNAGLKEYDOWNPROC aglKeyDown;
    #define aglAssignKeyDown(P) aglAssignFunc(aglKeyDown, P);

    typedef void  (APIENTRYP PFNAGLKEYUPPROC) (unsigned int Key);
    AGLAPI PFNAGLKEYUPPROC aglKeyUp;
    #define aglAssignKeyUp(P) aglAssignFunc(aglKeyUp, P);

    typedef void  (APIENTRYP PFNAGLMOUSEDOWNPROC) (unsigned int Button, int X, int Y);
    AGLAPI PFNAGLMOUSEDOWNPROC aglMouseDown;
    #define aglAssignMouseDown(P) aglAssignFunc(aglMouseDown, P);

    typedef void  (APIENTRYP PFNAGLMOUSEUPPROC) (unsigned int Button, int X, int Y);
    AGLAPI PFNAGLMOUSEUPPROC aglMouseUp;
    #define aglAssignMouseUp(P) aglAssignFunc(aglMouseUp, P);

    typedef void  (APIENTRYP PFNAGLMOUSEWHEELPROC) (int Direction, int X, int Y);
    AGLAPI PFNAGLMOUSEWHEELPROC aglMouseWheel;
    #define aglAssignMouseWheel(P) aglAssignFunc(aglMouseWheel, P);    

    typedef GLboolean (APIENTRYP PFNAGLREADFILEPROC) (char *filename, char *buffer);
    AGLAPI PFNAGLREADFILEPROC aglReadFile;
    #define aglAssignReadFile(P) aglAssignFunc(aglReadFile, P);

    AGLAPI HGLRC*    aglCreateWindow(char *title, int width, int height);
    AGLAPI GLboolean aglIsRunning(void);
    AGLAPI void      aglUpdate(void);
    AGLAPI GLint     aglGetMousePosX(void);
    AGLAPI GLint     aglGetMousePosY(void);
    AGLAPI void      aglSetFixedFrameRate(GLuint FramesPerSecond);
    AGLAPI GLuint    aglGetCurrentFPS(void);
    AGLAPI GLfloat   aglGetDelta(void);
    AGLAPI GLboolean aglCleanUp(void);

    AGLAPI GLint     aglCompileShader(const char *Source, GLenum Type);
    AGLAPI GLint     aglLoadAndCompileShader(char *Filename, GLenum Type);
    AGLAPI GLboolean aglLinkProgram(GLuint ProgramID, GLint *ShaderArray, GLuint Length);

    /* NOTE: THIS FUNCTIONS NEED TO BE IMPLEMENTED FOR EACH PLATFORM TO MAKE THIS API WORK!
       GLboolean 
       aglPlatformCreateContext(char *title, int width, int height)
       {
           // Create your window and opengl context and return GL_TRUE on success or GL_FALSE on failure.
       }
    */
    HGLRC*    aglPlatformCreateContext(char *title, int width, int height);
    GLboolean aglPlatformContextAlive(void);
    void      aglPlatformUpdateContext(void);
    void      aglPlatformFixedFrameRate(GLuint FramesPerSecond);
    void      aglPlatformBeginFrame(void);
    GLfloat   aglPlatformGetMilliSeconds(void);
    void      aglPlatformEndFrame(void);
    GLboolean aglPlatformDestroyContext(void);
    
#ifdef __cplusplus
}
#endif
    
#define AGL_H
#endif
