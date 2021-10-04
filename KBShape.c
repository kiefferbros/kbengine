//
//  KBShape.c
//  Star Hound
//
//  Created by KB Jonathan on 1/25/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBShape.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>


void
KBShapeDrawRect(KBType *drawableObj, KBFloat op) {
    
    op = KBDrawableGetDrawOpacity(drawableObj, op);
    KBDrawableBindDrawColor(drawableObj, op);
    
    KBShape *shape = drawableObj;
    
    glBindBuffer(GL_ARRAY_BUFFER, shape->vbo);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    if (KBNodeCast(drawableObj)->children) {
        KBDrawableDrawChildren(drawableObj, op);
    }
}

KBShape *
KBShapeInit(KBShape *shape, KBDestroyFunc destroyFunc, KBVec2 *vertices, int count, KBDrawFunc drawFunc) {
    KBDrawableInit((KBDrawable*)shape, destroyFunc, drawFunc, NULL);
    
    glGenBuffers(1, &shape->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, shape->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(KBVec2)*count, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    return shape;
}

KBShape *
KBShapeNewRect(KBVec2 s) {
    
    KBVec2 v[4] = {
        0.f, 0.f, 0.f, s.h,
        s.w, 0.f, s.w, s.h
    };
    
    return KBShapeInit(KBAlloc(sizeof(KBShape)), &KBShapeDestroy, v, 4, &KBShapeDrawRect);    
}

void
KBShapeDestroy(KBType *shapeObj) {
    KBShape *shape = shapeObj;
    glDeleteBuffers(1, &shape->vbo);
    
    KBDrawableDestroy(shapeObj);
}