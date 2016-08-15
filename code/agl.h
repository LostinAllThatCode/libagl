/*
  [Documentation here]
*/

#if !defined(AGL_H)

#if !defined(AGL_TYPES)
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

#if !defined(aglAssert)
    #define aglAssert(Condition)
#endif
//     #define aglAssert(Expression) if(!(Expression)) {*(int *)0 = 0;}

#if defined(AGL_EXTERN)
    #if defined(__cplusplus)
        #define AGLDEF extern "C"
    #else
        #define AGLDEF extern
    #endif
#else
    #define AGL_IMPLEMENTATION
    #define AGLDEF static
#endif
    
#if _WIN32
    #include <windows.h>		// Header File For Windows
    #include <windowsx.h>
    #if !defined(AGL_WND_CLASS_NAME)
        #define AGL_WND_CLASS_NAME "win32_agl_window_class"
    #endif
#endif

#include <GL\GL.h>
#if !defined(AGL_NO_MODERNGL)
    #include "glcorearb.h"
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
    PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4f;
    PFNGLUNIFORM3FPROC                  glUniform3f;
    PFNGLUNIFORM1FPROC                  glUniform1f;
    PFNGLGENBUFFERSPROC                 glGenBuffers;
    PFNGLBINDBUFFERPROC                 glBindBuffer;
    PFNGLDELETEBUFFERSPROC              glDeleteBuffers;
    PFNGLBUFFERDATAPROC                 glBufferData;
    PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays;
    PFNGLBINDVERTEXARRAYPROC            glBindVertexArray;
    PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray;
    PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray;
    PFNGLVERTEXATTRIBDIVISORPROC        glVertexAttribDivisor;
    PFNGLDRAWARRAYSINSTANCEDPROC        glDrawArraysInstanced;
    
    PFNGLSAMPLECOVERAGEPROC             glSampleCoverage;
#endif

typedef struct
{
    s32 Count;
    b32 EndedDown;
} agl_key_state;
    
typedef struct 
{
    agl_key_state Keys[256];
} agl_keyboad_input;
    
typedef struct
{
    s32 realX, realY;
    s32 X, Y, dX, dY, Wheel, dWheel;
    b32 Left, Middle, Right;
    b32 Locked;
} agl_mouse_input;

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
    b32 Active;
    s32 Width;
    s32 Height;
    s32 FPS;
    s32 TargetFPS;
    r32 Delta;
    r32 Time;
    HGLRC GLContext;
    b32 MultisampleSupported;
    s32 MultisampleFormat;
       
    agl_opengl_info GLInfo;
    agl_keyboad_input KeyboardInput;
    agl_mouse_input MouseInput;

//NOTE: Platform dependent variables
#if defined(_WIN32)
    HWND HWnd;
    HDC DC;
    WINDOWPLACEMENT Placement; // NOTE: Necessary for fullScreen feature!
    LARGE_INTEGER FrameBegin, FrameEnd, Frequency;
#elif // Other platform implementation
         
#endif
} agl_context;
agl_context __agl_Context; // global context to get access to static functions like win32 messageproc    

// NOTE: Platform specific functions. Implement these yourself if you want to. 
//       Define AGL_EXTERN beforce including this file. Ex.: #define AGL_EXTERN
//       When implementing your own platform function make sure you always apply changes
//       to __agl_Context, otherwise this api won't do anything.
AGLDEF void* aglPlatformGetProcAddress(char *Function);
AGLDEF b32   aglPlatformCreateWindow(char *Title);
AGLDEF void  aglPlatformCloseWindow();
AGLDEF void  aglPlatformDestroyWindow();
AGLDEF void  aglPlatformHandleEvents();
AGLDEF void  aglPlatformSetFixedFramerate(s32 FPS);
AGLDEF void  aglPlatformBeginFrame();
AGLDEF void  aglPlatformEndFrame();
AGLDEF void  aglPlatformSleep(s32 Time);
AGLDEF void  aglPlatformSwapBuffers();
AGLDEF void  aglPlatformToggleFullscreen();
AGLDEF void  aglPlatformSetWindowTitle(char *Title);
AGLDEF b32   aglPlatformIsActive();

typedef void (APIENTRYP agl_key_down_callback_proc) (char Key);
agl_key_down_callback_proc aglKeyDownCallback;
#define aglCallbackKeyDown(f) aglKeyDownCallback = f

typedef void (APIENTRYP agl_key_up_callback_proc) (char Key);
agl_key_up_callback_proc aglKeyUpCallback;
#define aglCallbackKeyUp(f) aglKeyUpCallback = f




#if defined(_WIN32) && defined(AGL_IMPLEMENTATION)
#include "wglext.h"

void *
aglPlatformGetProcAddress(char *Function)
{
    return wglGetProcAddress(Function);
}

void
aglPlatformToggleFullscreen()
{
    DWORD Style = GetWindowLong(__agl_Context.HWnd, GWL_STYLE);
    if(Style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO MonitorInfo = {sizeof(MonitorInfo)};
        if(GetWindowPlacement(__agl_Context.HWnd, &__agl_Context.Placement) &&
           GetMonitorInfo(MonitorFromWindow(__agl_Context.HWnd, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
        {
            SetWindowLong(__agl_Context.HWnd, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(__agl_Context.HWnd, HWND_TOP,
                         MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
                         MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
                         MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }

    }
    else
    {
        SetWindowLong(__agl_Context.HWnd, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(__agl_Context.HWnd, &__agl_Context.Placement);
        SetWindowPos(__agl_Context.HWnd, 0, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}

void
aglPlatformHandleEvents()
{
    if(!IsWindow(__agl_Context.HWnd))
    {
        __agl_Context.Running = false;
        timeEndPeriod(1);
    }
    
    MSG msg;
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE) && __agl_Context.Running)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
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
            agl_key_state *State = __agl_Context.KeyboardInput.Keys + Key;
            State->Count++;
            State->EndedDown = false;
            if(aglKeyDownCallback) aglKeyDownCallback(Key);
            return 0;
        }break;
        case WM_KEYUP:								// Has A Key Been Released?
        {
            u8 Key = (u8) wParam;
            agl_key_state *State = __agl_Context.KeyboardInput.Keys + Key;
            State->EndedDown = true;
            if(aglKeyUpCallback) aglKeyUpCallback(Key);
            return 0;
        }break;
        case WM_MOUSEMOVE:
        {                        
            s32 X = GET_X_LPARAM(lParam);
            s32 Y = GET_Y_LPARAM(lParam);
            __agl_Context.MouseInput.dX = X - __agl_Context.MouseInput.X;
            __agl_Context.MouseInput.dY = Y - __agl_Context.MouseInput.Y;
            __agl_Context.MouseInput.X = X;
            __agl_Context.MouseInput.Y = Y;
            return 0;
        }break;
        case WM_RBUTTONUP:
        {
            ReleaseCapture();
            __agl_Context.MouseInput.Right = false;
            //if(aglMouseReleased) aglMouseReleased(MK_RBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }break;
        case WM_MBUTTONUP:
        {            
            ReleaseCapture();
            __agl_Context.MouseInput.Middle = false;
            //if(aglMouseReleased) aglMouseReleased(MK_MBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }break;
        case WM_LBUTTONUP:
        {
            ReleaseCapture();
            __agl_Context.MouseInput.Left = false;
            //if(aglMouseReleased) aglMouseReleased(MK_LBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }break;
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        {
            SetCapture(__agl_Context.HWnd);
            __agl_Context.MouseInput.Left = (wParam == MK_LBUTTON || wParam == (MK_LBUTTON | MK_RBUTTON));
            __agl_Context.MouseInput.Right = (wParam == MK_RBUTTON || wParam == (MK_LBUTTON | MK_RBUTTON));
            __agl_Context.MouseInput.Middle = (wParam == MK_MBUTTON);
            //if(aglMousePressed) aglMousePressed(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }break;
        case WM_SIZE:
        {
            __agl_Context.Width = LOWORD(lParam);
            __agl_Context.Height = HIWORD(lParam);
            return 0;
        }break;
    }
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void
aglPlatformSetCursor(b32 Show)
{
    CURSORINFO CursorInfo;
    CursorInfo.cbSize = sizeof(CURSORINFO);
    GetCursorInfo(&CursorInfo);
    if(CursorInfo.flags == 0 && Show)
    {
        ShowCursor(true);
        __agl_Context.MouseInput.Locked = false;
//      ClipCursor(0);
    }
    else if(CursorInfo.flags == CURSOR_SHOWING && !Show)
    {
        ShowCursor(false);
        __agl_Context.MouseInput.Locked = true;
#if 0

        ClipCursor(&WindowRect);
#endif
    }
}

void
aglPlatformDestroyWindow()
{
    DestroyWindow(__agl_Context.HWnd);
    
    if(!UnregisterClass(AGL_WND_CLASS_NAME, GetModuleHandle(0)))
    {
        MessageBox(NULL,"Unregister failed..","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);  
    }   
    
    if (__agl_Context.GLContext)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(__agl_Context.DC, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);            
		}
        
		if (!wglDeleteContext(__agl_Context.GLContext))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
        
    }
    __agl_Context.GLContext = 0;
}

// WGLisExtensionSupported: This Is A Form Of The Extension For WGL
bool WGLisExtensionSupported(const char *extension)
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
bool InitMultisample(HINSTANCE hInstance,HWND hWnd,PIXELFORMATDESCRIPTOR pfd)
{  
    // See If The String Exists In WGL!
	if (!WGLisExtensionSupported("WGL_ARB_multisample"))
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
            WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
            WGL_SAMPLES_ARB,4,
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
	return  __agl_Context.MultisampleSupported;
}

b32
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
    //    dwStyle=WS_OVERLAPPEDWINDOW;
    dwStyle=WS_OVERLAPPEDWINDOW;
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(__agl_Context.HWnd = CreateWindowEx(	0,							// Extended Style For The Window
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
            16,											// 16Bit Z-Buffer (Depth Buffer)  
            0,											// No Stencil Buffer
            0,											// No Auxiliary Buffer
            PFD_MAIN_PLANE,								// Main Drawing Layer
            0,											// Reserved
            0, 0, 0										// Layer Masks Ignored
        };
    PIXELFORMATDESCRIPTOR *ppfd = &pfd;
    
	if (!(__agl_Context.DC = GetDC(__agl_Context.HWnd)))
	{
        aglPlatformDestroyWindow();	
		MessageBox(NULL,"Can't Create A GL Device __agl_Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

    if(!__agl_Context.MultisampleSupported)
    {
        if (!(PixelFormat=ChoosePixelFormat(__agl_Context.DC, ppfd)))
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
    
	if(!SetPixelFormat(__agl_Context.DC, PixelFormat, ppfd))
	{
        aglPlatformDestroyWindow();	
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
	}

	if (!(__agl_Context.GLContext = wglCreateContext(__agl_Context.DC)))
	{
        aglPlatformDestroyWindow();	
		MessageBox(NULL,"Can't Create A GL Rendering __agl_Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
	}

	if(!wglMakeCurrent(__agl_Context.DC, __agl_Context.GLContext))
	{
        aglPlatformDestroyWindow();	
		MessageBox(NULL,"Can't Activate The GL Rendering __agl_Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
	}

    if(!__agl_Context.MultisampleSupported)
    {
        if(InitMultisample(GetModuleHandle(0), __agl_Context.HWnd, pfd))
        {
            aglPlatformDestroyWindow();
            aglPlatformCreateWindow(Title);
        }
    }
    

#if 0
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
    if (wglCreateContextAttribsARB && wglChoosePixelFormatARB)
    {
         /*
        int Attribs[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 0,
                WGL_CONTEXT_FLAGS_ARB, 0 // NOTE(casey): Enable for testing WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
                ,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                0,
                };
         */
        int Attribs[] = {
            WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
            WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB,24,
            WGL_ALPHA_BITS_ARB,8,
            WGL_DEPTH_BITS_ARB,16,
            WGL_STENCIL_BITS_ARB,0,
            WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
            WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
            WGL_SAMPLES_ARB, 4 ,                        // Check For 4x Multisampling
            0,0};

        int pixelFormat;
        UINT numFormats;
        b32 Valid = wglChoosePixelFormatARB(__agl_Context.DC, Attribs, 0, 1, &pixelFormat, &numFormats);
        if(Valid && numFormats >= 1)
        {
            if(SetPixelFormat(__agl_Context.DC, pixelFormat, ppfd) == false)
            {
                printf("failed");
            }
        }
        
        HGLRC modern_hRC = wglCreateContextAttribsARB(__agl_Context.DC, 0, Attribs);
        if (modern_hRC)
        {           
            if(wglMakeCurrent(__agl_Context.DC, modern_hRC))
            {
                wglDeleteContext(__agl_Context.GLContext);
                __agl_Context.GLContext = modern_hRC;



            }
            

            
        }
        else
        {
            aglPlatformDestroyWindow();
            MessageBox(NULL,"Can't create modern opengl context. Version > 2.0" ,"ERROR",MB_OK|MB_ICONEXCLAMATION);
            return false;
        }
    }
#endif

	ShowWindow(__agl_Context.HWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(__agl_Context.HWnd);						// Slightly Higher Priority
    SetFocus(__agl_Context.HWnd);									// Sets Keyboard Focus To The Window

    QueryPerformanceFrequency(&__agl_Context.Frequency);
    timeBeginPeriod(1);
    
    return true;
}

void aglPlatformSwapBuffers() { SwapBuffers(__agl_Context.DC); }
void aglPlatformCloseWindow() { DestroyWindow(__agl_Context.HWnd); }
void aglPlatformSetWindowTitle(char *Title) { SetWindowText(__agl_Context.HWnd, Title); }
void aglPlatformSleep(s32 Time) { if(Time > 0) Sleep(Time); }
void aglPlatformBeginFrame() {QueryPerformanceCounter(&__agl_Context.FrameBegin); }
void aglPlatformEndFrame() {
    QueryPerformanceCounter(&__agl_Context.FrameEnd);
    __agl_Context.Delta = ((r32) (__agl_Context.FrameEnd.QuadPart - __agl_Context.FrameBegin.QuadPart) / (r32) __agl_Context.Frequency.QuadPart);

    __agl_Context.MouseInput.dX = 0;
    __agl_Context.MouseInput.dY = 0;
}
b32 aglPlatformIsActive() { return __agl_Context.Active; }


#elif // any other platform

#endif

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
        glUniformMatrix4f                  = (PFNGLUNIFORMMATRIX4FVPROC)         aglGetProcAddress("glUniformMatrix4fv");
        glUniform3f                        = (PFNGLUNIFORM3FPROC)                aglGetProcAddress("glUniform3f");
        glUniform1f                        = (PFNGLUNIFORM1FPROC)                aglGetProcAddress("glUniform1f");
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

        glSampleCoverage                   = (PFNGLSAMPLECOVERAGEPROC)          aglGetProcAddress("glSampleCoverage");
        
        Result = true;
    }
    return Result;
}

agl_context *
aglCreateWindow(char *Title = "agl default window", s32 Width = 1024, s32 Height = 768)
{
    __agl_Context.Width = Width;
    __agl_Context.Height = Height;

    if(aglPlatformCreateWindow(Title) && __agl_Context.GLContext)
    {
        __agl_Context.GLInfo.Vendor = (char *)glGetString(GL_VENDOR);
        __agl_Context.GLInfo.Renderer = (char *)glGetString(GL_RENDERER);
        __agl_Context.GLInfo.Version = (char *)glGetString(GL_VERSION);
        __agl_Context.GLInfo.ShadingLanguageVersion = (char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
        __agl_Context.GLInfo.Extensions = (char *)glGetString(GL_EXTENSIONS);
        
        __agl_Context.Running = aglInitModernGLImpl();
        aglPlatformBeginFrame();
        return &__agl_Context;
    }
    return 0;
}

void aglCloseWindow() { aglPlatformCloseWindow(); };

b32
aglHandleEvents()
{
    // Temporary key clear at the end of a frame
    for(s32 i=0; i< 256; i++) {
        agl_key_state *State = __agl_Context.KeyboardInput.Keys + i;
        if(State->EndedDown) {
            State->Count = 0;
            State->EndedDown = false;
        }
    }

    aglPlatformEndFrame();
    if(__agl_Context.TargetFPS > 0.0f)
    {
        r32 TargetFrameTime = 1.0f / __agl_Context.TargetFPS;
        if(__agl_Context.Delta < TargetFrameTime)
        {
            
            aglPlatformSleep((s32) ((TargetFrameTime - __agl_Context.Delta) * 1000));
            aglPlatformEndFrame();
        }
    }
    __agl_Context.Time += __agl_Context.Delta;
    __agl_Context.FPS = (u32) (1.0f /__agl_Context.Delta);
    
    aglPlatformBeginFrame();
    aglPlatformHandleEvents();
    return __agl_Context.Running;
}

r32  aglKeyDownTransition(char Key) { return (r32) __agl_Context.KeyboardInput.Keys[Key].Count * __agl_Context.Delta; }
b32  aglKeyDown(u8 Key) { return (!__agl_Context.KeyboardInput.Keys[Key].EndedDown && __agl_Context.KeyboardInput.Keys[Key].Count > 0); }
b32  aglKeyUp(u8 Key) { return __agl_Context.KeyboardInput.Keys[Key].EndedDown; }
void aglSwapBuffers() { aglPlatformSwapBuffers(); }
void aglSetFixedFPS(s32 FPS) { __agl_Context.TargetFPS = FPS; }
void aglToggleFullscreen() { aglPlatformToggleFullscreen(); }
void aglSetWindowTitle(char * Title) { aglPlatformSetWindowTitle(Title); }
void aglCleanup() { aglPlatformDestroyWindow(); }

#define AGL_H
#endif
