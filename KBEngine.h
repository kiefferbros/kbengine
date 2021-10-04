//
//  KBEngine.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/26/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBEngine_h
#define KBEngine_h

#include "KBScene.h"
#include "KBSprite.h"
#include "KBKeyAnimation.h"
#include "KBMenuScene.h"
#include "KBLabel.h"
#include "KBFramebufferTexture.h"
#include "KBDictionary.h"
#include "KBShape.h"
#include "KBSound.h"
#include "KBAtlasLabel.h"
#include "KBDelay.h"

typedef const char * (*KBResourcePathFunc)(const char *resoureName);

void KBEngineInit(KBResourcePathFunc textureResourceFunc, KBResourcePathFunc soundResourceFunc, KBResourcePathFunc defaultResourceFunc, KBFloat texScale, KBMask texOpts, KBFloat camZoom, KBVec2 camSize, KBBox viewport);
void KBEngineRelease();

KBTexture * KBEngineGetTexture(const char *name);
void KBEngineAddTexture(KBTexture *texture, const char *name);
KBFont * KBEngineGetFont(const char *name);
KBAtlas * KBEngineGetAtlas(const char *name);
KBSoundBuffer * KBEngineGetSoundBuffer(const char *name);

void KBEngineReleaseUnusedResources();

KBType *KBEngineGetCurrentScene();
void KBEngineSetCurrentScene(KBType *sceneObj);

KBFloat KBEngineGetDefaultTexScale();

void KBEngineSetDefaultCamera(KBFloat zoom, KBVec2 size, KBBox viewport);
KBFloat KBEngineGetDefaultCamZoom();
KBVec2 KBEngineGetDefaultCamSize();
KBBox KBEngineGetDefaultViewport();

GLuint KBEngineGetSpriteIBO();
unsigned int KBEngineGetSpriteIBOMax();
void KBEngineSetSpriteIBOMax(unsigned int count);

void KBEngineMakeAudioContextCurrent(KBBool current);
void KBEngineSuspendAudioContext();
void KBEngineProcessAudioContext();

/* convenience macros for 1.1 extentions */
#ifdef GL_VERSION_ES_CM_1_1
/* buffer mapping */
#define KB_glMapBuffer_ARRAY() glMapBufferOES(GL_ARRAY_BUFFER, GL_WRITE_ONLY_OES)
#define KB_glUnmapBuffer_ARRAY() glUnmapBufferOES(GL_ARRAY_BUFFER)
#define KB_glMapBuffer_ELEMENT() glMapBufferOES(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY_OES)
#define KB_glUnmapBuffer_ELEMENT() glUnmapBufferOES(GL_ELEMENT_ARRAY_BUFFER)

/* framebuffer object */
#define KB_glGet_FRAMEBUFFER(buffer) glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint*)buffer)
#define KB_glGenFramebuffer(buffer) glGenFramebuffersOES(1,buffer)
#define KB_glBindFramebuffer(buffer) glBindFramebufferOES(GL_FRAMEBUFFER_OES, buffer)
#define KB_glFramebufferTexture2D_COLOR(texName) glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, texName, 0)
#define KB_glDeleteFramebuffer(buffer) glDeleteFramebuffersOES(1, &buffer)
#else

#define KB_glMapBuffer_ARRAY() glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)
#define KB_glUnmapBuffer_ARRAY() glUnmapBuffer(GL_ARRAY_BUFFER)
#define KB_glMapBuffer_ELEMENT() glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY)
#define KB_glUnmapBuffer_ELEMENT() glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER)

#define KB_glGet_FRAMEBUFFER(buffer) glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)buffer)
#define KB_glGenFramebuffer(buffer) glGenFramebuffers(1,buffer)
#define KB_glBindFramebuffer(buffer) glBindFramebuffer(GL_FRAMEBUFFER, buffer)
#define KB_glFramebufferTexture2D_COLOR(buffer) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer, 0)
#define KB_glDeleteFramebuffer(buffer) glDeleteFramebuffers(1, &buffer)

#endif

#endif
