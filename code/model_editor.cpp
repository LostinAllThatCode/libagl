
#include "agl.h"
#include <stdio.h>

#pragma pack(1)

typedef struct
{
    char Format[21];         // File format name
    u8   __ign_1, __ign_2;   // Unknown bytes
    u32  Version;            // Format version
} fbx_file_format_header;

typedef struct
{
    u32 Length;
    
} fbx_file_format_data_type_binary;


struct fbx_string
{
    u32 Length;
    char Data[];
};

struct fbx_binary
{
    u32 Length;
    u8 Data[];
};

struct fbx_array
{
    u32 ArrayLength;
    u32 Encoding;
    u32 CompressedLength;
    union {
        r32 Float[];
        r64 Double[];
        s64 Long[];
        s32 Integer[];
        u32 Boolean[];
    };
};

typedef struct
{
    char TypeCode;
    union
    {
        s16 Y; u8  C; s32 I; r32 F; r64 D; s64 L;
        fbx_string String;
        fbx_binary Binary;
        fbx_array  Array;
    };   
} fbx_property;

typedef struct
{
    u32  EndOffset;
    u32  NumProperties;
    u32  PropertyListLen;
    u8   NameLen;
    char Name[];
} fbx_node;


#pragma pack()

void
fbx_parse_node(u8 *CurrentPos, u8 *Start, s32 Intend=0)
{
    fbx_node *Node;
    while(*CurrentPos)
    {
        Node = (fbx_node *) CurrentPos;;
        CurrentPos += sizeof(fbx_node) + Node->NameLen;
        for(int i=0; i < Intend; i++) printf(" ");
        printf("  %.*s ", Node->NameLen, Node->Name);
        
        while( CurrentPos != (Start + Node->EndOffset) )
        {
            fbx_property *P = (fbx_property *) CurrentPos;
            switch(P->TypeCode)
            {
                case 'R':
                
                case 'f':
                case 'd':
                case 'l':
                case 'i':
                case 'b':
                
                case 'Y':
                case 'C':
                case 'F':
                case 'D':
                case 'L':
                {printf("? %c\n", P->TypeCode); CurrentPos = Start + Node->EndOffset; }break;
                case 'I':
                {
                    printf(":  %i\n", P->I);
                    CurrentPos += sizeof(s32) + 1;
                }break;
                case 'S':
                {
                    printf(":  %.*s\n", P->String.Length, P->String.Data);
                    CurrentPos += sizeof(fbx_string) + P->String.Length + 1;
                }break;
                default:
                {
                    /*
                    printf("{\n");
                    fbx_parse_node(CurrentPos, Start, ++Intend);
                    for(int i=0; i < Intend; i++) printf(" ");
                    
                    printf("}\n");
                    Intend--;
                    CurrentPos = Start + Node->EndOffset;
                    */
                }break;
            }
        }
//        CurrentPos = Start + Node->EndOffset;
#if 0
        if(!Node->NumProperties)
        {
            printf("%.*s%.*s {\n", Intend, "               ", Node->NameLen, Node->Name);
            Intend += 2;
            fbx_parse_node(CurrentPos, Start, Intend);
            CurrentPos = Start + Node->EndOffset;
            Intend -= 2;
            printf("%.*s}\n",Intend, "               ");
            //;
        }
        else
        {
            printf("%.*s%.*s :", Intend, "               ", Node->NameLen, Node->Name);
            u8 *Pos = CurrentPos;
            for(s32 i = 1; i <= Node->NumProperties; i++)
            {
                size_t Offset = 0;
                fbx_property *P = (fbx_property *) (Pos);
                switch(P->TypeCode)
                {
                    case 'I':
                    {
                        printf("  %i\n", P->I);
                        Offset = 4 + sizeof(P->I);
                    }break;
                    case 'S':
                    {
                        printf("  %.*s\n", P->String.Length, P->String.Data);
                        Offset = 4 + P->String.Length + 1;
                    }break;
                }
                Pos += Offset;
            }
            CurrentPos = Start + Node->EndOffset;
        }
#endif
    }
}

int
main(int argc, char **argv)
{
    u8 *fbx_content;

    FILE *fp = fopen("../r2d2.fbx", "rb");
    if(fp)
    {
        size_t filesize;
        fseek(fp, 0, SEEK_END);
        filesize = ftell(fp);
        rewind(fp);
        
        fbx_content = (u8 *) calloc(1, filesize);
        fread(fbx_content, 1, filesize, fp);
        fclose(fp);
    }

    fbx_node *Node;
    u8 * BeginOfFile = fbx_content;
    u8 * CurrentPos = BeginOfFile + sizeof(fbx_file_format_header);
    fbx_parse_node(CurrentPos, BeginOfFile);
    
    free(fbx_content);
    
    
    
    return 0;
}
