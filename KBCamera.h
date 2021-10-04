//
//  KBCamera.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/20/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBCamera_h
#define KBCamera_h

#include "KBNode.h"

typedef struct _KBCamera {
    KBNode          super;

    KBVec2          pos;
    KBFloat         zoom;
    KBTransform     transform;
    KBBool          transformFlag;
    
    KBVec2          size;
    KBBox           viewport;
} KBCamera;

#define KBCameraCast(obj)((KBCamera*)obj)

KBCamera * KBCameraInit(KBCamera *camera, KBDestroyFunc destroyFunc, KBVec2 size, KBBox viewport, KBFloat zoom);
KBCamera * KBCameraNew(KBVec2 size, KBBox viewport, KBFloat zoom);
void KBCameraDestroy(KBType *cameraObj);

void KBCameraShape(KBCamera *camera);
void KBCameraReshape(KBCamera *camera, KBVec2 size, KBBox viewport);
void KBCameraLook(KBCamera *camera);

void KBCameraSetPosition(KBCamera *camera, KBVec2 pos);
void KBCameraSetZoom(KBCamera *camera, KBFloat zoom);

KBVec2 KBCameraViewportToGlobal(KBCamera *camera, KBVec2 screen);
KBVec2 KBCameraLocalToGlobal(KBCamera *camera, KBVec2 screen);
KBBox  KBCameraGetViewBox(KBCamera *camera);

void KBCameraSetAnimatedPosition(KBType *cameraObj, void *posPtr);

#endif
