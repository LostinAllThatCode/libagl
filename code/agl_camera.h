
struct agl_camera
{
    v3 Position = {0, 5, 25};
    v3 Direction;
    v3 Right;
    v3 Up;

    r32 HorizontalAngle = 3.14f;
    r32 VerticalAngle = 0.f;
   
    
    GLfloat Width;
    GLfloat Height;
    GLfloat FieldOfView; 
    GLfloat AspectRatio; 
    GLfloat Near;
    GLfloat Far;

    mat4x4 ProjectionMatrix = IdentityMat4x4();
    mat4x4 ViewMatrix = IdentityMat4x4();
};

void
aglCameraPerspective(agl_camera *Camera, GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat Near, GLfloat Far)
{
    Camera->ProjectionMatrix = {};
    Camera->Width = Width;
    Camera->Height = Height;
    Camera->FieldOfView = FOV;
    Camera->AspectRatio = Width / Height;
    Camera->Near = Near;
    Camera->Far = Far;

    Camera->ProjectionMatrix = PerspectiveMatrix(FOV, Width / Height, Near, Far);
    //Transpose(&Camera->ProjectionMatrix);
    

    glViewport(0, 0, Camera->Width, Camera->Height);
}

void
aglCameraLookAt(agl_camera *Camera, v3 Eye, v3 Center, v3 Up)
{
    Camera->ViewMatrix = LookAtMatrix(Eye, Center, Up);
}

void
aglCameraUpdate(agl_camera *Camera)
{
    Camera->Direction = V3( cos(Camera->VerticalAngle) * sin(Camera->HorizontalAngle),
                            sin(Camera->VerticalAngle),
                            cos(Camera->VerticalAngle) * cos(Camera->HorizontalAngle));
    
    Camera->Right = V3( sin(Camera->HorizontalAngle - 3.14f/2.0f), 0, cos(Camera->HorizontalAngle - 3.14f/2.0f));
    Camera->Up = CrossV3(Camera->Right, Camera->Direction);
    aglCameraLookAt(Camera, Camera->Position, Camera->Position + Camera->Direction, Camera->Up);
}

void
aglCameraCalcMVP(agl_camera *Camera, GLuint ID, mat4x4 *Model)
{
    mat4x4 Result;
    /*
    glGetFloatv(GL_MODELVIEW_MATRIX, Model.E);
    glLoadIdentity();
    glMultMatrixf(Camera->ProjectionMatrix.E);
    glMultMatrixf(Camera->ViewMatrix.E);
    glMultMatrixf(Model.E);  
    glGetFloatv(GL_MODELVIEW_MATRIX, Result.E);
    */
#if 0
    Result = MultMat4x4(IdentityMat4x4(), Camera->ProjectionMatrix);
    Result = MultMat4x4(Result, Camera->ViewMatrix);
    Result = MultMat4x4(Result, Model);
#else
    Result = MultMat4x4(IdentityMat4x4(), *Model);
    Result = MultMat4x4(Result, Camera->ViewMatrix);
    Result = MultMat4x4(Result, Camera->ProjectionMatrix);    
#endif
    glUniformMatrix4fv(ID, 1, GL_FALSE, (const float *) Result.E);
}
