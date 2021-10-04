//
//  KBDrawable.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/18/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#include "KBDrawable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* passes drawing onto children */
void
KBDrawableDefaultDraw (KBType *drawableObj, KBFloat opacity) {

    KBDrawable *drawable = (KBDrawable*)drawableObj;
    
    if (KBNodeCast(drawableObj)->children!=NULL) {
        
        KBTransform *t = KBDrawableGetTransform(drawable);
        glPushMatrix();
        glMultMatrixf(t->m);
    
        KBDrawableDrawChildren(drawable, KBDrawableGetDrawOpacity(drawableObj, opacity));

        glPopMatrix();
    }
}

KBType *
KBDrawableDefaultHitTest(KBType *drawableObj, KBVec2 p) {
    KBDrawable *drawable = drawableObj;
    
    KBArray *children = KBNodeCast(drawable)->children;
    
    if (children!=NULL) {
        for (int i=children->num-1; i>=0; i--) {
            KBDrawable *node = children->arr[i];
            KBDrawable *child = node->hitTestFunc(node, p);
            
            if (child!=NULL) {
                return child;
            }
        }
    }
    
    return KBBoxContainsVec(KBDrawableGetGlobalBoundBox(drawableObj), p) ? drawableObj : NULL;
}


KBDrawable * KBDrawableInit(KBDrawable *drawable, KBDestroyFunc destroyFunc, KBDrawFunc drawFunc, KBHitTestFunc hitTestFunc) {
    KBNodeInit((KBNode*)drawable, destroyFunc);
    
    drawable->drawFunc = drawFunc!=NULL ? drawFunc : KBDrawableDefaultDraw;
    
    drawable->pos = KBVec2Zero;
    drawable->rot = 0.f;
    drawable->scale = KBVec2One;
    drawable->anchor = KBVec2Zero;
    drawable->transform = KBTransformIdentity;
    drawable->transformFlag = KBFalse;
    
    drawable->tint = KBVec3MakeUniform(1.f);
    drawable->opacity = 1.f;
    drawable->inheritOpacity = KBTrue;
    //drawable->contentBox = KBBoxZero;
    drawable->contentBoxFunc = &KBDrawableGetDefaultContentBox;
    drawable->hitTestFunc = hitTestFunc!=NULL ? hitTestFunc : &KBDrawableDefaultHitTest;
    
    return drawable;
}

KBDrawable *
KBDrawableNew(KBDrawFunc drawFunc, KBHitTestFunc hitTestFunc) {
    return KBDrawableInit(KBAlloc(sizeof(KBDrawable)), &KBDrawableDestroy, drawFunc, hitTestFunc);
}

void
KBDrawableDestroy(KBType *obj) {
    KBNodeDestroy(obj);
}

void
KBDrawableBindDrawColor(KBType *drawableObj, KBFloat op) {
    KBDrawable *drawable = drawableObj;
    
    glColor4f(drawable->tint.r*op, drawable->tint.g*op, drawable->tint.b*op, op);
}

KBFloat
KBDrawableGetDrawOpacity(KBType *drawableObj, KBFloat opacity) {
    KBDrawable *drawable = drawableObj;
    return drawable->inheritOpacity ? opacity*drawable->opacity : drawable->opacity;
}

void
KBDrawableDraw (KBType *drawableObj, KBFloat opacity) {
    KBDrawable *drawable = drawableObj;
    drawable->drawFunc(drawableObj, opacity);
}

void KBDrawableDrawChildren(KBType *drawableObj, KBFloat op) {
    /* call each childs draw function */
    
    KBArray *children = KBNodeCast(drawableObj)->children;
    for (int i=0; i<children->num; i++) {
        KBDrawable *child = children->arr[i];
        child->drawFunc(child, op);
    }
}

KBBox KBDrawableGetContentBox(KBType *drawableObj) {
    return KBDrawableCast(drawableObj)->contentBoxFunc(drawableObj);
}

KBBox KBDrawableGetDefaultContentBox(KBType *drawableObj) {
    return KBBoxZero;
}


void
KBDrawableGetGlobalTransform(KBDrawable *drawableObj, KBTransform *t) {
    KBDrawable *parent = drawableObj;
    
    memcpy(t, KBDrawableGetTransform(parent), sizeof(KBTransform));
    
    while (KBNodeCast(parent)->parent!=KBNodeCast(parent)->scene) {
        parent = KBNodeCast(parent)->parent;
        KBTransform *pt = KBDrawableGetTransform(parent);
        KBTransformAffineMult(t, pt, t);
    }
}

KBBox
KBDrawableGetBoundBox(KBType *drawableObj) {
    KBDrawable *drawable = drawableObj;
    return KBBoxTransform(KBDrawableGetContentBox(drawableObj), KBDrawableGetTransform(drawable));
}

KBBox
KBDrawableGetGlobalBoundBox(KBType *drawableObj) {
    KBTransform t;
    KBDrawableGetGlobalTransform(drawableObj, &t);
    return KBBoxTransform(KBDrawableGetContentBox(drawableObj), &t);
}

KBVec2
KBDrawableGetGlobalPosition(KBType *drawObj) {
    KBTransform t;
    KBDrawableGetGlobalTransform(KBNodeGetParent(drawObj), &t);
    return KBVec2Transform(KBDrawableCast(drawObj)->pos, &t);
}

KBType *
KBDrawableHitTest(KBType *drawableObj, KBVec2 p) {
    KBDrawable *drawable = drawableObj;
    return drawable->hitTestFunc(drawableObj, p);
}


#pragma mark - Animation Setters
void
KBDrawableSetAnimatedPosition(KBType *drawableObj, void *posPtr) {
    KBDrawable *drawable = drawableObj;
    memcpy(&drawable->pos, posPtr, sizeof(KBVec2));
    drawable->transformFlag = KBTrue;
}

void
KBDrawableSetAnimatedPosX(KBType *drawableObj, void *xPtr) {
    KBDrawable *drawable = drawableObj;
    memcpy(&drawable->pos.x, xPtr, sizeof(KBFloat));
    drawable->transformFlag = KBTrue;
}

void
KBDrawableSetAnimatedPosY(KBType *drawableObj, void *yPtr) {
    KBDrawable *drawable = drawableObj;
    memcpy(&drawable->pos.y, yPtr, sizeof(KBFloat));
    drawable->transformFlag = KBTrue;
}

void
KBDrawableSetAnimatedRotation(KBType *drawableObj, void *rotPtr) {
    KBDrawable *drawable = drawableObj;
    memcpy(&drawable->rot, rotPtr, sizeof(KBFloat));
    drawable->transformFlag = KBTrue;
}

void
KBDrawableSetAnimatedScale(KBType *drawableObj, void *scalePtr) {
    KBDrawable *drawable = drawableObj;
    memcpy(&drawable->scale, scalePtr, sizeof(KBVec2));
    drawable->transformFlag = KBTrue;
}

void
KBDrawableSetAnimatedUniformScale(KBType *drawableObj, void *scalePtr) {
    KBDrawable *drawable = drawableObj;
    memcpy(&drawable->scale.x, scalePtr, sizeof(KBFloat));
    memcpy(&drawable->scale.y, scalePtr, sizeof(KBFloat));
    drawable->transformFlag = KBTrue;
}

void
KBDrawableSetAnimatedAnchor(KBType *drawableObj, void *anchorPtr) {
    KBDrawable *drawable = drawableObj;
    memcpy(&drawable->anchor, anchorPtr, sizeof(KBVec2));
    drawable->transformFlag = KBTrue;
}

void
KBDrawableSetAnimatedOpacity(KBType *drawableObj, void *opPtr) {
    KBDrawable *drawable = drawableObj;
    memcpy(&drawable->opacity, opPtr, sizeof(KBFloat));
}