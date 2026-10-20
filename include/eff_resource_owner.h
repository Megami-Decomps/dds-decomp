#ifndef EFF_RESOURCE_OWNER_H
#define EFF_RESOURCE_OWNER_H

#include "common.h"
#include "file.h"
#include "mdl.h"
#include "sdf.h"
#include "eff.h"

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
    SdfColorTrack colorTrack; /* 0x00 */
    SdfAlphaTrack alphaTrack; /* 0x24 */
    s32 plainEntry; /* Duration, also selects plain versus flagged entry. */
    f32 frameStep;
} EffResourceConfig;

/* Option-1 resource payload: copied as 0x74 bytes by effCreateResourceOwner.
 * Requests and signed indices are consumed by effResolveRequestPositionIntoVu. */
typedef struct EffResourceExtendedConfig {
    EffResourceConfig base;
    u8 enableExplicitEndpoints; /* 0x3C */
    u8 unknown3D[3];
    EffectVectorRequest start; /* 0x40 */
    EffectVectorRequest end; /* 0x48 */
    s16 startIndex; /* 0x50 */
    s16 endIndex; /* 0x52 */
    u8 preserveBaseScale; /* 0x54 */
    u8 unknown55[0x1F];
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
typedef char EffResourceConfig_alphaTrack_check[(u32)&((EffResourceConfig *)0)->alphaTrack == 0x24 ? 1 : -1];
typedef char EffResourceExtendedConfig_enable_check[(u32)&((EffResourceExtendedConfig *)0)->enableExplicitEndpoints == 0x3C ? 1 : -1];
typedef char EffResourceExtendedConfig_start_check[(u32)&((EffResourceExtendedConfig *)0)->start == 0x40 ? 1 : -1];
typedef char EffResourceExtendedConfig_end_check[(u32)&((EffResourceExtendedConfig *)0)->end == 0x48 ? 1 : -1];
typedef char EffResourceExtendedConfig_startIndex_check[(u32)&((EffResourceExtendedConfig *)0)->startIndex == 0x50 ? 1 : -1];
typedef char EffResourceExtendedConfig_endIndex_check[(u32)&((EffResourceExtendedConfig *)0)->endIndex == 0x52 ? 1 : -1];
typedef char EffResourceExtendedConfig_preserveScale_check[(u32)&((EffResourceExtendedConfig *)0)->preserveBaseScale == 0x54 ? 1 : -1];
typedef char EffResourceOwner_size_check[sizeof(EffResourceOwner) == 0xC4 ? 1 : -1];
typedef char EffResourceOwner_base_check[(u32)&((EffResourceOwner *)0)->base == 8 ? 1 : -1];
typedef char EffResourceOwner_extended_check[(u32)&((EffResourceOwner *)0)->extended == 0x44 ? 1 : -1];
typedef char EffResourceOwner_entries_check[(u32)&((EffResourceOwner *)0)->entries == 0xB8 ? 1 : -1];
#endif

#endif /* EFF_RESOURCE_OWNER_H */
