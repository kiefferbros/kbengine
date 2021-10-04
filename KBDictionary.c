//
//  KBDictionary.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 12/31/12.
//  Copyright (c) 2012 Kieffer Bros., LLC. All rights reserved.
//

#include "KBDictionary.h"
#include <stdlib.h>
#include <string.h>




KBDictEntry * KBDictEntryInit(KBDictEntry *entry, KBDestroyFunc destroyFunc, KBType *value, const char *key);
KBDictEntry * KBDictEntryNew(KBType *value, const char *key);
void KBDictEntryDestroy(KBType *entry);

#pragma mark - Entry
KBDictEntry *
KBDictEntryInit(KBDictEntry *entry, KBDestroyFunc destroyFunc, KBType *value, const char *key) {
    KBObjectInit(entry, destroyFunc);
    
    entry->value = KBRetain(value);
    
    entry->key = malloc(strlen(key)+1);
    strcpy(entry->key, key);
    
    return entry;
}

KBDictEntry *
KBDictEntryNew(KBType *value, const char *key) {
    return KBDictEntryInit(KBAlloc(sizeof(KBDictEntry)), &KBDictEntryDestroy, value, key);
}

void
KBDictEntryDestroy(KBType *obj) {
    KBDictEntry *entry = obj;
    
    KBRelease(entry->value);
    free(entry->key);
}


#pragma mark - Dictionary

KBDictionary *
KBDictionaryInit(KBDictionary *dictionary, KBDestroyFunc destroyFunc, int size) {
    KBObjectInit(dictionary, destroyFunc);
    
    dictionary->entries = KBArrayNew(size);
    return dictionary;
}

KBDictionary *
KBDictionaryNew(int size) {
    return KBDictionaryInit(KBAlloc(sizeof(KBDictionary)), &KBDictionaryDestroy, size);
}

void
KBDictionaryDestroy(KBType *obj) {
    KBDictionary *dictionary = obj;
    KBRelease(dictionary->entries);
}


KBDictEntry *KBDictionaryGetEntryWithKey(KBDictionary *dictionary, const char *key) {
    KBDictEntry *entry = NULL;
    for (int i=0; i<dictionary->entries->num; i++) {
        KBDictEntry *e = dictionary->entries->arr[i];
        if (strcmp(e->key, key)==0) {
            entry = e;
            break;
        }
    }
    return entry;
}

void
KBDictionarySetValue(KBDictionary *dictionary, KBType *value, const char *key) {
    /* does an entry exist already? */
    KBDictEntry *entry = KBDictionaryGetEntryWithKey(dictionary, key);
    
    if (entry != NULL) {
        /* the entry already exists so replace it */
        KBRelease(entry->value);
        entry->value = KBRetain(value);
    } else {
        /* create a new entry */
        entry = KBDictEntryNew(value, key);
        KBArrayPush(dictionary->entries, entry);
        KBRelease(entry);
    }
    
}

void
KBDictionaryRemoveKey(KBDictionary *dictionary, const char *key) {
    for (int i=0; i<dictionary->entries->num; i++) {
        KBDictEntry *e = dictionary->entries->arr[i];
        if (strcmp(e->key, key)==0) {
            KBArrayRemoveIndex(dictionary->entries, i);
            break;
        }
    }
}

KBType *
KBDictionaryGetValue(KBDictionary *dictionary, const char *key) {
    
    KBDictEntry *entry = KBDictionaryGetEntryWithKey(dictionary, key);
    
    return entry!=NULL ? entry->value : NULL;
}