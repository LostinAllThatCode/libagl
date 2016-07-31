#if !defined(AGL_RENDERER_H)


enum ModelDataRawType {
    WAVEFRONT_OBJ,
};

struct matrial_group
{
    uint32 VertexIndex;
    uint32 MaterialIndex;
};

struct material_map
{
    char    Name[4096];
    uint32  TextureID;
};

struct model_data
{
    uint32 VBO[3];                       //NOTE: 0=Vertex, 1=UV, 2=Normal
    material_map Materials[12];
    uint32 MaterialCount;  
    matrial_group GroupIndex[256];
    uint32 GroupCount;

    uint32 VertexCount;
    v3 *Vertices;
    v2 *UVs;
    v3 *Normals;
};

struct render_object
{
    model_data *Model;

    v3 RotVector = V3(0, 0, 0);
    v3 Scale = V3(1, 1, 1);
    v3 Position;
    r32 Rotation;

    b32 Locked = true;
};

#define AGL_RENDERER_H
#endif
