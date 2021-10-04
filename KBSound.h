//
//  KBSound.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/31/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

/* an andriod implementation is required */ 

#ifndef KBSound_h
#define KBSound_h

#include "KBTypes.h"

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>
#endif


typedef struct _KBSoundBuffer {
    KBObject        super;
    
#if __APPLE__
    ALuint          name;
#else
    unsigned int    name;
#endif
} KBSoundBuffer;

#define KBSoundBufferCast(obj)((KBSoundBuffer*)obj)

KBSoundBuffer * KBSoundBufferInit(KBSoundBuffer *buffer,  KBDestroyFunc destroyFunc, void *data, long int dataSize, int format, int sampleRate);
KBSoundBuffer * KBSoundBufferNew(void *data, long int dataSize, int format, int sampleRate);
void KBSoundBufferDestroy(KBType *bufferObj);

KBSoundBuffer * KBSoundBufferInitFile(KBSoundBuffer *buffer,  KBDestroyFunc destroyFunc,  const char *path);
KBSoundBuffer * KBSoundBufferNewFile(const char *path);


typedef struct _KBSound {
    KBObject        super;
    
#if __APPLE__
    ALuint          name;
#else
    unsigned int    name;
#endif
    
    KBSoundBuffer   *buffer;
} KBSound;

#define KBSoundCast(obj)((KBSound*)obj)

KBSound *KBSoundInit(KBSound *sound, KBDestroyFunc destroyFunc, KBSoundBuffer *buffer);
KBSound *KBSoundNew(KBSoundBuffer *buffer);
void KBSoundDestroy(KBType *soundObj);

void KBSoundPlay(KBSound *sound);
void KBSoundStop(KBSound *sound);
void KBSoundPause(KBSound *sound);

void KBSoundSetLoops(KBSound *sound, KBBool flag);
void KBSoundSetGain(KBSound *sound, KBFloat gain);
KBFloat KBSoundGetGain(KBSound *sound);
void KBSoundSetPitch(KBSound *sound, KBFloat pitch);

KBBool KBSoundGetPlaying(KBSound *sound);

typedef struct _KBMultiSound {
    KBObject        super;
#if __APPLE__
    ALuint          name;
#else
    unsigned int    name;
#endif
    
    KBSoundBuffer   *loopBuffer;
    KBSoundBuffer   *attackBuffer;
    KBSound         *releaseSound;
} KBMultiSound;

#define KBMultiSoundCast(obj)((KBMultiSound*)obj)

KBMultiSound *KBMultiSoundInit(KBMultiSound *sound, KBDestroyFunc destroyFunc, KBSoundBuffer *attackBuffer, KBSoundBuffer *loopBuffer, KBSoundBuffer *releaseBuffer);
KBMultiSound *KBMultiSoundNew(KBSoundBuffer *attackBuffer, KBSoundBuffer *loopBuffer, KBSoundBuffer *releaseBuffer);
void KBMultiSoundDestroy(KBType *soundObj);

void KBMultiSoundPlay(KBMultiSound *sound);
void KBMultiSoundStop(KBMultiSound *sound);
#endif


