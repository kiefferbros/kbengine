//
//  KBLabel.c
//  KBEngine
//
//  Created by Jonathan Kieffer on 1/7/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBLabel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "KBEngine.h"

void
KBLabelDraw(KBType *drawableObj, KBFloat opacity) {

    KBLabel *label = drawableObj;
    
    if (label->charCount) {
        
        KBTransform *tranform = KBDrawableGetTransform(label);
        glPushMatrix();
        glMultMatrixf(tranform->m);
        
        KBFloat op = KBDrawableCast(label)->inheritOpacity ? opacity*KBDrawableCast(label)->opacity : KBDrawableCast(label)->opacity;
        KBDrawableBindDrawColor(label, op);
        
        glBindTexture(GL_TEXTURE_2D, KBTextureCast(label->font)->name);
        glBindBuffer(GL_ARRAY_BUFFER, label->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, KBEngineGetSpriteIBO());
        
        glVertexPointer(2, GL_FLOAT, sizeof(KBVertex), 0);
        glTexCoordPointer(2, GL_FLOAT, sizeof(KBVertex), (GLvoid*)(sizeof(KBVec2)));
        
        glDrawElements(GL_TRIANGLE_STRIP, label->charCount*6, GL_UNSIGNED_SHORT, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        glPopMatrix();
    }
}

KBBox KBLabelGetContentBox(KBType *drawableObj) {
    return KBLabelCast(drawableObj)->contentBox;
}

KBLabel *
KBLabelInit(KBLabel *label, KBDestroyFunc destroyFunc, const char *text, KBFont *font, KBFloat size, KBTextAlignment align) {
    KBDrawableInit((KBDrawable*)label, destroyFunc, &KBLabelDraw, NULL);
    
    label->font = KBRetain(font);
    
    label->size = size;
    label->tracking = font->tracking;
    label->leading = font->leading;
    label->alignment = align;
    KBDrawableCast(label)->tint = KBVec3MakeUniform(0.f);
    label->text = NULL;
    label->contentBox = KBBoxZero;
    KBDrawableCast(label)->contentBoxFunc = &KBLabelGetContentBox;
    
    /* fill the buffer with data */
    glGenBuffers(1, &label->vbo);
    KBLabelSetText(label, text);

    return label;
}

KBLabel *
KBLabelNew(const char *text, KBFont *font, KBFloat size, KBTextAlignment align) {
    return KBLabelInit(KBAlloc(sizeof(KBLabel)), &KBLabelDestroy, text, font, size, align);
}

void
KBLabelDestroy(KBType *labelObj) {
    KBLabel *label = labelObj;
    
    free(label->text);
    KBRelease(label->font);
    glDeleteBuffers(1, &label->vbo);
    
    KBDrawableDestroy(labelObj);
}

void
KBLabelSetText(KBLabel *label, const char *text) {
    
    label->contentBox = KBBoxZero;
    label->charCount=0;

    if (!text) {
        free(label->text);
        label->text = NULL;
        return;
    }
    
    KBFont *font = label->font;
    KBTextAlignment align = label->alignment;
    
    
    int len = (int)strlen(text);
    free(label->text);
    label->text = malloc(len+1);
    strcpy(label->text, text);
    
    /* find the number of characters */
	for (int i=0; i<len; ++i) {
		if (KBFontGetCharacter(font, text[i]) != NULL) {
			label->charCount++;
        }
	}
    
    
    if (label->charCount) {
        
        /* make sure the index buffer is large enough */
        unsigned int spriteMax = KBEngineGetSpriteIBOMax();
        if (label->charCount > spriteMax)
            KBEngineSetSpriteIBOMax(label->charCount);
        
        glBindBuffer(GL_ARRAY_BUFFER, label->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(KBVertex)*label->charCount*4, NULL, GL_STATIC_DRAW);

        KBVertex *v = (KBVertex*)KB_glMapBuffer_ARRAY();
        
        KBFloat s = label->size/font->size;
        KBBox charBox = KBBoxZero;
        
        int len = (int)strlen(text);
        for (int i=0; i<len; i++) {
            KBFontChar *fontChar = KBFontGetCharacter(font, text[i]);
            if (fontChar) {
                
                charBox.r = charBox.l + fontChar->s.w*s;
                charBox.b = fontChar->offsetY*s;
                charBox.t = charBox.b + fontChar->s.h*s;
                
                v[0] = (KBVertex){charBox.l, charBox.b, fontChar->t.l, fontChar->t.b};
                v[1] = (KBVertex){charBox.l, charBox.t, fontChar->t.l, fontChar->t.t};
                v[2] = (KBVertex){charBox.r, charBox.b, fontChar->t.r, fontChar->t.b};
                v[3] = (KBVertex){charBox.r, charBox.t, fontChar->t.r, fontChar->t.t};
                
                label->contentBox.r = KBFloatMax(label->contentBox.r, charBox.r);
                label->contentBox.t = KBFloatMax(label->contentBox.t, charBox.t);
                
                charBox.l += (fontChar->s.w + label->tracking)*s;
                
                
                v += 4;
            } else if (text[i] == '\t') {
                charBox.l += font->tabWidth*s + label->tracking*s;
            } else {
                charBox.l += font->spaceWidth*s + label->tracking*s;
            }
        }
        
        charBox.l -= label->tracking*s;
        
        KB_glUnmapBuffer_ARRAY();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        /* adjust anchor for alignment */
        if (align == KBTextAlignmentCenter)
            KBDrawableSetAnchor(label, KBVec2Make(KBBoxGetWidth(label->contentBox)*0.5f, 0.f));
        else if (align == KBTextAlignmentRight)
            KBDrawableSetAnchor(label, KBVec2Make(KBBoxGetWidth(label->contentBox), 0.f));
    }
}

