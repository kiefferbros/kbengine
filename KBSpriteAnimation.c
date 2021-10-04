//
//  KBSpriteAnimation.c
//  KBEngine
//
//  Created by KB Jonathan on 6/5/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBSpriteAnimation.h"
#include "KBSprite.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char * KBSpriteAnimationDescriptionStr = "KBSpriteAnimation";

const char * KBSpriteAnimationGetDescription(KBType *nodeObj) {
    return KBSpriteAnimationDescriptionStr;
}

KBBool KBSpriteAnimationStep(KBType *animObj, KBFloat dt);
KBBool KBSpriteAnimationStepAtlas(KBType *animObj, KBFloat dt);

KBSpriteAnimation *
KBSpriteAnimationInit(KBSpriteAnimation       *anim,
                      KBDestroyFunc           destroyFunc,
                      int                     name,
                      KBAnimationEventFunc    startFunc,
                      KBAnimationEventFunc    stopFunc,
                      void                     *data,
                      KBTexture               **textures,
                      int                     nTextures,
                      KBFloat                 frameDuration)
{
    return anim;
}

KBSpriteAnimation *
KBSpriteAnimationNew(int                      name,
                     KBAnimationEventFunc     startFunc,
                     KBAnimationEventFunc     stopFunc,
                     void                     *data,
                     KBTexture                **textures,
                     int                      nTextures,
                     KBFloat                  frameDuration)
{
    return KBSpriteAnimationInit(KBAlloc(sizeof(KBSpriteAnimation)),
                                 &KBSpriteAnimationDestroy,
                                 name, startFunc, stopFunc, data,
                                 textures, nTextures, frameDuration);
}

KBSpriteAnimation *
KBSpriteAnimationInitAtlas(KBSpriteAnimation        *anim,
                           KBDestroyFunc            destroyFunc,
                           int                      name,
                           KBAnimationEventFunc     startFunc,
                           KBAnimationEventFunc     stopFunc,
                           void                     *data,
                           int                      *indexes,
                           int                      nIndexes,
                           KBFloat                  frameDuration)
{
    
    if ((anim = (KBSpriteAnimation*)KBAnimationInit((KBAnimation*)anim, destroyFunc,
                                                    name, &KBSpriteAnimationStepAtlas,
                                                    startFunc, stopFunc, data)))
    {
        KBObjectCast(anim)->descripFunc = &KBSpriteAnimationGetDescription;
        anim->frameDur = frameDuration;
        anim->nIndexes = nIndexes;
        anim->indexes = malloc(sizeof(int)*nIndexes);
        memcpy(anim->indexes, indexes, sizeof(int)*nIndexes);
    }

    return anim;
}

KBSpriteAnimation *
KBSpriteAnimationNewAtlas(int                       name,
                          KBAnimationEventFunc      startFunc,
                          KBAnimationEventFunc      stopFunc,
                          void                      *data,
                          int                       *indexes,
                          int                       nIndexes,
                          KBFloat                   frameDuration)
{
    return KBSpriteAnimationInitAtlas(KBAlloc(sizeof(KBSpriteAnimation)),
                                      &KBSpriteAnimationDestroy,
                                      name, startFunc, stopFunc, data,
                                      indexes, nIndexes, frameDuration);
}

void
KBSpriteAnimationDestroy(KBType *animObj) {
    KBSpriteAnimation *anim = animObj;
    
    free(anim->indexes);
    KBRelease(anim->textures);
    
    KBAnimationDestroy(animObj);
}

KBBool
KBSpriteAnimationStep(KBType *animObj, KBFloat dt) {
    return KBTrue;
}

KBBool
KBSpriteAnimationStepAtlas(KBType *animObj, KBFloat dt) {
    KBSpriteAnimation *anim = animObj;
    
    KBFloat oldTime, time;
    oldTime = anim->time;
    time = oldTime + dt;
    
    if (time>=0.f) {
        
        if (oldTime <= 0.f) {
            KBAnimationStart(animObj);
        }
        
        /* what keyframes should we use? */
        int frame = time/anim->frameDur;
        
        int idx = (frame < anim->nIndexes) ? anim->indexes[frame] : anim->indexes[anim->nIndexes-1];
        
        KBSpriteSetAtlasIndex(KBAnimationGetNode(animObj), idx);
        
        
        
        if (time>=anim->nIndexes*anim->frameDur) {
            if (anim->loops) {
                time = 0.f;
            } else {
                anim->finished = KBTrue;
                return KBFalse;
            }
        }
    }
    
    anim->time = time;
    
    return KBTrue;
}