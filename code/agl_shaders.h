#if !defined(AGL_SHADERS_H)
#define GLSL(source) "#version 330\n" #source

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
        in vec2 UV;

        out vec3 color;

        uniform sampler2D myTextureSampler;

        void main(){
                color = texture( myTextureSampler, UV ).rgb;
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
        layout(location = 3) in vec3 vertexColor;

        uniform mat4 matModelViewProj;
        uniform mat4 matModelView;
        uniform mat4 matInverse;

        out vec2 UV;
        out vec3 Normal;

        vec3 newPos = vertexPosition;
        void main(){
            newPos.x += mod(gl_InstanceID, 16.0) * 3;
            newPos.z += (gl_InstanceID / 16.0) * 3;
            gl_Position = matModelViewProj * vec4(newPos, 1);

            UV = vertexUV;
            Normal = vertexNormal;
        }
     );

const char *
AGL_16x16BATCHDRAW_VS = GLSL
    (
        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec2 vertexUV;
        layout(location = 2) in vec3 vertexNormal;
        layout(location = 3) in vec3 vertexColor;

        uniform mat4 matModelViewProj;
        uniform mat4 matModelView;
        uniform mat4 matInverse;

        out vec2 UV;
        out vec3 Normal;

        vec3 newPos = vertexPosition;
        void main(){
            newPos.x += mod(gl_InstanceID, 16.0) * 3;
            newPos.z += (gl_InstanceID / 16.0) * 3;
            gl_Position = matModelViewProj * vec4(newPos, 1);

            UV = vertexUV;
            Normal = vertexNormal;
        }
     );

#define AGL_SHADERS_H
#endif

