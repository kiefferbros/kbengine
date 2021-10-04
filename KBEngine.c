//
//  KBEngine.c
//  Rooftop Rufus
//
//  Created by Jonathan Kieffer on 1/4/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBEngine.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct _KBEngine {
    KBObject                super;
    
    /* resournce functions */
    KBResourcePathFunc      textureResourceFunc;
    KBResourcePathFunc      soundResourceFunc;
    KBResourcePathFunc      defaultResourceFunc;
    
    /* default settings */
    KBFloat                 defaultTexScale;
    KBMask                  defaultTexOptions;
    KBFloat                 defaultCamZoom;
    KBVec2                  defaultCamSize;
    KBBox                   defaultViewport;
    
    /* resources */
    KBDictionary            *textures;
    KBDictionary            *fonts;
    KBDictionary            *atlases;
    KBDictionary            *soundBuffers;
    KBType                  *currentScene;
    
    int                     spriteMax;
    GLuint                  spriteIBO;
    
#if __APPLE__
    /* audio */
    ALCdevice               *alDevice;
	ALCcontext              *alContext;
#endif
    
} KBEngine;


static KBEngine *sharedEngine = NULL;

void
KBEngineDestroy(KBType *obj) {
    KBEngine *engine = obj;
    KBRelease(engine->textures);
    KBRelease(engine->fonts);
    KBRelease(engine->atlases);
    KBRelease(engine->soundBuffers);
    KBRelease(engine->currentScene);
    
    
    if (engine->alContext)
        alcDestroyContext(engine->alContext);
    
	if (engine->alDevice)
        alcCloseDevice(engine->alDevice);
    
    if (engine->spriteIBO)
        glDeleteBuffers(1, &engine->spriteIBO);
}

void
KBEngineInit(KBResourcePathFunc textureResourceFunc, KBResourcePathFunc soundResourceFunc, KBResourcePathFunc defaultResourceFunc, KBFloat texScale, KBMask texOpts, KBFloat camZoom, KBVec2 camSize, KBBox viewport) {
    
    /* create the engine object */
    if (sharedEngine!=NULL) {
        printf("KBEngine already initialized.\n");
    }
    /* init open gl */
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    
    
    sharedEngine = KBAlloc(sizeof(KBEngine));
    KBObjectInit(sharedEngine, &KBEngineDestroy);
    
    /* init the engine object */
    sharedEngine->textureResourceFunc = textureResourceFunc;
    sharedEngine->soundResourceFunc = soundResourceFunc;
    sharedEngine->defaultResourceFunc = defaultResourceFunc;
    
    sharedEngine->defaultTexScale = texScale;
    sharedEngine->defaultTexOptions = texOpts;
    sharedEngine->defaultCamZoom = camZoom;
    sharedEngine->defaultCamSize = camSize;
    sharedEngine->defaultViewport = viewport;
    
    sharedEngine->textures = KBDictionaryNew(32);
    sharedEngine->soundBuffers = KBDictionaryNew(32);
    sharedEngine->fonts = KBDictionaryNew(4);
    sharedEngine->atlases = KBDictionaryNew(4);
    sharedEngine->currentScene = NULL;
    
    sharedEngine->spriteMax = 0;
    sharedEngine->spriteIBO = 0;
    
#if __APPLE__
    
    // set up OpenAL
    // clear the errors
    alGetError();
    
    // connect to the default device
    sharedEngine->alDevice = alcOpenDevice(NULL);
    
    if (!sharedEngine->alDevice) {
        printf("Error opening OpenAL device.\n");
    }
    
    // create a context to render the sound to
    sharedEngine->alContext = alcCreateContext(sharedEngine->alDevice, NULL);
    
    if (!sharedEngine->alContext) {
        printf("Error creating OpenAL context.\n");
    }
    
    alcMakeContextCurrent(sharedEngine->alContext);
#endif
}

void
KBEngineRelease() {
    KBRelease(sharedEngine);
    sharedEngine = NULL;
}

void
KBEngineClearUnusedEntries(KBArray *entries) {
    int i = 0;
    while (i<entries->num) {
        KBDictEntry *entry = entries->arr[i];
        KBObject *value = entry->value;
        if (value->retainCount == 1) {
            KBArrayRemoveIndex(entries, i);
        } else {
            i++;
        }
    }
}

void
KBEngineReleaseUnusedResources() {
    /* clear unused */
    KBEngineClearUnusedEntries(sharedEngine->textures->entries);
    KBEngineClearUnusedEntries(sharedEngine->fonts->entries);
    KBEngineClearUnusedEntries(sharedEngine->atlases->entries);
    KBEngineClearUnusedEntries(sharedEngine->soundBuffers->entries);
}

KBTexture *
KBEngineGetTexture(const char *name) {
    KBTexture *texture = KBDictionaryGetValue(sharedEngine->textures, name);
    
    if (texture == NULL) {
        const char *path = sharedEngine->textureResourceFunc(name);
        
        texture = KBTextureNewPNG(path, sharedEngine->defaultTexScale, sharedEngine->defaultTexOptions);
        KBDictionarySetValue(sharedEngine->textures, texture, name);
        KBRelease(texture);
    }
    
    return texture;
}

void
KBEngineAddTexture(KBTexture *texture, const char *name) {
    KBDictionarySetValue(sharedEngine->textures, texture, name);
}

KBFont * KBEngineGetFont(const char *name) {
    KBFont *font = KBDictionaryGetValue(sharedEngine->fonts, name);
    
    if (font == NULL) {
        int len = (int)strlen(name) + 5;
        
        char texName[len];
        snprintf(texName, len, "%s", name);
        const char *texPath = sharedEngine->textureResourceFunc(name);

        char headerName[len];
        snprintf(headerName, len, "%s.%s", name, "kbf");
        const char *headerPath = sharedEngine->defaultResourceFunc(headerName);
        
        
        font = KBFontNew(texPath, headerPath, sharedEngine->defaultTexScale, sharedEngine->defaultTexOptions);
        KBDictionarySetValue(sharedEngine->fonts, font, name);
        KBRelease(font);
    }
    
    return font;
}

KBAtlas * KBEngineGetAtlas(const char *name) {
    KBAtlas *atlas = KBDictionaryGetValue(sharedEngine->atlases, name);
    
    if (atlas == NULL) {
        int len = (int)strlen(name) + 5;
        
        char texName[len];
        snprintf(texName, len, "%s", name);
        const char *texPath = sharedEngine->textureResourceFunc(name);
        
        char headerName[len];
        snprintf(headerName, len, "%s.%s", name, "kba");
        const char *headerPath = sharedEngine->defaultResourceFunc(headerName);
        
        
        atlas = KBAtlasNew(texPath, headerPath, sharedEngine->defaultTexScale, sharedEngine->defaultTexOptions);
        KBDictionarySetValue(sharedEngine->atlases, atlas, name);
        KBRelease(atlas);
    }
    
    return atlas;
}

KBSoundBuffer *
KBEngineGetSoundBuffer(const char *name) {
    KBSoundBuffer *buffer = KBDictionaryGetValue(sharedEngine->soundBuffers, name);
    
    if (buffer == NULL) {
        const char *path = sharedEngine->soundResourceFunc(name);
        
        buffer = KBSoundBufferNewFile(path);
        KBDictionarySetValue(sharedEngine->soundBuffers, buffer, name);
        KBRelease(buffer);
    }
    
    return buffer;
}


KBType *
KBEngineGetCurrentScene() {
    return sharedEngine!=NULL ? sharedEngine->currentScene : NULL;
}

void
KBEngineSetCurrentScene(KBType *scene) {
    if (scene != sharedEngine->currentScene) {
        KBRetain(scene);
        KBRelease(sharedEngine->currentScene);
        
        KBArrayClear(KBSceneCast(scene)->inputs); /* clear inputs */
        sharedEngine->currentScene = scene;
    }
}


KBFloat
KBEngineGetDefaultTexScale() {
    return sharedEngine->defaultTexScale;
}

KBFloat
KBEngineGetDefaultCamZoom() {
    return sharedEngine->defaultCamZoom;
}

void KBEngineSetDefaultCamera(KBFloat zoom, KBVec2 size, KBBox viewport) {
    sharedEngine->defaultCamZoom = zoom;
    sharedEngine->defaultCamSize = size;
    sharedEngine->defaultViewport = viewport;
}

KBVec2
KBEngineGetDefaultCamSize() {
    return sharedEngine->defaultCamSize;
}

KBBox
KBEngineGetDefaultViewport() {
    return sharedEngine->defaultViewport;
}

GLuint
KBEngineGetSpriteIBO() {
    return sharedEngine->spriteIBO;
}

unsigned int
KBEngineGetSpriteIBOMax() {
    return sharedEngine->spriteMax;
}

void
KBEngineSetSpriteIBOMax(unsigned int count) {
    if (sharedEngine->spriteMax != count) {
        if (sharedEngine->spriteIBO == 0) {
            glGenBuffers(1, &sharedEngine->spriteIBO);
        }
        
        sharedEngine->spriteMax = count;
        
        /* generate the index buffer data */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sharedEngine->spriteIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*count*6, NULL, GL_STATIC_DRAW);
        
        GLushort *idxs = (GLushort *)KB_glMapBuffer_ELEMENT();
        
        GLushort j = 0;
		for (int i=0; i<count; ++i) {
			idxs[i*6] = idxs[i*6+1] = j;
			idxs[i*6+2] = j+1;
			idxs[i*6+3] = j+2;
			idxs[i*6+4] = idxs[i*6+5] = j+3;
			
			j += 4;
		}
        KB_glUnmapBuffer_ELEMENT();
        
        
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    }
}


void
KBEngineMakeAudioContextCurrent(KBBool current) {
    alGetError();
	alcMakeContextCurrent((current)?sharedEngine->alContext:NULL);
}

void
KBEngineSuspendAudioContext() {
    alcSuspendContext(sharedEngine->alContext);
}

void
KBEngineProcessAudioContext() {
    alcProcessContext(sharedEngine->alContext);
}
