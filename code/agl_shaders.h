#if !defined(AGL_SHADERS_H)
#define GLSL(source) "#version 330\n" #source

GLint
aglCompileShader(const char *Source, GLenum Type)
{
    GLint Result = glCreateShader(Type);
    if(Result > 0)
    {
        glShaderSource(Result, 1, &Source, 0);
        glCompileShader(Result);

        GLint Status;
        glGetShader(Result, GL_COMPILE_STATUS, &Status);
        if(!Status)
        {
            int Length = 0;
            char Info[4096];
            glGetShaderInfoLog(Result, 4096, &Length, Info);
            printf("%s\n", Info);
            aglAssert(!"Shader compile error");
        }
    }
    return Result;
}

void
aglAttachShaders(GLuint ProgramID, GLint *ShaderArray, GLuint Length)
{
    for(int i=0; i < Length;i++)
    {
        glAttachShader(ProgramID, ShaderArray[i]);
    }
}

void
aglDetachShaders(GLuint ProgramID, GLint *ShaderArray, GLuint Length)
{
    for(int i=0; i < Length;i++)
    {
        glDetachShader(ProgramID, ShaderArray[i]);
        glDeleteShader(ShaderArray[i]);
    }
}

b32
aglLinkProgram(GLuint ProgramID, GLint *ShaderArray, GLuint Length)
{
    GLboolean Result = GL_FALSE;
    if(ShaderArray && Length > 0)
    {
        aglAttachShaders(ProgramID, ShaderArray, Length);
        glLinkProgram(ProgramID);
        GLint Status;
        glGetProgram(ProgramID, GL_LINK_STATUS, &Status);
        if(Status != GL_TRUE)
        {
            int Length = 0;
            char Info[4096];
            glGetProgramInfoLog(ProgramID, 4096, &Length, Info);
            printf("%s", Info);
        } else {       
            Result = GL_TRUE;
        }
    }
    aglDetachShaders(ProgramID, ShaderArray, Length);
    return Result;
}

#define aglCreateShader(Var, ...)                                       \
    GLuint Var = glCreateProgram();                                     \
    GLint Shaders[] = { __VA_ARGS__ };                                  \
    GLuint Result = aglLinkProgram(Var, Shaders, sizeof(Shaders) / sizeof(Shaders[0])); \
    if(!Result) return -1

// FRAGMENT SHADERS
const char *
AGL_SHADERS_FRAG_1 = GLSL
    (
        out vec4 out_color;
    
        void main (void)
        {
            out_color = vec4(1.0, 1.0, 0.25, 1.0);  
        }
     );

const char *
AGL_SHADERS_FRAG_2 = GLSL
    (
        in vec4 color;
        
        void main() {
            gl_FragColor = color;
        }
);

const char *
AGL_SHADERS_FRAG_3 = GLSL
    (
        struct agl_material
        {
            vec3  ambient;
            vec3  diffuse;
            vec3  specular;
            float shininess;
            sampler2D texture2D;
        };
        
        struct agl_light
        {
            vec3 position;
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };
        
        uniform agl_material material;
        uniform agl_light light;
        uniform vec3 viewPos;

        in vec3 FragPos;
        in vec2 UV;
        in vec3 Normal;
        
        out vec4 color;
        
        void main(){
            // Ambient
            vec3  ambient = light.ambient * material.ambient;
            
            // Diffuse
            vec3  norm = normalize(Normal);
            vec3  lightDir = normalize(light.position - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3  diffuse = light.diffuse * (diff * material.diffuse);

            // Specular
            vec3  viewDir = normalize(viewPos - FragPos);
            vec3  reflectDir = reflect(-lightDir, norm);  
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            vec3  specular = light.specular * (spec * material.specular);
            
            //color = texture( texture2D, UV ).rgb * material.diffuse;
            color = vec4(ambient + diffuse + specular, 1);// * texture(texture2D, UV);
        }
     );

// VERTEX SHADERS
const char *
AGL_SHADERS_VERT_1 = GLSL
    (
        layout(location = 0) in vec4 in_position;

        void main()
        {
            gl_Position = in_position;
        }   
     );

const char *
AGL_SHADERS_VERT_2 = GLSL
    (
        layout(location = 0) in vec4 in_position;

        varying vec4 color;
        
        uniform mat4 model;
        uniform mat4 projection;
        uniform mat4 camera;
        
        void main(){
            gl_Position = projection * camera * model * in_position;
        }
     );

const char *
AGL_SHADERS_VERT_3 = GLSL
    (
        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec2 vertexUV;

        out vec2 UV;
        
        uniform mat4 currentMVP;
        
        void main(){
            gl_Position = currentMVP * vec4(vertexPosition, 1);

            UV = vertexUV;
        }
     );

const char *
AGL_SHADERS_VERT_4 = GLSL
    (
        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec2 vertexUV;
        layout(location = 2) in vec3 vertexNormal;

        uniform vec3 baseColor;
        
        uniform mat4 matrixModelViewProj;
        uniform mat4 matrixProj;
        uniform mat4 matrixModel;
        uniform mat4 matrixView;
        uniform mat4 matrixInverse;

        out vec2 UV;
        out vec3 Normal;
        out vec3 FragPos;
        
        void main(){
            gl_Position = matrixModelViewProj * vec4(vertexPosition, 1);
            UV = vertexUV;
            Normal = vertexNormal;
            FragPos = vec3(matrixModel * vec4(vertexPosition, 1));
        }
     );

const char *
AGL_16x16BATCHDRAW_VS = GLSL
    (
        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec2 vertexUV;
        layout(location = 2) in vec3 vertexNormal;
        
        uniform mat4 matrixModelViewProj;
        uniform mat4 matrixModelView;
        uniform mat4 matrixInverse;

        out vec2 UV;
        out vec3 Normal;

        void main(){
            gl_Position = matrixModelViewProj * vec4(newPos, 1);
            UV = vertexUV;
            Normal = vertexNormal;
        }
     );


const char *
AGL_SHADER_TEXT_VS = GLSL
    (
        layout(location = 0) in vec2 vertexPosition;
        layout(location = 1) in vec2 vertexUV;

        uniform vec2 viewport;

        out vec2 UV;
        
        void main()
        {
            vec2 finalPosition = vertexPosition - (viewport / 2);
            gl_Position = vec4(finalPosition / (viewport / 2), 0, 1);
            UV = vertexUV;
        }
     );

const char *
AGL_SHADER_TEXT_FS = GLSL
    (
        in vec2 UV;
        uniform sampler2D texture2D;
        
        out vec4 color;
        
        void main()
        {
            color = texture(texture2D, UV);
        }
);
#define AGL_SHADERS_H
#endif

