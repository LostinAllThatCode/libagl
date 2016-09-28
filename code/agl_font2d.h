#ifndef AGL_FONT2D_H

#ifndef STB_TRUETYPE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#endif

#ifndef __STB_INCLUDE_STB_TRUETYPE_H__
#error "Can't find stb_truetype.h source file."
#endif

#define AGL_FONT2D_WIDTH  512
#define AGL_FONT2D_HEIGHT 512
typedef struct
{
    
    stbtt_packedchar   CharData[255];
    unsigned int       Texture;
} agl_font;

void
aglFontCreate(agl_font *Font, char *Filename, int Size, int OversampleX = 1, int OversampleY = 1)
{
    stbtt_pack_context Context;
    
    // Load file into temporary buffer
    unsigned char *FileBuffer;
    long FileSize = 0;
    FILE *File = fopen(Filename, "rb");
    fseek(File, 0, SEEK_END);
    FileSize = ftell(File);
    rewind(File);

    FileBuffer = (unsigned char *) malloc(FileSize);
    fread(FileBuffer, 1, FileSize, File);
    fclose(File);

    // Create temporary bitmap buffer for the texture
    unsigned char *TextureBuffer = (unsigned char *) malloc( AGL_FONT2D_WIDTH * AGL_FONT2D_HEIGHT );

    stbtt_PackBegin(&Context, TextureBuffer, AGL_FONT2D_WIDTH, AGL_FONT2D_HEIGHT, 0, 1, 0);
    stbtt_PackSetOversampling(&Context, OversampleX, OversampleY);
    stbtt_PackFontRange(&Context, FileBuffer, 0, Size, 0, 255, Font->CharData); 
    stbtt_PackEnd(&Context);

    glGenTextures(1, &Font->Texture);
    glBindTexture(GL_TEXTURE_2D, Font->Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, AGL_FONT2D_WIDTH, AGL_FONT2D_HEIGHT, 0, GL_ALPHA, GL_UNSIGNED_BYTE, TextureBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // GL_ALPHA is deprecated since opengl 4. So this method of texture rendering would not work. Replace it with GL_RED and
    // use a shader to render the text with the red channel.
    
    free(TextureBuffer);
    free(FileBuffer);
}

void
aglFontDelete(agl_font *Font)
{
    glDeleteTextures(1, &Font->Texture);
}

void
aglDrawFontQuad(float x0, float y0, float x1, float y1, float s0, float t0, float s1, float t1)
{
   glTexCoord2f(s0,t0); glVertex2f(x0,y0);
   glTexCoord2f(s1,t0); glVertex2f(x1,y0);
   glTexCoord2f(s1,t1); glVertex2f(x1,y1);
   glTexCoord2f(s0,t1); glVertex2f(x0,y1);
}

void
aglDrawText(float x, float y, char *text, agl_font *font)
{
    glUseProgram(0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, font->Texture);
    glBegin(GL_QUADS);

    while (*text) {
        stbtt_aligned_quad q;
        int c = *text;
        switch(*text)
        {
            case 'Ä': { c = 196; } break;
            case 'ä': { c = 228; } break;
            case 'Ö': { c = 214; } break;
            case 'ö': { c = 246; } break;
            case 'Ü': { c = 220; } break;
            case 'ü': { c = 252; } break;
        }
        
        stbtt_GetPackedQuad(font->CharData, AGL_FONT2D_WIDTH, AGL_FONT2D_HEIGHT, c, &x, &y, &q, 0);
        aglDrawFontQuad(q.x0,q.y0,q.x1,q.y1, q.s0,q.t0,q.s1,q.t1);
        *text++;

    }
    glEnd();

    glDisable(GL_BLEND);
}

void
aglSetViewport2D(int Width, int Height, int Near = -1, int Far = 1)
{
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, Width, Height, 0, Near, Far);
   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

#define AGL_FONT2D_H
#endif
