//
//  KBKeyAnimation.h
//  KBEngine
//
//  Created by KB Jonathan on 1/13/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBKeyAnimation_h
#define KBKeyAnimation_h

#include "KBAnimation.h"
#include "KBArray.h"
#include "KBKeyframe.h"

#pragma mark - Interpolations Functions
typedef void (*KBKeyAnimationInterpolateFunc)(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue);
typedef void (*KBKeyAnimationSetValueFunc)(KBType *node, void *value);

/* stepped */
void KBInterpolateStepped(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue);

/* weighted */
void KBInterpolateFloatWeighted(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue);
void KBInterpolateVec2Weighted(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue);
void KBInterpolateVec3Weighted(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue);
void KBInterpolateVec4Weighted(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue);

/* linear */
void KBInterpolateFloatLinear(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue);
void KBInterpolateVec2Linear(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue);
void KBInterpolateVec3Linear(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue);
void KBInterpolateVec4Linear(KBKeyframe *k1, KBKeyframe *k2, KBFloat t, void **interpValue);

typedef struct _KBKeyAnimation {
    KBAnimation                     super;
    
    KBAnimationEventFunc            userStartFunc; /* other start function is overidden */

    KBKeyAnimationInterpolateFunc   interpFunc;
    KBKeyAnimationSetValueFunc      setFunc;
    
    KBArray                         *keyframes;
    KBFloat                         time;
    KBFloat                         repeatTime;
    
    int                             repeatCount;
    int                             repetition;
    KBBool                          finished;
} KBKeyAnimation;

#define KBKeyAnimationCast(obj)((KBKeyAnimation*)obj)

KBKeyAnimation * KBKeyAnimationInit(KBKeyAnimation                   *animation,
                                    KBDestroyFunc                   destroyFunc,
                                    int                             name,
                                    KBAnimationEventFunc            startFunc,
                                    KBAnimationEventFunc            stopFunc,
                                    void                            *eventData,
                                    KBKeyAnimationInterpolateFunc   interpFunc,
                                    KBKeyAnimationSetValueFunc      setFunc,
                                    KBKeyframe                      **keyframes,
                                    int                             count);

KBKeyAnimation * KBKeyAnimationNew(int                              name,
                                   KBAnimationEventFunc             startFunc,
                                   KBAnimationEventFunc             stopFunc,
                                   void                             *eventData,
                                   KBKeyAnimationInterpolateFunc    interpFunc,
                                   KBKeyAnimationSetValueFunc       setFunc,
                                   KBKeyframe                       **keyframes,
                                   int                              count);

void KBKeyAnimationDestroy(KBType *animationObj);

KBBool KBKeyAnimationStep(KBType *animationObj, KBFloat dt);

#endif
