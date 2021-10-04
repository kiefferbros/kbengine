//
//  KBDelay.h
//  KBEngine
//
//  Created by KB Jonathan on 7/15/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBDelay_h
#define KBDelay_h

#include "KBAnimation.h"

typedef struct _KBDelay {
    KBAnimation super;
    KBFloat time, duration;
} KBDelay;

KBDelay * KBDelayInit(KBDelay *delay, KBDestroyFunc destroyFunc, int name, KBFloat duration, KBAnimationEventFunc endDelay, void *data);
KBDelay * KBDelayNew(int name, KBFloat duration, KBAnimationEventFunc endDelay, void *data);


#endif
