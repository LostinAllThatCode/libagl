#include "agl_renderer.h"

#include "agl_object_parser.cpp"

model_data *
LoadModel(char *Filename, ModelDataRawType Type, void *MemoryArena)
{
    aglAssert(MemoryArena);

    model_data *Result = {};
    
    char Directory[4096] = {};
    for(int i=strlen(Filename); i >= 0; i--)
    {
        if(Filename[i] == '\\')
        {
            memcpy(Directory, Filename, i+1);
        }
    }
    
    FILE *File = fopen(Filename, "r");
    if(File)
    {
        switch(Type)
        {
            case WAVEFRONT_OBJ:
            {
                Result = ParseWavefrontOBJ(File, Directory, MemoryArena);
            } break;
            default : break;
        }
    }

    if(Result)
    {
        printf("Model \"%s\" loaded with %i vertices\n", Filename, Result->VertexCount);
    }
    else
    {
        printf("Error parsing file or file is not existing: \"%s\"\n", Filename);
    }
    return Result;
}

render_object
aglCreateRenderTarget(void *MemoryArena, char *Filename,
                      v3 Position = V3(0,0,0), v3 Scale = V3(1,1,1), v3 RotVector = V3(0,0,0),
                      r32 Rotation = 0.f, b32 Locked = true)
{
    render_object Result = {};
    if(Filename)
    {
        Result.Model = LoadModel(Filename, WAVEFRONT_OBJ, MemoryArena);
        Result.Position = Position;
        Result.Scale = Scale;
        Result.RotVector = RotVector;
        Result.Rotation = Rotation;
        Result.glRenderType = GL_TRIANGLES;
    }
    return Result;
}

b32
aglDeleteRenderTarget(render_object *Target)
{

    b32 Result = true;
    if(Target->Model)
    {
        printf("> Model resource cleanup:\n");
        for(int j=0; j<3; j++)
        {
            if(Target->Model->VBO[j] > 0)
            {
                printf("    VBO: %i, 0x%p", Target->Model->VBO[j], Target->Model->VBO + j);
                glDeleteBuffers(1, Target->Model->VBO + j);
                if(glGetError() != GL_NO_ERROR) {
                    Result = false;
                    printf(" FAILED");
                }
                printf("\n");
            }
        }
        for(int k=0; k< Target->Model->MaterialCount; k++)
        {
            if(Target->Model->Materials[k].TextureID > 0)
            {
                printf("    TEX: %i, 0x%p", Target->Model->Materials[k].TextureID, &Target->Model->Materials[k].TextureID);
                glDeleteTextures(1, &Target->Model->Materials[k].TextureID);
                if(glGetError() != GL_NO_ERROR)
                {
                    Result = false;
                    printf(" FAILED");
                }
                printf("\n");
            }
        }
        free(Target->Model);
    }
    return Result;
}

// NOTE: Temporary matrix parameters should get moved to a general way of handling matrices
void
aglDrawRenderTarget(render_object *Target, mat4x4 *Transform, mat4x4 *View, mat4x4 *Projection, u32 ShaderID)
{
    if(Target->Model)
    {
        model_data *Model = Target->Model;
        if(Model->VBO[0])
        {
            glBindBuffer(GL_ARRAY_BUFFER, Model->VBO[0]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*Model->Vertices), 0);
            glEnableVertexAttribArray(0);
        }

        if(Model->VBO[1])
        {
            glBindBuffer(GL_ARRAY_BUFFER, Model->VBO[1]);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(*Model->UVs), 0);
            glEnableVertexAttribArray(1);
        }
        
        if(Model->VBO[2])
        {
            glBindBuffer(GL_ARRAY_BUFFER, Model->VBO[2]);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(*Model->Normals), 0);
            glEnableVertexAttribArray(2);
        }

        u32 Uniform = glGetUniformLocation(ShaderID, "matModelViewProj");

        if(Model->GroupCount > 0)
        {
            for(int i=0; i < Model->GroupCount; i++)
            {
                uint32 GroupMat   = Model->GroupIndex[i].MaterialIndex;
                uint32 GroupBegin = Model->GroupIndex[i].VertexIndex;
                uint32 GroupEnd   = Model->GroupIndex[i+1].VertexIndex;

                material Material = Model->Materials[GroupMat];
                if(Material.TextureID >= 0) glBindTexture(GL_TEXTURE_2D, Material.TextureID);

                mat4x4 Result = MultMat4x4(*Transform, *View);
                Result = MultMat4x4(Result, *Projection);
                
                //glUniformMatrix4fv(Uniform, 1, GL_FALSE, (const float *) Result.E);
                glDrawArrays(Target->glRenderType, GroupBegin, GroupEnd - GroupBegin);
            }
        } else glDrawArrays(GL_TRIANGLES, 0, Model->VertexCount);

        for (int i=0; i<3; i++)
        {
            if(Model->VBO[i] > 0) glDisableVertexAttribArray(i);
        }
    }
}

void
aglDrawRenderBatch(render_object *Target, mat4x4 *Transform, mat4x4 *View, mat4x4 *Projection, u32 Uniform, u32 Count)
{
    if(Target->Model)
    {
        model_data *Model = Target->Model;
        if(Model->VBO[0])
        {
            glBindBuffer(GL_ARRAY_BUFFER, Model->VBO[0]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*Model->Vertices), 0);
            glEnableVertexAttribArray(0);
        }

        if(Model->VBO[1])
        {
            glBindBuffer(GL_ARRAY_BUFFER, Model->VBO[1]);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(*Model->UVs), 0);
            glEnableVertexAttribArray(1);
        }
        
        if(Model->VBO[2])
        {
            glBindBuffer(GL_ARRAY_BUFFER, Model->VBO[2]);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(*Model->Normals), 0);
            glEnableVertexAttribArray(2);
        }
        

        if(Model->GroupCount > 0)
        {
            for(int i=0; i < Model->GroupCount; i++)
            {
                uint32 GroupMat   = Model->GroupIndex[i].MaterialIndex;
                uint32 GroupBegin = Model->GroupIndex[i].VertexIndex;
                uint32 GroupEnd   = Model->GroupIndex[i+1].VertexIndex;

                material Material = Model->Materials[GroupMat];
                if(Material.TextureID >= 0) glBindTexture(GL_TEXTURE_2D, Material.TextureID);

                mat4x4 Result = MultMat4x4(*Transform, *View);
                Result = MultMat4x4(Result, *Projection);
                glUniformMatrix4f(Uniform, 1, GL_FALSE, (const float *) Result.E);
                glDrawArraysInstanced(Target->glRenderType, GroupBegin, GroupEnd - GroupBegin, Count);
            }
        } else glDrawArrays(GL_TRIANGLES, 0, Model->VertexCount);

        for (int i=0; i<3; i++)
        {
            if(Model->VBO[i] > 0) glDisableVertexAttribArray(i);
        }
    }
}

void
aglUseShader(uint32 ProgramID)
{
    glUseProgram(ProgramID);
}
