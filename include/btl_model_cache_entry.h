#ifndef BTL_MODEL_CACHE_ENTRY_H
#define BTL_MODEL_CACHE_ENTRY_H

#include "common.h"
#include "file_pac.h"
#include "snd_slot.h"

/* One linked cache owner for a requested battle model and its optional
 * packed-resource and sound-slot references. Bytes +0x0D..+0x0F are opaque. */
typedef struct BattleModelEntry {
    s32 kind;                         /* 0x00: model resource kind */
    s32 id;                           /* 0x04: model id */
    s32 refCount;                     /* 0x08: retained users */
    s8 state;                         /* 0x0C: nonzero bypasses request checks */
    u8 pad0D[3];
    FilePacRequest *packRequest;      /* 0x10 */
    SoundSlotOwner *soundOwner;       /* 0x14 */
    struct BattleModelEntry *prev;    /* 0x18 */
    struct BattleModelEntry *next;    /* 0x1C */
} BattleModelEntry;

typedef char BattleModelEntrySizeCheck[(sizeof(BattleModelEntry) == 0x20) ? 1 : -1];

#endif /* BTL_MODEL_CACHE_ENTRY_H */
