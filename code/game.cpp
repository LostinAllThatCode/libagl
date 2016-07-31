// 64 MB temporary memory space for model loading

#define ARRAY_COUNT(Array) sizeof(Array) / sizeof(Array[0])
#define GET_MAX_ELEMENT_COUNT(ByteSize, TypeSize) ByteSize / TypeSize
#define LOAD_MODEL_MEM_SIZE 1024 * 1024 * 16
#define VERTEX_BUFFER_SIZE LOAD_MODEL_MEM_SIZE / 16
#define INDEX_BUFFER_SIZE LOAD_MODEL_MEM_SIZE / 4

#include "agl.h"
#include "agl_shaders.h"

#include "platform.h"

#include "agl_math.h"
#include "agl_camera.h"

#include "agl_renderer.cpp"

#include <stdio.h>

agl_camera Camera;

GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[]  = { 50.0 };
GLfloat light_position[] = { 0.0, 3.0, 5.0, 0.0 };

r32 direction = 0.f;
r32 rot_speed = 0.f;
uint8 KeyState[256] = {};

void
init(void)
{
    /*
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    */
#if 1
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
//    glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
//    glDepthFunc(GL_LESS);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);	// Really Nice Perspective Calculations
#else
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Nice perspective corrections
#endif
}

void
resize(int width, int height)
{
    aglCameraPerspective(&Camera, 90, (GLfloat)width, (GLfloat) height, .01f, 1000.f);
    aglCameraUpdate(&Camera);
}

void
KeyDown(uint32 Key)
{
    KeyState[Key] = 1;
}

void
KeyUp(uint32 Key)
{
    KeyState[Key] = 0;
}

int
main(int argc, char **argv)
{
    HGLRC *GLContext;
    
    aglAssignResizeScene(resize);
    aglAssignKeyDown(KeyDown);
    aglAssignKeyUp(KeyUp);

    if (GLContext = aglCreateWindow("test", 1024, 768))
    {   
        char WindowTitle[128];
        
        agl_opengl_info Info = aglOpenGLInfo();
        printf("Vendor: %s\nRenderer: %s\nVersion: %s\nShader Version: %s\nExtensions:\n%s\n",
               Info.Vendor, Info.Renderer, Info.Version, Info.ShadingLanguageVersion, Info.Extensions);

        GLint ShaderID = glCreateProgram();
        GLint Shaders[] = {
            aglCompileShader(AGL_SHADERS_VERT_3, GL_VERTEX_SHADER),
            aglCompileShader(AGL_SHADERS_FRAG_3, GL_FRAGMENT_SHADER)
        };
        if(!aglLinkProgram(ShaderID, Shaders, ARRAY_COUNT(Shaders)))
        {
            printf("Error loading built-in shaders\n");
            return -1;
        }

        GLint mvp = glGetUniformLocation(ShaderID, "currentMVP");
        
        void *TempMemArena = malloc ( LOAD_MODEL_MEM_SIZE );
       
        render_object Models[] =
        {
#if 0
            aglCreateRenderTarget(TempMemArena, "models\\untitled.obj", V3(0,0,0), V3(5.f, .1f, 5.f)),
            aglCreateRenderTarget(TempMemArena, "models\\pokemon\\Charizard.obj", V3(0.f, 5.f, 0.f), V3(.25f, .25f, .25f), V3(1.f, 0.f, 0.f), 45.f),
            aglCreateRenderTarget(TempMemArena, "models\\girl_3.obj", V3(3.5f, 0, 3.5f), V3(2.f, 2.f, 2.f), V3(0.f, 1.f, 0.f), 45.f),
            aglCreateRenderTarget(TempMemArena, "models\\Lightning\\lightning_obj.obj", V3(-3.5f, 2.f, 3.5f), V3(.03f, .03f, .03f), V3(0.f, 1.f, 0.f), -45.f),
            aglCreateRenderTarget(TempMemArena, "models\\Stormtrooper\\Stormtrooper.obj", V3(-3.5f, 0.f, -3.5f), V3(1.f, 1.f, 1.f), V3(0.f, 1.f, 0.f), -135.f),
            aglCreateRenderTarget(TempMemArena, "models\\R2-D2\\R2-D2.obj", V3(3.5f, 0.f, -3.5f), V3(1.f, 1.f, 1.f), V3(0.f, 1.f, 0.f), 135.f),
            aglCreateRenderTarget(TempMemArena, "models\\Deadpool\\dead_123456.obj", V3(3.5f, 0.f, 0.f), V3(.02f, .02f, .02f)),
            aglCreateRenderTarget(TempMemArena, "models\\Hulk\\hulk.obj", V3(0.f, 0.f, -3.5f), V3(1.f, 1.f, 1.f), V3(0.f, 1.f, 0.f), 180.f),
            aglCreateRenderTarget(TempMemArena, "models\\Sonic\\Sonic.obj", V3(-3.5f, 0.f, 0.f), V3(.05f, .05f, .05f), V3(0.f, 1.f, 0.f), -90.f),
            //aglCreateRenderTarget(TempMemArena, "models\\Lara\\Lara_Croft_default.obj")
#else
            aglCreateRenderTarget(TempMemArena, "models\\stormtrooper\\stormtrooper.obj")
#endif
        };

        free(TempMemArena);

        //TODO: Add Show/Hide functionality to the agl api.
        win32_context *Ctx = aglGetWin32Context();
        ShowWindow(Ctx->Hwnd, SW_SHOW);
        SetForegroundWindow(Ctx->Hwnd);
        SetFocus(Ctx->Hwnd);
        
        r32 Speed = 7.5f;
        
        aglSetFixedFrameRate(60);

        glUseProgram(ShaderID);
        while(aglIsRunning())
        {

            sprintf(WindowTitle, "agl_engine_3d (testing) - fps: %i, dt: %f", aglGetCurrentFPS(), aglGetDelta());
            SetWindowText(Ctx->Hwnd, WindowTitle);
            
            r32 Delta = aglGetDelta();
            if(aglMouseInput.Left)
            {
                Camera.HorizontalAngle -= aglMouseInput.dX * Delta * (Speed * .05f);
                Camera.VerticalAngle -= aglMouseInput.dY * Delta * (Speed * .05f);
            }

            if(KeyState['W'])
            {
                Camera.Position += Camera.Direction * Delta * Speed;
            }
            if(KeyState['S'])
            {
                Camera.Position -= Camera.Direction * Delta * Speed;
            }            
            if(KeyState['A'])
            {
                Camera.Position -= Camera.Right * Delta * Speed;  
            }
            if(KeyState['D'])
            {
                Camera.Position += Camera.Right * Delta * Speed;
            }            

            aglCameraUpdate(&Camera);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
           
            for (int i=0; i < (sizeof(Models) / sizeof(Models[0])); i++)
            {
                render_object *Object = Models + i;
                
                glLoadIdentity();
                glTranslatef(Object->Position.x, Object->Position.y, Object->Position.z);
                
                glScalef(Object->Scale.x, Object->Scale.y, Object->Scale.z);
                glRotatef(Object->Rotation, Object->RotVector.x, Object->RotVector.y, Object->RotVector.z);
                
                aglCameraCalcMVP(&Camera, mvp);
                aglDrawRenderTarget(Models + i);
            }

            aglUpdate();
        }
        
        for (int i=0; i < (sizeof(Models) / sizeof(Models[0])); i++)
        {
            if(!aglDeleteRenderTarget(Models + i))
            {
                printf("Error cleaning up model data and/or opengl resource handles. [%i]\n", i);
            }
        }
        
        aglCleanUp();
    }
	return 0;
}
