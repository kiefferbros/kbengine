//
//  KBCamera.c
//  rope
//
//  Created by Jonathan Kieffer on 12/20/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#include "KBCamera.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "KBGeometry.h"


KBCamera *
KBCameraInit(KBCamera *camera, KBDestroyFunc destroyFunc, KBVec2 size, KBBox viewport, KBFloat zoom) {
    KBNodeInit((KBNode*)camera, destroyFunc);
    
    camera->pos = KBVec2Zero;
    camera->zoom = zoom;
    camera->transform = KBTransformIdentity;
    camera->transformFlag = KBTrue;
    camera->viewport = viewport;
    camera->size = size;
    
    KBCameraShape(camera);
    
    return camera;
}

KBCamera *
KBCameraNew(KBVec2 size, KBBox viewport, KBFloat zoom) {
    return KBCameraInit(KBAlloc(sizeof(KBCamera)), &KBCameraDestroy, size, viewport, zoom);
}

void
KBCameraDestroy(KBType *cameraObj) {
    KBNodeDestroy(cameraObj);
}


void
KBCameraShape(KBCamera *camera) {
    //Set up OpenGL projection matrix
	glViewport(camera->viewport.l, camera->viewport.b, camera->viewport.r, camera->viewport.t);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	

#ifdef GL_VERSION_ES_CM_1_1
	glOrthof(0, camera->size.x, 0, camera->size.y, -1, 1);
#else
	glOrtho(0, camera->size.x, 0, camera->size.y, -1, 1);
#endif

	
	glMatrixMode(GL_MODELVIEW);
}


void
KBCameraReshape(KBCamera *camera, KBVec2 size, KBBox viewport) {
    camera->viewport = viewport;
    camera->size = size;
    
    KBCameraShape(camera);
}

void
KBCameraLook(KBCamera *camera) {
    if (camera->transformFlag) {
        camera->transform.a = camera->zoom;
        camera->transform.d = camera->zoom;
        camera->transform.x = -camera->pos.x*camera->zoom;
        camera->transform.y = -camera->pos.y*camera->zoom;
        camera->transformFlag = KBFalse;
    }
    
    glLoadMatrixf(camera->transform.m);
}

void
KBCameraSetPosition(KBCamera *camera, KBVec2 pos) {
    camera->pos = pos;
    camera->transformFlag = KBTrue;
}

void KBCameraSetAnimatedPosition(KBType *cameraObj, void *posPtr) {
    KBCamera *camera = cameraObj;
    memcpy(&camera->pos, posPtr, sizeof(KBVec2));
    camera->transformFlag = KBTrue;
}

void
KBCameraSetZoom(KBCamera *camera, KBFloat zoom) {
    camera->zoom = zoom;
    camera->transformFlag = KBTrue;
}

KBVec2
KBCameraLocalToGlobal(KBCamera *camera, KBVec2 screen) {
    return KBVec2Add(KBVec2DivScale(screen, camera->zoom), camera->pos);
}

KBVec2 KBCameraViewportToGlobal(KBCamera *camera, KBVec2 viewport) {
    KBVec2 screen = KBVec2Mult(KBVec2Div(viewport, KBBoxGetSize(camera->viewport)), camera->size);
    return KBCameraLocalToGlobal(camera, screen);
}

KBBox
KBCameraGetViewBox(KBCamera *cam) {
    return KBBoxMake(cam->pos.x, cam->pos.y, cam->pos.x+cam->size.w/cam->zoom, cam->pos.y+cam->size.h/cam->zoom);
}



