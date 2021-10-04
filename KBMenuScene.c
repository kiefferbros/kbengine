//
//  KBMenuScene.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 1/3/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBMenuScene.h"
#include "KBControl.h"
#include <stdlib.h>
#include <stdio.h>

void
KBMenuSceneEventBegan(KBType *sceneObj, KBArray *inputs) {
   
    
    KBMenuScene *scene = sceneObj;
    for (int i=0; i<inputs->num && (scene->multiTouch || (!scene->multiTouch && scene->trackControls->num < 1)); i++) {
        KBInput *input = inputs->arr[i];
        
        KBVec2 global = KBCameraViewportToGlobal(KBSceneGetCamera(scene), input->pos);
        
        KBControl *control = KBSceneHitTest(sceneObj, global);
        
        if (control!=NULL && KBArrayContains(scene->controls, control)) {
             
            if (KBControlBeginTracking(control, global, input->data)) {
                KBArrayPush(scene->trackControls, control);
            }
        }
    }
}

void
KBMenuSceneEventMoved(KBType *sceneObj, KBArray *inputs) {
    KBMenuScene *scene = sceneObj;
    
    for (int i=0; i<inputs->num; i++) {
        KBInput *input = inputs->arr[i];
        KBVec2 global = KBCameraViewportToGlobal(KBSceneGetCamera(scene), input->pos);
        
        int j = 0;
        while (j<scene->trackControls->num) {
            KBControl *control = scene->trackControls->arr[j];
            
            if (control->trackData == input->data && !KBControlContinueTracking(control, global, input->data)) {
                KBArrayRemoveIndex(scene->trackControls, j);
            } else {
                j++;
            }
        }
        
    }
    
    
    
    
}

void
KBMenuSceneEventEnded(KBType *sceneObj, KBArray *inputs) {
    KBMenuScene *scene = sceneObj;
    
    
    for (int i=0; i<inputs->num; i++) {
        KBInput *input = inputs->arr[i];
        KBVec2 global = KBCameraViewportToGlobal(KBSceneGetCamera(scene), input->pos);
    
        int j = 0;
        while (j<scene->trackControls->num) {
            KBControl *control = scene->trackControls->arr[j];
            
            if (control->trackData == input->data) {
                KBControlEndTracking(control, global, input->data);
                KBArrayRemoveIndex(scene->trackControls, j);
            } else {
                j++;
            }
        }
        
    }

}

void
KBMenuSceneEventCanceled(KBType *sceneObj, KBArray *inputs) {
    KBMenuScene *scene = sceneObj;
    
    for (int i=0; i<inputs->num; i++) {
        KBInput *input = inputs->arr[i];
        KBVec2 global = KBCameraViewportToGlobal(KBSceneGetCamera(scene), input->pos);
    
        int j = 0;
        while (j<scene->trackControls->num) {
            KBControl *control = scene->trackControls->arr[j];
            
            if (control->trackData == input->data) {
                KBControlCancelTracking(control, global, input->data);
                KBArrayRemoveIndex(scene->trackControls, j);
            } else {
                j++;
            }
        }
    }
}

KBMenuScene *
KBMenuSceneInit(KBMenuScene *scene, KBDestroyFunc destroyFunc, KBVec2 camSize, KBBox viewport, KBFloat zoom) {
    KBSceneInit((KBScene*)scene, destroyFunc, camSize, viewport, zoom);
    scene->interactive = KBTrue;
    scene->multiTouch = KBFalse;
    
    KBSceneCast(scene)->inputBeganFunc = &KBMenuSceneEventBegan;
    KBSceneCast(scene)->inputMovedFunc = &KBMenuSceneEventMoved;
    KBSceneCast(scene)->inputEndedFunc = &KBMenuSceneEventEnded;
    KBSceneCast(scene)->inputCanceledFunc = &KBMenuSceneEventCanceled;
    
    scene->trackControls = KBArrayNew(2);
    scene->controls = KBArrayNew(4);
    
    return scene;
}

KBMenuScene *
KBMenuSceneNew(KBVec2 camSize, KBBox viewport, KBFloat zoom) {
    return KBMenuSceneInit(KBAlloc(sizeof(KBMenuScene)), &KBMenuSceneDestroy, camSize, viewport, zoom);
}

void
KBMenuSceneDestroy(KBType *sceneObj) {
    
    KBMenuScene *scene = sceneObj;
    KBRelease(scene->trackControls);
    KBRelease(scene->controls);
    KBSceneDestroy(sceneObj);
}

void
KBMenuSceneAddControl(KBMenuScene *scene, KBType *controlObj, KBType *parent) {
    KBArrayPush(scene->controls, controlObj);
    KBNodeAddChild(parent==NULL?scene:parent, controlObj);
}

void
KBMenuSceneRemoveControl(KBMenuScene *scene, KBType *controlObj) {
    
    KBArrayRemoveObject(scene->controls, controlObj);
    KBNodeDisownParent(controlObj);
    
}
