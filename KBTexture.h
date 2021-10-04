//
//  KBTexture.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/20/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBTexture_h
#define KBTexture_h

#include "KBTypes.h"



enum {
    KBTextureOptionsDefault = 0,  // clamp s to edge, clamp t to edge, min filter linear, mag filter linear, no mipmap generation
    KBTextureOptionRepeatS = 1,
    KBTextureOptionRepeatT = 2,
    KBTextureOptionMinFilterNearest = 4,
    KBTextureOptionMagFilterNearest = 8,
    KBTextureOptionGenerateMipmap = 16
};

typedef enum {
	KBTexturePixelFormatRGBA = 0,
    KBTexturePixelFormatRGB,
    KBTexturePixelFormatLuminance,
    KBTexturePixelFormatLuminanceAlpha,
	KBTexturePixelFormatPVRTC2 = 24,
	KBTexturePixelFormatPVRTC4
} KBTexturePixelFormat;

typedef struct _KBTexture {
    KBObject            super;
    
    GLuint              name;
    GLuint              pixelWidth;
    GLuint              pixelHeight;
    KBVec2              max;
    KBVec2              size;
    KBFloat             scale;
} KBTexture;

#define KBTextureCast(obj)((KBTexture*)obj)

KBTexture * KBTextureInit(KBTexture *texture,
                          KBDestroyFunc destroyFunc,
                          const void *data,
                          int format,
                          GLuint pixelWidth,
                          GLuint pixelHeight,
                          KBVec2 contentSize,
                          KBFloat scale,
                          KBMask options);

KBTexture * KBTextureNew(const void *data,
                         int format,
                         GLuint pixelWidth,
                         GLuint pixelHeight,
                         KBVec2 contentSize,
                         KBFloat scale,
                         KBMask options);

KBTexture * KBTextureInitPNG(KBTexture *texture, KBDestroyFunc destroyFunc,const char *path, KBFloat scale, KBMask options);

KBTexture * KBTextureNewPNG(const char *path, KBFloat scale, KBMask options);

void KBTextureDestroy(KBType *texture);

#endif
