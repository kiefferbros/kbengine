//
//  KBDictionary.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/31/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBDictionary_h
#define KBDictionary_h

#include "KBArray.h"

typedef struct _KBDictEntry {
    KBObject    super;
    
    KBType      *value;
    char        *key;
} KBDictEntry;

typedef struct _KBDictionary {
    KBObject    super;
    
    KBArray     *entries;
} KBDictionary;

#define KBDictionaryCast(obj)((KBDictionary*)obj)

KBDictionary * KBDictionaryInit(KBDictionary *dictionary, KBDestroyFunc destroyFunc, int size);
KBDictionary * KBDictionaryNew(int size);
void KBDictionaryDestroy(KBType *dictionary);

void KBDictionarySetValue(KBDictionary *dictionary, KBType *value, const char *key);
void KBDictionaryRemoveKey(KBDictionary *dictionary, const char *key);

KBType * KBDictionaryGetValue(KBDictionary *dictionary, const char *key);

#endif
