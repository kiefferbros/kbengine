//
//  KBSprite.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/20/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#include "KBSprite.h"
#include "KBScene.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static const char * KBSpriteDescriptionStr = "KBSprite";

const char * KBSpriteGetDescription(KBType *nodeObj) {
    return KBSpriteDescriptionStr;
}

void
KBSpriteDraw (void *drawableObj, KBFloat opacity) {
    KBSprite *sprite = drawableObj;
    
    KBFloat op = KBDrawableGetDrawOpacity(drawableObj, opacity);
    KBTransform *tranform = KBDrawableGetTransform(drawableObj);
    glPushMatrix();
    glMultMatrixf(tranform->m);
    
    
    KBDrawableBindDrawColor(sprite, op);
    
    // draw sprite
    KBTexture *tex = sprite->texture;
    glBindTexture(GL_TEXTURE_2D, tex->name);
    
    
    // set our open gl vertex arrays and draw
    glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
    glVertexPointer(2, GL_FLOAT, sizeof(KBVertex), (GLvoid*)0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(KBVertex), ((GLvoid*)sizeof(KBVec2)));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    if (KBNodeCast(drawableObj)->children!=NULL) {
        KBDrawableDrawChildren(drawableObj, op);
    }
    
    glPopMatrix();
}

KBBox KBSpriteGetContentBox(KBType *drawableObj) {
    KBSprite *sprite = drawableObj;
    return KBBoxMake2(KBVec2Zero, sprite->texture->size);
}

KBBox KBSpriteGetAtlasContentBox(KBType *drawableObj) {
    KBSprite *sprite = drawableObj;
    
    KBScene *scene = KBNodeGetScene(drawableObj);
    const KBMap *map = NULL;
    if (sprite->atlasIndex>-1 && scene && scene->atlas) {
        map = KBAtlasGetMapAtIndex(scene->atlas, sprite->atlasIndex);
    }

    return map ? KBBoxMake2(KBVec2Zero, map->s) : KBBoxZero;
}

KBSprite *
KBSpriteInit(KBSprite *sprite, KBDestroyFunc destroyFunc, KBTexture *tex) {
    if ((sprite = (KBSprite*)KBDrawableInit((KBDrawable*)sprite, destroyFunc, tex?&KBSpriteDraw:&KBNullDraw, NULL))) {
        sprite->texture = KBRetain(tex);
        KBDrawableCast(sprite)->contentBoxFunc = tex? &KBSpriteGetContentBox: &KBDrawableGetDefaultContentBox;
        
        
        KBObjectCast(sprite)->descripFunc = &KBSpriteGetDescription;
        
        glGenBuffers(1, &sprite->vbo);
        
        if (tex) {
            KBVertex v[4] = {
                {0.f, 0.f, 0.f, 0.f}, {0.f, tex->size.h, 0.f, tex->max.t},
                {tex->size.w, 0.f, tex->max.s, 0.f}, {tex->size.w, tex->size.h, tex->max.s, tex->max.t}
            };
            
            glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(KBVertex)*4, v, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        
        sprite->atlasIndex = -1;
    }
    
    return sprite;
}

KBSprite *
KBSpriteNew(KBTexture *texture) {
    return KBSpriteInit(KBAlloc(sizeof(KBSprite)), &KBSpriteDestroy, texture);
}

KBSprite * KBSpriteInitAtlas(KBSprite *sprite, KBDestroyFunc destroyFunc, int atlasIndex) {
    if ((sprite = (KBSprite*)KBDrawableInit((KBDrawable*)sprite, destroyFunc, KBNullDraw, NULL))) {
        KBObjectCast(sprite)->descripFunc = &KBSpriteGetDescription;
        sprite->atlasIndex = -1;
        KBSpriteSetAtlasIndex(sprite, atlasIndex);
    }
    return sprite;
}

KBSprite *
KBSpriteNewAtlas(int atlasIndex) {
    return KBSpriteInitAtlas(KBAlloc(sizeof(KBSprite)), &KBSpriteDestroy, atlasIndex);
}

void
KBSpriteDestroy(KBType *obj) {
    KBSprite *sprite = obj;
    KBRelease(sprite->texture);
    if (sprite->vbo)
        glDeleteBuffers(1, &sprite->vbo);
    KBDrawableDestroy(obj);
}

void
KBSpriteSetTexture(KBSprite *sprite, KBTexture *tex) {
    if (sprite->texture != tex) {
        
        KBRelease(sprite->texture);
        sprite->texture = KBRetain(tex);
        KBDrawableCast(sprite)->drawFunc = tex?&KBSpriteDraw:&KBNullDraw;
        KBDrawableCast(sprite)->contentBoxFunc = tex? &KBSpriteGetContentBox: &KBDrawableGetDefaultContentBox;
        
        if (tex) {
            KBVertex v[4] = {
                {0.f, 0.f, 0.f, 0.f}, {0.f, tex->size.h, 0.f, tex->max.t},
                {tex->size.w, 0.f, tex->max.s, 0.f}, {tex->size.w, tex->size.h, tex->max.s, tex->max.t}
            };
            
            glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(KBVertex)*4, v, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
}

void
KBSpriteSetAtlasIndex(KBSprite *sprite, int atlasIndex) {
    if (atlasIndex != sprite->atlasIndex) {
        sprite->atlasIndex = atlasIndex;
        KBDrawableCast(sprite)->contentBoxFunc = KBSpriteGetAtlasContentBox;
    }
}

void
KBSpriteUpdateAtlasVertices(KBSprite *sprite, KBAtlas *atlas, KBTintedVertex **verts, KBTransform *t, KBFloat o, int *count) {
    KBTransform nt;
    KBTransformAffineMult(KBDrawableGetTransform(sprite), t, &nt);
    KBFloat op = KBDrawableGetDrawOpacity(sprite, o);
    
    if (sprite->atlasIndex>-1 && sprite->atlasIndex<atlas->nMaps) {
        const KBVertex *m = KBAtlasGetVerticesForMapIndex(atlas, sprite->atlasIndex);
        KBTintedVertex *v = *verts;
        
        // position
        v[0].p = KBVec2Transform(m[0].p, &nt);
        v[1].p = KBVec2Transform(m[1].p, &nt);
        v[2].p = KBVec2Transform(m[2].p, &nt);
        v[3].p = KBVec2Transform(m[3].p, &nt);
        
        // tex coord
        v[0].t = m[0].t;
        v[1].t = m[1].t;
        v[2].t = m[2].t;
        v[3].t = m[3].t;
        
        // color
        KBVec3 tint = KBDrawableCast(sprite)->tint;
        KBColor4 c = KBColor4Make(tint.r*op*255, tint.g*op*255, tint.b*op*255, op*255);
        v[0].c = c; v[1].c = c; v[2].c = c; v[3].c = c;
        
        (*count)++;
        (*verts)+=4;
    }
    
    KBArray *children = KBNodeCast(sprite)->children;
    if (children) {
        for (int i=0; i<children->num; i++) {
            KBSpriteUpdateAtlasVertices(children->arr[i], atlas, verts, &nt, op, count);
        }
    }
    
}