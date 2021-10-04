//
//  KBAtlasLabel.h
//  KBEngine
//
//  Created by KB Jonathan on 6/10/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBAtlasLabel_h
#define KBAtlasLabel_h

#include "KBLabel.h"
#include "KBSprite.h"
#include "KBAtlasFont.h"

typedef struct _KBAtlasLabel {
    KBSprite            super;
    
    KBAtlasFont         *font;
    char                *text;
    KBTextAlignment     align;
} KBAtlasLabel;

#define KBAtlasLabelCast(obj)((KBAtlasLabel*)obj)

KBAtlasLabel * KBAtlasLabelInit(KBAtlasLabel *label, KBDestroyFunc destroyFunc, const char *text, KBAtlasFont *font, KBTextAlignment align);
KBAtlasLabel * KBAtlasLabelNew(const char *text, KBAtlasFont *font, KBTextAlignment align);
void KBAtlasLabelDestroy(KBType *labelObj);

void KBAtlasLabelSetText(KBAtlasLabel *label, const char *text);
#endif
