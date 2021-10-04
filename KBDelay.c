//
//  KBDelay.c
//  KBEngine
//
//  Created by KB Jonathan on 7/15/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBDelay.h"
#include <stdio.h>

KBBool
KBDelayStep(KBType *animObj, KBFloat dt) {
    
    KBDelay *delay = animObj;
    delay->time += dt;
    return delay->time<delay->duration;
}

KBDelay * KBDelayInit(KBDelay *delay, KBDestroyFunc destroyFunc, int name, KBFloat duration, KBAnimationEventFunc endDelay, void *data) {
    if ((delay = (KBDelay*)KBAnimationInit((KBAnimation*)delay, destroyFunc, name, &KBDelayStep, NULL, endDelay, data))) {
        delay->duration = duration;
    }
    return delay;
}

KBDelay * KBDelayNew(int name, KBFloat duration, KBAnimationEventFunc endDelay, void *data) {
    return KBDelayInit(KBAlloc(sizeof(KBDelay)), &KBAnimationDestroy, name, duration, endDelay, data);
}