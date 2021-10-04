//
//  KBScene.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/20/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#include "KBScene.h"
#include <stdlib.h>
#include <stdio.h>
#include "KBEngine.h"

void KBSceneDrawDefault(KBType *sceneObj);
void KBSceneDrawAtlas(KBType *sceneObj);

KBScene *
KBSceneInit(KBScene *scene, KBDestroyFunc destroyFunc, KBVec2 camSize, KBBox viewport, KBFloat zoom) {
    if ((scene=(KBScene*)KBNodeInit((KBNode*)scene, destroyFunc))) {
        KBNodeCast(scene)->scene = scene;
        scene->camera = KBCameraNew(camSize, viewport, zoom);
        KBNodeCast(scene->camera)->scene = scene;
        scene->drawFunc = &KBSceneDrawDefault;
        scene->animations = KBArrayNew(64);
        scene->inQueue = KBArrayNew(64);
        scene->outQueue = KBArrayNew(8);
        scene->inputs = KBArrayNew(4);
    }
    
    return scene;
}

KBScene *
KBSceneNew(KBVec2 camSize, KBBox viewport, KBFloat zoom) {
    return KBSceneInit(KBAlloc(sizeof(KBScene)), &KBSceneDestroy, camSize, viewport, zoom);
}

void
KBSceneDestroy(KBType *obj) {
    KBScene *scene = obj;
    KBRelease(scene->camera);
    
    KBRelease(scene->animations);
    KBRelease(scene->inQueue);
    KBRelease(scene->outQueue);
    
    KBRelease(scene->inputs);
    
    KBSceneSetAtlas(scene, NULL);
    
    KBNodeDestroy(obj);
}

void
KBSceneStepAnimations(KBScene *scene, KBFloat dt) {
    if (scene->outQueue->num) {
        KBArrayCull(scene->animations, scene->outQueue);
		KBArrayClear(scene->outQueue);
	}
	
	if (scene->inQueue->num) {
        KBArrayAppend(scene->animations, scene->inQueue);
        KBArrayClear(scene->inQueue);
	}
    
    for (int i=0; i<scene->animations->num; i++) {
        KBAnimation *animation = scene->animations->arr[i];
        
        
#if DEBUG
        if (animation == NULL && !KBArrayContains(scene->outQueue, animation)) {
            printf("Animation node is NULL!\n");
            KBDescription(animation);
        }
#endif
        KBRetain(animation->node);
        
        if (!KBAnimationStep(animation, dt)) {
            KBNodeRemoveAnimation(animation->node, animation);
        }
        KBRelease(animation->node);
    }
}

void
KBSceneDrawDefault(KBType *sceneObj) {
    /* call each childs draw function */
    KBScene *scene = sceneObj;
    KBCameraLook(scene->camera);
    KBArray *children = KBNodeCast(scene)->children;
    
    for (int i=0; i<children->num; i++) {
        KBDrawable *child = children->arr[i];
        child->drawFunc(child, 1.f);
    }
}



void
KBSceneDrawStatic(KBScene *scene) {
    scene->drawFunc(scene);
}

void
KBSceneDraw(KBScene *scene, KBFloat dt) {
    /* retain the scene in case it is released during an animation callback or some similar event */
    KBRetain(scene);
    
    /* animate the scene and draw the scene */
    KBSceneStepAnimations(scene, dt);
    scene->drawFunc(scene);
    
    KBRelease(scene);
}

void
KBSceneDrawAtlas(KBType *sceneObj) {
    KBScene *scene = sceneObj;
    glBindBuffer(GL_ARRAY_BUFFER, scene->atlasVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, KBEngineGetSpriteIBO());
    glBindTexture(GL_TEXTURE_2D, KBTextureCast(scene->atlas)->name);
    
    KBTintedVertex *v = KB_glMapBuffer_ARRAY();
    KBTransform t = KBTransformIdentity;
    KBFloat o = 1.f;
    int count = 0;
    
    KBArray *children = KBNodeCast(scene)->children;
    for (int i=0; i<children->num; i++) {
        KBSprite *child = children->arr[i];
        KBSpriteUpdateAtlasVertices(child, scene->atlas, &v, &t, o, &count);
    }
    
    KB_glUnmapBuffer_ARRAY();
    
    KBCameraLook(scene->camera);
    
    glVertexPointer(2, GL_FLOAT, sizeof(KBTintedVertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(KBTintedVertex), (GLvoid*)sizeof(KBVec2));
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(KBTintedVertex), (GLvoid*)sizeof(KBVec4));
    glDrawElements(GL_TRIANGLE_STRIP, count*6, GL_UNSIGNED_SHORT, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
KBSceneSetAtlas(KBScene *scene, KBAtlas *atlas) {
    KBRelease(scene->atlas);
    scene->atlas = KBRetain(atlas);
    
    if (atlas && scene->drawFunc!=&KBSceneDrawAtlas) {
        scene->drawFunc = &KBSceneDrawAtlas;
        glGenBuffers(1, &scene->atlasVBO);
        
        int nMaxSprites = 1000;
        
        KBEngineSetSpriteIBOMax(nMaxSprites);
        
        glBindBuffer(GL_ARRAY_BUFFER, scene->atlasVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(KBTintedVertex)*nMaxSprites*4, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glEnableClientState(GL_COLOR_ARRAY);
        
    } else if (!atlas && scene->drawFunc!=&KBSceneDrawDefault) {
        scene->drawFunc = &KBSceneDrawDefault;
        
        glDeleteBuffers(1, &scene->atlasVBO);
        scene->atlasVBO = 0;
        
        glDisableClientState(GL_COLOR_ARRAY);
    }
}

void
KBSceneDidAddNode(KBType *sceneObj, KBType *nodeObj) {
    
}

void KBSceneWillRemoveNode(KBType *sceneObj, KBType *nodeObj) {
    
}

void
KBSceneAddAnimation(KBScene *scene, KBAnimation *animation) {
    KBArrayPush(scene->inQueue, animation);
    KBArrayRemoveObject(scene->outQueue, animation);
}

void
KBSceneRemoveAnimation(KBScene *scene, KBAnimation *animation) {
    
    KBArrayPush(scene->outQueue, animation);
    KBArrayRemoveObject(scene->inQueue, animation);

}

void
KBSceneAddAnimations(KBScene *scene, KBArray *animations) {

    KBArrayAppend(scene->inQueue, animations);
    KBArrayCull(scene->outQueue, animations);
}

void
KBSceneRemoveAnimations(KBScene *scene, KBArray *animations) {
    KBArrayAppend(scene->outQueue, animations);
    KBArrayCull(scene->inQueue, animations);
}

KBType * KBSceneHitTest(KBType *sceneObj, KBVec2 p) {

    KBArray *children = KBNodeCast(sceneObj)->children;
    if (children!=NULL) {
        for (int i=children->num-1; i>=0; i--) {
            KBDrawable *node = children->arr[i];
            
            KBDrawable *child = node->hitTestFunc(node, p);
            
            if (child!=NULL) {
                return child;
            }
        }
    }
    
    return NULL;
    
}

KBArray * KBSceneGetInputs(KBType *sceneObj) {
    return KBSceneCast(sceneObj)->inputs;
}

void KBSceneInputEventBegan(KBType *sceneObj, KBArray *inputs) {
    KBScene *scene = sceneObj;
    
    if (scene->inputEndedFunc)
        KBArrayAppend(scene->inputs, inputs);
    
    if (scene->inputBeganFunc) {
        KBRetain(sceneObj);
        scene->inputBeganFunc(sceneObj, inputs);
        KBRelease(sceneObj);
    }
}

void KBSceneInputEventMoved(KBType *sceneObj, KBArray *inputs) {
    KBScene *scene = sceneObj;
    if (scene->inputMovedFunc) {
        KBRetain(sceneObj);
        scene->inputMovedFunc(sceneObj, inputs);
        KBRelease(sceneObj);
    }
}

void KBSceneInputEventEnded(KBType *sceneObj, KBArray *inputs) {
    KBScene *scene = sceneObj;
    KBRetain(sceneObj);
    
    if (scene->inputEndedFunc) {
        scene->inputEndedFunc(sceneObj, inputs);
    }
    
    KBArrayCull(scene->inputs, inputs);
    KBRelease(sceneObj);
}

void KBSceneInputEventCanceled(KBType *sceneObj, KBArray *inputs) {
    KBScene *scene = sceneObj;
    
    KBRetain(sceneObj);
    if (scene->inputCanceledFunc) {
        scene->inputCanceledFunc(sceneObj, inputs);
    }
    
    KBArrayCull(scene->inputs, inputs);
    
    KBRelease(sceneObj);
}



