//
//  KBTouch.h
//  KBEngine
//
//  Created by KB Jonathan on 5/2/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBInput_h
#define KBInput_h

#include "KBTypes.h"


enum {
    KBInputTouch,
};

typedef struct _KBInput {
    KBObject        super;
    
    int             type;
    KBVec2          pos, pPos;
    double          time, pTime;
    
    void            *data;          // a platform object 
} KBInput;

#define KBInputCast(obj)((KBInput*)obj)

KBInput * KBInputInitTouch(KBInput *touch, KBDestroyFunc destroyFunc, KBVec2 pos, double timestamp, void *data);
KBInput * KBInputNewTouch(KBVec2 pos, double timestamp, void *data);
void KBInputUpdateTouch(KBInput *input, KBVec2 pos, double timestamp);

KBBool KBInputEqualsPlatform(KBInput *input, void *data);



#endif
