//
//  KBNode.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/20/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBNode_h
#define KBNode_h

#include "KBArray.h"
#include "KBAnimation.h"

typedef void (*KBNodeEventFunc)(KBType *nodeObj);

typedef struct _KBNode {
    KBObject            super;
    
    int                  tag;
    
    KBArray             *children;
    KBType              *parent;
    KBType              *scene;
    
    KBNodeEventFunc     willChangeSceneFunc;
    KBNodeEventFunc     didChangeSceneFunc;
    
    KBArray             *animations;
} KBNode;

#define KBNodeCast(obj)((KBNode*)obj)

KBNode * KBNodeInit(KBNode *node, KBDestroyFunc destroyFunc);
KBNode * KBNodeNew();
void KBNodeDestroy(KBType *node);

static inline int
KBNodeGetTag(KBType *nodeObj) {
    return KBNodeCast(nodeObj)->tag;
}

static inline void
KBNodeSetTag(KBType *nodeObj, int tag) {
    KBNodeCast(nodeObj)->tag = tag;
}

static inline KBType *
KBNodeGetScene(KBType *nodeObj) {
    return KBNodeCast(nodeObj)->scene;
}

static inline KBType *
KBNodeGetParent(KBType *nodeObj) {
    return KBNodeCast(nodeObj)->parent;
}

static inline KBArray *
KBNodeGetChildren(KBType *nodeObj) {
    return KBNodeCast(nodeObj)->children;
}

void KBNodeAddChild(KBType *parent, KBType *child);
void KBNodeInsertChild(KBType *parent, KBType *child, int idx);
void KBNodeDisownAllChildren(KBType *node);
void KBNodeDisownParent(KBType *node);

void KBNodeAddAnimation(KBType *node, KBType *animationObj);
void KBNodeRemoveAnimation(KBType *node, KBType *animationObj);
void KBNodeRemoveAnimationWithName(KBType *node, int name);
void KBNodeRemoveAllAnimations(KBType *nodeObj);
KBBool KBNodeContainsAnimationWithName(KBType *node, int name);

#endif
