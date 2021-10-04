/* Copyright (c) 2007 Scott Lembcke
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef KBArray_h
#define KBArray_h

#include "KBTypes.h"

typedef struct KBArray {
    KBObject super;

	int num;
	int max;
	KBType **arr;
} KBArray;

#define KBArrayCast(obj)((KBArray*)obj)

typedef void (*KBArrayIter)(KBType *obj, void *data);

KBArray * KBArrayInit(KBArray *array, KBDestroyFunc destroyFunc, int size);
KBArray * KBArrayNew(int size);
KBArray * KBArrayNewWithObjects(KBType **objects, int count);
void KBArrayDestroy(KBType *array);

void KBArrayClear(KBArray *array);

void KBArrayPush(KBArray *array, KBType *object);
void KBArrayPop(KBArray *array);

void KBArrayInsert(KBArray *array, KBType *object, int idx);

void KBArrayRemoveIndex(KBArray *array, int idx);
void KBArrayRemoveObject(KBArray *array, KBType *obj);

void KBArrayShiftFrontToBack(KBArray* array);

void KBArrayAppend(KBArray *array, KBArray *other);
void KBArrayCull(KBArray *array, KBArray *other);
void KBArrayMerge(KBArray *array, KBArray *other);

void KBArrayEach(KBArray *array, KBArrayIter iterFunc, void *data);

int KBArrayGetFirstIndex(KBArray *array, KBType *obj);

KBBool KBArrayContains(KBArray *array, KBType *obj);

KBType * KBArrayGetObjectAtIndex(KBArray *array, int index);
KBType * KBArrayGetLastObject(KBArray *array);
int KBArrayGetCount(KBArray *array);

#endif
