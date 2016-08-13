#include "agl.h"


r32 time;

//#include "agl_math.h"
#include "agl_core3d.h"
//#include "agl_camera.h"
#include "agl_shaders.h"

#define GET_MAX_ELEMENT_COUNT(ByteSize, TypeSize) ByteSize / TypeSize
#define LOAD_MODEL_MEM_SIZE 1024 * 1024 * 16
#define VERTEX_BUFFER_SIZE LOAD_MODEL_MEM_SIZE / 16
#define INDEX_BUFFER_SIZE LOAD_MODEL_MEM_SIZE / 4
#include "agl_renderer.cpp"

agl_context Ctx;
mat4x4 Projection, View;

void
DrawGrid(u32 Width, u32 Height, mat4x4 *P, mat4x4 *V)
{
    glUseProgram(0);
    s32 StartX = -(Width / 2);
    s32 EndX   = (Width / 2);
    s32 StartZ = -(Height / 2);
    s32 EndZ   = (Height / 2);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(P->E);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(V->E);

    glDisable(GL_DEPTH_TEST);
    for(s32 x=StartX; x < EndX; x++)
    {
        for(s32 z=StartZ; z < EndZ; z++)
        {
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
    
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -1.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
    }
    glEnd();

    glEnable(GL_DEPTH_TEST);
}

void
BeginScene(agl_camera *Camera, agl_context *Context)
{
    aglCameraUpdate(Camera, Context);
    glViewport(0, 0, Context->Width, Context->Height);
    
    Projection = PerspectiveMatrix(Camera->FoV, (r32)Context->Width / (r32)Context->Height, .1f, 100.0f);
    View = aglCameraView(Camera);
}

void 
EndScene()
{
    Projection = IdentityMat4x4();
    View = IdentityMat4x4();
}

void
InitGL()
{
	//glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_CULL_FACE);    
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations  
}

int
main(int argc, char **argv)
{
    char Title[128];
    agl_camera Camera;
    
    Ctx = aglCreateWindow();
    InitGL();

    agl_drawable Quad = {};
    Quad.GLRenderMode = GL_TRIANGLES;
    Quad.Material = aglMaterialDefault();
    aglGenQuad(&Quad.Mesh, 1);
    aglGenBuffer(&Quad.Mesh);
    
    agl_drawable Light = {};
    Light.GLRenderMode = GL_TRIANGLES;
    Light.Material = aglMaterialDefault();
    Light.Material.Ambient = V3(1,1,1);
    Light.Material.Diffuse = V3(1,1,1);
    Light.Material.Specular = V3(1,1,1);
    aglGenQuad(&Light.Mesh, 1);
    aglGenBuffer(&Light.Mesh);

    agl_shader *DefShader = aglInitDefaultShader();

    /*
    void *TempMemArena = malloc ( LOAD_MODEL_MEM_SIZE );
    render_object Models[] = { aglCreateRenderTarget(TempMemArena, "..\\models\\stormtrooper\\stormtrooper.obj")};
    free(TempMemArena);
    */
    aglCameraInit(&Camera, V3(0, 5, 15));
    Camera.Mode = AGL_CAMERA_FREE;
    aglSetFixedFPS(&Ctx, 120);
    
    while(aglHandleEvents(&Ctx))
    {
        time += Ctx.Delta;
        if(aglKeyDown(&Ctx, VK_F1)) aglSetFixedFPS(&Ctx, 120);
        if(aglKeyDown(&Ctx, VK_F2)) aglSetFixedFPS(&Ctx, 0);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        sprintf(Title, "%f, %i\n", Ctx.Delta, Ctx.FPS); 
        SetWindowText(Ctx.HWnd, Title);

        /*
        BeginScene(&Camera, &Ctx);
            DrawGrid(32, 32, &Projection, &View);
            glUseProgram(ShaderID);
            aglDrawRenderTarget(Models, &IdentityMat4x4(), &View, &Projection, ShaderID); 
        EndScene();    
        */
        aglBeginScene3D(&Ctx, &Camera);
            DrawGrid(32, 32, &CurrentProjectionMatrix, &CurrentViewMatrix);
            aglDraw(&Quad, DefShader);
            aglDraw(&Light, DefShader, MultMat4x4(ScaleMatrix(.25f, .25f, .25f), TranslationMatrix(5.0f, 5+sinf(time/2)*5.0f, -5.f)));
        aglEndScene3D(&Ctx, &Camera);
        glUseProgram(0);
        aglSwapBuffers(&Ctx);        
    }
/*
    for (int i=0; i < (sizeof(Models) / sizeof(Models[0])); i++)
    {
        if(!aglDeleteRenderTarget(Models + i))
        {
            printf("Error cleaning up model data and/or opengl resource handles. [%i]\n", i);
        }
    }
*/
    aglMeshDelete(&Quad.Mesh);
    aglMeshDelete(&Light.Mesh);
    aglDestroyWindow(&Ctx);
    return 0;
}

// v  -0.544744 2.718269 -0.216881
// vn -0.575900 -0.554200 -0.600900
