#include "agl.cpp"

HGLRC hRC;
win32_context Context;
GLboolean Running;

agl_keyboad_input KeybInput = {};

LARGE_INTEGER PerformanceFrequency;
LARGE_INTEGER BeginFrame, EndFrame;
GLfloat FixedFrameRateMicro;
GLfloat CurrentFrameRateMicro = 0;
#define TIMER_RESOLUTION 1

int FrameCount = 0;

win32_context *
aglGetWin32Context(void)
{
    return &Context;
}

void *
aglPlatformGetProcAddress(char *procname)
{
    void *ptr = 0;
    if (!(ptr = wglGetProcAddress(procname)))
        MessageBox(0, "wglGetProcAddress failed.", "fatal error", MB_OK);
    return ptr;
}

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

#if 0
void
ToggleFullscreen()
{
    // NOTE(casey): This follows Raymond Chen's prescription
    // for fullscreen toggling, see:
    // http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx
    DWORD Style = GetWindowLong(Platform.WindowHandle, GWL_STYLE);
    if(Style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO MonitorInfo = {sizeof(MonitorInfo)};
        if(GetWindowPlacement(Platform.WindowHandle, &Platform.LastWindowPosition) &&
           GetMonitorInfo(MonitorFromWindow(Platform.WindowHandle, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
        {
            SetWindowLong(Platform.WindowHandle, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(Platform.WindowHandle, HWND_TOP,
                         MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
                         MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
                         MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
    else
    {
        SetWindowLong(Platform.WindowHandle, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(Platform.WindowHandle, &Platform.LastWindowPosition);
        SetWindowPos(Platform.WindowHandle, 0, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED); 
    }
}
#endif

GLboolean
aglPlatformContextAlive(void)
{
    MSG msg;
    while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return Running;
}

void
aglPlatformFixedFrameRate(GLuint FramesPerSecond)
{
    FixedFrameRateMicro = 1.0f / FramesPerSecond;
    if (!(timeBeginPeriod(TIMER_RESOLUTION) == TIMERR_NOERROR)) printf("FUCK THAT WIN32 TIMER SHIT");
}

void
aglPlatformBeginFrame(void)
{
    QueryPerformanceCounter(&BeginFrame);
    FrameCount++;

    POINT MouseCoords;
    RECT WindowRect;
    GetWindowRect(Context.Hwnd, &WindowRect);
    GetCursorPos(&MouseCoords);
    if(!(MouseCoords.x > WindowRect.left && MouseCoords.x < WindowRect.right &&
         MouseCoords.y > WindowRect.top && MouseCoords.y < WindowRect.bottom))
    {
        aglMouseInput.Left = GL_FALSE;
        aglMouseInput.Middle = GL_FALSE;
        aglMouseInput.Right = GL_FALSE;
    }
}

void
aglPlatformEndFrame(void)
{
    QueryPerformanceCounter(&EndFrame);
    CurrentFrameRateMicro = ((GLfloat) (EndFrame.QuadPart - BeginFrame.QuadPart) / (GLfloat) PerformanceFrequency.QuadPart);
    if(FixedFrameRateMicro > 0)
    {
        if(CurrentFrameRateMicro < FixedFrameRateMicro)
        {
            LARGE_INTEGER PreSleep, PostSleep;
            GLfloat SleepTime = (FixedFrameRateMicro - CurrentFrameRateMicro) * 1000;
            if(SleepTime > 0)
            {
                Sleep(SleepTime);
                LARGE_INTEGER PostSleep;
                QueryPerformanceCounter(&PostSleep);
                CurrentFrameRateMicro = ((GLfloat) (PostSleep.QuadPart - BeginFrame.QuadPart) / (GLfloat) PerformanceFrequency.QuadPart);
            }
        }
    }

    aglMouseInput.dX = 0;
    aglMouseInput.dY = 0;
}

GLfloat
aglPlatformGetMilliSeconds(void)
{
    return CurrentFrameRateMicro * 1000;
}

void
aglPlatformUpdateContext()
{
    SwapBuffers(Context.DC);
}

GLboolean
aglPlatformDestroyContext(void)
{
    GLboolean Result = GL_TRUE;
    timeEndPeriod(TIMER_RESOLUTION);

    printf("Frames drawn: %i\n", FrameCount); 
            
    //    ToggleFullscreen();
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(Context.DC, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
            Result = GL_FALSE;
		}
        
		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
            Result =  GL_FALSE;
		}
        
        hRC = 0;
	}
    
    if (Context.DC && !ReleaseDC(Context.Hwnd, Context.DC))
    {
        MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        Context.DC = 0;
        Result =  GL_FALSE;
    }

    if (Context.Hwnd && !DestroyWindow(Context.Hwnd))
    {
        MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        Context.Hwnd = 0;
        Result =  GL_FALSE;
    }

    if (!UnregisterClass(AGL_WND_CLASS_NAME, Context.Instance))			// Are We Able To Unregister Class
    {
        MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        Context.Instance = 0;
        Result =  GL_FALSE;
    }
   
    return Result;
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
HGLRC *
aglPlatformCreateContext(char* title, int width, int height)
{
    QueryPerformanceFrequency(&PerformanceFrequency);
    
    //aglAssignGetProcAddress(Win32GetProcAddress);
    
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Value
    
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= Context.Instance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= AGL_WND_CLASS_NAME;					    // Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;											// Return FALSE
	}

    dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
    dwStyle=WS_OVERLAPPEDWINDOW;

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(Context.Hwnd = CreateWindowEx(	0,							// Extended Style For The Window
								AGL_WND_CLASS_NAME,							// Class Name
 								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								Context.Instance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		aglPlatformDestroyContext();
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;
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
	
	if (!(Context.DC = GetDC(Context.Hwnd)))
	{
        aglPlatformDestroyContext();
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	if (!(PixelFormat=ChoosePixelFormat(Context.DC, &pfd)))
	{
        aglPlatformDestroyContext();
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	if(!SetPixelFormat(Context.DC, PixelFormat, &pfd))
	{
		aglPlatformDestroyContext();
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	if (!(hRC = wglCreateContext(Context.DC)))
	{
        aglPlatformDestroyContext();	
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	if(!wglMakeCurrent(Context.DC, hRC))
	{
        aglPlatformDestroyContext();	
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

#if 0
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

        HGLRC modern_hRC = wglCreateContextAttribsARB(hDC, 0, Attribs);
        if (modern_hRC)
        {
            if(wglMakeCurrent(hDC, modern_hRC))
            {
                wglDeleteContext(hRC);
                hRC = modern_hRC;
            }
            
        }
        else
        {
            aglPlatformDestroyContext();	
            MessageBox(NULL,"Can't create modern opengl context. Version > 2.0" ,"ERROR",MB_OK|MB_ICONEXCLAMATION);
            return 0;
        }
    }
#endif

    
#if 1
	ShowWindow(Context.Hwnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(Context.Hwnd);						// Slightly Higher Priority
    SetFocus(Context.Hwnd);									// Sets Keyboard Focus To The Window
#endif
    //MessageBoxA(0,(char*)glGetString(GL_SHADING_LANGUAGE_VERSION), "OPENGL VERSION",0);
    return &hRC;
}

GLboolean
aglPlatformKeyDown(GLchar Key)
{
    return KeybInput.Keys[Key];
}

void
aglPlatformSetTitle(char *Title)
{
    SetWindowText(Context.Hwnd, Title);
}

#define sign(a) ( ( (a) < 0 )  ?  -1   : ( (a) > 0 ) )
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
void
win32UpdateMouseCoords(LPARAM lParam)
{
    GLint X = GET_X_LPARAM(lParam);
    GLint Y = GET_Y_LPARAM(lParam);
    aglMouseInput.dX = X - aglMouseInput.X;
    aglMouseInput.dY = Y - aglMouseInput.Y;
    aglMouseInput.X = X;
    aglMouseInput.Y = Y;
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				Running = GL_TRUE;						// Program Is Active
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
        case WM_MOUSEMOVE:
        {
            win32UpdateMouseCoords(lParam);
            return 0;
        } 
        case WM_MOUSEWHEEL:
        {
            int Direction = sign((int) wParam);
            aglMouseInput.Wheel += Direction;
            if(aglMouseWheel) aglMouseWheel( ((int)wParam) < 0 ? -1 : 1, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
            return 0;
        }
        case WM_RBUTTONUP:

        {
            win32UpdateMouseCoords(lParam);
            aglMouseInput.Right = GL_FALSE;
                
            if(aglMouseUp) aglMouseUp(MK_RBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }
        case WM_MBUTTONUP:
        {
            aglMouseInput.Middle = GL_FALSE;
            //TODO: mbutton function to user
            return 0;
        }
        case WM_LBUTTONUP:
        {
            win32UpdateMouseCoords(lParam);
            aglMouseInput.Left = GL_FALSE;
            
            if(aglMouseUp) aglMouseUp(MK_LBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        {
            win32UpdateMouseCoords(lParam);
            aglMouseInput.Left = (wParam == MK_LBUTTON);
            aglMouseInput.Middle = (wParam == MK_MBUTTON);
            aglMouseInput.Right = (wParam == MK_RBUTTON);
            
            if(aglMouseDown) aglMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }
        case WM_CLOSE:								// Did We Receive A Close Message?
		{
            Running = false;
		} return 0;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
            KeybInput.Keys[wParam] = 1;
            //if(aglKeyDown) aglKeyDown(wParam);
			return 0;								// Jump Back            
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
            KeybInput.Keys[wParam] = 0;
//            if(aglKeyUp) aglKeyUp(wParam);
			return 0;								// Jump Back
		}
		case WM_SIZE:								// Resize The OpenGL Window
		{
            if(aglResizeScene) aglResizeScene(LOWORD(lParam),HIWORD(lParam));
			return 0;								// Jump Back
		}
	}
	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved )
{
    
    switch ( nReason )
    {
        case DLL_PROCESS_ATTACH:
        {
            Context.Instance = hDllHandle;
            aglAssignGetProcAddress(aglPlatformGetProcAddress);
        } break;
        case DLL_PROCESS_DETACH:
            break;
    }

    if (!Context.Instance) return false;
    return true;
}
