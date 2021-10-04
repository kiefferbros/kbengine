//
//  KBSprite.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/20/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBSprite_h
#define KBSprite_h

#include "KBDrawable.h"
#include "KBAtlas.h"
#include "KBSpriteAnimation.h"

typedef struct _KBSprite {
    KBDrawable          super; // "inherit" drawable
    
    KBTexture           *texture;
    GLuint              vbo;
    
    int                 atlasIndex;
    int                 atlasVBOIndex;
} KBSprite;

#define KBSpriteCast(obj)((KBSprite*)obj)

KBSprite * KBSpriteInit(KBSprite *sprite, KBDestroyFunc destroyFunc, KBTexture *texture);
KBSprite * KBSpriteNew(KBTexture *texture);
KBSprite * KBSpriteInitAtlas(KBSprite *sprite, KBDestroyFunc destroyFunc, int atlasIndex);
KBSprite * KBSpriteNewAtlas(int atlasIndex);

void KBSpriteDestroy(KBType *sprite);

void KBSpriteSetTexture(KBSprite *sprite, KBTexture *texture);

void KBSpriteSetAtlasIndex(KBSprite *sprite, int atlasIndex);
void KBSpriteUpdateAtlasVertices(KBSprite *sprite, KBAtlas *atlas, KBTintedVertex **v, KBTransform *t, KBFloat o, int *c);
#endif
