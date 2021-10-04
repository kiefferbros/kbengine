//
//  KBKeyframe.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/27/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#include "KBKeyframe.h"

#include <string.h>
#include <stdlib.h>

KBKeyframe *
KBKeyframeInit(KBKeyframe *keyframe,
               KBDestroyFunc destroyFunc,
               void *value,
               unsigned long valueLen,
               KBBool retain,
               KBFloat time,
               KBVec2 preWeight,
               KBVec2 postWeight)
{
    KBObjectInit(keyframe, destroyFunc);
    
    keyframe->len = valueLen;
    
    if (retain) {
        keyframe->value = KBRetain(value);
    } else {
        keyframe->value = malloc(valueLen);
        memcpy(keyframe->value, value, valueLen);
    }
    
    keyframe->retainValue = retain;
    keyframe->time = time;
    keyframe->pre = preWeight;
    keyframe->post = postWeight; 
    
    return keyframe;
}

KBKeyframe *
KBKeyframeNew(void *value, unsigned long valueLen, KBBool retain, KBFloat time, KBVec2 preWeight, KBVec2 postWeight) {
    return KBKeyframeInit(KBAlloc(sizeof(KBKeyframe)), &KBKeyframeDestroy, value, valueLen, retain, time, preWeight, postWeight);
}

void
KBKeyframeDestroy(KBType *obj) {
    KBKeyframe *keyframe = obj;
    if (keyframe->retainValue) {
        KBRelease(keyframe->value);
    } else {
        free(keyframe->value);
    }
}