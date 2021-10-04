//
//  KBScene.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/20/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBScene_h
#define KBScene_h

#include "KBNode.h"
#include "KBCamera.h"
#include "KBInput.h"
#include "KBAtlas.h"

typedef void (*KBInputEventFunc)(KBType *sceneObj, KBArray *inputs);
typedef void (*KBSceneDrawFunc)(KBType *sceneObj);

typedef struct _KBScene {
    KBNode              super;

    KBCamera            *camera;
    
    KBSceneDrawFunc     drawFunc;
    
    KBArray             *animations;
    KBArray             *inQueue, *outQueue;
    
    KBAtlas             *atlas;
    GLuint              atlasVBO;
    
    KBArray             *inputs;
    KBInputEventFunc    inputBeganFunc;
    KBInputEventFunc    inputMovedFunc;
    KBInputEventFunc    inputEndedFunc;
    KBInputEventFunc    inputCanceledFunc;
} KBScene;

#define KBSceneCast(obj)((KBScene*)obj)

KBScene *KBSceneInit(KBScene *scene, KBDestroyFunc destroyFunc, KBVec2 camSize, KBBox viewport, KBFloat zoom);
KBScene *KBSceneNew(KBVec2 camSize, KBBox viewport, KBFloat zoom);
void KBSceneDestroy(KBType *scene);

static inline KBCamera *
KBSceneGetCamera(KBType *sceneObj) {
    if (sceneObj) {
        return KBSceneCast(sceneObj)->camera;
    }
    
    return (KBCamera*)0;
}

void KBSceneDrawStatic(KBScene *scene);
void KBSceneDraw(KBScene *scene, KBFloat dt);

void KBSceneSetAtlas(KBScene *scene, KBAtlas *atlas);

void KBSceneAddAnimation(KBScene *scene, KBAnimation *animation);
void KBSceneRemoveAnimation(KBScene *scene, KBAnimation *animation);
void KBSceneAddAnimations(KBScene *scene, KBArray *animations);
void KBSceneRemoveAnimations(KBScene *scene, KBArray *animations);

void KBSceneDidAddNode(KBType *sceneObj, KBType *nodeObj);
void KBSceneWillRemoveNode(KBType *sceneObj, KBType *nodeObj);


KBType * KBSceneHitTest(KBType *sceneObj, KBVec2 p);
KBArray * KBSceneGetInputs(KBType *sceneObj);
void KBSceneInputEventBegan(KBType *sceneObj, KBArray *inputs);
void KBSceneInputEventMoved(KBType *sceneObj, KBArray *inputs);
void KBSceneInputEventEnded(KBType *sceneObj, KBArray *inputs);
void KBSceneInputEventCanceled(KBType *sceneObj, KBArray *inputs);

#endif
