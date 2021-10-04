//
//  KBControl.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 1/3/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBControl.h"
#include <stdlib.h>
#include <stdio.h>

KBType *
KBControlDefaultHitTest(KBType *drawableObj, KBVec2 p) {
    KBDrawable *drawable = drawableObj;
    
    if (KBNodeCast(drawableObj)->children!=NULL) {
        for (int i= KBNodeCast(drawable)->children->num-1; i>=0; i--) {
            KBDrawable *node = KBNodeCast(drawable)->children->arr[i];
            
            KBDrawable *child = node->hitTestFunc(node, p);
            
            if (child!=NULL) {
                return drawableObj;
            }
        }
    }
    
    return KBBoxContainsVec(KBDrawableGetGlobalBoundBox(drawableObj), p) ? drawableObj : NULL;
}

KBControl * KBControlInit(KBControl                 *control,
                          KBDestroyFunc             destroyFunc,
                          KBControlTrackingFunc     beginFunc,
                          KBControlTrackingFunc     continueFunc,
                          KBControlTrackingFunc     endFunc,
                          KBControlTrackingFunc     cancelFunc,
                          KBControlActionFunc       actionFunc,
                          void                      *actionData)
{
    if ((control = (KBControl*)KBSpriteInit((KBSprite*)control, destroyFunc, NULL))) {
        
        KBDrawableCast(control)->hitTestFunc = &KBControlDefaultHitTest;
        
        control->tracking = KBFalse;
    
        control->beginFunc = beginFunc;
        control->continueFunc = continueFunc;
        control->endFunc = endFunc;
        control->cancelFunc = cancelFunc;
    
        control->actionFunc = actionFunc;
        control->actionData = actionData;
    }
    
    return control;
}

KBControl * KBControlNew(KBControlTrackingFunc      beginFunc,
                         KBControlTrackingFunc      continueFunc,
                         KBControlTrackingFunc      endFunc,
                         KBControlTrackingFunc      cancelFunc,
                         KBControlActionFunc        actionFunc,
                         void                       *actionData)
{
    return KBControlInit(KBAlloc(sizeof(KBControl)), &KBControlDestroy, beginFunc, continueFunc, endFunc, cancelFunc, actionFunc, actionData);
}

void
KBControlDestroy(KBType *controlObj) {
    KBSpriteDestroy(controlObj);
}

KBBool KBControlBeginTracking(KBType *controlObj, KBVec2 p, void *data) {
    KBControl *control = controlObj;
    
    KBBool track = control->beginFunc(controlObj, p, data);
    if (track) {
        control->tracking = KBTrue;
        control->trackData = data;
    }
    
    return track;
}

KBBool KBControlContinueTracking(KBType *controlObj, KBVec2 p, void *data) {
    KBControl *control = controlObj;
    if (!control->continueFunc(controlObj, p, data)) {
        control->tracking = KBFalse;
        control->trackData = NULL;
        return KBFalse;
    }
    
    return KBTrue;
}


void KBControlEndTracking(KBType *controlObj, KBVec2 p, void *data) {
    KBControl *control = controlObj;

    control->endFunc(controlObj, p, data);
    control->tracking = KBFalse;
    control->trackData = NULL;
    
}
void KBControlCancelTracking(KBType *controlObj, KBVec2 p, void *data) {
    KBControl *control = controlObj;
    control->cancelFunc(controlObj, p, data);
    control->tracking = KBFalse;
    control->trackData = NULL;
}

void KBControlAction(KBType *controlObj) {
    KBControl *control = controlObj;
    if (control->actionFunc) {
        control->actionFunc(controlObj, control->actionData);
    }
}

