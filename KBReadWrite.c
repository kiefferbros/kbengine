//
//  KBReadWrite.c
//  KBEngine
//
//  Created by KB Jonathan on 5/25/13.
//  Copyright (c) 2013 Kieffer Bros., LLC. All rights reserved.
//

#include "KBReadWrite.h"
#include <string.h>

KBEndianness KBGetSystemEndianness() {
    int i = 1;
    char *p = (char *)&i;
    return  (p[0] == 1) ? KBLittleEndian : KBBigEndian;
}

void revbytes(char *c, int num) {
    char d[num];
    memcpy(d, c, num);
    for (int i=0;i<num;i++) {
        c[i] = d[num-(i+1)];
    }
}

uint32_t KBUInt32HostToBig(uint32_t v) {
    if (KBGetSystemEndianness() == KBLittleEndian) {
        revbytes((char*)&v, 4);
    }
    return v;
}

uint32_t KBUInt32BigToHost(uint32_t v) {
    if (KBGetSystemEndianness() == KBLittleEndian) {
        revbytes((char*)&v, 4);
    }
    return v;
}

int32_t KBInt32HostToBig(int32_t v) {
    if (KBGetSystemEndianness() == KBLittleEndian) {
        revbytes((char*)&v, 4);
    }
    return v;
}

int32_t KBInt32BigToHost(int32_t v) {
    if (KBGetSystemEndianness() == KBLittleEndian) {
        revbytes((char*)&v, 4);
    }
    return v;
}

uint16_t KBUInt16HostToBig(uint16_t v) {
    if (KBGetSystemEndianness() == KBLittleEndian) {
        revbytes((char*)&v, 2);
    }
    return v;
}

uint16_t KBUInt16BigToHost(uint16_t v) {
    if (KBGetSystemEndianness() == KBLittleEndian) {
        revbytes((char*)&v, 2);
    }
    return v;
}

int16_t KBInt16HostToBig(int16_t v) {
    if (KBGetSystemEndianness() == KBLittleEndian) {
        revbytes((char*)&v, 2);
    }
    return v;
}

int16_t KBInt16BigToHost(int16_t v) {
    if (KBGetSystemEndianness() == KBLittleEndian) {
        revbytes((char*)&v, 2);
    }
    return v;
}


KBFloat KBFloat32HostToBig(KBFloat v) {
    if (KBGetSystemEndianness() == KBLittleEndian) {
        revbytes((char*)&v, 4);
    }
    return v;
}

KBFloat KBFloat32BigToHost(KBFloat v) {
    if (KBGetSystemEndianness() == KBLittleEndian) {
        revbytes((char*)&v, 4);
    }
    return v;
}