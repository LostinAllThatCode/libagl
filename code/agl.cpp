#include "agl.h"

agl_opengl_info
aglOpenGLInfo()
{
    agl_opengl_info Result = {};
    Result.Vendor = (char *)glGetString(GL_VENDOR);
    Result.Renderer = (char *)glGetString(GL_RENDERER);
    Result.Version = (char *)glGetString(GL_VERSION);
    Result.ShadingLanguageVersion = (char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
    Result.Extensions = (char *)glGetString(GL_EXTENSIONS);
    return Result;
}

b32
aglReadFileDefault(char *Filename, char *Buffer)
{
    FILE *Handle = fopen(Filename, "rb");
    if(Handle)
    {
        long Size;
        fseek(Handle, 0, SEEK_END);
        Size = ftell(Handle);
        fseek(Handle, 0, SEEK_SET);
        fread(Buffer, 1, Size, Handle);
        fclose(Handle);
        Buffer[Size] = 0;
        return GL_TRUE;
    }
    return GL_FALSE;
}

void
aglInitGLDefault(void)
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);    
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations  
};

GLboolean
aglLoadOpenGLAPI(void)
{
    if(*aglGetProcAddress)
    {
        glDebugMessageCallback             = (PFNGLDEBUGMESSAGECALLBACKPROC)     aglGetProcAddress("glDebugMessageCallback");
        
        glCreateShader                     = (PFNGLCREATESHADERPROC)             aglGetProcAddress("glCreateShader");
        glDeleteShader                     = (PFNGLDELETESHADERPROC)             aglGetProcAddress("glDeleteShader");
        glShaderSource                     = (PFNGLSHADERSOURCEPROC)             aglGetProcAddress("glShaderSource");
        glCompileShader                    = (PFNGLCOMPILESHADERPROC)            aglGetProcAddress("glCompileShader");
        glGetShader                        = (PFNGLGETSHADERIVPROC)              aglGetProcAddress("glGetShaderiv");
        glGetShaderInfoLog                 = (PFNGLGETSHADERINFOLOGPROC)         aglGetProcAddress("glGetShaderInfoLog");
        glCreateProgram                    = (PFNGLCREATEPROGRAMPROC)            aglGetProcAddress("glCreateProgram");
        glGetProgram                       = (PFNGLGETPROGRAMIVPROC)             aglGetProcAddress("glGetProgramiv");
        glGetProgramInfoLog                = (PFNGLGETPROGRAMINFOLOGPROC)        aglGetProcAddress("glGetProgramInfoLog");
        glAttachShader                     = (PFNGLATTACHSHADERPROC)             aglGetProcAddress("glAttachShader");
        glDetachShader                     = (PFNGLDETACHSHADERPROC)             aglGetProcAddress("glDetachShader");
        glLinkProgram                      = (PFNGLLINKPROGRAMPROC)              aglGetProcAddress("glLinkProgram");
        glUseProgram                       = (PFNGLUSEPROGRAMPROC)               aglGetProcAddress("glUseProgram");

        glActiveTexture                    = (PFNGLACTIVETEXTUREPROC)            aglGetProcAddress("glActiveTexture");

        glGetUniformLocation               = (PFNGLGETUNIFORMLOCATIONPROC)       aglGetProcAddress("glGetUniformLocation");
        glUniformMatrix4fv                 = (PFNGLUNIFORMMATRIX4FVPROC)       aglGetProcAddress("glUniformMatrix4fv");

        glGenBuffers                       = (PFNGLGENBUFFERSPROC)               aglGetProcAddress("glGenBuffers");
        glBindBuffer                       = (PFNGLBINDBUFFERPROC)               aglGetProcAddress("glBindBuffer");
        glDeleteBuffers                    = (PFNGLDELETEBUFFERSPROC)            aglGetProcAddress("glDeleteBuffers");
        glBufferData                       = (PFNGLBUFFERDATAPROC)               aglGetProcAddress("glBufferData");
        glGenVertexArrays                  = (PFNGLGENVERTEXARRAYSPROC)          aglGetProcAddress("glGenVertexArrays");
        glBindVertexArray                  = (PFNGLBINDVERTEXARRAYPROC)          aglGetProcAddress("glBindVertexArray");
        glEnableVertexAttribArray          = (PFNGLENABLEVERTEXATTRIBARRAYPROC)  aglGetProcAddress("glEnableVertexAttribArray");
        glVertexAttribPointer              = (PFNGLVERTEXATTRIBPOINTERPROC)      aglGetProcAddress("glVertexAttribPointer");
        glDisableVertexAttribArray         = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) aglGetProcAddress("glDisableVertexAttribArray");

        glVertexAttribDivisor              = (PFNGLVERTEXATTRIBDIVISORPROC)      aglGetProcAddress("glVertexAttribDivisor");
        glDrawArraysInstanced              = (PFNGLDRAWARRAYSINSTANCEDPROC)      aglGetProcAddress("glDrawArraysInstanced");
        
        return GL_TRUE;
    }
    return GL_FALSE;
}

void
aglResizeSceneDefault(int width, int height)
{
	if (height==0) height=1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Calculate The Aspect Ratio Of The Window
	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
    glOrtho( -(width * .5f), (width * .5f), -(height * .5f), (height * .5f), -1, 1 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

HGLRC *
aglCreateWindow(char *Title, int Width, int Height)
{
    HGLRC *OpenGLContext = aglPlatformCreateContext(Title, Width, Height);
    if(OpenGLContext && aglLoadOpenGLAPI())
    {
        // Sets up opengl states. Default or user-specified.
        if(aglInitGL) aglInitGL(); else aglInitGLDefault();

        if(!aglReadFile) aglReadFile = aglReadFileDefault;
        if(!aglResizeScene) aglResizeScene = aglResizeSceneDefault;

        aglUpdate();
        // Current weird workaroung due first SwapBuffer error
        glGetError();
    } else {
        OpenGLContext = 0;
    }
    return OpenGLContext;
}

b32
aglCleanUp(void)
{
    return aglPlatformDestroyContext();
}

b32
aglIsRunning(void)
{
    GLboolean Result = aglPlatformContextAlive();
    if(Result) aglPlatformBeginFrame();
    return Result;
}

void
aglUpdate(void)
{
    aglPlatformUpdateContext();
    aglPlatformEndFrame();
}

void
aglSetFixedFrameRate(GLuint FramesPerSecond)
{
    aglPlatformFixedFrameRate(FramesPerSecond);
}

GLfloat
aglGetDelta(void)
{
    return aglPlatformGetMilliSeconds() / 1000;
}

GLuint
aglGetCurrentFPS(void)
{
    return 1000 / aglPlatformGetMilliSeconds();
}

b32
aglKeyDown(char Key)
{
    return aglPlatformKeyDown(Key);
}

void
aglToggleFullscreen(void)
{
    aglPlatformToggleFullscreen();
}
