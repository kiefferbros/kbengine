//
//  KBAtlasFont.c
//  KBEngine
//
//  Created by KB Jonathan on 6/9/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBAtlasFont.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

KBAtlasFont *
KBAtlasFontInit(KBAtlasFont *font, KBDestroyFunc destroyFunc, KBAtlas *atlas, const char *fontName) {
    if ((font = KBObjectInit(font, destroyFunc))) {
        
        font->atlas = KBRetain(atlas);
        
        
        char mapName[16];
        
        for (int i=0; i<94; i++) {
            snprintf(mapName, 16, "%s-%i", fontName, i);
            font->charIndexes[i] = KBAtlasGetMapIndex(atlas, mapName);
            if (font->charIndexes[i]>-1 && font->spaceWidth==0.f) {
                const KBMap *map = KBAtlasGetMapAtIndex(atlas, font->charIndexes[i]);
                font->spaceWidth = map->s.w;
            }
        }
    }
    
    return font;
}

KBAtlasFont *
KBAtlasFontNew(KBAtlas *atlas, const char *name) {
    return KBAtlasFontInit(KBAlloc(sizeof(KBAtlasFont)), &KBAtlasFontDestroy, atlas, name);
}

void
KBAtlasFontDestroy(KBType *fontObj) {
    KBAtlasFont *font = fontObj;
    KBRelease(font->atlas);
}

int KBAtlasFontGetIndex(KBAtlasFont *font, char cChar) {
    return font->charIndexes[cChar-'!'];
}