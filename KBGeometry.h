//
//  KBGeometry.h
//  KBEngine
//
//  Created by Jonathan on 4/29/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "KBTypes.h"
#ifndef KBGeometry_h
#define KBGeometry_h

#pragma mark - Transform
static const KBTransform KBTransformIdentity = {1.0f,0.0f,0.0f,0.0f,
												0.0f,1.0f,0.0f,0.0f,
												0.0f,0.0f,1.0f,0.0f,
												0.0f,0.0f,0.0f,1.0f};

static inline KBBool
KBTransformIsAffineIdentity(const KBTransform *t) {
    return (t->a==1.f && t->b==0.f && t->c==0.f && t->d==1.f && t->x==0.f && t->y==0.f);
}

static inline void
KBTransformSetIdentity (KBTransform *t) {
    t->m11 = 1.f; t->m12 = 0.f; t->m13 = 0.f; t->m14 = 0.f;
    t->m21 = 0.f; t->m22 = 1.f; t->m23 = 0.f; t->m24 = 0.f;
    t->m31 = 0.f; t->m32 = 0.f; t->m33 = 1.f; t->m34 = 0.f;
    t->m41 = 0.f; t->m42 = 0.f; t->m43 = 0.f; t->m44 = 1.f;
}

static inline void
KBTransformSetAffine (KBTransform *t, KBVec2 p, KBFloat r, KBVec2 s, KBVec2 a) {
    KBTransformSetIdentity(t);
	KBFloat sr, cr;
	
	sr = KBFastSine(r);
	cr = KBFastCosine(r);
	
	t->m11 = s.x*cr;  t->m12 = s.x*sr;
	t->m21 = -s.y*sr; t->m22 = s.y*cr;
	t->m41 = -a.x*t->m11-a.y*t->m21+p.x;
	t->m42 = -a.x*t->m12-a.y*t->m22+p.y;
}

static inline void
KBTransformAffineSetSimple (KBTransform *t, KBVec2 p, KBFloat r) {
	//KBTransform t = KBTransformIdentity;
    KBTransformSetIdentity(t);
	KBFloat sr, cr;
	
	sr = KBFastSine(r);
	cr = KBFastCosine(r);
	
	t->m11 = cr;  t->m12 = sr;
	t->m21 = sr;  t->m22 = cr;
	t->m41 = p.x;
	t->m42 = p.y;
}

// result p can point to a or b as well third transform
static inline void
KBTransformAffineMult (KBTransform *a, KBTransform *b, KBTransform *p) {
    
    
    KBFloat m11, m12, m21, m22, m41, m42;
    m11 = a->m11*b->m11 + a->m12*b->m21; m12 = a->m11*b->m12 + a->m12*b->m22;
    m21 = a->m21*b->m11 + a->m22*b->m21; m22 = a->m21*b->m12 + a->m22*b->m22;
    m41 = a->m41*b->m11 + a->m42*b->m21 + b->m41;
    m42 = a->m41*b->m12 + a->m42*b->m22 + b->m42;
    
    p->m11 = m11; p->m12 = m12;
    p->m21 = m21; p->m22 = m22;
    p->m41 = m41; p->m42 = m42;
    
    
	//KBTransform result = {
	//	a.m11*b.m11 + a.m12*b.m21,				/**/ a.m11*b.m12 + a.m12*b.m22,		         /**/ 0.f,	 /**/ 0.f,
		
	//	a.m21*b.m11 + a.m22*b.m21,				/**/ a.m21*b.m12 + a.m22*b.m22,		         /**/ 0.f,	 /**/ 0.f,
	
	//	0.f,									/**/ 0.f,							         /**/ 1.f,	 /**/ 0.f,
		 
	//	a.m41*b.m11 + a.m42*b.m21 + b.m41,      /**/ a.m41*b.m12 + a.m42*b.m22 + b.m42,      /**/ 0.f,	 /**/ 1.f
	//};

}

static inline KBBool
KBTransformAffineInvert (KBTransform *t) {
    KBFloat det = t->m11*t->m22 - t->m12*t->m21;
    
    if (det == 0.f) return 0; // no inverse exists
    det = 1.f/det; // inverse
    
    KBFloat C, F;

    C = t->m21*t->m42 - t->m22*t->m41;
    F = t->m41*t->m12 - t->m11*t->m42;
    
    t->m11 = t->m22*det; t->m12 = -t->m12*det;
    t->m21 = -t->m21*det; t->m22 = t->m11*det;
    t->m41 = C*det; t->m42 = F*det;
    
    return 1;
}

KBTransform KBTransform3DMult (KBTransform m1, KBTransform m2);

static inline KBBool
KBTransform3DInvert (KBTransform *t) {

    KBFloat inv[16], *m, det;
    m = t->m;
    
    inv[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15] + m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
    inv[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15] - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
    inv[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15] + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
    inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14] - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
    inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15] - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
    inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15] + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
    inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15] - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
    inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14] + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
    inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15] + m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
    inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15] - m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
    inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15] + m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
    inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14] - m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
    inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11] - m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
    inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11] + m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
    inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11] - m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
    inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10] + m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];
    
    det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
    if (det == 0) return 0;
    
    det = 1.0 / det;
    
    m[0] = inv[0] * det; m[1] = inv[1] * det; m[2] = inv[2] * det; m[3] = inv[3] * det;
    m[4] = inv[4] * det; m[5] = inv[5] * det; m[6] = inv[6] * det; m[7] = inv[7] * det;
    m[8] = inv[8] * det; m[9] = inv[9] * det; m[10] = inv[10] * det; m[11] = inv[11] * det;
    m[12] = inv[12] * det; m[13] = inv[13] * det; m[14] = inv[14] * det; m[15] = inv[15] * det;
        
    return 1;
}

#pragma mark - Vec2

static const KBVec2 KBVec2Zero={0.0f,0.0f};
static const KBVec2 KBVec2One = {1.f, 1.f};

static inline KBVec2
KBVec2Make(KBFloat x, KBFloat y) {
	KBVec2 v = {x, y};
	return v;
}

static inline KBVec2
KBVec2MakeUniform (KBFloat s) {
	KBVec2 v = {s, s};
	return v;
}

static inline KBVec2
KBVec2Neg(const KBVec2 v)
{
	return KBVec2Make(-v.x, -v.y);
}

static inline KBVec2
KBVec2Add(const KBVec2 v1, const KBVec2 v2)
{
	return KBVec2Make(v1.x + v2.x, v1.y + v2.y);
}

static inline KBVec2
KBVec2Sub(const KBVec2 v1, const KBVec2 v2)
{
	return KBVec2Make(v1.x - v2.x, v1.y - v2.y);
}

static inline KBVec2
KBVec2Mult(const KBVec2 v1, const KBVec2 v2)
{
	return KBVec2Make(v1.x*v2.x, v1.y*v2.y);
}

static inline KBVec2
KBVec2Div (const KBVec2 v1, const KBVec2 v2)
{
	return KBVec2Make(v1.x/v2.x, v1.y/v2.y);
}

static inline KBVec2
KBVec2Scale(const KBVec2 v, const KBFloat s)
{
	return KBVec2Make(v.x*s, v.y*s);
}

static inline KBVec2
KBVec2DivScale(const KBVec2 v, const KBFloat s)
{
	return KBVec2Make(v.x/s, v.y/s);
}

static inline KBFloat
KBVec2Dot(const KBVec2 v1, const KBVec2 v2)
{
	return v1.x*v2.x + v1.y*v2.y;
}

static inline KBFloat
KBVec2Cross(const KBVec2 v1, const KBVec2 v2)
{
	return v1.x*v2.y - v1.y*v2.x;
}

static inline KBVec2
KBVec2Perp(const KBVec2 v)
{
	return KBVec2Make(-v.y, v.x);
}

static inline KBVec2
KBVec2RevPerp(const KBVec2 v)
{
	return KBVec2Make(v.y, -v.x);
}

static inline KBVec2
KBVec2Project(const KBVec2 v1, const KBVec2 v2)
{
	return KBVec2Scale(v2, KBVec2Dot(v1, v2)/KBVec2Dot(v2, v2));
}

static inline KBVec2
KBVec2Rotate(const KBVec2 v1, const KBVec2 v2)
{
	return KBVec2Make(v1.x*v2.x - v1.y*v2.y, v1.x*v2.y + v1.y*v2.x);
}

static inline KBVec2
KBVec2Unrotate(const KBVec2 v1, const KBVec2 v2)
{
	return KBVec2Make(v1.x*v2.x + v1.y*v2.y, v1.y*v2.x - v1.x*v2.y);
}

// non-inlined functions
KBFloat KBVec2Length(const KBVec2 v);
KBVec2 KBVec2Slerp(const KBVec2 v1, const KBVec2 v2, const KBFloat t);
KBVec2 KBVec2SlerpConst(const KBVec2 v1, const KBVec2 v2, const KBFloat a);
KBVec2 KBVec2FromAngle(const KBFloat a); // convert radians to a normalized vector
KBFloat KBVec2ToAngle(const KBVec2 v); // convert a vector to radians

static inline KBFloat
KBVec2LengthSq(const KBVec2 v)
{
	return KBVec2Dot(v, v);
}

static inline KBVec2
KBVec2Lerp(const KBVec2 v1, const KBVec2 v2, const KBFloat t)
{
	return KBVec2Make(KBFloatLerp(v1.x, v2.x, t), KBFloatLerp(v1.y, v2.y, t));
}

static inline KBVec2
KBVec2Werp(const KBVec2 v1, const KBVec2 v2, KBFloat t, KBFloat aT, KBFloat aV, KBFloat bT, KBFloat bV)
{
	return KBVec2Make(KBFloatWerp(v1.x, v2.x, t, aT, aV, bT, bV), KBFloatWerp(v1.y, v2.y, t, aT, aV, bT, bV));
}

static inline KBVec2
KBVec2Normalize(const KBVec2 v)
{
	return KBVec2DivScale(v, KBVec2Length(v));
}

static inline KBVec2
KBVec2Clamp(const KBVec2 v, const KBFloat len)
{
	return (KBVec2Dot(v,v) > len*len) ? KBVec2Scale(KBVec2Normalize(v), len) : v;
}

static inline KBVec2
KBVec2LerpConst(KBVec2 v1, KBVec2 v2, KBFloat d)
{
	return KBVec2Add(v1, KBVec2Clamp(KBVec2Sub(v2, v1), d));
}

static inline KBFloat
KBVec2Dist(const KBVec2 v1, const KBVec2 v2)
{
	return KBVec2Length(KBVec2Sub(v1, v2));
}

static inline KBFloat
KBVec2DistSq(const KBVec2 v1, const KBVec2 v2)
{
	return KBVec2LengthSq(KBVec2Sub(v1, v2));
}

static inline KBVec2 
KBVec2Transform(KBVec2 v, KBTransform *t) {
	KBVec2 ov;
	ov.x = t->m11*v.x + t->m21*v.y + t->m41;
	ov.y = t->m12*v.x + t->m22*v.y + t->m42;
	return ov;
}

static inline int 
KBVec2Equal (KBVec2 v1, KBVec2 v2) {
	return (v1.x == v2.x && v1.y == v2.y);
}

KBVec2 KBVec2Round (KBVec2 v);

void KBVec2Print(const KBVec2 v);

#pragma mark - Vec3
static inline KBVec3
KBVec3Make (const KBFloat x, const KBFloat y, const KBFloat z) {
    KBVec3 v = {x, y, z};
	return v;
}

static inline KBVec3
KBVec3MakeUniform (const KBFloat s) {
    KBVec3 v = {s, s, s};
	return v;
}

static inline KBVec3
KBVec3Neg (KBVec3 v) {
	return KBVec3Make(-v.x, -v.y, -v.z);
}

static inline KBVec3
KBVec3Add (KBVec3 v1, KBVec3 v2) {
	return KBVec3Make(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

static inline KBVec3
KBVec3Sub (KBVec3 v1, KBVec3 v2) {
	return KBVec3Make(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

static inline KBVec3
KBVec3Mult(const KBVec3 v1, const KBVec3 v2) {
	return KBVec3Make(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

static inline KBVec3
KBVec3Div(const KBVec3 v1, const KBVec3 v2) {
	return KBVec3Make(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z);
}

static inline KBVec3
KBVec3Scale(const KBVec3 v1, const KBFloat s) {
	return KBVec3Make(v1.x*s, v1.y*s, v1.z*s);
}

static inline KBVec3
KBVec3DivScale(const KBVec3 v, const KBFloat s)
{
	return KBVec3Make(v.x/s, v.y/s, v.z/s);
}

static inline KBFloat
KBVec3Dot(KBVec3 v1, KBVec3 v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

static inline KBVec3
KBVec3Cross(KBVec3 v1, KBVec3 v2) {
	return KBVec3Make(v1.y*v2.z - v1.z*v2.y,
					   v1.z*v2.x - v1.x*v2.z,
					   v1.x*v2.y - v1.y*v2.x);
}

KBFloat KBVec3Length(KBVec3 v);

static inline KBFloat 
KBVec3LengthSq(KBVec3 v) {
	return KBVec3Dot(v, v);
}

static inline KBVec3
KBVec3DNormalize(const KBVec3 v)
{
	return KBVec3DivScale(v, KBVec3Length(v));
}

static inline KBFloat
KBVec3Dist(const KBVec3 v1, const KBVec3 v2)
{
	return KBVec3Length(KBVec3Sub(v1, v2));
}

static inline KBFloat
KBVec3DistSq(const KBVec3 v1, const KBVec3 v2)
{
	return KBVec3LengthSq(KBVec3Sub(v1, v2));
}

static inline KBVec3
KBVec3Transform(KBVec3 v, KBTransform t) {	
	return KBVec3Make(v.x*t.m11 + v.y*t.m21 + v.z*t.m31 + t.m41,
                      v.x*t.m12 + v.y*t.m22 + v.z*t.m32 + t.m42,
                      v.x*t.m13 + v.y*t.m23 + v.z*t.m33 + t.m43);
}

static inline KBVec3
KBVec3Lerp(const KBVec3 v1, const KBVec3 v2, const KBFloat t)
{
	return KBVec3Make(KBFloatLerp(v1.x, v2.x, t), 
                      KBFloatLerp(v1.y, v2.y, t),
                      KBFloatLerp(v1.z, v2.z, t));
}

static inline KBVec3
KBVec3Werp(const KBVec3 v1, const KBVec3 v2, KBFloat t, KBFloat aT, KBFloat aV, KBFloat bT, KBFloat bV)
{
	return KBVec3Make(KBFloatWerp(v1.x, v2.x, t, aT, aV, bT, bV), 
                      KBFloatWerp(v1.y, v2.y, t, aT, aV, bT, bV), 
                      KBFloatWerp(v1.z, v2.z, t, aT, aV, bT, bV));
}

#pragma mark - Vec4
static inline KBVec4
KBVec4Make (const KBFloat x, const KBFloat y, const KBFloat z, const KBFloat w) {
    KBVec4 v = {x, y, z, w};
	return v;
}

static inline KBVec4
KBVec4Add (KBVec4 v1, KBVec4 v2) {
	return KBVec4Make(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

static inline KBVec4
KBVec4Mult(const KBVec4 v1, const KBVec4 v2)
{
	return KBVec4Make(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w);
}

static inline KBVec4
KBVec4Scale(const KBVec4 v, const KBFloat s)
{
	return KBVec4Make(v.x*s, v.y*s, v.z*s, v.w*s);
}

static inline KBVec4
KBVec4Lerp(const KBVec4 v1, const KBVec4 v2, const KBFloat t)
{
	return KBVec4Make(KBFloatLerp(v1.x, v2.x, t), KBFloatLerp(v1.y, v2.y, t), KBFloatLerp(v1.z, v2.z, t), KBFloatLerp(v1.w, v2.w, t));
}

static inline KBVec4
KBVec4Werp(const KBVec4 v1, const KBVec4 v2, KBFloat t, KBFloat aT, KBFloat aV, KBFloat bT, KBFloat bV)
{
	return KBVec4Make(KBFloatWerp(v1.x, v2.x, t, aT, aV, bT, bV),
                      KBFloatWerp(v1.y, v2.y, t, aT, aV, bT, bV),
                      KBFloatWerp(v1.z, v2.z, t, aT, aV, bT, bV),
                      KBFloatWerp(v1.w, v2.w, t, aT, aV, bT, bV));
}


#pragma mark - Box
static const KBBox KBBoxZero={0.0f,0.0f,0.0f,0.0f};

static inline KBBox
KBBoxMake(const KBFloat l, const KBFloat b,
		  const KBFloat r, const KBFloat t)
{
	KBBox bb = {l, b, r, t};
	return bb;
}

static inline KBBox
KBBoxMake2(const KBVec2 p, const KBVec2 s) {
    KBBox b = {p.x, p.y, p.x+s.w, p.y+s.h};
    return b;
}

static inline int
KBBoxIntersects(const KBBox a, const KBBox b)
{
	return (a.l<=b.r && b.l<=a.r && a.b<=b.t && b.b<=a.t);
}

static int
KBBoxOverlaps(const KBBox a, const KBBox b)
{
	return (a.l<b.r && b.l<a.r && a.b<b.t && b.b<a.t);
}

static inline int
KBBoxContainsBox(const KBBox bb, const KBBox other)
{
	return (bb.l < other.l && bb.r > other.r && bb.b < other.b && bb.t > other.t);
}

static inline int
KBBoxContainsVec(const KBBox bb, const KBVec2 v)
{
	return (bb.l < v.x && bb.r > v.x && bb.b < v.y && bb.t > v.y);
}

static inline KBBox
KBBoxMerge(const KBBox a, const KBBox b){
	return KBBoxMake(
				   KBFloatMin(a.l, b.l),
				   KBFloatMin(a.b, b.b),
				   KBFloatMax(a.r, b.r),
				   KBFloatMax(a.t, b.t)
				   );
}

static inline KBBox
KBBoxExpand(const KBBox bb, const KBVec2 v){
	return KBBoxMake(
				   KBFloatMin(bb.l, v.x),
				   KBFloatMin(bb.b, v.y),
				   KBFloatMax(bb.r, v.x),
				   KBFloatMax(bb.t, v.y)
				   );
}

static inline KBFloat 
KBBoxGetWidth (KBBox box) {
	return box.r-box.l;
}

static inline KBFloat 
KBBoxGetHeight (KBBox box) {
	return box.t-box.b;
}

static inline KBVec2
KBBoxGetSize (KBBox box) {
    KBVec2 s = {box.r-box.l, box.t-box.b};
    return s;
}

static inline KBVec2
KBBoxGetOrigin(KBBox box) {
    KBVec2 p = {box.l, box.r};
    return p;
}

static inline KBVec2
KBBoxGetCenter (KBBox box) {
    KBVec2 c = {KBFloatLerp(box.l, box.r, 0.5f), KBFloatLerp(box.b, box.t, 0.5f)};
    return c;
}

static inline KBBox
KBBoxInset(KBBox box, KBFloat inset) {
	return KBBoxMake(box.l+inset, box.b+inset, box.r-inset, box.t-inset);
}

static inline KBBox
KBBoxOffset(KBBox box, KBVec2 offset) {
	return KBBoxMake(box.l+offset.x, box.b+offset.y, box.r+offset.x, box.t+offset.y);
}

KBVec2 KBBoxGetCorner(KBBox bb, int corner);
KBBox KBBoxTransform(KBBox bb, KBTransform *t);

KBVec2 KBBoxClampVec(const KBBox bb, const KBVec2 v); // clamps the vector to lie within the bbox


void KBBoxPrint(const KBBox box);

#endif
