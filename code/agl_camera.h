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
aglCameraUpdate(agl_camera *Camera)
{
    agl_camera_mode Mode = Camera->Mode;
    r32 Speed = aglGetDelta() * Camera->Speed;
    switch(Mode)
    {
        case FREE:
        {
            if(aglKeyDown('W')) Camera->Position += Camera->Front * Speed;
            if(aglKeyDown('S')) Camera->Position -= Camera->Front * Speed;
            if(aglKeyDown('D')) Camera->Position += Camera->Right * Speed;
            if(aglKeyDown('A')) Camera->Position -= Camera->Right * Speed;
            if(aglMouseInput.Left)
            {
                Camera->Yaw -= aglMouseInput.dX * Camera->Sensitivity;
                Camera->Pitch -= aglMouseInput.dY * Camera->Sensitivity;
                Camera->Front = V3(cos(Camera->Pitch) * sin(Camera->Yaw), sin(Camera->Pitch), cos(Camera->Pitch) * cos(Camera->Yaw));
                Camera->Right = V3(sin(Camera->Yaw - M_PI/2.0f), 0, cos(Camera->Yaw - M_PI/2.0f));
                Camera->Up = CrossV3(Camera->Right, Camera->Front);
            }
        }break;
    }
}

