#if !defined(AGL_MATH_H)

#define _USE_MATH_DEFINES
#include <math.h>



// NOTE: 2D vector implementation
union v2
{
    struct
    {
        real32 x, y;
    };
    struct
    {
        real32 u, v;
    };
    real32 E[2];
};

// NOTE: 2d vector initialization
inline v2
V2(r32 x, r32 y)
{
    v2 Result = { x, y };
    return Result;
}

inline v2
V2i(s32 x, s32 y)
{
    v2 Result = { (r32) x, (r32) y };
    return Result;
}

// NOTE: 2D vector operator overloading
inline v2
operator*(real32 A, v2 B)
{
    v2 Result;

    Result.x = A*B.x;
    Result.y = A*B.y;
    
    return(Result);
}

inline v2
operator*(v2 B, real32 A)
{
    v2 Result = A*B;

    return(Result);
}

inline v2 &
operator*=(v2 &B, real32 A)
{
    B = A * B;

    return(B);
}

inline v2
operator-(v2 A)
{
    v2 Result;

    Result.x = -A.x;
    Result.y = -A.y;

    return(Result);
}

inline v2
operator+(v2 A, v2 B)
{
    v2 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;

    return(Result);
}

inline v2 &
operator+=(v2 &A, v2 B)
{
    A = A + B;

    return(A);
}

inline v2
operator-(v2 A, v2 B)
{
    v2 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;

    return(Result);
}

inline v2 &
operator-=(v2 &A, v2 B)
{
    A = A - B;

    return(A);
}

// NOTE: 3D vector implementation

union v3
{
    struct { r32 x, y, z; };
    struct { r32 u, v, w; };
    struct { r32 r, g, b; };
    struct { v2 xy; r32 Ignored0_; };
    struct { r32 Ignored1_; v2 yz; };
    struct { v2 uv; r32 Ignored2_; };
    struct { r32 Ignored3_; v2 vw; };
    real32 E[3];
};

// NOTE: 3D vector initialization
inline v3
V3(r32 x, r32 y, r32 z)
{
    v3 Result = { x, y, z };
    return Result;
}

inline v3
V3(v2 xy, r32 z)
{
    v3 Result = { xy.x, xy.y, z };
    return Result;
}

// NOTE: 3D vector operator overloading
inline v3
operator*(real32 A, v3 B)
{
    v3 Result;

    Result.x = A*B.x;
    Result.y = A*B.y;
    Result.z = A*B.z;
    
    return(Result);
}

inline v3
operator*(v3 B, real32 A)
{
    v3 Result = A*B;

    return(Result);
}

inline v3 &
operator*=(v3 &B, real32 A)
{
    B = A * B;

    return(B);
}

inline v3
operator-(v3 A)
{
    v3 Result;

    Result.x = -A.x;
    Result.y = -A.y;
    Result.z = -A.z;

    return(Result);
}

inline v3
operator+(v3 A, v3 B)
{
    v3 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;

    return(Result);
}

inline v3 &
operator+=(v3 &A, v3 B)
{
    A = A + B;

    return(A);
}

inline v3
operator-(v3 A, v3 B)
{
    v3 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;

    return(Result);
}

inline v3 &
operator-=(v3 &A, v3 B)
{
    A = A - B;

    return(A);
}

// NOTE: 4D vector implementation
union v4
{
    struct
    {
        union
        {
            v3 xyz;
            struct { r32 x, y, z; };
        };
        r32 w;        
    };
    
    struct
    {
        union
        {
            v3 rgb;
            struct { r32 r, g, b; };
        };
        r32 a;        
    };
    
    struct { v2 xy; r32 Ignored0_; r32 Ignored1_; };
    struct { r32 Ignored2_; v2 yz; r32 Ignored3_; };
    struct { r32 Ignored4_; r32 Ignored5_; v2 zw; };
    real32 E[4];
};

// NOTE: 4D vector initialization
inline v4
V4(r32 x, r32 y, r32 z, r32 w)
{
    v4 Result = { x, y, z, w };
    return Result;
}

inline v4
V4(v3 xyz, r32 w)
{
    v4 Result = { xyz.x, xyz.y, xyz.z, w};
    return Result;
}

// NOTE: 4D vector operator overloading
inline v4
operator*(real32 A, v4 B)
{
    v4 Result;

    Result.x = A*B.x;
    Result.y = A*B.y;
    Result.z = A*B.z;
    Result.w = A*B.w;
    
    return(Result);
}

inline v4
operator*(v4 B, real32 A)
{
    v4 Result = A*B;

    return(Result);
}

inline v4 &
operator*=(v4 &B, real32 A)
{
    B = A * B;

    return(B);
}

inline v4
operator-(v4 A)
{
    v4 Result;

    Result.x = -A.x;
    Result.y = -A.y;
    Result.z = -A.z;
    Result.w = -A.w;

    return(Result);
}

inline v4
operator+(v4 A, v4 B)
{
    v4 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;
    Result.w = A.w + B.w;

    return(Result);
}

inline v4 &
operator+=(v4 &A, v4 B)
{
    A = A + B;

    return(A);
}

inline v4
operator-(v4 A, v4 B)
{
    v4 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;
    Result.w = A.w - B.w;

    return(Result);
}

inline v4 &
operator-=(v4 &A, v4 B)
{
    A = A - B;

    return(A);
}

// NOTE: General vector math operations

// NOTE: Dot product / inner product of two vectors
inline r32
DotV3(v3 A, v3 B)
{
    r32 Result = A.x*B.x + A.y*B.y + A.z*B.z;

    return(Result);
}

inline r32
DotV4(v4 A, v4 B)
{
    r32 Result = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;

    return(Result);
}

inline v3
CrossV3(v3 A, v3 B)
{
    v3 Result;
    Result.x = (A.y*B.z) - (A.z*B.y);
    Result.y = (A.z*B.x) - (A.x*B.z);
    Result.z = (A.x*B.y) - (A.y*B.x);
    return Result;
}

inline v4
CrossV4(v4 A, v4 B)
{
    v4 Result;
    Result.x = (A.y*B.z) - (A.z*B.y);
    Result.y = (A.z*B.w) - (A.w*B.z);
    Result.z = (A.w*B.x) - (A.x*B.w);
    Result.w = (A.x*B.y) - (A.y*B.x);
    return Result;
}

inline r32
LengthSqV3(v3 A)
{
    r32 Result = DotV3(A, A);
    return(Result);
}

inline r32
LengthSqV4(v4 A)
{
    r32 Result = DotV4(A, A);
    return(Result);
}

inline r32
LengthV3(v3 A)
{
    r32 Result = sqrt(LengthSqV3(A));
    return(Result);
}

inline r32
LengthV4(v4 A)
{
    r32 Result = sqrt(LengthSqV4(A));
    return(Result);
}

inline v3
NormalizeV3(v3 A)
{
    v3 Result = A * (1.0f / LengthV3(A));
    return(Result);
}

inline v4
NormalizeV4(v4 A)
{
    v4 Result = A * (1.0f / LengthV4(A));
    return(Result);
}

// NOTE: matrix implementation (opengl coloum order)
union mat3x3
{
    struct
    {
        r32 m0, m3, m6;
        r32 m1, m4, m7;
        r32 m2, m5, m8;
    };
    r32 E[9];
};

union mat4x4
{
    struct
    {
        r32 m0, m4, m8,  m12;
        r32 m1, m5, m9,  m13;
        r32 m2, m6, m10, m14;
        r32 m3, m7, m11, m15;
    };
    r32 E[16];
};

inline mat3x3
NullMat3x3()
{
    mat3x3 Result = {};
    return Result;
}

inline mat4x4
NullMat4x4()
{
    mat4x4 Result = {};
    return Result;
}

inline mat3x3
IdentityMat3x3()
{
    mat3x3 Result = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    return Result;
}

inline mat4x4
IdentityMat4x4()
{
    mat4x4 Result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return Result;
}

inline mat4x4
TranslateMat4x4(r32 x, r32 y, r32 z)
{
    mat4x4 Result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x,    y,    z,    1.0f
    };
    return Result;
}

// NOTE: General matrix operations
inline mat3x3
MultMat3x3(mat3x3 A, mat3x3 B)
{
    mat3x3 Result;
    Result.m0 = (A.m0 * B.m0) + (A.m3 * B.m1) + (A.m6 * B.m2);
    Result.m3 = (A.m0 * B.m3) + (A.m3 * B.m4) + (A.m6 * B.m5);
    Result.m6 = (A.m0 * B.m6) + (A.m3 * B.m7) + (A.m6 * B.m8);

    Result.m1 = (A.m1 * B.m0) + (A.m4 * B.m1) + (A.m7 * B.m2);
    Result.m4 = (A.m1 * B.m3) + (A.m4 * B.m4) + (A.m7 * B.m5);
    Result.m7 = (A.m1 * B.m6) + (A.m4 * B.m7) + (A.m7 * B.m8);

    Result.m2 = (A.m2 * B.m0) + (A.m5 * B.m1) + (A.m8 * B.m2);
    Result.m5 = (A.m2 * B.m3) + (A.m5 * B.m4) + (A.m8 * B.m5);
    Result.m8 = (A.m2 * B.m6) + (A.m5 * B.m7) + (A.m8 * B.m8);
    return Result;
}

// NOTE: General matrix operations
inline mat4x4
MultMat4x4(mat4x4 A, mat4x4 B)
{
    mat4x4 Result;
    Result.m0  = (A.m0 * B.m0)  + (A.m4 * B.m1)  + (A.m8  * B.m2)  + (A.m12 * B.m3);
    Result.m1  = (A.m1 * B.m0)  + (A.m5 * B.m1)  + (A.m9  * B.m2)  + (A.m13 * B.m3);
    Result.m2  = (A.m2 * B.m0)  + (A.m6 * B.m1)  + (A.m10 * B.m2)  + (A.m14 * B.m3);
    Result.m3  = (A.m3 * B.m0)  + (A.m7 * B.m1)  + (A.m11 * B.m2)  + (A.m15 * B.m3);
    Result.m4  = (A.m0 * B.m4)  + (A.m4 * B.m5)  + (A.m8  * B.m6)  + (A.m12 * B.m7);
    Result.m5  = (A.m1 * B.m4)  + (A.m5 * B.m5)  + (A.m9  * B.m6)  + (A.m13 * B.m7);
    Result.m6  = (A.m2 * B.m4)  + (A.m6 * B.m5)  + (A.m10 * B.m6)  + (A.m14 * B.m7);
    Result.m7  = (A.m3 * B.m4)  + (A.m7 * B.m5)  + (A.m11 * B.m6)  + (A.m15 * B.m7);
    Result.m8  = (A.m0 * B.m8)  + (A.m4 * B.m9)  + (A.m8  * B.m10) + (A.m12 * B.m11);
    Result.m9  = (A.m1 * B.m8)  + (A.m5 * B.m9)  + (A.m9  * B.m10) + (A.m13 * B.m11);
    Result.m10 = (A.m2 * B.m8)  + (A.m6 * B.m9)  + (A.m10 * B.m10) + (A.m14 * B.m11);
    Result.m11 = (A.m3 * B.m8)  + (A.m7 * B.m9)  + (A.m11 * B.m10) + (A.m15 * B.m11);
    Result.m12 = (A.m0 * B.m12) + (A.m4 * B.m13) + (A.m8  * B.m14) + (A.m12 * B.m15);
    Result.m13 = (A.m1 * B.m12) + (A.m5 * B.m13) + (A.m9  * B.m14) + (A.m13 * B.m15);
    Result.m14 = (A.m2 * B.m12) + (A.m6 * B.m13) + (A.m10 * B.m14) + (A.m14 * B.m15);
    Result.m15 = (A.m3 * B.m12) + (A.m7 * B.m13) + (A.m11 * B.m14) + (A.m15 * B.m15);
    return Result;
}

inline void
Transpose(mat4x4 *M)
{
    mat4x4 Temp;

    Temp.m0 = M->m0;
    Temp.m1 = M->m4;
    Temp.m2 = M->m8;
    Temp.m3 = M->m12;
    Temp.m4 = M->m1;
    Temp.m5 = M->m5;
    Temp.m6 = M->m9;
    Temp.m7 = M->m13;
    Temp.m8 = M->m2;
    Temp.m9 = M->m6;
    Temp.m10 = M->m10;
    Temp.m11 = M->m14;
    Temp.m12 = M->m3;
    Temp.m13 = M->m7;
    Temp.m14 = M->m11;
    Temp.m15 = M->m15;

    *M = Temp;
}

inline mat4x4
FrustumMatrix(r32 Left, r32 Right, r32 Bottom, r32 Top, r32 Near, r32 Far)
{
    mat4x4 Result = NullMat4x4();

    Result.m0  = (Near * 2.0f) / (Right - Left);
    Result.m5  = (Near * 2.0f) / (Top - Bottom);
    Result.m8  = (Right + Left) / (Right - Left);
    Result.m9  = (Top + Bottom) / (Top - Bottom);
    Result.m10 = - (Far + Near) / (Far - Near);
    Result.m11 = -1.0f;
    Result.m14 = - (2.0f * Far * Near) / (Far - Near);
    
    return Result;
}

inline mat4x4
PerspectiveMatrix(r32 FoV, r32 Aspect, r32 Near, r32 Far)
{
    r32 Top   = Near * tanf(FoV * M_PI / 360);
    r32 Right = Top * Aspect;
    return FrustumMatrix(-Right, Right, -Top, Top, Near, Far);
}

inline mat4x4
LookAtMatrix(v3 Eye, v3 Target, v3 Up)
{
    mat4x4 Result = NullMat4x4();
    v3 z = NormalizeV3(Eye - Target);
    v3 x = NormalizeV3(CrossV3(Up, z));
    v3 y = NormalizeV3(CrossV3(z, x));

    Result.m0 = x.x;
    Result.m1 = x.y;
    Result.m2 = x.z;
    Result.m3 = -((x.x*Eye.x) + (x.y*Eye.y) + (x.z*Eye.z));
    Result.m4 = y.x;
    Result.m5 = y.y;
    Result.m6 = y.z;
    Result.m7 = -((y.x*Eye.x) + (y.y*Eye.y) + (y.z*Eye.z));
    Result.m8 = z.x;
    Result.m9 = z.y;
    Result.m10 = z.z;
    Result.m11 = -((z.x*Eye.x) + (z.y*Eye.y) + (z.z*Eye.z));
    Result.m12 = 0.0f;
    Result.m13 = 0.0f;
    Result.m14 = 0.0f;
    Result.m15 = 1.0f;

    return Result;
}



#if 0
inline mat4
ScalingMatrix(v3 A)
{
    mat4 Result = Mat4x4(1);
    Result.R[0][0] = A.E[0];
    Result.R[1][1] = A.E[1];
    Result.R[2][2] = A.E[2];
    Result.R[3][3] = 1;
    return Result;
}

inline mat4
RotationMatrix(r32 Angle, v3 A)
{
    Angle = Angle * (PI/180);
    
    mat4 Result = Mat4x4(1);
    Result.R[0][0] = (1-cos(Angle)) * pow(A.x, 2) + cos(Angle);
    Result.R[0][1] = (1-cos(Angle)) * (A.x * A.y) - (sin(Angle) * A.z);
    Result.R[0][2] = (1-cos(Angle)) * (A.x * A.z) + (sin(Angle) * A.y);
    Result.R[1][0] = (1-cos(Angle)) * (A.x * A.y) + (sin(Angle) * A.z);
    Result.R[1][1] = (1-cos(Angle)) * pow(A.y, 2) + cos(Angle);
    Result.R[1][2] = (1-cos(Angle)) * (A.y * A.z) - (sin(Angle) * A.x);
    Result.R[2][0] = (1-cos(Angle)) * (A.x * A.z) - (sin(Angle) * A.y);
    Result.R[2][1] = (1-cos(Angle)) * (A.y * A.z) + (sin(Angle) * A.x);
    Result.R[2][2] = (1-cos(Angle)) * pow(A.z, 2) + cos(Angle);
    Result.R[3][3] = 1;
    return Result;
}

inline mat4
TranslationMatrix(v3 A)
{
    mat4 Result = Mat4x4(1);
    Result.R[0][3] = A.E[0];
    Result.R[1][3] = A.E[1];
    Result.R[2][3] = A.E[2];
    Result.R[3][3] = 1.0f;
    return Result;
}


// Matrix operator overloading
inline mat4
operator*(mat4 A, mat4 B)
{
    mat4 Result = {};
    for (int i=0; i < 4; i++)
    {
        for (int j=0; j < 4; j++)
        {
            for (int k=0; k < 4; k++)
            {
                Result.R[i][j] += A.R[i][k] * B.R[k][j];
            }
        }
    }
    return Result;
}

inline mat4
operator*=(mat4 &A, mat4 B)
{
    mat4 Temp = Mat4x4();
    for (int i=0; i < 4; i++)
    {
        for (int j=0; j < 4; j++)
        {
            for (int k=0; k < 4; k++)
            {
                Temp.R[i][j] += A.R[i][k] * B.R[k][j];
            }
        }
    }
    A = Temp;
    return A;
}

inline void
Mat4x4Scale(mat4 &M, v3 A)
{ 
    M *= ScalingMatrix(A);
}

inline void
Mat4x4Rotate(mat4 &M, r32 Angle, v3 A)
{ 
    M *= RotationMatrix(Angle, A);
}


inline void
Mat4x4Translate(mat4 &M, v3 A)
{ 
    M *= TranslationMatrix(A);
}

void
_MatPrint(r32 *M, u32 Elements)
{
    u32 Size = sqrt(Elements);
    for (int i=0;i<Size;i++)
    {
        for (int j=0;j<Size;j++)
        {
            r32 *Value = M + (i*Size) + j; 
            printf("[%i,%i]%.2f\t", i, j, *Value);
        }
        printf("\n");
    }
}

#define MatPrint(M) _MatPrint((r32 *)&M, sizeof(M) / sizeof(M.E[0]));
#endif



#define AGL_MATH_H
#endif
