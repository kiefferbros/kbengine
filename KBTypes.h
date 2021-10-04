/*
 *  KBTypes.h
 *  KBEngine
 *
 *  Created by Jonathan on 4/22/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef KBTypes_h
#define KBTypes_h

#ifdef __APPLE__

    /* osx or ios */
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #include <OpenGLES/ES1/gl.h>
        #include <OpenGLES/ES1/glext.h>
    #else
        #include <OpenGL/gl.h>
    #endif

#else
/* android */
    #include <jni.h>
    #include <GLES/gl.h>
    #include <GLES/glext.h>
#endif


typedef GLfloat KBFloat;
typedef GLubyte KBUByte;
typedef unsigned char KBBool;
typedef unsigned int KBMask;

enum {
    KBFalse = 0,
    KBTrue = 1
};

/*
typedef struct _KBVec2 {
    KBFloat x, y;
} KBVec2;

typedef struct _KBVec3 { 
    KBFloat x, y, z;
} KBVec3;

typedef struct _KBVec4 {
    KBFloat x, y, z, w;
} KBVec4;*/


typedef union _KBVec2 {
    struct { KBFloat x, y; };
    struct { KBFloat w, h; };
    struct { KBFloat s, t; };
    KBFloat v[2];
} KBVec2;

typedef union _KBVec3 {
    struct { KBFloat x, y, z; };
    struct { KBFloat r, g, b; };
    struct { KBFloat s, t, p; };
    KBFloat v[3];
} KBVec3;

typedef union _KBVec4 {
    struct { KBFloat x, y, z, w; };
    struct { KBFloat r, g, b, a; };
    struct { KBFloat s, t, p, q; };
    KBFloat v[4];
} KBVec4;

typedef struct _KBColor4 {
	KBUByte r, g, b, a;
} KBColor4;

typedef struct _KBColor3 {
	KBUByte r, g, b, a;
} KBColor3;

typedef struct _KBVertex {
	KBVec2       p;
    KBVec2       t;
} KBVertex;

typedef struct _KBTintedVertex {
    KBVec2       p;
    KBVec2       t;
	KBColor4     c;
} KBTintedVertex;

typedef struct _KBColorVertex {
	KBVec2       p;
	KBColor4     c;
} KBColorVertex;

typedef union _KBTransform {
    struct {
        KBFloat m11, m12, m13, m14;
        KBFloat m21, m22, m23, m24;
        KBFloat m31, m32, m33, m34;
        KBFloat m41, m42, m43, m44;
    };
    struct {
        KBFloat a, b, e0, e1;
        KBFloat c, d, e2, e3;
        KBFloat e4, e5, e6, e7;
        KBFloat x, y, e8, e9;
    };
    KBFloat m[16];
} KBTransform;

typedef void KBType;

typedef void (*KBDestroyFunc)(KBType *obj);
typedef const char * (*KBDescriptionFunc)(KBType *obj);

typedef struct _KBObject {
    int                 retainCount;
    KBDestroyFunc       destroyFunc;
    KBDescriptionFunc   descripFunc;
} KBObject;

#define KBObjectCast(obj)((KBObject*)obj)

KBType * KBAlloc(long int size);
KBType * KBObjectInit(KBType *obj, KBDestroyFunc destroyFunc);
KBType * KBRetain(KBType *obj);
void KBRelease(KBType *obj);
void KBReleaseBatch(KBType **obj, int count);

void KBDescription(KBType *obj);

typedef struct _KBBox {
    KBFloat l, b, r, t; 
} KBBox;

#if !defined(MIN)
#define MIN(A,B)	((A) < (B) ? (A) : (B))
#endif

#if !defined(MAX)
#define MAX(A,B)	((A) > (B) ? (A) : (B))
#endif

static inline int
KBNextPOT(int n) {
    if((n != 1) && (n & (n - 1))) {
		int i = 1;
		while(i < n)
			i *= 2;
		n = i;
	}
    return n;
}

#pragma mark - Float
static inline KBFloat
KBFloatMax(KBFloat a, KBFloat b)
{
	return (a > b) ? a : b;
}

static inline KBFloat
KBFloatMin(KBFloat a, KBFloat b)
{
	return (a < b) ? a : b;
}

static inline KBFloat
KBFloatAbs(KBFloat n)
{
	return (n < 0) ? -n : n;
}

void KBFloatRandomSeed(unsigned int seed);
KBFloat KBFloatRandom(void);

static inline KBFloat
KBFloatClamp(KBFloat f, KBFloat min, KBFloat max)
{
	return KBFloatMin(KBFloatMax(f, min), max);
}


static inline KBFloat
KBFloatLerpConst(KBFloat f1, KBFloat f2, KBFloat d)
{
	return f1 + KBFloatClamp(f2 - f1, -d, d);
}

static inline KBBool
KBFloatBetween(KBFloat f, KBFloat min, KBFloat max) {
	return (f > min && f < max);
}

// linear interpolation
static inline KBFloat
KBFloatLerp (KBFloat a, KBFloat b, KBFloat delta) {    
    return (b-a)*delta+a;
}

// weighted interpolation
static inline KBFloat
KBFloatWerp (KBFloat a, KBFloat b, KBFloat t, KBFloat aT, KBFloat aV, KBFloat bT, KBFloat bV) {   
     
    aT = -0.33333333f*KBFloatClamp(aT,-1.f,1.f) + 0.33333333f;
    bT = 1.f - (-0.33333333f*KBFloatClamp(bT,-1.f,1.f) + 0.33333333f);
    
    aV = -0.33333333f*aV + 0.33333333f;
    bV = 1.f - (-0.33333333f*bV + 0.33333333f);
    
    KBFloat OmT = 1.f-t;
    KBFloat d = 3*(OmT*OmT)*t*aT + 3*OmT*(t*t)*bT + t*t*t;

    KBFloat OmD = 1.f-d;
    KBFloat v = 3*(OmD*OmD)*d*aV + 3*OmD*(d*d)*bV + d*d*d;
    return (b-a)*v + a;
}

static const KBFloat KBPi		= 3.141592653589793f;
static const KBFloat KBPi2		= 6.283185307179586f;
static const KBFloat KBPiD2		= 1.570796326794897f;
static const KBFloat KBPiSQR	= 9.869604401089357f;
KBFloat KBFastSine(KBFloat x);
KBFloat KBFastCosine(KBFloat x);

KBFloat KBSimpleSine(KBFloat x);
KBFloat KBSimpleCosine(KBFloat x);

#pragma mark - Color RGBA (unsigned bytes)
static inline KBColor4
KBColor4Make (KBUByte r, KBUByte g, KBUByte b, KBUByte a) {
	KBColor4 c = { r, g, b, a };
	return c;
}

static inline KBColor4
KBColor4MakeWhite (KBUByte val) {
	KBColor4 c = {val, val, val, 255};
	return c;
}

static KBBool // boolean
KBColor4Equal (KBColor4 c1, KBColor4 c2) {
	return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a);
}

static const KBColor4 KBColor4Clear = {0, 0, 0, 0};
static const KBColor4 KBColor4Black = {0, 0, 0, 255};
static const KBColor4 KBColor4White = {255, 255, 255, 255};

#endif

