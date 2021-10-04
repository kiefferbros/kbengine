//
//  KBTouch.c
//  Star Hound
//
//  Created by KB Jonathan on 5/2/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBInput.h"
#include <stdio.h>


KBInput *
KBInputInitTouch(KBInput *touch, KBDestroyFunc destroyFunc, KBVec2 pos, double timestamp, void *data) {
    
    if ((touch = KBObjectInit(touch, destroyFunc))) {
        touch->type = KBInputTouch;
        touch->pos = pos;
        touch->pPos = pos;
        touch->time = timestamp;
        touch->pTime = timestamp;
        touch->data = data;
    }
    
    return touch;
}

KBInput *
KBInputNewTouch(KBVec2 pos, double timestamp, void *data) {
    return KBInputInitTouch(KBAlloc(sizeof(KBInput)), NULL, pos, timestamp, data);
}

void
KBInputUpdateTouch(KBInput *input, KBVec2 pos, double timestamp) {
    input->pPos = input->pos;
    input->pTime = input->time;
    
    input->pos = pos;
    input->time = timestamp;
}

KBBool
KBInputEqualsPlatform(KBInput *input, void *data) {
    return input->data == data;
}
