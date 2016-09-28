#if !defined(AGL_SHADERS_H)

// Quick and simple hashtable implementation
struct agl_hashtable_entry_s {
	char *Key;
    int Value;
	agl_hashtable_entry_s *Next;
};
typedef struct agl_hashtable_entry_s agl_hashtable_entry;

struct agl_hashtable_s {
	int Size;
	agl_hashtable_entry_s **Table;	
};
typedef struct agl_hashtable_s agl_hashtable;

agl_hashtable *
aglHashTableCreate(int Size)
{
	agl_hashtable *HashTable = 0;
	if(Size < 1) return 0;

    HashTable = (agl_hashtable *) malloc( sizeof(agl_hashtable) );
    if(!HashTable) return 0;

    HashTable->Table = (agl_hashtable_entry **) malloc( sizeof(agl_hashtable_entry *) * Size );
    memset(HashTable->Table, 0, sizeof(agl_hashtable_entry *) * Size);

	HashTable->Size = Size;

	return HashTable;	
}

int
aglHashTableGen(agl_hashtable *HashTable, char *Key)
{
	unsigned long int Hash = 0;
	int i = 0;
	while( Hash < ULONG_MAX && i < strlen( Key ) ) {
		Hash = Hash << 8;
		Hash += Key[ i ];
		i++;
	}
	return Hash % HashTable->Size;
}

agl_hashtable_entry *
aglHashTableNewPair(char *Key, int Value) {
	agl_hashtable_entry *Pair;
    Pair = (agl_hashtable_entry *) malloc(sizeof(agl_hashtable_entry));
    if(!Pair) return 0;

    Pair->Key = strdup(Key);
    if(!Pair->Key) return 0;

    Pair->Value = Value;
	Pair->Next = 0;
	return Pair;
}

b32
aglHashTableHasEntry(agl_hashtable *HashTable, char *Key)
{
    return ( (HashTable->Table[aglHashTableGen(HashTable, Key)]) != 0 );
}


void
aglHashTableSet(agl_hashtable *HashTable, char *Key, int Value) {
	agl_hashtable_entry *Pair = 0;
	agl_hashtable_entry *Next = 0;
	agl_hashtable_entry *Last = 0;

    int Idx = aglHashTableGen(HashTable, Key);
	Next = HashTable->Table[Idx];

	while(Next != 0 && Next->Key != 0 && strcmp(Key, Next->Key) > 0 ) {
		Last = Next;
		Next = Next->Next;
	}
    
	if(Next != 0 && Next->Key != 0 && strcmp(Key, Next->Key) == 0) {
		Next->Value = Value;
	} else {
		Pair = aglHashTableNewPair(Key, Value);
		if(Next == HashTable->Table[Idx]) {
			Pair->Next = Next;
			HashTable->Table[Idx] = Pair;
		} else if (Next == 0) {
			Last->Next = Pair;
        } else {
			Pair->Next = Next;
			Last->Next = Pair;
		}
	}
}

agl_hashtable_entry *
aglHashTableGet(agl_hashtable *HashTable, char *Key) {
	agl_hashtable_entry *Pair;
	int Idx = aglHashTableGen( HashTable, Key );
    
	Pair = HashTable->Table[Idx];
	while(Pair != 0 && Pair->Key != 0 && strcmp(Key, Pair->Key) > 0 ) {
		Pair = Pair->Next;
	}
    
	if(Pair == 0 || Pair->Key == 0 || strcmp(Key, Pair->Key) != 0 ) {
		return 0;
	} else {
		return Pair;
	}
}

void
aglHashTableDelete(agl_hashtable *HashTable)
{
    if(!HashTable) return;

    for(int i=0; i < HashTable->Size; i++)
    {
        agl_hashtable_entry *Entry = HashTable->Table[i];
        if(Entry) free(Entry);
    }    
}


// Opengl utility functions. Make opengl stuff easier to use.
typedef struct
{
    s32 Id;
    agl_hashtable *Buffers;
    agl_hashtable *Uniform;
} agl_shader;


AGLDEF agl_shader *
aglShaderCreate()
{
    agl_shader *Result = (agl_shader *) malloc(sizeof(agl_shader));
    if(!Result) return 0;
    
    Result->Id = glCreateProgram();
    Result->Buffers = aglHashTableCreate(12);
    Result->Uniform = aglHashTableCreate(32);
    if(!Result->Uniform || !Result->Buffers) {
        free(Result);
        return 0;
    }
    return Result;
}

AGLDEF void
aglShaderDelete(agl_shader *Shader)
{
    if(!Shader) return;
    aglHashTableDelete(Shader->Uniform);
    glDeleteProgram(Shader->Id);
    free(Shader);
}

AGLDEF b32
aglShaderCompileAndAttach(agl_shader *Shader, const char *ShaderSource, GLenum Type)
{
    b32 Result = false;
    s32 ShaderID = glCreateShader(Type);
    s32 Program = Shader->Id;
    
    if(glGetError() != GL_INVALID_ENUM)
    {
        glShaderSource(ShaderID, 1, &ShaderSource, 0);
        glCompileShader(ShaderID);

        s32 Status;
        glGetShaderStatus(ShaderID, &Status);
        if(Status == GL_FALSE)
        {
            s32 Length = 0;
            char ErrorMessage[1024];
            glGetShaderInfoLog(ShaderID, 1024, &Length, ErrorMessage);
            AGL_PRINT("%s\n", ErrorMessage);
            AGL_ASSERT(!"Shader compile error");
            Result = false;
        } else {
            glAttachShader(Program, ShaderID);
            Result = true;
        }
    }
    else
    {
        AGL_PRINT("%s\n", ShaderSource);
        AGL_ASSERT(!"Shader compile error");
    }
    return Result;
}

AGLDEF b32
aglShaderLink(agl_shader *Shader)
{
    b32 Result = false;
    s32 Program = Shader->Id;
    
    glLinkProgram(Program);
    if(glGetError() != GL_INVALID_VALUE && glGetError() != GL_INVALID_OPERATION)
    {
        s32 Status;
        glGetProgramStatus(Program, &Status);
        if(Status == GL_FALSE)
        {
            s32 Length = 0;
            char ErrorMessage[1024];
            glGetProgramInfoLog(Program, 1024, &Length, ErrorMessage);
            AGL_PRINT("%s\n", ErrorMessage);
            AGL_ASSERT(!"Shader link error");
            Result = false;
        } else {
            u32 ShaderIds[12] = {};
            s32 AttachedShaders = 0;
            glGetAttachedShaders(Program, sizeof(ShaderIds) / sizeof(ShaderIds[0]), &AttachedShaders, ShaderIds);
            for(s32 i=0; i < 12; i++)
            {
                if(ShaderIds[i] != 0) {
                    glDetachShader(Program, ShaderIds[i]);
                    glDeleteShader(ShaderIds[i]);
                }
            }
            glUseProgram(Program);
            Result = true;
        }
    } else AGL_ASSERT(!"Shader link error");
    return Result;
}

inline AGLDEF s32
aglShaderGetBufferId(agl_shader *Shader, char* Buffer)
{
    agl_hashtable_entry *Result = aglHashTableGet(Shader->Buffers, Buffer);
    if(!Result) return 0;
    return Result->Value;
}

inline AGLDEF s32
aglShaderGetUniformId(agl_shader *Shader, char *Uniform)
{
    agl_hashtable_entry *Result = aglHashTableGet(Shader->Uniform, Uniform);
    if(Result) return Result->Value;
    
    s32 UID = glGetUniformLocation(Shader->Id, Uniform);
    aglHashTableSet(Shader->Uniform, Uniform, UID);
    return UID;
}

inline AGLDEF void
aglShaderSetUniform1i(agl_shader *Shader, char *Uniform, s32 A)
{
    glUniform1i(aglShaderGetUniformId(Shader, Uniform), A);
}

inline AGLDEF void
aglShaderSetUniform2i(agl_shader *Shader, char *Uniform, s32 A, s32 B)
{
    glUniform2i(aglShaderGetUniformId(Shader, Uniform), A, B);
}

inline AGLDEF void
aglShaderSetUniform3i(agl_shader *Shader, char *Uniform, s32 A, s32 B, s32 C)
{
    glUniform3i(aglShaderGetUniformId(Shader, Uniform), A, B, C);
}

inline AGLDEF void
aglShaderSetUniform4i(agl_shader *Shader, char *Uniform, s32 A, s32 B, s32 C, s32 D)
{
    glUniform4i(aglShaderGetUniformId(Shader, Uniform), A, B, C, D);
}

inline AGLDEF void
aglShaderSetUniform1f(agl_shader *Shader, char *Uniform, r32 A)
{
    glUniform1f(aglShaderGetUniformId(Shader, Uniform), A);
}

inline AGLDEF void
aglShaderSetUniform2f(agl_shader *Shader, char *Uniform, r32 A, r32 B)
{
    glUniform2f(aglShaderGetUniformId(Shader, Uniform), A, B);
}

inline AGLDEF void
aglShaderSetUniform3f(agl_shader *Shader, char *Uniform, r32 A, r32 B, r32 C)
{
    glUniform3f(aglShaderGetUniformId(Shader, Uniform), A, B, C);
}

inline AGLDEF void
aglShaderSetUniform4f(agl_shader *Shader, char *Uniform, r32 A, r32 B, r32 C, r32 D)
{
    glUniform4f(aglShaderGetUniformId(Shader, Uniform), A, B, C, D);
}

inline AGLDEF void
aglShaderSetUniformMat3fv(agl_shader *Shader, char *Uniform, r32 *Matrix)
{
    glUniformMatrix3fv(aglShaderGetUniformId(Shader, Uniform), 1, GL_FALSE, Matrix);
}

inline AGLDEF void
aglShaderSetUniformMat4fv(agl_shader *Shader, char *Uniform, r32 *Matrix)
{
    glUniformMatrix4fv(aglShaderGetUniformId(Shader, Uniform), 1, GL_FALSE, Matrix);
}


#ifdef AGL_PREDEFINED_SHADERS

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

static u32 __aglsl_shadowmap_fbo, __aglsl_shadowmap_tex;
static v2  __aglsl_shadowmap_size;
static u32 __aglsl_font2d_vao;

static agl_shader *__aglsl_skybox, *__aglsl_shadowmap, *__aglsl_font2d;

void
aglInitPredefinedShaders()
{
    
    const char __aglsl_skybox_vs[] = {
        "#version 330\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "out vec3 _texcoord;\n"
        "void main() {\n"
        "    vec4 pos = projection * view * vec4(position, 1.0);\n"
        "    gl_Position = pos.xyww;\n"
        "    _texcoord = position;\n"
        "}"
    };
    const char __aglsl_skybox_fs[] = {
        "#version 330\n"
        "in vec3 _texcoord;\n"
        "uniform samplerCube skybox;\n"
        "out vec4 _color;\n"
        "void main() {"    
        "    _color = texture(skybox, _texcoord);\n"
        "}"
    };
    const char __aglsl_shadowmap_vs[] = {
        "#version 330\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 lightview;\n"
        "uniform mat4 model;\n"
        "void main(){\n"
        "    gl_Position = lightview * model * vec4(position, 1.0f);\n"
        "}"   
    };
    const char __aglsl_shadowmap_fs[] = {
        "#version 330\n"
        "void main() {}\n"
    };
    const char __aglsl_font2d_vs[] = {
        "#version 330\n"
        "layout (location = 0) in vec4 combined;\n"
        "out vec2 _texcoord;\n"
        "void main() {\n"
        "    gl_Position = vec4(combined.xy, 0, 1);\n"
        "    _texcoord = combined.zw;\n"
        "};"
    };
    const char __aglsl_font2d_fs[] = {
        "#version 330\n"
        "in vec2 _texcoord;\n"
        "uniform sampler2D texture;\n"
        "uniform vec4 color;\n"
        "void main() {\n"
        "    gl_FragColor = vec4(1, 1, 1, texture2D(texture, _texcoord).r) * color;\n"
        "}\n"
    };
    
    // Skybox shader init
    u32 _ign;
    static b32 __Initialized = false;
    if(!__Initialized)
    {
        
        __aglsl_skybox = aglShaderCreate();
        aglShaderCompileAndAttach(__aglsl_skybox, __aglsl_skybox_vs, GL_VERTEX_SHADER);
        aglShaderCompileAndAttach(__aglsl_skybox, __aglsl_skybox_fs, GL_FRAGMENT_SHADER);
        aglShaderLink(__aglsl_skybox);

        float __aglsl_skybox_verts[] = {
            -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
        };
        u32 SkyBoxTexture, SkyBoxVAO;
        glGenTextures(1, &SkyBoxTexture);
        glGenVertexArrays(1, &SkyBoxVAO);
        glBindVertexArray(SkyBoxVAO);
        glGenBuffers(1, &_ign);
        glBindBuffer(GL_ARRAY_BUFFER, _ign);
        glBufferData(GL_ARRAY_BUFFER, sizeof(__aglsl_skybox_verts), __aglsl_skybox_verts, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexArray(0);

        aglHashTableSet(__aglsl_skybox->Buffers, "tex", SkyBoxTexture);
        aglHashTableSet(__aglsl_skybox->Buffers, "vao", SkyBoxVAO);

        // Shadowmap shader init
        __aglsl_shadowmap = aglShaderCreate();
        aglShaderCompileAndAttach(__aglsl_shadowmap, __aglsl_shadowmap_vs, GL_VERTEX_SHADER);
        aglShaderCompileAndAttach(__aglsl_shadowmap, __aglsl_shadowmap_fs, GL_FRAGMENT_SHADER);
        aglShaderLink(__aglsl_shadowmap);    

        if(__aglsl_shadowmap_size.w == 0 && __aglsl_shadowmap_size.h == 0) __aglsl_shadowmap_size = V2i(4096, 4096);
        glGenTextures(1, &__aglsl_shadowmap_tex);
        glBindTexture(GL_TEXTURE_2D, __aglsl_shadowmap_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, __aglsl_shadowmap_size.w, __aglsl_shadowmap_size.h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
        glGenFramebuffers(1, &__aglsl_shadowmap_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, __aglsl_shadowmap_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, __aglsl_shadowmap_tex, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2D font shader
        __aglsl_font2d = aglShaderCreate();
        aglShaderCompileAndAttach(__aglsl_font2d, __aglsl_font2d_vs, GL_VERTEX_SHADER);
        aglShaderCompileAndAttach(__aglsl_font2d, __aglsl_font2d_fs, GL_FRAGMENT_SHADER);
        aglShaderLink(__aglsl_font2d);
        
        glGenVertexArrays(1, &__aglsl_font2d_vao);
        glBindVertexArray(__aglsl_font2d_vao);
        glGenBuffers(1, &_ign);
        glBindBuffer(GL_ARRAY_BUFFER, _ign);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexArray(0);
  
        __Initialized = true;
    }
}

void
aglDeInitPredefinedShaders()
{
    u32 Tex = (u32) aglShaderGetBufferId(__aglsl_skybox, "tex");
    glDeleteTextures(1, &Tex);

    u32 VAO = (u32) aglShaderGetBufferId(__aglsl_skybox, "vao");
    glDeleteVertexArrays(1, &VAO);
    
    aglShaderDelete(__aglsl_skybox);   

    aglShaderDelete(__aglsl_shadowmap);
    glDeleteTextures(1, &__aglsl_shadowmap_tex);
    glDeleteFramebuffers(1, &__aglsl_shadowmap_fbo);

    aglShaderDelete(__aglsl_font2d);
    glDeleteVertexArrays(1, &__aglsl_font2d_vao);
}

void
aglSkyboxTextures(char **Textures)
{
    glActiveTexture(GL_TEXTURE0);
    
    u32 Texture = (u32) aglHashTableGet(__aglsl_skybox->Buffers, "tex")->Value;
    s32 Width, Height, Components;
    u8 *image;
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
    for( s32 i=0; i < 6; i++)
    {
        image = stbi_load(Textures[i], &Width, &Height, &Components, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, Width, Height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, image);
        
        stbi_image_free(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void aglSkyboxTextures(char *Front, char *Back, char *Up, char *Down, char *Right, char *Left)
{
    char *CubemapTextures[] = { Front, Back, Up, Down , Right, Left };
    aglSkyboxTextures(CubemapTextures);
}

void aglSkyboxRender(mat4x4 Projection, mat4x4 View)
{
    u32 Texture = (u32) aglHashTableGet(__aglsl_skybox->Buffers, "tex")->Value;
    u32 VAO     = (u32) aglHashTableGet(__aglsl_skybox->Buffers, "vao")->Value;

    glUseProgram(__aglsl_skybox->Id);
    
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
    
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);            

    mat4x4 p = Projection;
    mat4x4 v = View;

    v.m3 = 0.f; v.m7 = 0.f; v.m11 = 0.f; v.m12 = 0.f; v.m13 = 0.f; v.m14 = 0.f;
    
    aglShaderSetUniformMat4fv(__aglsl_skybox, "projection", p.E);
    aglShaderSetUniformMat4fv(__aglsl_skybox, "view", v.E);
    aglShaderSetUniform1i(__aglsl_skybox, "mainTex", 0);
            
    glDrawArrays(GL_TRIANGLES, 0, 36);            

    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

void aglShadowMapSize(s32 Width, s32 Height)
{
    __aglsl_shadowmap_size = V2i(Width, Height);
    if(__aglsl_shadowmap_tex != 0)
    {
        glBindTexture(GL_TEXTURE_2D, __aglsl_shadowmap_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, __aglsl_shadowmap_size.w, __aglsl_shadowmap_size.h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    }
}

void aglShadowStageBegin(mat4x4 LightView)
{
    glUseProgram(__aglsl_shadowmap->Id);
    
    glViewport(0, 0, __aglsl_shadowmap_size.w, __aglsl_shadowmap_size.h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    aglShaderSetUniformMat4fv(__aglsl_shadowmap, "lightview", LightView.E);

    glBindFramebuffer(GL_FRAMEBUFFER, __aglsl_shadowmap_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void aglShadowStageEnd()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

u32 aglShadowMap()
{
    return __aglsl_shadowmap_tex;
}
#endif

#define AGL_SHADERS_H
#endif

