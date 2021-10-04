//
//  KBAtlas.c
//  atlas
//
//  Created by KB Jonathan on 5/14/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBAtlas.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

KBAtlas *
KBAtlasInit(KBAtlas *atlas, KBDestroyFunc destroyFunc, const char *texPath, const char *headerPath, KBFloat scale, KBMask texOpts) {
    if ((atlas = (KBAtlas*)KBTextureInitPNG((KBTexture*)atlas, destroyFunc, texPath, scale, texOpts))) {
        /* open the header */
        FILE *file = fopen(headerPath, "r");
        if (file==NULL) {
            printf("No font header file: %s\n", headerPath);
            KBRelease(atlas);
            return NULL;
        }
        
        /* read the header */
        KBAtlasFileHeader header;
        fread(&header, sizeof(KBAtlasFileHeader), 1, file);
        if (header.tag != KBAtlasFileTag) {
            printf("Wrong header tag: %s\n", headerPath);
            fclose(file);
            KBRelease(atlas);
            return NULL;
        }
        
        /* read the maps in file format */
        KBAtlasFileMap fileMaps[header.nMaps];
        fread(fileMaps, sizeof(KBAtlasFileMap)*header.nMaps, 1, file);
        fclose(file);
        
        atlas->nMaps = header.nMaps;
        atlas->maps = malloc(sizeof(KBMap)*header.nMaps);
        atlas->vertices = malloc(sizeof(KBVertex)*header.nMaps*4);
        
        /* fill the maps array */
        KBFloat w, h;
        w = KBTextureCast(atlas)->pixelWidth;
        h = KBTextureCast(atlas)->pixelHeight;
        
        
        for (int i=0; i<header.nMaps; i++) {
            KBMap *map = &atlas->maps[i];
            
            strncpy(map->name, (char*)fileMaps[i].name, 16);
            map->index = i;
            map->t.l = fileMaps[i].l/w;
            map->t.b = fileMaps[i].b/h;
            map->t.r = fileMaps[i].r/w;
            map->t.t = fileMaps[i].t/h;
        
            map->s.w = (fileMaps[i].r-fileMaps[i].l)/scale;
            map->s.h = (fileMaps[i].t-fileMaps[i].b)/scale;
            
            KBVertex *v = &atlas->vertices[i*4];
            v[0] = (KBVertex){0.f, 0.f, map->t.l, map->t.b};
            v[1] = (KBVertex){0.f, map->s.h, map->t.l, map->t.t};
            v[2] = (KBVertex){map->s.w, 0.f, map->t.r, map->t.b};
            v[3] = (KBVertex){map->s.w, map->s.h, map->t.r, map->t.t};
        }
        
    }
    
    return atlas;
}

KBAtlas *
KBAtlasNew(const char *texPath, const char *headerPath, KBFloat scale, KBMask texOpts) {
    return KBAtlasInit(KBAlloc(sizeof(KBAtlas)), &KBAtlasDestroy, texPath, headerPath, scale, texOpts);
}

void
KBAtlasDestroy(KBType *atlasObj) {
    KBAtlas *atlas = atlasObj;
    free(atlas->maps);
    free(atlas->vertices);

    KBTextureDestroy(atlasObj);
}

int
KBAtlasGetMapIndex(KBAtlas *atlas, const char *name) {
    for (int i=0; i<atlas->nMaps; i++) {
        KBMap *map = &atlas->maps[i];
        if (strncmp(map->name, name, 16) == 0) {
            return i;
        }
    }
    return -1;
}

const KBMap *
KBAtlasGetMapAtIndex(KBAtlas *atlas, int index) {
    return &atlas->maps[index];
}

const KBMap *
KBAtlasGetMapNamed(KBAtlas *atlas, const char *name) {
    for (int i=0; i<atlas->nMaps; i++) {
        KBMap *map = &atlas->maps[i];
        if (strcmp(map->name, name) == 0) {
            return map;
        }
    }
    return NULL;
}

const KBVertex *
KBAtlasGetVerticesForMapIndex(KBAtlas *atlas, int index) {
#if DEBUG
    if (index>=atlas->nMaps) {
        printf("KBError index (%d) exceeds atlas map count (%d)", index, atlas->nMaps);
    }
#endif
    return &atlas->vertices[index*4];
}

void
KBAtlasPrintNames(KBAtlas *atlas) {
    for (int i=0; i<atlas->nMaps; i++) {
        KBMap *map = &atlas->maps[i];
        printf("%s\n", map->name);
    }

}