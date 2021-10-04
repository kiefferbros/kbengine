//
//  KBLabel.h
//  KBEngine
//
//  Created by Jonathan Kieffer on 1/7/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBLabel_h
#define KBLabel_h

#include "KBDrawable.h"
#include "KBFont.h"

typedef enum {
	KBTextAlignmentLeft = 0,
	KBTextAlignmentCenter,
	KBTextAlignmentRight
} KBTextAlignment;


typedef struct _KBLabel {
    KBDrawable          super;
    
    KBFont       *font;
    
    KBTextAlignment     alignment;
    
    KBFloat             size;
    KBFloat             tracking;
    KBFloat             leading;    
    char                *text;
    
    GLuint              vbo;
    int                 charCount;
    KBBox               contentBox;
} KBLabel;

#define KBLabelCast(obj)((KBLabel*)obj)

KBLabel * KBLabelInit(KBLabel *label, KBDestroyFunc destroyFunc, const char *text, KBFont *font, KBFloat size, KBTextAlignment align);
KBLabel * KBLabelNew(const char *text, KBFont *font, KBFloat size, KBTextAlignment align);
void KBLabelDestroy(KBType *labelObj);

void KBLabelSetText(KBLabel *label, const char *text);



#endif
