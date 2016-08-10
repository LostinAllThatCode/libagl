/*
  [Documentation here]
*/

#if !defined(AGL_H)

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

#define aglAssert(Expression) if(!(Expression)) {*(int *)0 = 0;}

#ifdef AGL_EXTERN
    #define AGLDEF extern
#else
    #define AGLDEF static
#endif

#ifdef __cplusplus
extern "C" {
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

    #else
    
    #endif
    
    #include <GL\GL.h>
    #include "glcorearb.h"
    
    #ifndef AGL_NO_MODERNGL
        PFNGLDEBUGMESSAGECALLBACKPROC       glDebugMessageCallback;
        PFNGLCREATESHADERPROC               glCreateShader;
        PFNGLDELETESHADERPROC               glDeleteShader;
        PFNGLSHADERSOURCEPROC               glShaderSource;
        PFNGLCOMPILESHADERPROC              glCompileShader;
        PFNGLGETSHADERIVPROC                glGetShader;
        PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog;
        PFNGLCREATEPROGRAMPROC              glCreateProgram;
        PFNGLGETPROGRAMIVPROC               glGetProgram;
        PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog;
        PFNGLATTACHSHADERPROC               glAttachShader;
        PFNGLDETACHSHADERPROC               glDetachShader;
        PFNGLLINKPROGRAMPROC                glLinkProgram;
        PFNGLUSEPROGRAMPROC                 glUseProgram;
        PFNGLACTIVETEXTUREPROC              glActiveTexture;
        PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation;
        PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv;
        PFNGLGENBUFFERSPROC                 glGenBuffers;
        PFNGLBINDBUFFERPROC                 glBindBuffer;
        PFNGLDELETEBUFFERSPROC              glDeleteBuffers;
        PFNGLBUFFERDATAPROC                 glBufferData;
        PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays;
        PFNGLBINDVERTEXARRAYPROC            glBindVertexArray;
        PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray;
        PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer;
        PFNGLDISABLEVERTEXATTRIBARRAYPROC    glDisableVertexAttribArray;
        PFNGLVERTEXATTRIBDIVISORPROC        glVertexAttribDivisor;
        PFNGLDRAWARRAYSINSTANCEDPROC        glDrawArraysInstanced;
    #endif

    typedef struct 
    {
        u8 Keys[256];
    } agl_keyboad_io;
    
    typedef struct
    {
        s32 X, Y, dX, dY, Wheel, dWheel;
        b32 Left, Middle, Right;
    } agl_mouse_io;

    typedef struct 
    {
        char *Vendor;
        char *Renderer;
        char *Version;
        char *ShadingLanguageVersion;
        char *Extensions;
    } agl_opengl_info;
    
    typedef struct
    {
        b32 Running;
        r32 Delta;
        s32 Width;
        s32 Height;
        s32 FPS;
        s32 TargetFPS;
        HGLRC GLContext;
       
        agl_opengl_info GLInfo;
        agl_keyboad_io KeyboardIO;
        agl_mouse_io MouseIO;

        //NOTE: Platform dependent variables
        #if _WIN32
            HWND HWnd;
            HDC DC;
            WINDOWPLACEMENT Placement; // NOTE: Necessary for fullScreen feature!
            LARGE_INTEGER FrameBegin, FrameEnd, Frequency;
        #elif // Other platform implementation
         
        #endif
    } agl_context;

    // NOTE: Platform specific functions. Implement these yourself if you want to. 
    //       Define AGL_EXTERN beforce including this file. Ex.: #define AGL_EXTERN
    AGLDEF void* aglPlatformGetProcAddress(char *Function);
    AGLDEF b32   aglPlatformCreateWindow(agl_context *Context, char *Title);
    AGLDEF void  aglPlatformDestroyWindow(agl_context *Context);
    AGLDEF void  aglPlatformHandleEvents(agl_context *Context);
    AGLDEF void  aglPlatformSetFixedFramerate(agl_context *Context, s32 FPS);
    AGLDEF void  aglPlatformBeginFrame(agl_context *Context);
    AGLDEF void  aglPlatformEndFrame(agl_context *Context);
    AGLDEF void  aglPlatformSleep(agl_context *Context, s32 Time);
    AGLDEF void  aglPlatformSwapBuffers(agl_context *Context);
    
#ifdef __cplusplus
}
#endif

#ifndef AGL_EXTERN
#define AGL_IMPLEMENTATION
#if _WIN32

AGLDEF void *
aglPlatformGetProcAddress(char *Function)
{
    return wglGetProcAddress(Function);
}

AGLDEF LRESULT CALLBACK
WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)									// Check For Windows Messages
    {
        case WM_ACTIVATE:							// Watch For Window Activate Message
        {
            if (!HIWORD(wParam))					// Check Minimization State
            {
                //Running = GL_TRUE;						// Program Is Active
            }
            return 0;								// Return To The Message Loop
        }
        case WM_SYSCOMMAND:							// Intercept System Commands
        {
            switch (wParam)							// Check System Calls
            {
                case SC_SCREENSAVE:					// Screensaver Trying To Start?
                case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
                    return 0;							// Prevent From Happening
            }
            break;									// Exit
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }break;
    }
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

AGLDEF void
aglPlatformDestroyWindow(agl_context *Context)
{
    timeEndPeriod(1);
    if (Context->GLContext)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(Context->DC, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);            
		}
        
		if (!wglDeleteContext(Context->GLContext))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
        
        Context->GLContext = 0;
	}
}

AGLDEF b32
aglPlatformCreateWindow(agl_context *Context, char *Title)
{
    GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Value
    
	WindowRect.left     = (long) 0;
	WindowRect.right    = (long) Context->Width;
	WindowRect.top      = (long) 0;
	WindowRect.bottom   = (long) Context->Height;

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

	// Create The Window
	if (!(Context->HWnd = CreateWindowEx(	0,							// Extended Style For The Window
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
                                            Context)))								// Dont Pass Anything To WM_CREATE
	{
        aglPlatformDestroyWindow(Context);	
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
            16,											// 16Bit Z-Buffer (Depth Buffer)  
            0,											// No Stencil Buffer
            0,											// No Auxiliary Buffer
            PFD_MAIN_PLANE,								// Main Drawing Layer
            0,											// Reserved
            0, 0, 0										// Layer Masks Ignored
        };
	
	if (!(Context->DC = GetDC(Context->HWnd)))
	{
        aglPlatformDestroyWindow(Context);	
		MessageBox(NULL,"Can't Create A GL Device Context->","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if (!(PixelFormat=ChoosePixelFormat(Context->DC, &pfd)))
	{
        aglPlatformDestroyWindow(Context);	
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
    }
    
	if(!SetPixelFormat(Context->DC, PixelFormat, &pfd))
	{
        aglPlatformDestroyWindow(Context);	
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
	}

	if (!(Context->GLContext = wglCreateContext(Context->DC)))
	{
        aglPlatformDestroyWindow(Context);	
		MessageBox(NULL,"Can't Create A GL Rendering Context->","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
	}

	if(!wglMakeCurrent(Context->DC, Context->GLContext))
	{
        aglPlatformDestroyWindow(Context);	
		MessageBox(NULL,"Can't Activate The GL Rendering Context->","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
	}

#if 1
    wgl_create_context_attribts_arb *wglCreateContextAttribsARB = (wgl_create_context_attribts_arb *)wglGetProcAddress("wglCreateContextAttribsARB");
    if (wglCreateContextAttribsARB)
    {
        int Attribs[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 0,
                WGL_CONTEXT_FLAGS_ARB, 0 // NOTE(casey): Enable for testing WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
                ,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                0,
            };
        /*
          int Attribs[] = {
          WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
          WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
          WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
          WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
          WGL_COLOR_BITS_ARB, 32,
          WGL_DEPTH_BITS_ARB, 24,
          WGL_STENCIL_BITS_ARB, 8,
          WGL_SAMPLE_BUFFERS_ARB, 1, //Number of buffers (must be 1 at time of writing)
          WGL_SAMPLES_ARB, 4,        //Number of samples
          0
          };
        */
        HGLRC modern_hRC = wglCreateContextAttribsARB(Context->DC, 0, Attribs);
        if (modern_hRC)
        {
            if(wglMakeCurrent(Context->DC, modern_hRC))
            {
                wglDeleteContext(Context->GLContext);
                Context->GLContext = modern_hRC;
            }
        }
        else
        {
            aglPlatformDestroyWindow(Context);
            MessageBox(NULL,"Can't create modern opengl context. Version > 2.0" ,"ERROR",MB_OK|MB_ICONEXCLAMATION);
            return false;
        }
    }
#endif

	ShowWindow(Context->HWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(Context->HWnd);						// Slightly Higher Priority
    SetFocus(Context->HWnd);									// Sets Keyboard Focus To The Window

    QueryPerformanceFrequency(&Context->Frequency);
    timeBeginPeriod(1);
    
    return true;
}

AGLDEF void
aglPlatformHandleEvents(agl_context *Context)
{
    MSG msg;
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE) && Context->Running)
    {
        switch(msg.message)
        {
            case WM_QUIT:
            {
                Context->Running = false;
                aglPlatformDestroyWindow(Context);
            }break;
            case WM_KEYDOWN:
            {
                Context->KeyboardIO.Keys[msg.wParam] = 1;
                // Handle escape event to close window
                if(Context->KeyboardIO.Keys[VK_ESCAPE]) PostQuitMessage(0);
            }break;
            case WM_KEYUP:								// Has A Key Been Released?
            {
                Context->KeyboardIO.Keys[msg.wParam] = 0;
            }break;
            case WM_MOUSEMOVE:
            {
                s32 X = ((int)(short)LOWORD(msg.lParam));
                s32 Y = ((int)(short)HIWORD(msg.lParam));
                Context->MouseIO.dX = X - Context->MouseIO.X;
                Context->MouseIO.dY = Y - Context->MouseIO.Y;
                Context->MouseIO.X = X;
                Context->MouseIO.Y = Y;
            }break;
            case WM_RBUTTONUP:
            {
                Context->MouseIO.Right = false;
                //if(aglMouseReleased) aglMouseReleased(MK_RBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }break;
            case WM_MBUTTONUP:
            {
                Context->MouseIO.Middle = false;
                //if(aglMouseReleased) aglMouseReleased(MK_MBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }break;
            case WM_LBUTTONUP:
            {
                Context->MouseIO.Left = false;
                //if(aglMouseReleased) aglMouseReleased(MK_LBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }break;
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONDOWN:
            {
                Context->MouseIO.Left = (msg.wParam == MK_LBUTTON);
                Context->MouseIO.Middle = (msg.wParam == MK_MBUTTON);
                Context->MouseIO.Right = (msg.wParam == MK_RBUTTON);
                //if(aglMousePressed) aglMousePressed(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }break;
            
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

AGLDEF void aglPlatformSleep(agl_context *Context, r32 Time) { if(Time > 0) Sleep(Time); }
AGLDEF void aglPlatformBeginFrame(agl_context *Context) { QueryPerformanceCounter(&Context->FrameBegin); }
AGLDEF void aglPlatformEndFrame(agl_context *Context) {
    QueryPerformanceCounter(&Context->FrameEnd);
    Context->Delta = ((r32) (Context->FrameEnd.QuadPart - Context->FrameBegin.QuadPart) / (r32) Context->Frequency.QuadPart);
}

AGLDEF void aglPlatformSwapBuffers(agl_context *Context) {
    SwapBuffers(Context->DC);
    Context->MouseIO.dX = 0;
    Context->MouseIO.dY = 0;
}

#elif

#endif // PLATFORM SPECIFIC FUNCTIONS

#endif // AGL_NOIMPLEMENTATION

// NOTE: General api code for handling the window and opengl context.
void *
aglGetProcAddress(char *Function)
{
    void *Result = aglPlatformGetProcAddress(Function);
    aglAssert(Result);
    return Result;
}

b32
aglInitModernGLImpl()
{
    b32 Result = false;
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
        
        Result = true;
    }
    return Result;
}

agl_context
aglCreateWindow(char *Title = "agl default window", s32 Width = 1024, s32 Height = 768)
{
    agl_context Context = {};
    Context.Width = Width;
    Context.Height = Height;

    if(aglPlatformCreateWindow(&Context, Title) && Context.GLContext)
    {
        Context.GLInfo.Vendor = (char *)glGetString(GL_VENDOR);
        Context.GLInfo.Renderer = (char *)glGetString(GL_RENDERER);
        Context.GLInfo.Version = (char *)glGetString(GL_VERSION);
        Context.GLInfo.ShadingLanguageVersion = (char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
        Context.GLInfo.Extensions = (char *)glGetString(GL_EXTENSIONS);
        
        Context.Running = aglInitModernGLImpl();
    }
    return Context;
}

void
aglDestroyWindow(agl_context *Context)
{
	aglPlatformDestroyWindow(Context);
}

b32
aglHandleEvents(agl_context *Context)
{
    aglPlatformBeginFrame(Context);
    aglPlatformHandleEvents(Context);
    return Context->Running;
}

b32
aglKeyDown(agl_context *Context, char Key)
{
    return Context->KeyboardIO.Keys[Key];
}

b32
aglKeyUp(agl_context *Context, char Key)
{
    return !Context->KeyboardIO.Keys[Key];
}

void
aglSwapBuffers(agl_context *Context)
{
    aglPlatformEndFrame(Context);
    if(Context->TargetFPS > 0)
    {
        r32 TargetFrameTime = 1.0f / Context->TargetFPS;
        if(Context->Delta < TargetFrameTime)
        {
            aglPlatformSleep(Context, (TargetFrameTime - Context->Delta) * 1000);
            aglPlatformEndFrame(Context);
        }
    }
    Context->FPS = (u32) (1.0f /Context->Delta);
    aglPlatformSwapBuffers(Context);
    glGetError();
}

void
aglSetFixedFPS(agl_context *Context, s32 FPS)
{
    Context->TargetFPS = FPS;
}


#define AGL_H
#endif
