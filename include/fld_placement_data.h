#ifndef FLD_PLACEMENT_DATA_H
#define FLD_PLACEMENT_DATA_H

#include "common.h"

struct FldSpecialPoint;

/* Type-10 placement tag values select both spawn behavior and payload view. */
typedef u32 FldPlacementKind;

enum FldPlacementKind {
    FLD_PLACEMENT_KIND_POINT = 0,
    FLD_PLACEMENT_KIND_REGISTERED_POINT = 1,
    FLD_PLACEMENT_KIND_EVENT_PLACEMENT = 2,
    FLD_PLACEMENT_KIND_WORD_ARRAY_PLACEMENT = 3,
    FLD_PLACEMENT_KIND_PENDING_SOUND = 7,
    FLD_PLACEMENT_KIND_SPECIAL_POINT = 8
};

/* The kind word at +0 selects the pointed-to view of this relocated payload.
 * Kind 8 points to a two-word special point. Kinds 2 and 3 are consumed as
 * word arrays, while kind 7 carries a float followed by an integer. Keep the
 * +0x08 bytes in the enclosing record opaque. */
typedef struct FldPendingSoundParams {
    f32 scalar;
    u32 value;
} FldPendingSoundParams;

typedef union FldPlacementPayloadRef {
    void *raw;
    struct FldSpecialPoint *specialPoint;
    u32 *words;
    FldPendingSoundParams *pendingSound;
} FldPlacementPayloadRef;

typedef struct FldSpecialPoint {
    u32 kind;
    u32 id;
} FldSpecialPoint;

/* Type-10 placement records contain a four-word serialized header. */
typedef struct FldPlacementData {
    FldPlacementKind kind; /* 0x00 */
    s32 event;            /* 0x04 */
    u8 opaque08[4];
    FldPlacementPayloadRef payload; /* 0x0C */
} FldPlacementData;

typedef char FldPlacementPayloadRefSizeCheck[
    sizeof(FldPlacementPayloadRef) == 4 ? 1 : -1];
typedef char FldPendingSoundParamsSizeCheck[
    sizeof(FldPendingSoundParams) == 8 ? 1 : -1];
typedef char FldPlacementDataSizeCheck[(sizeof(FldPlacementData) == 0x10) ? 1 : -1];
typedef char FldPlacementDataOpaqueGapOffsetCheck[
    ((u32)&((FldPlacementData *)0)->opaque08 == 8) ? 1 : -1];
typedef char FldPlacementDataPayloadOffsetCheck[
    ((u32)&((FldPlacementData *)0)->payload == 0xC) ? 1 : -1];
typedef char FldPlacementPayloadWordsOffsetCheck[
    ((u32)&((FldPlacementPayloadRef *)0)->words == 0) ? 1 : -1];
typedef char FldPendingSoundValueOffsetCheck[
    ((u32)&((FldPendingSoundParams *)0)->value == 4) ? 1 : -1];
typedef char FldSpecialPointSizeCheck[(sizeof(FldSpecialPoint) == 8) ? 1 : -1];
typedef char FldSpecialPointKindOffsetCheck[
    ((u32)&((FldSpecialPoint *)0)->kind == 0) ? 1 : -1];
typedef char FldSpecialPointIdOffsetCheck[
    ((u32)&((FldSpecialPoint *)0)->id == 4) ? 1 : -1];

#endif
