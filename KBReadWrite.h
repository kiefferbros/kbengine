//
//  KBReadWrite.h
//  KBEngine
//
//  Created by KB Jonathan on 5/25/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#ifndef KBReadWrite_h
#define KBReadWrite_h

#include "KBTypes.h"
#include <stdint.h>

typedef enum _KBEndianness {
    KBBigEndian,
    KBLittleEndian
} KBEndianness;


KBEndianness KBGetSystemEndianness();

uint32_t KBUInt32HostToBig(uint32_t v);
uint32_t KBUInt32BigToHost(uint32_t v);
int32_t KBInt32HostToBig(int32_t v);
int32_t KBInt32BigToHost(int32_t v);

uint16_t KBUInt16HostToBig(uint16_t v);
uint16_t KBUInt16BigToHost(uint16_t v);

int16_t KBInt16HostToBig(int16_t v);
int16_t KBInt16BigToHost(int16_t v);

KBFloat KBFloat32HostToBig(KBFloat v);
KBFloat KBFloat32BigToHost(KBFloat v);

#endif
