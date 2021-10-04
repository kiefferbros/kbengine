//
//  KBTypes.m
//  KBEngine
//
//  Created by Jonathan on 9/13/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "KBTypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#pragma mark - Random Float
static unsigned int mirand = 1;

void KBFloatRandomSeed(unsigned int seed) {
	mirand = MAX(seed, 1);
}

// returns range of [0, 1]
KBFloat KBFloatRandom(void) {
	//static unsigned int mirand = 1; //(unsigned int)(mach_absolute_time() & 0xFFFFFFFF);
	
	unsigned int a;
	
	mirand *= 16807;
	
	// mask out sign and exponent
	// set the exponent to 127 (which is offset to zero)
	a = (mirand&0x007fffff) | 0x3f800000;
	
	return ( *((KBFloat*)&a) - 1.f );
}

#pragma mark - Fast Sine and Cosine

// const float B = 4/KBPI;
// const float C = -4/(KBPISQR);
const KBFloat B			= 1.273239544735163f;
const KBFloat C			= -0.405284734569351f;


KBFloat KBFastSine(KBFloat x)
{
	//int hrots = x/KBPi;
	//int rots = x/KBPi2;
    
    //x = (x - rots*KBPi2) - (hrots%2)*KBPi2;	
    
    KBFloat hrots = (int)(x/KBPi)%2;
    KBFloat rots = (int)(x/KBPi2);
    
    x = (x - rots*KBPi2) - hrots*KBPi2;	
    
    KBFloat y = B * x + C * x * fabs(x);
    
	// extra precision
    //  const float Q = 0.775;
	const KBFloat P = 0.225;
	y = P * (y * fabs(y) - y) + y;   // Q * y + P * y * abs(y)
    
	
	return y;
}

KBFloat KBFastCosine(KBFloat x) {
	return KBFastSine(x+KBPiD2);
}

KBFloat KBSimpleSine(KBFloat x) {
	//int hrots = x/KBPi;
	//int rots = x/KBPi2;
	
	//x = (x - rots*KBPi2) - (hrots%2)*KBPi2;
    
    KBFloat hrots = (int)(x/KBPi)%2;
	KBFloat rots = (int)(x/KBPi2);
	
	x = (x - rots*KBPi2) - hrots*KBPi2;
	
    KBFloat y = B * x + C * x * fabs(x);
	
	return y;
}

KBFloat KBSimpleCosine(KBFloat x) {
	return KBSimpleSine(x+KBPiD2);
}

#pragma mark - KBObject
KBType * KBAlloc(long int size) {
    KBType *obj = malloc(size);
    memset(obj, 0, size);
    return obj;
}

KBType *
KBObjectInit(KBType *obj, KBDestroyFunc destroyFunc) {
    KBObject *object = obj;
    object->retainCount = 1;
    object->destroyFunc = destroyFunc;
    return obj;
}

KBType *
KBRetain(KBType *obj) {
    if (obj != NULL) {
        KBObject *object = obj;
#if DEBUG
        if (object->retainCount == 0) {
            printf("retaining released object");
        }
#endif
        object->retainCount++;
    }
    return obj;
}

void
KBRelease(KBType *obj) {
    if (obj != NULL) {
        KBObject *object = obj;
        object->retainCount--;
        if (object->retainCount==0) {
            if (object->destroyFunc) {
                object->destroyFunc(obj);
            }
            free(obj);
        }
#if DEBUG
        else if (object->retainCount<0) {
            printf("over-release!\n");
        }
#endif
    }
}

void
KBReleaseBatch(KBType **obj, int count) {
    for (int i=0; i<count; i++) {
        KBRelease(obj[i]);
    }
}

void
KBDescription(KBType *obj) {
    KBObject *object = obj;
    if (object->descripFunc) {
        printf("%s\n", object->descripFunc(obj));
    } else {
        printf("No description for object\n");
    }
}
