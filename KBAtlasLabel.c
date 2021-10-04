//
//  KBAtlasLabel.c
//  Dolomite Editor
//
//  Created by KB Jonathan on 6/10/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBAtlasLabel.h"
#include <stdio.h>
#include <string.h>

KBAtlasLabel *
KBAtlasLabelInit(KBAtlasLabel *label, KBDestroyFunc destroyFunc, const char *text, KBAtlasFont *font, KBTextAlignment align) {
    if ((label = (KBAtlasLabel*)KBSpriteInitAtlas((KBSprite*)label, destroyFunc, -1))) {
        label->font = KBRetain(font);
        label->align = align;
        KBAtlasLabelSetText(label, text);
    }
    return label;
}

KBAtlasLabel *
KBAtlasLabelNew(const char *text, KBAtlasFont *font, KBTextAlignment align) {
    return KBAtlasLabelInit(KBAlloc(sizeof(KBAtlasLabel)), &KBAtlasLabelDestroy, text, font, align);
}

void
KBAtlasLabelDestroy(KBType *labelObj) {
    KBAtlasLabel *label = labelObj;
    KBRelease(label->font);
    free(label->text);
    
    KBSpriteDestroy(labelObj);
}

void
KBAtlasLabelSetText(KBAtlasLabel *label, const char *text) {
    /* copy the text data */
    int len = (int)strlen(text);
    free(label->text);
    label->text = malloc(len+1);
    strcpy(label->text, text);
    
    /* find the number of characters */
    int charCount = 0;
	for (int i=0; i<len; ++i) {
		if (KBAtlasFontGetIndex(label->font, text[i]) > -1) {
			charCount++;
        }
	}
    
    if (charCount) {

        /* furnish adequate number of character sprites */
        KBArray *children = KBNodeGetChildren(label);
        int childCount = children ? KBArrayGetCount(children) : 0;
        while (charCount != childCount) {
            if (childCount<charCount) {
                KBSprite *charSprite = KBSpriteNewAtlas(-1);
                KBNodeAddChild(label, charSprite);
                children = KBNodeGetChildren(label);
            } else {
                KBNodeDisownParent(KBArrayGetLastObject(children));
            }
            
            childCount = KBArrayGetCount(children);
        }
        
        /* position and set atlas indexes of the character sprites */
        int len = (int)strlen(text);
        int j = 0;
        KBFloat x = 0.f;
        for (int i=0; i<len; i++) {
            int atlasIndex = KBAtlasFontGetIndex(label->font, text[i]);

            if (atlasIndex>-1) {
                KBSprite *charSprite = KBArrayGetObjectAtIndex(children, j);
                
                KBSpriteSetAtlasIndex(charSprite, atlasIndex);
                KBDrawableSetPosition(charSprite, KBVec2Make(x, 0.f));
                
                x += KBAtlasGetMapAtIndex(label->font->atlas, atlasIndex)->s.w;
                j++;
            } else {
                x += label->font->spaceWidth;
            }
        }
        
        /* adjust anchor for alignment */
        if (label->align == KBTextAlignmentCenter) {
            KBDrawableSetAnchor(label, KBVec2Make(x*0.5f, 0.f));
        } else if (label->align == KBTextAlignmentRight) {
            KBDrawableSetAnchor(label, KBVec2Make(x, 0.f));
        } else {
            KBDrawableSetAnchor(label, KBVec2Zero);
        }
    } else {
        KBNodeDisownAllChildren(label);
    }
    
}