#ifndef BTL_MODEL_CACHE_ENTRY_H
#define BTL_MODEL_CACHE_ENTRY_H

#include "common.h"
#include "btl_model_key.h"
#include "file_pac.h"
#include "snd_slot.h"

/* One linked cache owner for a requested battle model and its optional
 * packed-resource and sound-slot references. Bytes +0x0D..+0x0F are opaque. */
#define BTL_MODEL_ENTRY_UNFINALIZED 0
#define BTL_MODEL_ENTRY_FINALIZED 1

typedef struct BattleModelEntry {
    BtlModelResourceKind kind;        /* 0x00: model resource kind */
    BtlModelId id;                    /* 0x04: model id */
    s32 refCount;                     /* 0x08: retained users */
    s8 finalized;                     /* 0x0C: pack-request finalization completed */
    u8 pad0D[3];
    FilePacRequest *packRequest;      /* 0x10 */
    SoundSlotOwner *soundOwner;       /* 0x14 */
    struct BattleModelEntry *prev;    /* 0x18 */
    struct BattleModelEntry *next;    /* 0x1C */
} BattleModelEntry;

typedef char BattleModelEntrySizeCheck[(sizeof(BattleModelEntry) == 0x20) ? 1 : -1];
typedef char BattleModelEntryFinalizedOffsetCheck[
    ((u32)&((BattleModelEntry *)0)->finalized == 0x0C) ? 1 : -1];
typedef char BattleModelEntryFinalizedSizeCheck[
    (sizeof(((BattleModelEntry *)0)->finalized) == 1) ? 1 : -1];

#endif /* BTL_MODEL_CACHE_ENTRY_H */
