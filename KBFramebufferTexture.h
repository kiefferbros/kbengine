//
//  KBFramebufferTexture.h
//  KBEngine
//
//  Created by KB Jonathan on 1/12/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBFramebufferTexture_h
#define KBFramebufferTexture_h

#include "KBTexture.h"

typedef struct _KBFramebufferTexture {
    KBTexture       super;
    
    GLuint          framebuffer;
} KBFramebufferTexture;

#define KBFramebufferTextureCast(obj)((KBFramebufferTexture*)obj)

KBFramebufferTexture * KBFramebufferTextureInit(KBFramebufferTexture *tex, KBDestroyFunc destroyFunc, KBVec2 size, KBFloat scale);
KBFramebufferTexture * KBFramebufferTextureNew(KBVec2 size, KBFloat scale);
void KBFramebufferTextureDestroy(KBType *texObj);

#endif
