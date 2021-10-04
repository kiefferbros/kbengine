//
//  KBAnimation.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/27/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#include "KBAnimation.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma mark - Animation

KBBool
KBAnimationDefaultEqual(KBType *animObj1, KBType *animObj2) {
    return KBAnimationCast(animObj1)->stepFunc == KBAnimationCast(animObj2)->stepFunc;
}

KBAnimation *
KBAnimationInit(KBAnimation                   *animation,
                KBDestroyFunc                 destroyFunc,
                int                           name,
                KBAnimationStepFunc           stepFunc,
                KBAnimationEventFunc          startFunc,
                KBAnimationEventFunc          stopFunc,
                void                          *eventData)
{
    KBObjectInit(animation, destroyFunc);
    
    animation->name = name;
    animation->stepFunc = stepFunc;
    animation->startFunc = startFunc;
    animation->stopFunc = stopFunc;
    animation->eventData = eventData;
    animation->node = NULL;
    animation->stopped = KBFalse;
    animation->equalFunc = KBAnimationDefaultEqual;
    
    return animation;
}


KBAnimation *
KBAnimationNew(int                           name,
               KBAnimationStepFunc           stepFunc,
               KBAnimationEventFunc          startFunc,
               KBAnimationEventFunc          stopFunc,
               void                          *eventData)
{
    
    return KBAnimationInit(KBAlloc(sizeof(KBAnimation)),
                           &KBAnimationDestroy,
                           name,
                           stepFunc,
                           startFunc,
                           stopFunc,
                           eventData);
}

void
KBAnimationDestroy(KBType *obj) {
    
}

void
KBAnimationStart(KBType *animationObj) {
    KBAnimation *animation = animationObj;
    if (animation->startFunc) {
        animation->startFunc(animation, animation->eventData);
    }
}

void
KBAnimationStop(KBType *animationObj) {
    KBAnimation *animation = animationObj;
    if (!animation->stopped) {
        animation->stopped = KBTrue;
        if (animation->stopFunc) {
            KBRetain(animation->node);
            animation->stopFunc(animation, animation->eventData);
            KBRelease(animation->node);
        }
        animation->node = NULL;
    }
}

KBBool
KBAnimationStep(KBType *animationObj, KBFloat dt) {
    KBAnimation *animation = animationObj;
    
    
    return animation->node==NULL? KBTrue : animation->stepFunc(animationObj, dt);
}

KBBool
KBAnimationEqual(KBType *animObj1, KBType *animObj2) {
    KBAnimation *anim1 = animObj1;
    KBAnimation *anim2 = animObj2;
    
    if (anim1->equalFunc == anim2->equalFunc) {
        return anim1->equalFunc(animObj1, animObj2);
    }
    
    return KBFalse;
}

KBType *
KBAnimationGetNode(KBType *animObj) {
    return KBAnimationCast(animObj)->node;
}

