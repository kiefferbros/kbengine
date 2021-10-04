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


#include "KBArray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


KBArray *
KBArrayInit(KBArray *array, KBDestroyFunc destroyFunc, int size) {
    KBObjectInit(array, destroyFunc);
    array->num = 0;
    array->max = (size ? size : 4);
    array->arr = (void **)calloc(array->max, sizeof(KBType*));
    return array;
}

KBArray*
KBArrayNew(int size)
{    
    return KBArrayInit(KBAlloc(sizeof(KBArray)), &KBArrayDestroy, size);
}

KBArray * KBArrayNewWithObjects(KBType **objects, int count) {
    KBArray *array = KBArrayNew(count);
    array->num = count;
    for (int i=0; i<count; i++) {
        array->arr[i] = KBRetain(objects[i]);
    }
    return array;
}

void
KBArrayDestroy(KBType *obj) {
    KBArray *array = obj;
    for (int i=0; i<array->num; i++) {
        KBRelease(array->arr[i]);
    }
    free(array->arr);
}

void
KBArrayClear(KBArray *array) {
    for (int i=0; i<array->num; i++) {
        KBRelease(array->arr[i]);
        array->arr[i] = NULL;
    }
    array->num = 0;
}

void
KBArrayPush(KBArray *arr, KBType *object)
{
	if(arr->num == arr->max){
		arr->max *= 2;
		arr->arr = (void **)realloc(arr->arr, arr->max*sizeof(KBType*));
	}
	
	arr->arr[arr->num] = KBRetain(object);
	arr->num++;
}

void
KBArrayPop(KBArray *arr)
{
	arr->num--;
	
	KBType *value = arr->arr[arr->num];
    KBRelease(value);
	arr->arr[arr->num] = NULL;
}

void
KBArrayInsert(KBArray *array, KBType *object, int idx) {
    if (array->num == array->max){
		array->max++;
		array->arr = (void **)realloc(array->arr, array->max*sizeof(KBType*));
	}
    
    for (int i=array->num; i>idx; i--) {
        array->arr[i] = array->arr[i-1];
    }
    
    array->arr[idx] = KBRetain(object);
    array->num++;
}

void
KBArrayRemoveIndex(KBArray *array, int idx)
{
	array->num--;
    
	KBType *value = array->arr[idx];
    KBRelease(value);
    
    /* keep the array ordered */
    for (int i=idx; i<array->num; i++) {
        array->arr[i] = array->arr[i+1];
    }
}

void
KBArrayRemoveObject(KBArray *array, void *obj)
{
	for(int i=0; i<array->num; i++){
		if(array->arr[i] == obj){
			KBArrayRemoveIndex(array, i);
			return;
		}
	}
}

int
KBArrayGetFirstIndex(KBArray *array, KBType *obj) {
    for(int i=0; i<array->num; i++){
		if(array->arr[i] == obj){
			return i;
		}
	}
    
    return -1;
}

void
KBArrayShiftFrontToBack(KBArray* array) {
    KBType *first = array->arr[0];
    for (int i=0; i<array->num-1; i++) {
        array->arr[i] = array->arr[i+1];
    }
    array->arr[array->num-1] = first;
}

void
KBArrayAppend(KBArray *array, KBArray *other)
{
	
	int oldNum = array->num;
	array->num += other->num;
	if(array->num >= array->max){
		array->max = array->num;
		array->arr = (void **)realloc(array->arr, array->max*sizeof(KBType*));
	}
    
    KBType **tail = array->arr + oldNum;
	
    for (int i=0; i<other->num; i++) {
        tail[i] = KBRetain(other->arr[i]);
    }
}

void
KBArrayMerge(KBArray *array, KBArray *other) {
    
	
    int oldNum = array->num;
    int newNum = oldNum + other->num;
	if(newNum >= array->max){
		array->max = newNum;
		array->arr = (void **)realloc(array->arr, array->max*sizeof(KBType*));
	}
    
    KBType **tail = array->arr + oldNum;
	
    for (int i=0; i<other->num; i++) {
        
        /* does it already have this object? */
        KBBool add = KBTrue;
        for(int j=0; j<oldNum; j++) {
            if(array->arr[j] == other->arr[i]) {
                printf("already has this object\n");
                add = KBFalse;
                break;
            }
        }
        
        /* if not, then append it */
        if (add) {
            tail[i] = KBRetain(other->arr[i]);
            array->num++;
        }
        
    }
}

void KBArrayCull(KBArray *array, KBArray *other) {    
    for (int i=0; i<other->num; i++) {
        for (int j=0; j<array->num; j++) {
            if (array->arr[j] == other->arr[i]) {
                KBArrayRemoveIndex(array, j);
                break;
            }
        }
    }
}

void
KBArrayEach(KBArray *arr, KBArrayIter iterFunc, void *data)
{
	for(int i=0; i<arr->num; i++)
		iterFunc(arr->arr[i], data);
}

KBBool
KBArrayContains(KBArray *arr, void *ptr)
{
	for(int i=0; i<arr->num; i++)
		if(arr->arr[i] == ptr) return 1;
	
	return 0;
}

KBType *
KBArrayGetObjectAtIndex(KBArray *array, int index) {
    return array->arr[index];
}

KBType *
KBArrayGetLastObject(KBArray *array) {
    return array->arr[array->num-1];
}

int
KBArrayGetCount(KBArray *array) {
    return array ? array->num : -1;
}

