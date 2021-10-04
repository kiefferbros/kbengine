//
//  KBDrawable.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/17/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBDrawable_h
#define KBDrawable_h

#include <stdlib.h>
#include "KBNode.h"
#include "KBGeometry.h"

typedef void (*KBDrawFunc)(KBType *drawableObj, KBFloat opacity);
typedef KBType * (*KBHitTestFunc)(KBType *drawableObj, KBVec2 p);
typedef KBBox (*KBContentBoxFunc)(KBType *drawableObj);

static inline KBType *
KBNullHitTest(KBType *drawableObj, KBVec2 p) {
    return NULL;
}

static inline void
KBNullDraw(KBType *drawableObj, KBFloat opacity) {
}

typedef struct _KBDrawable {
    KBNode              super;
    
    KBVec2              pos; //position
    KBFloat             rot; //rotation
    KBVec2              scale; //scale
    KBVec2              anchor; //anchor
    KBTransform         transform; // cached transform
    KBBool              transformFlag;
    
    KBDrawFunc          drawFunc;
    KBVec3              tint;
    KBFloat             opacity;
    KBBool              inheritOpacity;
    
    //KBBox             contentBox;
    KBContentBoxFunc    contentBoxFunc;
    KBHitTestFunc       hitTestFunc;
} KBDrawable;

#define KBDrawableCast(obj)((KBDrawable*)obj)

/*inheritable structs require init and destroy functions*/

KBDrawable * KBDrawableInit(KBDrawable *drawable, KBDestroyFunc destroyFunc, KBDrawFunc drawFunc, KBHitTestFunc hitTestFunc);
KBDrawable * KBDrawableNew(KBDrawFunc drawFunc, KBHitTestFunc hitTestFunc);
/* if 'sub-structing' call this at the end of your destroy function */
void KBDrawableDestroy(KBType *drawable);

void KBDrawableBindDrawColor(KBType *drawableObj, KBFloat opacity);
KBFloat KBDrawableGetDrawOpacity(KBType *drawableObj, KBFloat parentOpacity);
void KBDrawableDraw (KBType *drawableObj, KBFloat opacity);
void KBDrawableDefaultDraw (KBType *drawableObj, KBFloat opacity);

/* this function is slightly dangerous. one must check to see if the child array exists before calling (drawable->node->children!=NULL) */
void KBDrawableDrawChildren(KBType *drawableObj, KBFloat op);

KBBox KBDrawableGetContentBox(KBType *drawableObj);
KBBox KBDrawableGetDefaultContentBox(KBType *drawableObj);

void KBDrawableGetGlobalTransform(KBDrawable *drawableObj, KBTransform *t);

KBBox KBDrawableGetBoundBox(KBType *drawableObj);
KBBox KBDrawableGetGlobalBoundBox(KBType *drawableObj);
KBVec2 KBDrawableGetGlobalPosition(KBType *drawObj);

KBType * KBDrawableHitTest(KBType *drawableObj, KBVec2 p);

/* drawable functions */
static inline void
KBDrawableSetPosition(KBType *drawableObj, KBVec2 p) {
    KBDrawable *drawable = drawableObj;
    drawable->pos = p;
    drawable->transformFlag = KBTrue;
}

static inline KBVec2
KBDrawableGetPosition(KBType *drawableObj) {
    return KBDrawableCast(drawableObj)->pos;
}

static inline void
KBDrawableSetRotation(KBType *drawableObj, KBFloat r) {
    KBDrawable *drawable = drawableObj;
    drawable->rot = r;
    drawable->transformFlag = KBTrue;
}

static inline KBFloat   
KBDrawableGetRotation(KBType *drawableObj) {
    return KBDrawableCast(drawableObj)->rot;
}

static inline void
KBDrawableSetScale(KBType *drawableObj, KBVec2 s) {
    KBDrawable *drawable = drawableObj;
    drawable->scale = s;
    drawable->transformFlag = KBTrue;
}

static inline KBVec2
KBDrawableGetScale(KBType *drawableObj) {
    return KBDrawableCast(drawableObj)->scale;
}

static inline void
KBDrawableSetAnchor(KBType *drawableObj, KBVec2 a) {
    KBDrawable *drawable = drawableObj;
    drawable->anchor = a;
    drawable->transformFlag = KBTrue;
}

static inline KBVec2
KBDrawableGetAnchor(KBType *drawableObj) {
    return KBDrawableCast(drawableObj)->anchor;
}


static inline void
KBDrawableSetAnchorInPlace(KBType *drawableObj, KBVec2 a) {
    KBDrawable *drawable = drawableObj;
    KBVec2 dif = KBVec2Sub(a, drawable->anchor);
    drawable->anchor = a;
    drawable->pos = KBVec2Add(drawable->pos, dif);
    drawable->transformFlag = KBTrue;
}

static inline void
KBDrawableCacheTransform(KBType *drawableObj) {
    KBDrawable *drawable = drawableObj;
    KBTransformSetAffine(&drawable->transform, drawable->pos, drawable->rot, drawable->scale, drawable->anchor);
    drawable->transformFlag = KBFalse;
}

static inline KBTransform *
KBDrawableGetTransform(KBType *drawableObj) {
    KBDrawable *drawable = drawableObj;
    if (drawable->transformFlag) {
        KBDrawableCacheTransform(drawable);
    }
    
    return &drawable->transform;
}

static inline void
KBDrawableSetTint(KBType *drawableObj, KBVec3 tint) {
    KBDrawableCast(drawableObj)->tint = tint;
}

static inline void
KBDrawableSetOpacity(KBType *drawableObj, KBFloat op) {
    KBDrawableCast(drawableObj)->opacity = op;
}

/* animation functions */
void KBDrawableSetAnimatedPosition(KBType *drawableObj, void *posPtr);
void KBDrawableSetAnimatedPosX(KBType *drawableObj, void *xPtr);
void KBDrawableSetAnimatedPosY(KBType *drawableObj, void *yPtr);
void KBDrawableSetAnimatedRotation(KBType *drawableObj, void *rotPtr);
void KBDrawableSetAnimatedScale(KBType *drawableObj, void *scalePtr);
void KBDrawableSetAnimatedUniformScale(KBType *drawableObj, void *scalePtr);
void KBDrawableSetAnimatedAnchor(KBType *drawableObj, void *anchorPtr);
void KBDrawableSetAnimatedOpacity(KBType *drawableObj, void *opPtr);

#endif
