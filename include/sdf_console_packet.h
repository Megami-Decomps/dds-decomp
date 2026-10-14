#ifndef SDF_CONSOLE_PACKET_H
#define SDF_CONSOLE_PACKET_H

#include "common.h"

/* Fixed setup block emitted before the variable-length formatted glyph stream. */
typedef struct SdfConsolePacketHeader {
    u16 quadwordCount;
    u8 reserved02;
    u8 tagKind;
    u32 nextAddress;
    u32 vifNop;
    u32 vifDirect;
    u64 setupTag;
    u64 setupRegisters;
    u64 textureClamp;
    u64 textureClampRegister;
    u64 textureFilter;
    u64 textureFilterRegister;
    u64 textureState;
    u64 textureStateRegister;
    u64 glyphTag;
    u64 glyphRegisters;
} SdfConsolePacketHeader;

/* Two of these register tuples, plus padding when needed, fill five qwords. */
typedef struct SdfConsoleGlyph {
    u64 color;
    u64 firstUv;
    u64 firstPosition;
    u64 secondUv;
    u64 secondPosition;
} SdfConsoleGlyph;

typedef char SdfConsolePacketHeader_size_must_be_0x60[(sizeof(SdfConsolePacketHeader) == 0x60) ? 1 : -1];
typedef char SdfConsolePacketHeader_setupTag_at_0x10[((u32)&((SdfConsolePacketHeader *)0)->setupTag == 0x10) ? 1 : -1];
typedef char SdfConsolePacketHeader_glyphTag_at_0x50[((u32)&((SdfConsolePacketHeader *)0)->glyphTag == 0x50) ? 1 : -1];
typedef char SdfConsoleGlyph_size_must_be_0x28[(sizeof(SdfConsoleGlyph) == 0x28) ? 1 : -1];

#endif
