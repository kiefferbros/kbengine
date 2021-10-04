//
//  KBKeyframe.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/27/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBKeyframe_h
#define KBKeyframe_h

#include "KBGeometry.h"

typedef struct _KBKeyframe {
    KBObject        super;
    
    void            *value;
    unsigned long   len;
    KBBool          retainValue;
    KBFloat         time;
    KBVec2          pre;
    KBVec2          post;
} KBKeyframe;

#define KBKeyframeCast(obj)((KBKeyframe*)obj)

KBKeyframe * KBKeyframeInit(KBKeyframe *keyframe, KBDestroyFunc destroyFunc, void *value, unsigned long valueLen, KBBool retain, KBFloat time, KBVec2 preWeight, KBVec2 postWeight);
KBKeyframe * KBKeyframeNew(void *value, unsigned long valueLen, KBBool retain, KBFloat time, KBVec2 preWeight, KBVec2 postWeight);
void KBKeyframeDestroy(KBType *keyframe);


/* convenience creators */
static inline KBKeyframe *
KBKeyframeFloat(KBFloat value, KBFloat time) {
    KBKeyframe * kf = KBKeyframeNew(&value, sizeof(KBFloat), KBFalse, time, KBVec2Zero, KBVec2Zero);
    return kf;
}

static inline KBKeyframe *
KBKeyframeVec2(KBVec2 value, KBFloat time) {
    KBVec2 v = value;
    KBKeyframe * kf = KBKeyframeNew(&v, sizeof(KBVec2), KBFalse, time, KBVec2Zero, KBVec2Zero);
    return kf;
}

static inline KBKeyframe *
KBKeyframeVec3(KBVec3 value, KBFloat time) {
    KBKeyframe * kf = KBKeyframeNew(&value, sizeof(KBVec3), KBFalse, time, KBVec2Zero, KBVec2Zero);
    return kf;
}

static inline KBKeyframe *
KBKeyframeVec4(KBVec4 value, KBFloat time) {
    KBKeyframe * kf = KBKeyframeNew(&value, sizeof(KBVec4), KBFalse, time, KBVec2Zero, KBVec2Zero);
    return kf;
}

static inline KBKeyframe *
KBKeyframeFloatWeight(KBFloat value, KBFloat time, KBVec2 preWeight, KBVec2 postWeight) {
    KBKeyframe * kf = KBKeyframeNew(&value, sizeof(KBFloat), KBFalse, time, preWeight, postWeight);
    return kf;
}

static inline KBKeyframe *
KBKeyframeVec2Weight(KBVec2 value, KBFloat time, KBVec2 preWeight, KBVec2 postWeight) {
    KBKeyframe * kf = KBKeyframeNew(&value, sizeof(KBVec2), KBFalse, time, preWeight, postWeight);
    return kf;
}

static inline KBKeyframe *
KBKeyframeVec3Weight(KBVec3 value, KBFloat time, KBVec2 preWeight, KBVec2 postWeight) {
    KBKeyframe * kf = KBKeyframeNew(&value, sizeof(KBVec3), KBFalse, time, preWeight, postWeight);
    return kf;
}

static inline KBKeyframe *
KBKeyframeVec4Weight(KBVec4 value, KBFloat time, KBVec2 preWeight, KBVec2 postWeight) {
    KBKeyframe * kf = KBKeyframeNew(&value, sizeof(KBVec4), KBFalse, time, preWeight, postWeight);
    return kf;
}


#endif
