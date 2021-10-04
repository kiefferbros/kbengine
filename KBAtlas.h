//
//  KBAtlas.h
//  KBEngine
//
//  Created by KB Jonathan on 5/14/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBAtlas_h
#define KBAtlas_h

#include "KBTexture.h"
#include "KBReadWrite.h"

#define KBAtlasFileTag 'KBAT'

typedef struct _KBAtlasFileHeader {
    uint32_t     tag;
    uint32_t     nMaps;
} KBAtlasFileHeader;

typedef struct _KBAtlasFileMap {
    int8_t       name[16];
    uint32_t     l, b, r, t;
} KBAtlasFileMap;

typedef struct _KBMap {
    char        name[16];
    int         index;
    KBBox       t;
    KBVec2      s;
} KBMap;

typedef struct _KBAtlas {
    KBTexture   super;
    
    int         nMaps;
    KBMap       *maps;
    
    KBVertex  *vertices;
} KBAtlas;

#define KBAtlasCast(obj)((KBAtlas*)obj)

KBAtlas * KBAtlasInit(KBAtlas *atlas, KBDestroyFunc destroyFunc, const char *texPath, const char *headerPath, KBFloat scale, KBMask texOpts);
KBAtlas * KBAtlasNew(const char *texPath, const char *headerPath, KBFloat scale, KBMask texOpts);
void KBAtlasDestroy(KBType *atlasObj);

const KBMap * KBAtlasGetMapAtIndex(KBAtlas *atlas, int index);
const KBMap * KBAtlasGetMapNamed(KBAtlas *atlas, const char *name);

int KBAtlasGetMapIndex(KBAtlas *atlas, const char *name);

const KBVertex * KBAtlasGetVerticesForMapIndex(KBAtlas *atlas, int index);

void KBAtlasPrintNames(KBAtlas *atlas);

#endif
