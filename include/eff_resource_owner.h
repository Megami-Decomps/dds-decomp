#ifndef EFF_RESOURCE_OWNER_H
#define EFF_RESOURCE_OWNER_H

#include "common.h"
#include "file.h"
#include "mdl.h"
#include "sdf.h"

/* An effect resource owner: the lifetime fields shared by both games, the
 * animation header copied from the resource at +8 (DDS1's shorter payload is
 * EffectModelHeaderCopy's 0x3C bytes), and the retained queue array. */
#ifdef VERSION_DDS1
typedef struct EffResourceOwner {
    u32 count;
    u32 unk04;
    SdfColorTrack colorTrack; /* 0x08 */
    SdfAlphaTrack alphaTrack; /* 0x2C */
    u32 plainEntry; // 0x3C: add the model entry plain instead of flagged
    f32 frameStep; /* 0x40 */
    struct FileQueue **entries; /* 0x44: retained queue-pointer array */
    struct SdfMemBlock *queueAllocation; /* 0x48: queue-pointer array descriptor */
    MdlCtx *model;
} EffResourceOwner;
typedef char EffResourceOwner_size_check[(sizeof(EffResourceOwner) == 0x50) ? 1 : -1];
typedef char EffResourceOwner_plainEntry_offset_check[
    ((u32)&((EffResourceOwner *)0)->plainEntry == 0x3C) ? 1 : -1];
typedef char EffResourceOwner_entries_offset_check[
    ((u32)&((EffResourceOwner *)0)->entries == 0x44) ? 1 : -1];
#else
typedef struct EffResourceConfig {
    u8 unknown00[0x34];
    s32 plainEntry; /* Duration, also selects plain versus flagged entry. */
    f32 frameStep;
} EffResourceConfig;

typedef struct EffResourceExtendedConfig {
    EffResourceConfig base;
    u8 unknown3C[0x38];
} EffResourceExtendedConfig;

typedef struct EffResourceOwner {
    u32 count;
    s32 unk04;
    EffResourceConfig base;
    EffResourceExtendedConfig extended;
    FileQueue **entries;
    SdfMemBlock *buffer;
    MdlCtx *model;
} EffResourceOwner;

typedef char EffResourceConfig_size_check[sizeof(EffResourceConfig) == 0x3C ? 1 : -1];
typedef char EffResourceExtendedConfig_size_check[sizeof(EffResourceExtendedConfig) == 0x74 ? 1 : -1];
typedef char EffResourceOwner_size_check[sizeof(EffResourceOwner) == 0xC4 ? 1 : -1];
typedef char EffResourceOwner_base_check[(u32)&((EffResourceOwner *)0)->base == 8 ? 1 : -1];
typedef char EffResourceOwner_extended_check[(u32)&((EffResourceOwner *)0)->extended == 0x44 ? 1 : -1];
typedef char EffResourceOwner_entries_check[(u32)&((EffResourceOwner *)0)->entries == 0xB8 ? 1 : -1];
#endif

#endif /* EFF_RESOURCE_OWNER_H */
