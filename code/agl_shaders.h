#if !defined(AGL_SHADERS_H)

GLint
aglShaderCompile(const char *Source, GLenum Type)
{
    GLint Result = glCreateShader(Type);
    if(Result > 0)
    {
        glShaderSource(Result, 1, &Source, 0);
        glCompileShader(Result);

        GLint Status;
        glGetShaderStatus(Result, &Status);
        if(!Status)
        {
            int Length = 0;
            char Info[4096];
            glGetShaderInfoLog(Result, 4096, &Length, Info);
            printf("%s\n", Info);
            AGL_ASSERT(!"Shader compile error");
        }
    }
    return Result;
}

void
aglShaderAttach(GLuint ProgramID, GLint *ShaderArray, GLuint Length)
{
    for(int i=0; i < Length;i++)
    {
        glAttachShader(ProgramID, ShaderArray[i]);
    }
}

void
aglShaderDetach(GLuint ProgramID, GLint *ShaderArray, GLuint Length)
{
    for(int i=0; i < Length;i++)
    {
        glDetachShader(ProgramID, ShaderArray[i]);
        glDeleteShader(ShaderArray[i]);
    }
}

b32
aglShaderLink(GLuint ProgramID, GLint *ShaderArray, GLuint Length)
{
    GLboolean Result = GL_FALSE;
    if(ShaderArray && Length > 0)
    {
        aglShaderAttach(ProgramID, ShaderArray, Length);
        glLinkProgram(ProgramID);
        GLint Status;
        glGetProgramStatus(ProgramID, &Status);
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
    aglShaderDetach(ProgramID, ShaderArray, Length);
    return Result;
}

// FRAGMENT SHADERS
const char *
AGL_SHADERS_FRAG_1 = GLSL
    ( 430,
        out vec4 out_color;
    
        void main (void)
        {
            out_color = vec4(1.0, 1.0, 0.25, 1.0);  
        }
     );

const char *
AGL_SHADERS_FRAG_2 = GLSL
    ( 430,
        in vec4 color;
        
        void main() {
            gl_FragColor = color;
        }
);

const char *
AGL_SHADERS_FRAG_3 = GLSL
    ( 430,
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

        uniform sampler2D shadowMap;
        
        uniform agl_material material;
        uniform agl_light light;
        uniform vec3 viewPos;

        in vec3 FragPos;
        in vec2 UV;
        in vec3 Normal;
        in vec4 FragPosLightSpace;

        uniform bool blinn = false;
        out vec4 FragColor;

        float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
        {
            vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
            projCoords = projCoords * 0.5 + 0.5;
            float closestDepth = texture(shadowMap, projCoords.xy).r; 
            if(projCoords.z > 1.0) return 0.0;
            
            float currentDepth = projCoords.z;
            float bias = 0.000125;
            float shadow = 0.0;
            vec2 texelSize = .33 / textureSize(shadowMap, 0);
            for(int x = -1; x <= 1; ++x)
            {
                for(int y = -1; y <= 1; ++y)
                {
                    float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                    shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
                }    
            }
            shadow /= 24.0;        

            return shadow;
        } 
        
        void main(){
            vec3  color = material.ambient;
            vec3  normal = normalize(Normal);
            vec3  lightColor = vec3(1.0);
            
            vec3  ambient = 0.15 * color;
            
            vec3  lightDir = normalize(light.position - FragPos);
            float diff = max(dot(lightDir, normal), 0.0);
            vec3  diffuse = diff * lightColor;

            float spec;
            vec3 viewDir = normalize(viewPos - FragPos);
            if(blinn == true)
            {
                vec3 reflectDir = reflect(-lightDir, normal);  
                spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            } else {
                vec3 halfwayDir = normalize(lightDir + viewDir);
                spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
            }
            vec3 specular = spec * lightColor;
            float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);
            
            vec3 lighting = ((ambient + (2.0 - shadow)) * (diffuse + specular)) * color;
            FragColor = vec4(lighting, 1.0);
        }
     );

// VERTEX SHADERS
const char *
AGL_SHADERS_VERT_1 = GLSL
    ( 430,
        layout(location = 0) in vec4 in_position;

        void main()
        {
            gl_Position = in_position;
        }   
     );

const char *
AGL_SHADERS_VERT_2 = GLSL
    ( 430,
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
    ( 430,
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
    ( 430,
        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec2 vertexUV;
        layout(location = 2) in vec3 vertexNormal;
        layout(location = 3) in vec3 vertexColor;
        
        uniform mat4 matrixModelViewProj;
        uniform mat4 matrixModel;
        uniform mat4 matrixView;
        uniform mat4 matrixSpace;

        out vec2 UV;
        out vec3 Normal;
        out vec3 FragPos;
        out vec4 FragPosLightSpace;
        
        void main(){
            gl_Position = matrixModelViewProj * vec4(vertexPosition, 1);
            UV = vertexUV;
            FragPos = vec3(matrixModel * vec4(vertexPosition, 1));
            Normal = transpose(inverse(mat3(matrixModel))) * vertexNormal;
            FragPosLightSpace = matrixSpace * vec4(FragPos, 1.0);
        }
     );

const char *
AGL_SHADERS_VERT_GENERAL = GLSL
    ( 430,
        layout(location = 0) in vec3 Position;
        layout(location = 1) in vec2 TextureCoord;
        layout(location = 2) in vec3 Normal;
        layout(location = 3) in vec3 Color;

        uniform mat4 MatrixMVP;
        uniform mat4 MatrixModel;
        uniform mat4 MatrixView;
        uniform mat4 MatrixSpace;

        out vec2 Pass;
        out vec3 Normal;
        out vec3 FragPos;
        out vec4 FragPosLightSpace;
        
        void main(){
            gl_Position = matrixModelViewProj * vec4(vertexPosition, 1);
            UV = vertexUV;
            FragPos = vec3(matrixModel * vec4(vertexPosition, 1));
            Normal = transpose(inverse(mat3(matrixModel))) * vertexNormal;
            FragPosLightSpace = matrixSpace * vec4(FragPos, 1.0);
        }
     );


const char *
AGL_16x16BATCHDRAW_VS = GLSL
    ( 430,
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

#define AGL_SHADERS_H
#endif

