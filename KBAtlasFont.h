//
//  KBAtlasFont.h
//  KBEngine
//
//  Created by KB Jonathan on 6/9/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBAtlasFont_h
#define KBAtlasFont_h

#include "KBAtlas.h"

typedef struct _KBAtlasFont {
    KBObject    super;
    
    KBAtlas     *atlas;
    int         charIndexes[94];
    KBFloat     spaceWidth;
} KBAtlasFont;

#define KBAtlasFontCast(obj)((KBAtlasFont*)obj)

KBAtlasFont * KBAtlasFontInit(KBAtlasFont *font, KBDestroyFunc destroyFunc, KBAtlas *atlas, const char *name);
KBAtlasFont * KBAtlasFontNew(KBAtlas *atlas, const char *name);
void KBAtlasFontDestroy(KBType *fontObj);

int KBAtlasFontGetIndex(KBAtlasFont *font, char cChar);

#endif
