#if !defined(AGL_CAMERA_H)

enum agl_camera_mode
{
    STATIC, FREE, FIRSTPERSON
};

struct agl_camera
{
    agl_camera_mode Mode;
    v3 Position;
    v3 Front;
    v3 Right;
    v3 Up;
    r32 Speed;
    r32 Sensitivity;
    r32 Yaw;
    r32 Pitch;      
    r32 FoV;
};


void
aglCameraSetTarget(agl_camera *Camera, v3 Target)
{
    Camera->Mode = FIRSTPERSON;
    Camera->Front = Target;
}

void
aglCameraInit(agl_camera *Camera, v3 Position = V3(0, 0, 0),
              r32 FoV = 45.0f, r32 Speed = 9.0f, r32 Sensitivity = 0.0025f,
              r32 Yaw = M_PI, r32 Pitch = 0.0f, agl_camera_mode Mode = FREE)
{
    Camera->Mode = Mode;
    Camera->Position = Position;  
    Camera->Speed = Speed;
    Camera->Sensitivity = Sensitivity;
    Camera->Yaw = Yaw;
    Camera->Pitch = Pitch;
    Camera->FoV = FoV;
    Camera->Front = V3(cos(Camera->Pitch) * sin(Camera->Yaw), sin(Camera->Pitch), cos(Camera->Pitch) * cos(Camera->Yaw));
    Camera->Right = V3(sin(Camera->Yaw - M_PI/2.0f), 0, cos(Camera->Yaw - M_PI/2.0f));
    Camera->Up = CrossV3(Camera->Right, Camera->Front);
}

void
aglCameraUpdate(agl_camera *Camera, agl_context *Context)
{
    agl_camera_mode Mode = Camera->Mode;
    r32 Speed = Context->Delta * Camera->Speed;
    switch(Mode)
    {
        case FREE:
        {
            if(aglKeyDown(Context, 'W')) Camera->Position += Camera->Front * Speed;
            if(aglKeyDown(Context, 'S')) Camera->Position -= Camera->Front * Speed;
            if(aglKeyDown(Context, 'D')) Camera->Position += Camera->Right * Speed;
            if(aglKeyDown(Context, 'A')) Camera->Position -= Camera->Right * Speed;
            if(Context->MouseIO.Left)
            {
                Camera->Yaw -= Context->MouseIO.dX * Camera->Sensitivity;
                Camera->Pitch -= Context->MouseIO.dY * Camera->Sensitivity;
                Camera->Front = V3(cos(Camera->Pitch) * sin(Camera->Yaw), sin(Camera->Pitch), cos(Camera->Pitch) * cos(Camera->Yaw));
                Camera->Right = V3(sin(Camera->Yaw - M_PI/2.0f), 0, cos(Camera->Yaw - M_PI/2.0f));
                Camera->Up = CrossV3(Camera->Right, Camera->Front);
            }

        }break;
        case FIRSTPERSON:
        {
            // not implemented yet
        }break;        
        case STATIC:
        {
            // not implemented yet
        }break;
    }
}

inline mat4x4
aglCameraView(agl_camera *Camera)
{
    mat4x4 Result;
    switch(Camera->Mode)
    {
        case FREE:
        {
            Result = LookAtMatrix(Camera->Position, Camera->Position + Camera->Front, Camera->Up);
        }break;
        case FIRSTPERSON:
        {
            // not implemented yet
        }break;
        case STATIC:
        {
            Result = LookAtMatrix(Camera->Position, Camera->Front, Camera->Up);
        }break;
    }
    return Result;
}

#define AGL_CAMERA_H
#endif
