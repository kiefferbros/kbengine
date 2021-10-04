//
//  KBGeometry.c
//  KBEngine
//
//  Created by Jonathan on 4/29/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "KBGeometry.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#pragma mark - Vec2
KBVec2 KBVec2Round (KBVec2 v) {
	return KBVec2Make(roundf(v.x), roundf(v.y));
}

KBFloat
KBVec2Length(const KBVec2 v)
{
	return sqrtf( KBVec2Dot(v, v) );
}

KBVec2
KBVec2Slerp(const KBVec2 v1, const KBVec2 v2, const KBFloat t)
{
	KBFloat omega = acosf(KBVec2Dot(v1, v2));
	
	if(omega){
		KBFloat denom = 1.0f/sinf(omega);
		return KBVec2Add(KBVec2Scale(v1, sinf((1.0f - t)*omega)*denom), KBVec2Scale(v2, sinf(t*omega)*denom));
	} else {
		return v1;
	}
}

KBVec2
KBVec2SlerpConst(const KBVec2 v1, const KBVec2 v2, const KBFloat a)
{
	KBFloat angle = acosf(KBVec2Dot(v1, v2));
	return KBVec2Slerp(v1, v2, KBFloatMin(a, angle)/angle);
}

KBVec2
KBVec2FromAngle(const KBFloat a)
{
	return KBVec2Make(cosf(a), sinf(a));
}

KBFloat
KBVec2ToAngle(const KBVec2 v)
{
	return atan2f(v.y, v.x);
}

void KBVec2Print(const KBVec2 v) {
	printf("KBVec(%f, %f)\n", v.x, v.y);
}

#pragma mark - Vec3
KBFloat
KBVec3Length(const KBVec3 v)
{
	return sqrtf( KBVec3Dot(v, v) );
}

#pragma mark - Transform

#pragma mark - Transform 3D
KBTransform KBTransform3DMult (KBTransform m1, KBTransform m2) {
    KBTransform result;
    result.m[0] = m1.m[0] * m2.m[0] + m1.m[4] * m2.m[1] + m1.m[8] * m2.m[2] + m1.m[12] * m2.m[3];
    result.m[1] = m1.m[1] * m2.m[0] + m1.m[5] * m2.m[1] + m1.m[9] * m2.m[2] + m1.m[13] * m2.m[3];
    result.m[2] = m1.m[2] * m2.m[0] + m1.m[6] * m2.m[1] + m1.m[10] * m2.m[2] + m1.m[14] * m2.m[3];
    result.m[3] = m1.m[3] * m2.m[0] + m1.m[7] * m2.m[1] + m1.m[11] * m2.m[2] + m1.m[15] * m2.m[3];
    
    result.m[4] = m1.m[0] * m2.m[4] + m1.m[4] * m2.m[5] + m1.m[8] * m2.m[6] + m1.m[12] * m2.m[7];
    result.m[5] = m1.m[1] * m2.m[4] + m1.m[5] * m2.m[5] + m1.m[9] * m2.m[6] + m1.m[13] * m2.m[7];
    result.m[6] = m1.m[2] * m2.m[4] + m1.m[6] * m2.m[5] + m1.m[10] * m2.m[6] + m1.m[14] * m2.m[7];
    result.m[7] = m1.m[3] * m2.m[4] + m1.m[7] * m2.m[5] + m1.m[11] * m2.m[6] + m1.m[15] * m2.m[7];
    
    result.m[8] = m1.m[0] * m2.m[8] + m1.m[4] * m2.m[9] + m1.m[8] * m2.m[10] + m1.m[12] * m2.m[11];
    result.m[9] = m1.m[1] * m2.m[8] + m1.m[5] * m2.m[9] + m1.m[9] * m2.m[10] + m1.m[13] * m2.m[11];
    result.m[10] = m1.m[2] * m2.m[8] + m1.m[6] * m2.m[9] + m1.m[10] * m2.m[10] + m1.m[14] * m2.m[11];
    result.m[11] = m1.m[3] * m2.m[8] + m1.m[7] * m2.m[9] + m1.m[11] * m2.m[10] + m1.m[15] * m2.m[11];
    
    result.m[12] = m1.m[0] * m2.m[12] + m1.m[4] * m2.m[13] + m1.m[8] * m2.m[14] + m1.m[12] * m2.m[15];
    result.m[13] = m1.m[1] * m2.m[12] + m1.m[5] * m2.m[13] + m1.m[9] * m2.m[14] + m1.m[13] * m2.m[15];
    result.m[14] = m1.m[2] * m2.m[12] + m1.m[6] * m2.m[13] + m1.m[10] * m2.m[14] + m1.m[14] * m2.m[15];
    result.m[15] = m1.m[3] * m2.m[12] + m1.m[7] * m2.m[13] + m1.m[11] * m2.m[14] + m1.m[15] * m2.m[15];
    
    return result;
}

#pragma mark - Box
KBVec2
KBBoxClampVec(const KBBox bb, const KBVec2 v)
{
	KBFloat x = KBFloatClamp(v.x, bb.l, bb.r);
	KBFloat y = KBFloatClamp(v.y, bb.b, bb.t);
	return KBVec2Make(x, y);
}

KBBox 
KBBoxTransform(KBBox bb, KBTransform *t) {
	KBBox bbT;
	KBVec2 corners[4] = {
		KBVec2Transform(KBVec2Make(bb.l, bb.b), t),
		KBVec2Transform(KBVec2Make(bb.l, bb.t), t),
		KBVec2Transform(KBVec2Make(bb.r, bb.b), t),
		KBVec2Transform(KBVec2Make(bb.r, bb.t), t)
	};
	
	bbT = KBBoxMake(corners[0].x, corners[0].y, corners[0].x, corners[0].y);
	for (int i=1;i<4;++i) {
		if (corners[i].x < bbT.l) bbT.l = corners[i].x;
		else if (corners[i].x > bbT.r) bbT.r = corners[i].x;
		if (corners[i].y < bbT.b) bbT.b = corners[i].y;
		else if (corners[i].y > bbT.t) bbT.t = corners[i].y;
	}
	
	return bbT;	
}

KBVec2 
KBBoxGetCorner(KBBox bb, int corner) {
	KBVec2 v;
	switch (corner) {
		case 0:
			v = KBVec2Make(bb.l,bb.b);
			break;
		case 1:
			v = KBVec2Make(bb.l,bb.t);
			break;
		case 2:
			v = KBVec2Make(bb.r,bb.b);
			break;
		case 3:
			v = KBVec2Make(bb.r,bb.t);
			break;
	}
	
	return v;
}

void KBBoxPrint(const KBBox box) {
	printf("KBBox(%f, %f, %f, %f)\n", box.l, box.b, box.r, box.t);
}