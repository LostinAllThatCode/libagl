#if 1
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

model_data *
ParseWavefrontOBJ(FILE *File, char *Directory, void *MemoryArena)
{
    void *TempMemory = MemoryArena;
    
    model_data *Result = (model_data *) malloc( sizeof(model_data));        
    memset(Result, 0, sizeof(model_data));
    
    int Offset = 1;
    
    b32 ParsedFaces = false;
    b32 HasSeperateModelData = false;
    int Mode = 0;

    uint32 GrpCount=0;
    size_t VertexBufferSize = VERTEX_BUFFER_SIZE;
    size_t IndexBufferSize = INDEX_BUFFER_SIZE;
                
    uint32 iV=0, iUV=0, iNormal=0, jV=0, jUV=0, jNormal=0;
                
    v3 *BufVertex = (v3 *) TempMemory;
    uint32 *IndicesVertex = (uint32 *) ((uint8 *) BufVertex + VertexBufferSize);
                
    v2 *BufUV = (v2 *) ((uint8 *) IndicesVertex + IndexBufferSize);
    uint32 *IndicesUV = (uint32 *) ((uint8 *) BufUV + VertexBufferSize);

    v3 *BufNormal = (v3 *) ((uint8 *) IndicesUV + IndexBufferSize);
    uint32 *IndicesNormal = (uint32 *) ((uint8 *) BufNormal + VertexBufferSize);

    for(;;)
    {
        if(iV == 0)
            int a= 1;
        aglAssert(
            iV < GET_MAX_ELEMENT_COUNT(VertexBufferSize, sizeof(v3)) ||
            iUV < GET_MAX_ELEMENT_COUNT(VertexBufferSize, sizeof(v2)) ||
            iNormal < GET_MAX_ELEMENT_COUNT(VertexBufferSize, sizeof(v3))
               );
                    
        aglAssert(
            jV < GET_MAX_ELEMENT_COUNT(IndexBufferSize, sizeof(uint32)) ||
            jUV < GET_MAX_ELEMENT_COUNT(IndexBufferSize, sizeof(uint32)) ||
            jNormal < GET_MAX_ELEMENT_COUNT(IndexBufferSize, sizeof(uint32))
               );
                    
        char LineStart[64];
        if(fscanf(File, "%s", LineStart) == EOF) break;

        // NOTE: Matrial file parsing.
        else if(strcmp(LineStart, "mtllib") == 0)
        {
            char MatrialLibName[256];
            fscanf(File, "%s", MatrialLibName);
            char MaterialFullPath[4096];
            
            sprintf(MaterialFullPath, "%s%s", Directory, MatrialLibName);
            FILE *MaterialFile = fopen(MaterialFullPath, "r");
            if(MaterialFile)
            {
                for(;;)
                {
                    char LineBuffer[128];
                    if(fscanf(MaterialFile, "%s", LineBuffer) == EOF) break;
                    else if(strcmp(LineBuffer, "newmtl") == 0)
                    {
                        char MaterialName[4096] = {};
                        fscanf(MaterialFile, "%s", MaterialName);
                        memcpy(Result->Materials[Result->MaterialCount].Name, MaterialName, strlen(MaterialName));
                        Result->MaterialCount++;
                    }
                    else if(strcmp(LineBuffer, "Ka") == 0)
                    {
                        v3 Ambient = {};
                        if(fscanf(MaterialFile, "%f %f %f", &Ambient.r, &Ambient.g, &Ambient.b) == 3)
                        {
                            Result->Materials[Result->MaterialCount-1].Ambient = Ambient;
                        }
                    }
                    else if(strcmp(LineBuffer, "Kd") == 0)
                    {
                        v3 Diffuse = {};
                        if(fscanf(MaterialFile, "%f %f %f", &Diffuse.r, &Diffuse.g, &Diffuse.b) == 3)
                        {
                            Result->Materials[Result->MaterialCount-1].Diffuse = Diffuse;
                        }                        
                    }
                    else if(strcmp(LineBuffer, "Ks") == 0)
                    {
                        v3 Specular = {};
                        if(fscanf(MaterialFile, "%f %f %f", &Specular.r, &Specular.g, &Specular.b) == 3)
                        {
                            Result->Materials[Result->MaterialCount-1].Specular = Specular;
                        }                                                
                    }
                    else if(strcmp(LineBuffer, "Ns") == 0)
                    {
                        uint32 SpecExp;
                        if(fscanf(MaterialFile, "%u", &SpecExp) == 1)
                        {
                            Result->Materials[Result->MaterialCount-1].SpecExp = SpecExp;
                        }
                    }
                    
                    else if(strcmp(LineBuffer, "map_Kd") == 0)
                    {
                        char TextureFullPath[4096] = {};
                        char TextureFile[4096] = {};
                        
                        fscanf(MaterialFile, "%s", TextureFile);
                        if(strcmp(TextureFile, ".") == 0)
                        {
                            sprintf(TextureFullPath, "%sdefault.tga", Directory);
                        } else
                        {
                            sprintf(TextureFullPath, "%s%s", Directory, TextureFile);
                        } 
                        
                        // TODO: MOVE THIS TEXTURE LOADING OUTAHERE
                        glGenTextures(1, &Result->Materials[Result->MaterialCount-1].TextureID);
                        glBindTexture(GL_TEXTURE_2D, Result->Materials[Result->MaterialCount-1].TextureID);

                        stbi_set_flip_vertically_on_load(true);
                        
                        int x,y,n;
                        unsigned char *data = stbi_load(TextureFullPath, &x, &y, &n, 0);
                        if(n == 3)
                        {
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                        } else if(n == 4) {
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                        }
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                        stbi_image_free(data);
                    }
                }
            }
            
        }

        // NODE: Object file parsing
        else if(strcmp(LineStart, "v") == 0)
        {
            v3 TempVertex;
            if(fscanf(File, "%f %f %f", &TempVertex.x, &TempVertex.y, &TempVertex.z) == 3)
            {
                BufVertex[iV++] = TempVertex;
            }
        }
        else if(strcmp(LineStart, "vt") == 0)
        {
            v2 TempUV;
            if(fscanf(File, "%f %f", &TempUV.u, &TempUV.v) == 2)
            {
                BufUV[iUV++] = TempUV;
            } 
        }
        else if(strcmp(LineStart, "vn") == 0)
        {
            v3 TempNormal;
            if(fscanf(File, "%f %f %f", &TempNormal.x, &TempNormal.y, &TempNormal.z) == 3)
            {
                BufNormal[iNormal++] = TempNormal;
            }
        }
        else if(strcmp(LineStart, "f") == 0)
        {
            if(!Mode)
            {
                fpos_t FilePosition;
                char FaceSpec[32] = {};
                fgetpos(File, &FilePosition);
                fscanf(File, "%s", FaceSpec);

                char *CharPointer = FaceSpec;
                for(;*CharPointer;CharPointer++){
                    if(*CharPointer == '/') {
                        Mode++;
                        char *next = CharPointer + 1;
                        if(*next == '/')
                        {
                            Mode = 3;
                            break;
                        }
                    }
                }
                if(!Mode) return 0;
                fsetpos(File, &FilePosition);
            }
            switch(Mode)
            {
                case 1:
                {
                    uint32 vertexIndex[3], uvIndex[3];
                    if(fscanf(File, "%d/%d %d/%d %d/%d\n",
                              &vertexIndex[0], &uvIndex[0],
                              &vertexIndex[1], &uvIndex[1],
                              &vertexIndex[2], &uvIndex[2]) == 6)
                    {
                        IndicesVertex[jV++] = vertexIndex[0];
                        IndicesVertex[jV++] = vertexIndex[1];
                        IndicesVertex[jV++] = vertexIndex[2];

                        IndicesUV[jUV++] = uvIndex[0];
                        IndicesUV[jUV++] = uvIndex[1];
                        IndicesUV[jUV++] = uvIndex[2];
                    }
                } break;
                case 2:
                {
                    uint32 vertexIndex[3], uvIndex[3], normalIndex[3], __ignored;
                    if(fscanf(File, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                              &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                              &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                              &vertexIndex[2], &uvIndex[2], &normalIndex[2]) == 9)
                    {
                        IndicesVertex[jV++] = vertexIndex[0];
                        IndicesVertex[jV++] = vertexIndex[1];
                        IndicesVertex[jV++] = vertexIndex[2];

                        IndicesUV[jUV++] = uvIndex[0];
                        IndicesUV[jUV++] = uvIndex[1];
                        IndicesUV[jUV++] = uvIndex[2];

                        IndicesNormal[jNormal++] = normalIndex[0];
                        IndicesNormal[jNormal++] = normalIndex[1];
                        IndicesNormal[jNormal++] = normalIndex[2];
                    }
                } break;
                case 3:
                {
                    uint32 vertexIndex[3], normalIndex[3];
                    if(fscanf(File, "%d//%d %d//%d %d//%d\n",
                              &vertexIndex[0], &normalIndex[0],
                              &vertexIndex[1], &normalIndex[1],
                              &vertexIndex[2], &normalIndex[2]) == 6)
                    {
                        IndicesVertex[jV++] = vertexIndex[0];
                        IndicesVertex[jV++] = vertexIndex[1];
                        IndicesVertex[jV++] = vertexIndex[2];

                        IndicesNormal[jNormal++] = normalIndex[0];
                        IndicesNormal[jNormal++] = normalIndex[1];
                        IndicesNormal[jNormal++] = normalIndex[2];
                    }
                }break;
                default:
                {
                    // Not implemented yet!
                    printf("Not implemented yet!\n");
                    return 0;
                }break;
            }
        }
        else if(strcmp(LineStart, "usemtl") == 0)
        {
            Result->GroupIndex[Result->GroupCount].VertexIndex = jV;

            char MatrialName[4096];
            fscanf(File, "%s", MatrialName);
            for(int i=0; i < Result->MaterialCount; i++)
            {
                if(strcmp(MatrialName, Result->Materials[i].Name) == 0)
                {
                    Result->GroupIndex[Result->GroupCount].MaterialIndex = i;
                }
            }
            Result->GroupCount++;
        }
    }
    Result->GroupIndex[Result->GroupCount].VertexIndex = jV;
    
    if(jV > 0 && iV > 0)
    {
        Result->Vertices = (v3 *) malloc( sizeof(v3) * jV );
        for(int i=0; i < jV; i++)
        {
            uint32 VertexIndex = IndicesVertex[i] - Offset;
            Result->Vertices[i] = BufVertex[VertexIndex]; 
        }
        Result->VertexCount = jV;
                    
        // NOTE: Possibly move this out of parsing so it can be used for other formats as well!
        glGenBuffers(1, Result->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Result->VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, Result->VertexCount * sizeof(*Result->Vertices), Result->Vertices, GL_STATIC_DRAW);
    }

    if(jUV > 0 && iUV > 0)
    {
        Result->UVs = (v2 *) malloc( sizeof(v2) * jUV );
        for(int i=0; i < jUV; i++)
        {
            uint32 VertexIndex = IndicesUV[i] - Offset;
            Result->UVs[i] = BufUV[VertexIndex]; 
        }

        // NOTE: Possibly move this out of parsing so it can be used for other formats as well!
        glGenBuffers(1, Result->VBO + 1);
        glBindBuffer(GL_ARRAY_BUFFER, Result->VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, Result->VertexCount * sizeof(*Result->UVs), Result->UVs, GL_STATIC_DRAW);
    }
    
    if(jNormal > 0 && iNormal > 0)
    {
        Result->Normals = (v3 *) malloc( sizeof(v3) * jNormal );
        for(int i=0; i < jNormal; i++)
        {
            uint32 VertexIndex = IndicesNormal[i] - Offset;
            Result->Normals[i] = BufNormal[VertexIndex];
        }
        glGenBuffers(1, Result->VBO + 2);
        glBindBuffer(GL_ARRAY_BUFFER, Result->VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, Result->VertexCount * sizeof(*Result->Normals), Result->Normals, GL_STATIC_DRAW);
    }

    if(Result->VertexCount == 0) free(Result);

    printf("(%zu kb) ",
           ((sizeof(uint32) * jV) + (sizeof(uint32) * jUV) + (sizeof(uint32) * jNormal) +
            (sizeof(v3) * iV) + (sizeof(v2) * iUV) + (sizeof(v3) * iNormal)) / 1024);
    
    return Result;
}
