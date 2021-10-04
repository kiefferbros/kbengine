//
//  KBControl.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 1/3/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBControl_h
#define KBControl_h

#include "KBSprite.h"


typedef KBBool (*KBControlTrackingFunc)(KBType *controlObj, KBVec2 p, void *data);
typedef void (*KBControlActionFunc)(KBType *controlObj, void *data);

typedef struct _KBControl {
    KBSprite                super;
    
    KBBool                  tracking;
    
    KBControlTrackingFunc   beginFunc;
    KBControlTrackingFunc   continueFunc;
    KBControlTrackingFunc   endFunc;
    KBControlTrackingFunc   cancelFunc;
    void                    *trackData;
    
    KBControlActionFunc     actionFunc;
    void                    *actionData;
    
} KBControl;

#define KBControlCast(obj)((KBControl*)obj)

KBControl * KBControlInit(KBControl                 *control,
                          KBDestroyFunc             destroyFunc,
                          KBControlTrackingFunc     beginFunc,
                          KBControlTrackingFunc     continueFunc,
                          KBControlTrackingFunc     endFunc,
                          KBControlTrackingFunc     cancelFunc,
                          KBControlActionFunc       actionFunc,
                          void                      *actionData);

KBControl * KBControlNew(KBControlTrackingFunc      beginFunc,
                         KBControlTrackingFunc      continueFunc,
                         KBControlTrackingFunc      endFunc,
                         KBControlTrackingFunc      cancelFunc,
                         KBControlActionFunc        actionFunc,
                         void                       *actionData);

void KBControlDestroy(KBType *controlObj);

KBBool KBControlBeginTracking(KBType *controlObj, KBVec2 p, void *data);
KBBool KBControlContinueTracking(KBType *controlObj, KBVec2 p, void *data);
void KBControlEndTracking(KBType *controlObj, KBVec2 p, void *data);
void KBControlCancelTracking(KBType *controlObj, KBVec2 p, void *data);

void KBControlAction(KBType *controlObj);

#endif
