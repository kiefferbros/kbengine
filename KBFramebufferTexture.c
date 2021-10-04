//
//  KBFramebufferTexture.c
//  KBEngine
//
//  Created by KB Jonathan on 1/12/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBFramebufferTexture.h"
#include <stdio.h>
#include <stdlib.h>
#include "KBEngine.h"

KBFramebufferTexture *
KBFramebufferTextureInit(KBFramebufferTexture *tex, KBDestroyFunc destroyFunc, KBVec2 size, KBFloat scale) {
    
    GLuint width, height;
    size = KBVec2Scale(size, scale);
    width = KBNextPOT(size.w);
	height = KBNextPOT(size.h);

    KBTextureInit((KBTexture*)tex,
                  destroyFunc,
                  NULL,
                  KBTexturePixelFormatRGBA,
                  width,
                  height,
                  size,
                  scale,
                  KBTextureOptionMinFilterNearest|KBTextureOptionMagFilterNearest);

    /*
    tex->texture->size = size;
    tex->texture->scale = scale;
    tex->texture->pixelWidth = width;
    tex->texture->pixelHeight = height;
    tex->texture->max = KBVec2Div(KBVec2Scale(size,scale), KBVec2Make(width, height));
     */
    
    /* create the gl texture */
    /*
    glGenTextures(1, &tex->texture->name);
    glBindTexture(GL_TEXTURE_2D, tex->texture->name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
     */
    
    /* save the current frame buffer */
    GLuint saveBuffer;
    KB_glGet_FRAMEBUFFER(&saveBuffer);
    
    /* create the framebuffer */
    KB_glGenFramebuffer(&tex->framebuffer);
    KB_glBindFramebuffer(tex->framebuffer);
    KB_glFramebufferTexture2D_COLOR(KBTextureCast(tex)->name);
    KB_glBindFramebuffer(saveBuffer);
    
    return tex;
}

KBFramebufferTexture *
KBFramebufferTextureNew(KBVec2 size, KBFloat scale) {
    return KBFramebufferTextureInit(KBAlloc(sizeof(KBFramebufferTexture)), &KBFramebufferTextureDestroy, size, scale);
}

void
KBFramebufferTextureDestroy(KBType *texObj) {
    KBFramebufferTexture * tex = texObj;
    KB_glDeleteFramebuffer(tex->framebuffer);
    
    KBTextureDestroy(texObj);
}