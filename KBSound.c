//
//  KBSound.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/31/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#include "KBSound.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma mark - Buffer Object

KBSoundBuffer *
KBSoundBufferInit(KBSoundBuffer *sound, KBDestroyFunc destroyFunc, void *data, long int dataSize, int format, int sampleRate) {
    KBObjectInit(sound, destroyFunc);
    
    /* create a buffer */
#ifdef __APPLE__
    alGenBuffers(1, &sound->name);
    alBufferData(sound->name, format, data, (ALsizei)dataSize, sampleRate);
#endif  
    
    return sound;
}

KBSoundBuffer *
KBSoundBufferNew(void *data, long int dataSize, int format, int sampleRate) {
    return KBSoundBufferInit(KBAlloc(sizeof(KBSoundBuffer)), &KBSoundBufferDestroy, data, dataSize, format, sampleRate);
}

void
KBSoundBufferDestroy(KBType *soundObj) {
    KBSoundBuffer *sound = soundObj;
    if (sound->name) {
#ifdef __APPLE__
        alDeleteBuffers(1, &sound->name);
#endif
    }
}

#ifdef __APPLE__
static ALenum
GetALFormat(AudioStreamBasicDescription inFileFormat)
{
	if (inFileFormat.mFormatID != kAudioFormatLinearPCM)
		return AL_INVALID_VALUE;
	
	if ((inFileFormat.mChannelsPerFrame > 2) || (inFileFormat.mChannelsPerFrame < 1))
		return AL_INVALID_VALUE;
	
	if(inFileFormat.mBitsPerChannel == 8)
		return (inFileFormat.mChannelsPerFrame == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
	else if(inFileFormat.mBitsPerChannel == 16)
		return (inFileFormat.mChannelsPerFrame == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	
	return AL_INVALID_VALUE;
}
#endif

KBSoundBuffer *
KBSoundBufferInitFile(KBSoundBuffer *sound,  KBDestroyFunc destroyFunc, const char *path) {
#ifdef __APPLE__
    AudioFileID fileID;
    AudioStreamBasicDescription format;
    UInt64 fileSize = 0;
    UInt32 dataSize = 0;
    void *data = NULL;
    
    /* create a url object to open */
    CFURLRef theURL = CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault, (UInt8*)path, strlen(path), false);

    /* open the audio file object */
    AudioFileOpenURL(theURL, kAudioFileReadPermission, 0, &fileID);
    CFRelease(theURL);
    
    /* get the format of the audio file */
    UInt32 thePropSize = sizeof(format);
    AudioFileGetProperty(fileID, kAudioFilePropertyDataFormat, &thePropSize, &format);
    
    /* get the data size of the audio file */
    thePropSize = sizeof(UInt64);
    AudioFileGetProperty(fileID, kAudioFilePropertyAudioDataByteCount, &thePropSize, &fileSize);
    
    /* make room for the audio data */
    dataSize = (UInt32)fileSize;
    data = malloc(dataSize);
    
    /* read the audio file */
    AudioFileReadBytes(fileID, false, 0, &dataSize, data);

    KBSoundBufferInit(sound, destroyFunc, data, dataSize, GetALFormat(format), format.mSampleRate);
    
    /* free the data */
    free(data);
    /* close the audio file */
    AudioFileClose(fileID);
#endif
    
    return sound;
    
}

KBSoundBuffer *
KBSoundBufferNewFile(const char *path) {
    return KBSoundBufferInitFile(KBAlloc(sizeof(KBSoundBuffer)), &KBSoundBufferDestroy, path);
}

#pragma mark - Source Object
KBSound *
KBSoundInit(KBSound *sound, KBDestroyFunc destroyFunc, KBSoundBuffer *buffer) {
    KBObjectInit(sound, destroyFunc);
    
    sound->buffer = KBRetain(buffer);
#ifdef __APPLE__    
    alGenSources(1, &sound->name);
    alSourcei(sound->name, AL_BUFFER, buffer->name);
#endif
    
    return  sound;
}

KBSound *
KBSoundNew(KBSoundBuffer *buffer) {
    return KBSoundInit(KBAlloc(sizeof(KBSound)), &KBSoundDestroy, buffer);
}

void
KBSoundDestroy(KBType *soundObj) {
    KBSound *sound = soundObj;
    
    KBRelease(sound->buffer);
    
    if (sound->name) {
#ifdef __APPLE__
        alDeleteSources(1, &sound->name);
#endif
    }
    
}

void
KBSoundPlay(KBSound *sound) {
#ifdef __APPLE__
    alSourcePlay(sound->name);
#endif
}

void
KBSoundStop(KBSound *sound) {
#ifdef __APPLE__
    alSourceStop(sound->name);
#endif
}

void
KBSoundPause(KBSound *sound) {
#ifdef __APPLE__
    alSourcePause(sound->name);
#endif
}

void
KBSoundSetLoops(KBSound *sound, KBBool flag) {
#ifdef __APPLE__
    alSourcei(sound->name, AL_LOOPING, (ALint)flag);
#endif
}

void
KBSoundSetGain(KBSound *sound, KBFloat gain) {
#ifdef __APPLE__
    alSourcef(sound->name, AL_GAIN, gain);
#endif
}

KBFloat
KBSoundGetGain(KBSound *sound) {
#ifdef __APPLE__
    ALfloat gain;
    alGetSourcef(sound->name, AL_GAIN, &gain);
    return gain;
#else
    return 0.f;
#endif
    
}

void
KBSoundSetPitch(KBSound *sound, KBFloat pitch) {
#ifdef __APPLE__
    alSourcef(sound->name, AL_PITCH, pitch);
#endif
}

KBBool KBSoundGetPlaying(KBSound *sound) {
#ifdef __APPLE__
    ALint state;
    alGetSourcei(sound->name, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
#else
    return KBFalse;
#endif
}

#pragma mark - Multi Buffer Source Object
KBMultiSound *
KBMultiSoundInit(KBMultiSound *sound, KBDestroyFunc destroyFunc, KBSoundBuffer *attackBuffer, KBSoundBuffer *loopBuffer, KBSoundBuffer *releaseBuffer) {
    KBObjectInit(sound, destroyFunc);
    
    sound->attackBuffer = KBRetain(attackBuffer);
    sound->loopBuffer = KBRetain(loopBuffer);
    sound->releaseSound = KBSoundNew(releaseBuffer);
    
#ifdef __APPLE__
    alGenSources(1, &sound->name);
    
    ALuint buffers[64] = {
        attackBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name,
        loopBuffer->name, loopBuffer->name, loopBuffer->name,loopBuffer->name, loopBuffer->name, loopBuffer->name,
        loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name,
        loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name,
        loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name,
        loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name,
        loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name,
        loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name,
        loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name,
        loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name,
        loopBuffer->name, loopBuffer->name, loopBuffer->name, loopBuffer->name
    };
    
    alSourceQueueBuffers(sound->name, 64, buffers);
#endif
    
    return sound;
    
}

KBMultiSound *
KBMultiSoundNew(KBSoundBuffer *attackBuffer, KBSoundBuffer *loopBuffer, KBSoundBuffer *releaseBuffer) {
    return KBMultiSoundInit(KBAlloc(sizeof(KBMultiSound)), &KBMultiSoundDestroy, attackBuffer, loopBuffer, releaseBuffer);
}

void
KBMultiSoundDestroy(KBType *soundObj) {
    KBMultiSound *sound = soundObj;
    
    KBRelease(sound->attackBuffer);
    KBRelease(sound->loopBuffer);
    KBRelease(sound->releaseSound);

    if (sound->name) {
#ifdef __APPLE__
        alDeleteSources(1, &sound->name);
#endif
    }
}

void
KBMultiSoundPlay(KBMultiSound *sound) {
#ifdef __APPLE__
    alSourcePlay(sound->name);
#endif
}

void
KBMultiSoundStop(KBMultiSound *sound) {
#ifdef __APPLE__
    alSourceStop(sound->name);
#endif
    KBSoundPlay(sound->releaseSound);
}