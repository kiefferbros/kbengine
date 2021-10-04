//
//  KBTexture.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/20/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#include "KBTexture.h"
#include "KBGeometry.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
/* coregraphics is required for the apple png implementation */
#include <CoreGraphics/CoreGraphics.h>
#else
/* android */
#include <android/bitmap.h>
#endif

KBTexture *
KBTextureInit(KBTexture *texture,
              KBDestroyFunc destroyFunc,
              const void *data,
              int format,
              GLuint pixelWidth,
              GLuint pixelHeight,
              KBVec2 contentSize,
              KBFloat scale,
              KBMask options)
{
    KBObjectInit(texture, destroyFunc);
    
    GLenum					err;
	
    glGetError(); // clear any opengl errors
    glGenTextures(1, &texture->name);
    glBindTexture(GL_TEXTURE_2D, texture->name);
    
    if ((options&KBTextureOptionGenerateMipmap)!=0) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (options&KBTextureOptionMinFilterNearest)!=0 ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (options&KBTextureOptionMinFilterNearest)!=0 ? GL_NEAREST : GL_LINEAR);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (options&KBTextureOptionMinFilterNearest)!=0 ? GL_NEAREST : GL_LINEAR);
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (options&KBTextureOptionRepeatS)!=0 ? GL_REPEAT : GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (options&KBTextureOptionRepeatT)!=0 ? GL_REPEAT : GL_CLAMP_TO_EDGE);
    
    switch(format) {
        case KBTexturePixelFormatRGBA:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pixelWidth, pixelHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        case KBTexturePixelFormatRGB:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pixelWidth, pixelHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case KBTexturePixelFormatLuminance:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, pixelWidth, pixelHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
            break;
        case KBTexturePixelFormatLuminanceAlpha:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, pixelWidth, pixelHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
            break;
    }
    
    err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("Error uploading texture glError: 0x%04X\n", err);
        glBindTexture(GL_TEXTURE_2D, 0);
        KBRelease(texture);
        return NULL;
    }
    
    
    if ((options&KBTextureOptionGenerateMipmap)!=0) {
#if GL_VERSION_ES_CM_1_1
        glGenerateMipmapOES(GL_TEXTURE_2D);
#else
        glGenerateMipmap(GL_TEXTURE_2D);
#endif
    }
    
    texture->pixelWidth = pixelWidth;
    texture->pixelHeight = pixelHeight;
    texture->max.s = contentSize.x / (float)pixelWidth;
    texture->max.t = contentSize.y / (float)pixelHeight;
    texture->scale = scale;
    texture->size = KBVec2DivScale(contentSize, scale);
    
    return texture;
}

KBTexture *
KBTextureNew(const void *data, int format, GLuint pixelWidth, GLuint pixelHeight, KBVec2 contentSize, KBFloat scale, KBMask options) {
    return KBTextureInit(KBAlloc(sizeof(KBTexture)), &KBTextureDestroy, data, format, pixelWidth, pixelHeight, contentSize, scale, options);
}


KBTexture *
KBTextureInitPNG(KBTexture *texture, KBDestroyFunc destroyFunc,const char *path, KBFloat scale, KBMask options) {
#ifdef __APPLE__ 
    GLsizei                 width, height;
	CGContextRef			context = NULL;
	void*					data = NULL;
	CGColorSpaceRef			colorSpace;
	CGSize					imageSize;
    CGImageRef              image;
	CGDataProviderRef       provider;
    
    provider = CGDataProviderCreateWithFilename(path);
	image = CGImageCreateWithPNGDataProvider(provider, NULL, FALSE, kCGRenderingIntentDefault);
    CGDataProviderRelease(provider);
    
    
	if(image == NULL) {
		printf("CGImage is Null: %s\n", path);
        free(texture); /* just call free not KBRelease because the object has not been initialized yet */
		return NULL;
	}
    
	imageSize = CGSizeMake(CGImageGetWidth(image), CGImageGetHeight(image));
	
	width = KBNextPOT(imageSize.width);
	height = KBNextPOT(imageSize.height);
    
    colorSpace = CGColorSpaceCreateDeviceRGB();
    data = calloc(height, width*4);
    context = CGBitmapContextCreate(data, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
	
	CGContextTranslateCTM(context, 0, height);
	CGContextScaleCTM(context, 1.0, -1.0);
	
	CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(image), CGImageGetHeight(image)), image);
    CGContextRelease(context);
    CGImageRelease(image);
    
    
    KBTextureInit(texture, destroyFunc, data, KBTexturePixelFormatRGBA, width, height, KBVec2Make(imageSize.width, imageSize.height), scale, options);
    free(data);
#else
    /*android*/
    
#endif
    
    return texture;
}



KBTexture * KBTextureNewPNG(const char *path, KBFloat scale, KBMask options) {
    return KBTextureInitPNG(KBAlloc(sizeof(KBTexture)), &KBTextureDestroy, path, scale, options);
}





void KBTextureDestroy(KBType *obj) {
    KBTexture *texture = obj;
    glDeleteTextures(1, &texture->name);
}



