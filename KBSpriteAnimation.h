//
//  KBSpriteAnimation.h
//  KBEngine
//
//  Created by KB Jonathan on 6/5/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBSpriteAnimation_h
#define KBSpriteAnimation_h

#include "KBAnimation.h"
#include "KBArray.h"
#include "KBTexture.h"

typedef struct _KBSpriteAnimation {
    KBAnimation         super;
    KBFloat             time;
    KBFloat             frameDur;
    
    KBArray             *textures;
    int                 *indexes;
    int                 nIndexes;
    
    KBBool              loops;
    KBBool              finished;
} KBSpriteAnimation;

#define KBSpriteAnimationCast(obj)((KBSpriteAnimation*)obj)

KBSpriteAnimation * KBSpriteAnimationInit(KBSpriteAnimation       *anim,
                                          KBDestroyFunc           destroyFunc,
                                          int                     name,
                                          KBAnimationEventFunc    startFunc,
                                          KBAnimationEventFunc    stopFunc,
                                          void                     *data,
                                          KBTexture               **textures,
                                          int                     nTextures,
                                          KBFloat                 frameDuration);

KBSpriteAnimation * KBSpriteAnimationNew(int                      name,
                                         KBAnimationEventFunc     startFunc,
                                         KBAnimationEventFunc     stopFunc,
                                         void                     *data,
                                         KBTexture                **textures,
                                         int                      nTextures,
                                         KBFloat                  frameDuration);

KBSpriteAnimation * KBSpriteAnimationInitAtlas(KBSpriteAnimation        *anim,
                                               KBDestroyFunc            destroyFunc,
                                               int                      name,
                                               KBAnimationEventFunc     startFunc,
                                               KBAnimationEventFunc     stopFunc,
                                               void                     *data,
                                               int                      *indexes,
                                               int                      nIndexes,
                                               KBFloat                  frameDuration);

KBSpriteAnimation * KBSpriteAnimationNewAtlas(int                       name,
                                              KBAnimationEventFunc      startFunc,
                                              KBAnimationEventFunc      stopFunc,
                                              void                     *data,
                                              int                       *indexes,
                                              int                       nIndexes,
                                              KBFloat                   frameDuration);

void KBSpriteAnimationDestroy(KBType *animObj);
#endif
