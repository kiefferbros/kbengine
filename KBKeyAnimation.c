//
//  KBKeyAnimation.c
//  Star Hound
//
//  Created by KB Jonathan on 1/13/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBKeyAnimation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma mark - Interpolation Functions
/* stepped */
void
KBInterpolateStepped(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue) {
    memcpy(*interpValue, k1->value, k1->len);
}

/* weighted */
void
KBInterpolateFloatWeighted(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue) {
    KBFloat v, a, b;
    memcpy(&a, k1->value, k1->len);
    memcpy(&b, k2->value, k2->len);
    
    v = KBFloatWerp(a, b, t, k1->post.x, k1->post.y, k2->pre.x, k2->pre.y);
    
    memcpy(*interpValue, &v, k1->len);
}

void
KBInterpolateVec2Weighted(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue) {
    KBVec2 v, a, b;
    memcpy(&a, k1->value, k1->len);
    memcpy(&b, k2->value, k2->len);
    
    v = KBVec2Werp(a, b, t, k1->post.x, k1->post.y, k2->pre.x, k2->pre.y);
    
    memcpy(*interpValue, &v, k1->len);
}

void
KBInterpolateVec3Weighted(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue) {
    KBVec3 v, a, b;
    memcpy(&a, k1->value, k1->len);
    memcpy(&b, k2->value, k2->len);
    
    v = KBVec3Werp(a, b, t, k1->post.x, k1->post.y, k2->pre.x, k2->pre.y);
    
    memcpy(*interpValue, &v, k1->len);
}

void
KBInterpolateVec4Weighted(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue) {
    KBVec4 v, a, b;
    memcpy(&a, k1->value, k1->len);
    memcpy(&b, k2->value, k2->len);
    
    v = KBVec4Werp(a, b, t, k1->post.x, k1->post.y, k2->pre.x, k2->pre.y);
    
    memcpy(*interpValue, &v, k1->len);
}

/* linear */
void
KBInterpolateFloatLinear(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue) {
    KBFloat v, a, b;
    memcpy(&a, k1->value, k1->len);
    memcpy(&b, k2->value, k2->len);
    
    v = KBFloatWerp(a, b, t, k1->post.x, k1->post.y, k2->pre.x, k2->pre.y);
    
    memcpy(*interpValue, &v, k1->len);
}

void
KBInterpolateVec2Linear(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue) {
    KBVec2 v, a, b;
    memcpy(&a, k1->value, k1->len);
    memcpy(&b, k2->value, k2->len);
    
    v = KBVec2Lerp(a, b, t);
    
    memcpy(*interpValue, &v, k1->len);
}

void
KBInterpolateVec3Linear(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue) {
    KBVec3 v, a, b;
    memcpy(&a, k1->value, k1->len);
    memcpy(&b, k2->value, k2->len);
    
    v = KBVec3Lerp(a, b, t);
    
    memcpy(*interpValue, &v, k1->len);
}

void
KBInterpolateVec4Linear(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue) {
    KBVec4 v, a, b;
    memcpy(&a, k1->value, k1->len);
    memcpy(&b, k2->value, k2->len);
    
    v = KBVec4Lerp(a, b, t);
    
    memcpy(*interpValue, &v, k1->len);
}

#pragma mark - Animation
KBBool
KBKeyAnimationStep(KBType *animationObj, KBFloat dt) {
    KBKeyAnimation *animation = animationObj;
    
    KBFloat oldTime, time;
    oldTime = animation->time;
    time = oldTime + dt;
    
    if (time>=0.f) {
    
        if (oldTime <= 0.f && animation->repetition==0) {
            animation->repetition = 1;
            KBAnimationStart(animation);
        }
        
        /* what keyframes should we use? */
        
        /* if we are outside the range of times clamp to outside values */
        KBKeyframe **kf = (KBKeyframe**)animation->keyframes->arr;
        int count = animation->keyframes->num;
        
        if (time <= kf[0]->time) {
            animation->setFunc(KBAnimationCast(animation)->node, kf[0]->value);
            return KBTrue;
        } else if (count>1 && time >= kf[count-1]->time) {
            
            if (animation->repeatCount==-1 || animation->repetition-1 <= animation->repeatCount) {
                time = animation->repeatTime;
                animation->repetition++;
            
            } else {
                
                if (!animation->finished) {
                    animation->finished = KBTrue;
                    animation->setFunc(KBAnimationCast(animation)->node, kf[count-1]->value);
                }
                return KBFalse;
            }
        }
        
        /* find keyframes i and j */
        int i=1;
        while (time > kf[i]->time) ++i;
        int j=i-1;
        
        KBFloat spread = kf[i]->time-kf[j]->time;
        KBFloat t = (time - kf[j]->time)/spread;
        
        void *value = malloc(kf[i]->len);
        
        /* interpolate a value */
        animation->interpFunc(kf[j], kf[i], t, &value);
        animation->setFunc(KBAnimationCast(animation)->node, value);
        
        free(value);
    }
    
    animation->time = time;
        
    
    return KBTrue;
}


void
KBKeyAnimationStart(KBType *animationObj, void *eventData) {
    KBKeyAnimation *animation = animationObj;
    animation->repetition++;
    if (animation->userStartFunc) {
        animation->userStartFunc(animation, eventData);
    }
}

KBBool
KBKeyAnimationEqual(KBType *animObj1, KBType *animObj2) {
    KBKeyAnimation *anim1 = animObj1;
    KBKeyAnimation *anim2 = animObj2;
    
    return anim1->setFunc == anim2->setFunc;
}



KBKeyAnimation *
KBKeyAnimationInit(KBKeyAnimation               *animation,
                   KBDestroyFunc                 destroyFunc,
                   int                           name,
                   KBAnimationEventFunc          startFunc,
                   KBAnimationEventFunc          stopFunc,
                   void                          *eventData,
                   KBKeyAnimationInterpolateFunc interpFunc,
                   KBKeyAnimationSetValueFunc    setFunc,
                   KBKeyframe                    **keyframes,
                   int                           count)

{
    KBAnimationInit((KBAnimation*)animation, destroyFunc, name, &KBKeyAnimationStep, &KBKeyAnimationStart, stopFunc, eventData);
    
    KBAnimationCast(animation)->equalFunc = &KBKeyAnimationEqual;
    animation->userStartFunc = startFunc;
    animation->interpFunc = interpFunc;
    animation->setFunc = setFunc;
    
    animation->keyframes = KBArrayNewWithObjects((KBType **)keyframes, count);
    animation->time = 0.f;
    
    animation->repeatCount = 0;
    animation->repetition = 0;
    animation->finished = KBFalse;
    
    return animation;
}


KBKeyAnimation *
KBKeyAnimationNew(int                           name,
                  KBAnimationEventFunc          startFunc,
                  KBAnimationEventFunc          stopFunc,
                  void                          *eventData,
                  KBKeyAnimationInterpolateFunc interpFunc,
                  KBKeyAnimationSetValueFunc    setFunc,
                  KBKeyframe                    **keyframes,
                  int                           count)

{
    
    return KBKeyAnimationInit(KBAlloc(sizeof(KBKeyAnimation)),
                              &KBKeyAnimationDestroy,
                              name,
                              startFunc,
                              stopFunc,
                              eventData,
                              interpFunc,
                              setFunc,
                              keyframes,
                              count);
}

void
KBKeyAnimationDestroy(KBType *obj) {
    KBKeyAnimation *animation = obj;
    KBRelease(animation->keyframes);
    
    KBAnimationDestroy(obj);
}

