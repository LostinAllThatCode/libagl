// 64 MB temporary memory space for model loading
#define ARRAY_COUNT(Array) sizeof(Array) / sizeof(Array[0])
#define GET_MAX_ELEMENT_COUNT(ByteSize, TypeSize) ByteSize / TypeSize
#define LOAD_MODEL_MEM_SIZE 1024 * 1024 * 16
#define VERTEX_BUFFER_SIZE LOAD_MODEL_MEM_SIZE / 16
#define INDEX_BUFFER_SIZE LOAD_MODEL_MEM_SIZE / 4

#include "agl.h"
#include "agl_shaders.h"
#include "agl_math.h"
#include "agl_camera.h"

#include "agl_renderer.cpp"
#include <stdio.h>

agl_camera Camera;
mat4x4 Projection;
mat4x4 View;

s32 GlobalWidth = 1024;
s32 GlobalHeight = 768;

LARGE_INTEGER Freq;
#define BEGIN_TIMED_SECTION(name)                          \
    LARGE_INTEGER name ## Begin;                           \
    QueryPerformanceCounter(&name ## Begin)

#define END_TIMED_SECTION(name)                                         \
    LARGE_INTEGER name ## End;                                          \
    QueryPerformanceCounter(&name ## End);                              \
    printf("Section[%s] : %f ms\n", #name, 1000 * ((GLfloat) (name ## End ## .QuadPart - name ## Begin ## .QuadPart) / (GLfloat) Freq.QuadPart))
    


// TODO: Make this a vbo and pass it to the shader, cause this is slow AF.
void
DrawGrid(u32 Width, u32 Height)
{     
    s32 StartX = -(Width / 2);
    s32 EndX   = (Width / 2);
    s32 StartZ = -(Height / 2);
    s32 EndZ   = (Height / 2);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(Projection.E);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(View.E);
    
    glDisable(GL_DEPTH_TEST);
    for(s32 x=StartX; x < EndX; x++)
    {
        for(s32 z=StartZ; z < EndZ; z++)
        {
            #if 0
            glColor3f(.2f, .2f, .2f);
            glBegin(GL_QUADS);
            {
                glVertex3f(x, 0, z);
                glVertex3f(x+1, 0, z);
                glVertex3f(x+1, 0, z+1);
                glVertex3f(x, 0, z+1);
            }
            glEnd();
            #endif
            glColor3f(.6f, .6f, .6f);
            glBegin(GL_LINE_LOOP);
            {
                glVertex3i(x, 0, z);
                glVertex3i(x+1, 0, z);
                glVertex3i(x+1, 0, z+1);
                glVertex3i(x, 0, z+1);
            }
            glEnd();
        }
    }

    glLineWidth(2);
    glBegin(GL_LINES);
    {
        glColor3f(1.f, 0.0f, 0.0f);
        glVertex3f(-0.5f, 0.0f, 0.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.f, 0.0f);
        glVertex3f(0.0f, -0.5f, 0.0f);
        glVertex3f(0.0f, 0.5f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.f);
        glVertex3f(0.0f, 0.0f, -0.5f);
        glVertex3f(0.0f, 0.0f, 0.5f);
    }
    glEnd();
    glLineWidth(1);
    glEnable(GL_DEPTH_TEST);
}

void
BeginScene(agl_camera *Camera)
{
    aglCameraUpdate(Camera);
    glViewport(0, 0, GlobalWidth, GlobalHeight);
    
    Projection = PerspectiveMatrix(Camera->FoV, (r32)GlobalWidth / (r32)GlobalHeight, .1f, 100.0f);
    View = aglCameraView(Camera);
}

void 
EndScene()
{
    Projection = IdentityMat4x4();
    View = IdentityMat4x4();
}

void
ResizeScene(s32 Width, s32 Height)
{
    GlobalWidth = Width;
    GlobalHeight = Height;
}

int
main(int argc, char **argv)
{
    QueryPerformanceFrequency(&Freq);
    
    win32_context *Ctx;
    HGLRC *GLContext;
    aglAssignResizeScene(ResizeScene);
    if (GLContext = aglCreateWindow("test", GlobalWidth, GlobalHeight))
    {
        Ctx = aglGetWin32Context();
        
        char WindowTitle[128];
        agl_opengl_info Info = aglOpenGLInfo();
#if 0
        printf("Vendor: %s\nRenderer: %s\nVersion: %s\nShader Version: %s\nExtensions:\n%s\n",
               Info.Vendor, Info.Renderer, Info.Version, Info.ShadingLanguageVersion, Info.Extensions);
#endif

        GLint ShaderID = glCreateProgram();
        GLint Shaders[] = {
            aglCompileShader(AGL_SHADERS_VERT_4, GL_VERTEX_SHADER),
            aglCompileShader(AGL_SHADERS_FRAG_3, GL_FRAGMENT_SHADER)
        };
        if(!aglLinkProgram(ShaderID, Shaders, ARRAY_COUNT(Shaders)))
        {
            aglCleanUp();
            printf("Error loading built-in shaders\nPress enter to exit");
            scanf("%c", WindowTitle); 
            return -1;
        }

        GLint mvp = glGetUniformLocation(ShaderID, "matModelViewProj");

        BEGIN_TIMED_SECTION(LoadingModel);
        void *TempMemArena = malloc ( LOAD_MODEL_MEM_SIZE );
        render_object Models[] = { aglCreateRenderTarget(TempMemArena, "models\\stormtrooper\\stormtrooper.obj")};
        free(TempMemArena);
        END_TIMED_SECTION(LoadingModel);
        
        aglCameraInit(&Camera, V3(0, 5, 15));
        aglSetFixedFrameRate(60);


        while(aglIsRunning())
        {
            // TODO: Move this to the platform layer (win32_agl.cpp) and provide access to it
            sprintf(WindowTitle, "%s, %s, %s - fps: %i, dt: %f", Info.Renderer, Info.Version, Info.ShadingLanguageVersion, aglGetCurrentFPS(), aglGetDelta());
            SetWindowText(Ctx->Hwnd, WindowTitle);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            BeginScene(&Camera);
            {
        BEGIN_TIMED_SECTION(GRIDDRAW);
        DrawGrid(32, 32);
        END_TIMED_SECTION(GRIDDRAW);
                glUseProgram(ShaderID);
                aglDrawRenderBatch(Models, &TranslationMatrix(-3, 0, -3), &View, &Projection, mvp, 3);
                aglDrawRenderBatch(Models, &TranslationMatrix(-3, 0, 0), &View, &Projection, mvp, 3);
                aglDrawRenderBatch(Models, &TranslationMatrix(-3, 0, 3), &View, &Projection, mvp, 3);
            }
            EndScene();
            
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
