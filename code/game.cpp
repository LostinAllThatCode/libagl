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

void
DrawGrid(u32 Width, u32 Height)
{
    s32 StartX = -(Width / 2);
    s32 EndX   = (Width / 2);
    s32 StartZ = -(Height / 2);
    s32 EndZ   = (Height / 2);

    glLoadIdentity();
    glMultMatrixf(Camera.ProjectionMatrix.E);
    glMultMatrixf(Camera.ViewMatrix.E);
    
    glDisable(GL_DEPTH_TEST);
    // grid
    for(s32 x=StartX; x < EndX; x++)
    {
        for(s32 z=StartZ; z < EndZ; z++)
        {
#if 1       // Set to one if you want to see a flooring
            glColor3f(.2f, .2f, .2f);
            glBegin(GL_QUADS);
                glVertex3f(x, 0, z);
                glVertex3f(x+1, 0, z);
                glVertex3f(x+1, 0, z+1);
                glVertex3f(x, 0, z+1);
            glEnd();
#endif
            glColor3f(.6f, .6f, .6f);
            glBegin(GL_LINE_LOOP);
                glVertex3i(x, 0, z);
                glVertex3i(x+1, 0, z);
                glVertex3i(x+1, 0, z+1);
                glVertex3i(x, 0, z+1);
            glEnd();
        }
    }

    glBegin(GL_LINES);
        // x axis 
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        // y axis
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        // z axis
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -1.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();
    
    glEnable(GL_DEPTH_TEST);
}

void
resize(int width, int height)
{
    aglCameraPerspective(&Camera, 60, (GLfloat)width, (GLfloat) height, .01f, 1000.f);
    aglCameraUpdate(&Camera);
}

int
main(int argc, char **argv)
{
    aglAssignResizeScene(resize);
    HGLRC *GLContext;
    if (GLContext = aglCreateWindow("test", 1024, 768))
    {   
        char WindowTitle[128];
        agl_opengl_info Info = aglOpenGLInfo();
        printf("Vendor: %s\nRenderer: %s\nVersion: %s\nShader Version: %s\nExtensions:\n%s\n",
               Info.Vendor, Info.Renderer, Info.Version, Info.ShadingLanguageVersion, Info.Extensions);

        GLint ShaderID = glCreateProgram();
        GLint Shaders[] = {
            aglCompileShader(AGL_SHADERS_VERT_4, GL_VERTEX_SHADER),
            aglCompileShader(AGL_SHADERS_FRAG_3, GL_FRAGMENT_SHADER)
        };
        if(!aglLinkProgram(ShaderID, Shaders, ARRAY_COUNT(Shaders)))
        {
            printf("Error loading built-in shaders\nPress enter to exit");
            scanf("%c", WindowTitle); 
            return -1;
        }

        GLint mvp = glGetUniformLocation(ShaderID, "ModelViewProjection");
        void *TempMemArena = malloc ( LOAD_MODEL_MEM_SIZE );
        render_object Models[] =
        {
            aglCreateRenderTarget(TempMemArena, "models\\stormtrooper\\stormtrooper.obj")
        };
        free(TempMemArena);

        //TODO: Add Show/Hide functionality to the agl api.
        win32_context *Ctx = aglGetWin32Context();        
        r32 Speed = 7.5f;
        
        aglSetFixedFrameRate(60);

        while(aglIsRunning())
        {
            sprintf(WindowTitle, "agl_engine_3d (testing) - fps: %i, dt: %f", aglGetCurrentFPS(), aglGetDelta());
            SetWindowText(Ctx->Hwnd, WindowTitle);
            
            r32 Delta = aglGetDelta();
            if(aglKeyDown('W') || aglKeyDown('A') || aglKeyDown('S') || aglKeyDown('D') || aglMouseInput.Left)
            {
                if(aglKeyDown('W'))
                {
                    Camera.Position += Camera.Direction * Delta * Speed;
                }
                if(aglKeyDown('S'))
                {
                    Camera.Position -= Camera.Direction * Delta * Speed;
                }            
                if(aglKeyDown('A'))
                {
                    Camera.Position -= Camera.Right * Delta * Speed;  
                }
                if(aglKeyDown('D'))
                {
                    Camera.Position += Camera.Right * Delta * Speed;
                }
                if(aglMouseInput.Left)
                {
                    Camera.HorizontalAngle -= aglMouseInput.dX * Delta * (Speed * .05f);
                    Camera.VerticalAngle -= aglMouseInput.dY * Delta * (Speed * .05f);
                }
                aglCameraUpdate(&Camera);
            }

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            DrawGrid(64, 64);

            glUseProgram(ShaderID);

            for (int i=0; i < (sizeof(Models) / sizeof(Models[0])); i++)
            {
                render_object *Object = Models + i;

                int T = 16;
                for(int j=-T; j <= T; j+=4)
                {
                    for(int k=-T; k <= T; k+=4)
                    {
                        glLoadIdentity();
                        glTranslatef(Object->Position.x + j, Object->Position.y, Object->Position.z + k);
                
                        glScalef(Object->Scale.x, Object->Scale.y, Object->Scale.z);
                        glRotatef(Object->Rotation, Object->RotVector.x, Object->RotVector.y, Object->RotVector.z);
                
                        aglCameraCalcMVP(&Camera, mvp);
                        aglDrawRenderTarget(Models + i);
                    }
                }
            }
            
            glUseProgram(0);
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
