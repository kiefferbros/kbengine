//
//  KBFont.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 1/7/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBFont.h"
#include <stdlib.h>
#include <stdio.h>


KBFont *
KBFontInit(KBFont *font, KBDestroyFunc destroyFunc, const char *texPath, const char *headerPath, KBFloat scale, KBMask texOpts) {
    
    if ((font = (KBFont*)KBTextureInitPNG((KBTexture*)font, destroyFunc, texPath, scale, texOpts|KBTextureOptionGenerateMipmap))) {
    
        /* open the header */
        FILE *file = fopen(headerPath, "r");
        if (file==NULL) {
            printf("No font header file: %s\n", headerPath);
            KBRelease(font);
            return NULL;
        }
        
        /* read the header */
        KBFontHeader header;
        fread(&header, sizeof(KBFontHeader), 1, file);
        
        font->nCharacters = header.nCharacters;
        font->size = header.size;
        font->spaceWidth = header.spaceWidth;
        font->tabWidth = header.tabWidth;
        font->tracking = header.tracking;
        font->leading = header.leading;
        
        /* read the characters */
        font->characters = malloc(sizeof(KBFontChar)*header.nCharacters);
        fread(font->characters, sizeof(KBFontChar)*header.nCharacters, 1, file);
        fclose(file);
    }
    
    return font;
}

KBFont *
KBFontNew(const char *texPath, const char *headerPath, KBFloat scale, KBMask texOpts) {
    return KBFontInit(KBAlloc(sizeof(KBFont)), &KBFontDestroy, texPath, headerPath, scale, texOpts);
}

void
KBFontDestroy(KBType *fontObj) {
    KBFont *font = fontObj;
    free(font->characters);
    
    KBTextureDestroy(fontObj);
}

KBFontChar *
KBFontGetCharacter(KBFont *font, char cChar) {
    for (int i=0; i< font->nCharacters; ++i) {
        if (font->characters[i].cChar == cChar) {
			return &font->characters[i];
		}
	}
	
	return NULL;
}
