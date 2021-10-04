//
//  KBMenuScene.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 1/3/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBMenuScene_h
#define KBMenuScene_h

#include "KBScene.h"
#include "KBArray.h"

typedef struct _KBMenuScene {
    KBScene     super;
    
    KBBool      interactive;
    KBBool      multiTouch;
    
    KBArray     *trackControls;
    KBArray     *controls;
} KBMenuScene;

#define KBMenuSceneCast(obj)((KBMenuScene*)obj)

KBMenuScene * KBMenuSceneInit(KBMenuScene *scene, KBDestroyFunc destroyFunc, KBVec2 camSize, KBBox viewport, KBFloat zoom);
KBMenuScene * KBMenuSceneNew(KBVec2 camSize, KBBox viewport, KBFloat zoom);
void KBMenuSceneDestroy(KBType *sceneObj);

void KBMenuSceneAddControl(KBMenuScene *scene, KBType *controlObj, KBType *parent);
void KBMenuSceneRemoveControl(KBMenuScene *scene, KBType *controlObj);
#endif
