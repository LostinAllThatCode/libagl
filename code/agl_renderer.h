#if !defined(AGL_RENDERER_H)

enum ModelDataRawType {
    WAVEFRONT_OBJ,
};

struct render_group
{
    u32 VertexIndex;
    uint32 MaterialIndex;
};

struct material
{
    char    Name[512];
    v3      Ambient;
    v3      Diffuse;
    v3      Specular;
    u32     SpecExp;  
    u32     IlluminationMode;  
    u32     TextureID;
};

struct model_data
{
    u32 VBO[3];                       //NOTE: 0=Vertex, 1=UV, 2=Normal
    material Materials[12];
    uint32 MaterialCount;  
    render_group GroupIndex[256];
    u32 GroupCount;

    u32 VertexCount;
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

    GLenum glRenderType;
};

struct texture_mapping
{
    u32      Index;
    material *Material;
};

struct mesh
{
    u32 VAO;
    u32 VBO[8];
    v3  *Vertices;
    v2  *TextureCoords;
    v3  *Normals;
    u32 VertexCount;
    u32 TriangleCount;
};

#define AGL_RENDERER_H
#endif
