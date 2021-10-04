//
//  KBNode.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/20/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#include "KBNode.h"
#include <stdlib.h>
#include <stdio.h>
#include "KBScene.h"

void KBNodeSetScene(KBNode *node, KBScene *scene);

static const char * KBNodeDescriptionStr = "KBNode";

const char * KBNodeGetDescription(KBType *nodeObj) {
    return KBNodeDescriptionStr;
}

KBNode * KBNodeInit(KBNode *node, KBDestroyFunc destroyFunc) {
    KBObjectInit(node, destroyFunc);
    
    KBObjectCast(node)->descripFunc = KBNodeGetDescription;
    
    node->children = NULL;
    node->parent = NULL;
    node->scene = NULL;
    node->willChangeSceneFunc = NULL;
    node->didChangeSceneFunc = NULL;
    node->animations = NULL;
    return node;
}

KBNode *
KBNodeNew(void *node) {    
    return KBNodeInit(KBAlloc(sizeof(KBNode)), &KBNodeDestroy);
}


void
KBNodeDestroy(KBType *obj) {
    KBNode *node = obj;
    
    KBRelease(node->children);
    KBRelease(node->animations);
}

void
KBNodeWillChangeScene(KBNode *node, KBScene *scene) {
    // call the event function
    if (node->willChangeSceneFunc) {
        node->willChangeSceneFunc(node);
    }
    
    if (scene==NULL) {
        KBSceneWillRemoveNode(scene, node);
    }
    
    if (node->children != NULL) {
        for (int i=0; i<node->children->num; i++) {
            KBNode *child = node->children->arr[i];
            KBNodeWillChangeScene(child, scene);
        }
    }
}

void
KBNodeDidChangeScene(KBNode *node, KBScene *scene) {
    // call the event function
    if (node->didChangeSceneFunc) {
        node->didChangeSceneFunc(node);
    }
    
    if (scene!=NULL) {
        KBSceneDidAddNode(scene, node);
    }
    
    // have each child call its event functions
    if (node->children != NULL) {
        for (int i=0; i<node->children->num; i++) {
            KBNode *child = node->children->arr[i];
            KBNodeDidChangeScene(child, scene);
        }
    }
}

void
KBNodeSetScene_(KBNode *node, KBScene *scene) {
    // remove animations from old scene
    if (node->scene && node->animations) {
        KBSceneRemoveAnimations(node->scene, node->animations);
    }
    
    node->scene = scene;
    
    // add animations to new scene
    if (node->scene && node->animations) {
        KBSceneAddAnimations(node->scene, node->animations);
    }
    
    if (node->children != NULL) {
        for (int i=0; i<node->children->num; i++) {
            KBNode *child = node->children->arr[i];
            KBNodeSetScene_(child, scene);
        }
    }
}

void
KBNodeSetScene(KBNode *node, KBScene *scene) {
    if (scene != node->scene) {
        KBNodeWillChangeScene(node, scene);
        KBNodeSetScene_(node, scene);
        KBNodeDidChangeScene(node, scene);
    }
}

void
KBNodeAddChild(KBType *parent, KBType *child) {

    KBNode *parentNode = parent;
    KBNode *childNode = child;
    
    /* can only be a part of one node at a time */
    KBNode *oldParent = childNode->parent;
    if (oldParent != NULL) {
        KBRetain(childNode);
        KBNodeDisownParent(childNode);
    }
    
    if (parentNode->children==NULL)
        parentNode->children = KBArrayNew(4);
    
    KBArrayPush(parentNode->children, child);
    childNode->parent = parentNode;
    KBNodeSetScene(childNode, parentNode->scene);
    
    if (oldParent != NULL) {
        KBRelease(childNode);
    }
    
}

void KBNodeInsertChild(KBType *parent, KBType *child, int idx) {
    KBNode *parentNode = parent;
    KBNode *childNode = child;
    
    KBRetain(childNode);
    
    /* can only be a part of one node at a time */
    KBNode *oldParent = childNode->parent;
    if (oldParent != NULL) {
        KBNodeDisownParent(childNode);
    }
    
    if (parentNode->children==NULL)
        parentNode->children = KBArrayNew(4);
    
    KBArrayInsert(parentNode->children, child, idx);
    childNode->parent = parentNode;
    KBNodeSetScene(childNode, parentNode->scene);

    KBRelease(childNode);    
}

void
KBNodeDisownParent(KBType *nodeObj) {
    
    KBNode *node = nodeObj;
    KBNode *parent = node->parent;
    
    if (parent!=NULL && parent->children!=NULL) {
        KBNodeSetScene(node, NULL);
        node->parent = NULL;
        KBArrayRemoveObject(parent->children, node);
    }
}

void
KBNodeDisownAllChildren(KBType *node) {
    KBArray *children = KBNodeGetChildren(node);
    if (children) {
        while (children->num) {
            KBNode *child = KBArrayGetObjectAtIndex(children, 0);
            KBNodeDisownParent(child);
        }
    }
}

void
KBNodeAddAnimation(KBType *nodeObj, KBType *animationObj) {
    KBNode *node = nodeObj;
    KBAnimation *animation = animationObj;
    
    if (node->animations==NULL)
        node->animations = KBArrayNew(0);
    
    
    // remove any animation with same setter function
    for (int i=0; i<node->animations->num; i++) {
        KBAnimation *anim = node->animations->arr[i];
        if (KBAnimationEqual(animationObj, anim)) {
            KBAnimationStop(anim);
            KBArrayRemoveIndex(node->animations, i);
            
            if (node->scene) {
                KBSceneRemoveAnimation(node->scene, anim);
            }
            break;
        }
    }
    
    
    KBArrayPush(node->animations, animation);
    animation->node = node;
    
    if (node->scene) {
        KBSceneAddAnimation(node->scene, animation);
    }
}

void
KBNodeRemoveAnimation(KBType *nodeObj, KBType *animationObj) {
    KBNode *node = nodeObj;
    
    KBAnimationStop(animationObj);
    KBArrayRemoveObject(node->animations, animationObj);
    
    if (node->scene) {
        KBSceneRemoveAnimation(node->scene, animationObj);
    }
}

void
KBNodeRemoveAnimationWithName(KBType *nodeObj, int name) {
    KBNode *node = nodeObj;
    
    if (node->animations!=NULL) {
        for (int i=0; i<node->animations->num; i++) {
            KBAnimation *animation = node->animations->arr[i];
            
            if (animation->name == name) {
                KBAnimationStop(animation);
                KBArrayRemoveIndex(node->animations, i);
                
                if (node->scene) {
                    KBSceneRemoveAnimation(node->scene, animation);
                }
                break;
            }
        }
    }
}

void
KBNodeRemoveAllAnimations(KBType *nodeObj) {
    KBNode *node = nodeObj;
    if (node->animations!=NULL) {
        
        while (node->animations->num) {
            KBAnimation *animation = KBArrayGetLastObject(node->animations);
            
            KBAnimationStop(animation);
            KBArrayPop(node->animations);
            
            if (node->scene) {
                KBSceneRemoveAnimation(node->scene, animation);
            }
        }

    }
}

KBBool
KBNodeContainsAnimationWithName(KBType *nodeObj, int name) {
    KBNode *node = nodeObj;
    
    if (node->animations!=NULL) {
        for (int i=0; i<node->animations->num; i++) {
            KBAnimation *animation = node->animations->arr[i];
            if (animation->name == name) {
                return KBTrue;
            }
 
        }
    }
    return KBFalse;
}




