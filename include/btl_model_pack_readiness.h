#ifndef BTL_MODEL_PACK_READINESS_H
#define BTL_MODEL_PACK_READINESS_H

#include "common.h"

typedef s8 BtlModelPackReadiness;
enum {
    BTL_MODEL_PACK_NOT_READY = 0,
    BTL_MODEL_PACK_READY = 1,
};

/* Readiness includes mdlPrepare and optional PAC work; missing entries are
 * not ready. */
struct BattleModelEntry;
BtlModelPackReadiness btlIsModelPackEntryReady(struct BattleModelEntry *entry);
BtlModelPackReadiness btlIsModelEntryReady(s32 kind, s32 id);

#endif /* BTL_MODEL_PACK_READINESS_H */
