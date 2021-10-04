//
//  KBFont.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 1/7/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBFont_h
#define KBFont_h

#include "KBTexture.h"

typedef struct _KBFontChar {
    char            cChar;
    KBBox           t;                  /* texture coords */
    KBVec2          s;                  /* in points */
    KBFloat         offsetY;            /* in points */
} KBFontChar;

typedef struct _KBFontHeader {
    int             nCharacters;
    KBFloat         size;
    KBFloat         spaceWidth;
    KBFloat         tabWidth;
    KBFloat         tracking;
    KBFloat         leading;
} KBFontHeader;

typedef struct _KBFont {
    KBTexture       super;
    
    int             nCharacters;
    KBFontChar      *characters;
    KBFloat         size;
    KBFloat         spaceWidth;
    KBFloat         tabWidth;
    KBFloat         tracking;
    KBFloat         leading;
} KBFont;

#define KBFontCast(obj)((KBFont*)obj)

KBFont * KBFontInit(KBFont *font, KBDestroyFunc destroyFunc, const char *texPath, const char *headerPath, KBFloat scale, KBMask texOpts);
KBFont * KBFontNew(const char *texPath, const char *headerPath, KBFloat scale, KBMask texOpts);
void KBFontDestroy(KBType *fontObj);

KBFontChar *KBFontGetCharacter(KBFont *font, char cChar);

#endif
