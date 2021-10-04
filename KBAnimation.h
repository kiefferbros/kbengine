//
//  KBAnimation.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/20/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBAnimation_h
#define KBAnimation_h


#include "KBTypes.h"

typedef KBBool (*KBAnimationEqualFunc)(KBType *animObj1, KBType *animObj2);
typedef KBBool (*KBAnimationStepFunc)(KBType *animObj, KBFloat dt);
typedef void (*KBAnimationEventFunc)(KBType *animObj, void *userData);

typedef struct _KBAnimation {
    KBObject                    super;
    
    int                         name;
    
    KBAnimationEqualFunc        equalFunc;
    
    KBAnimationStepFunc         stepFunc;
    KBAnimationEventFunc        startFunc;
    KBAnimationEventFunc        stopFunc;
    void                        *eventData;
    
    KBType                      *node;
    KBBool                      stopped;
} KBAnimation;

#define KBAnimationCast(obj)((KBAnimation*)obj)

KBAnimation * KBAnimationInit(KBAnimation                   *animation,
                              KBDestroyFunc                 destroyFunc,
                              int                           name,
                              KBAnimationStepFunc           stepFunc,
                              KBAnimationEventFunc          startFunc,
                              KBAnimationEventFunc          stopFunc,
                              void                          *eventData);

KBAnimation * KBAnimationNew(int                           name,
                             KBAnimationStepFunc           stepFunc,
                             KBAnimationEventFunc          startFunc,
                             KBAnimationEventFunc          stopFunc,
                             void                          *eventData);

void KBAnimationDestroy(KBType *animationObj);

void KBAnimationStart(KBType *animationObj);
void KBAnimationStop(KBType *animationObj);

KBBool KBAnimationStep(KBType *animationObj, KBFloat dt);

KBBool KBAnimationEqual(KBType *animObj1, KBType *animObj2);

KBType * KBAnimationGetNode(KBType *animObj);

#endif
