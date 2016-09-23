#ifndef AGL_FONT2D_H

#ifndef STB_TRUETYPE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#endif

#ifndef __STB_INCLUDE_STB_TRUETYPE_H__
#error "Can't find stb_truetype.h source file."
#endif

#include <GL\glu.h>
#pragma comment(lib, "glu32.lib")

#define AGL_FONT2D_WIDTH  512
#define AGL_FONT2D_HEIGHT 512
typedef struct
{
    stbtt_packedchar CharData[255];
    unsigned int     Texture;
} agl_font;

void
aglFontCreate(agl_font *Font, char *Filename, int Size, int OversampleX = 1, int OversampleY = 1)
{
    stbtt_pack_context PackContext;

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

    stbtt_PackBegin(&PackContext, TextureBuffer, AGL_FONT2D_WIDTH, AGL_FONT2D_HEIGHT, 0, 1, 0);
    stbtt_PackSetOversampling(&PackContext, OversampleX, OversampleY);
    stbtt_PackFontRange(&PackContext, FileBuffer, 0, Size, 32, 95, Font->CharData + 32); 
    stbtt_PackEnd(&PackContext);

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, font->Texture);
    glBegin(GL_QUADS);
    while (*text) {
        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(font->CharData, AGL_FONT2D_WIDTH, AGL_FONT2D_HEIGHT, *text++, &x, &y, &q, 0);
        aglDrawFontQuad(q.x0,q.y0,q.x1,q.y1, q.s0,q.t0,q.s1,q.t1);
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
    gluOrtho2D(0, Width, Height, 0);
   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

#define AGL_FONT2D_H
#endif
