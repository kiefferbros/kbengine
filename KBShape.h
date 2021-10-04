//
//  KBShape.h
//  Star Hound
//
//  Created by KB Jonathan on 1/25/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef Star_Hound_KBShape_h
#define Star_Hound_KBShape_h

#include "KBDrawable.h"

typedef struct _KBShape {
    KBDrawable super;
    
    GLsizei     count;
    GLuint      vbo;
} KBShape;

#define KBShapeCast(obj)((KBShape*)obj)

KBShape * KBShapeInit(KBShape *shape, KBDestroyFunc destroyFunc, KBVec2 *vertices, int count, KBDrawFunc drawFunc);
KBShape * KBShapeNewRect(KBVec2 size);
void KBShapeDestroy(KBType *shapeObj);

#endif
