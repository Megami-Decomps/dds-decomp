#ifndef EFF_PARAM_OPERATIONS_H
#define EFF_PARAM_OPERATIONS_H

#include "common.h"

/* Native 0x28-byte operation row, indexed by effect kind.
 * The scale operation takes a float in addition to its payload.
 * In the extended table, a duplicate callback selects raw-source creation
 * and cloning; its absence selects the kind/tableIndex fallback table. */
typedef struct EffDispatchEntry {
    void *(*create)(void *);               /* 0x00 */
    void (*dispatch)(void *);              /* 0x04 */
    void (*release)(void *);               /* 0x08 */
    void *(*duplicate)(void *);            /* 0x0C */
    void (*callback0)(void *, void *);     /* 0x10 */
    void (*setScale)(void *, f32);         /* 0x14 */
    void (*callback2)(void *, void *);     /* 0x18 */
    void (*callback3)(void *, u32);        /* 0x1C */
    void (*callback4)(void *, void *);     /* 0x20 */
    void (*callback5)(void *, f32);        /* 0x24 */
} EffDispatchEntry; /* 0x28 */

typedef char EffDispatchEntry_layout_must_match_native_row[
    (sizeof(EffDispatchEntry) == 0x28 &&
     (u32)&((EffDispatchEntry *)0)->create == 0x00 &&
     (u32)&((EffDispatchEntry *)0)->dispatch == 0x04 &&
     (u32)&((EffDispatchEntry *)0)->release == 0x08 &&
     (u32)&((EffDispatchEntry *)0)->duplicate == 0x0C &&
     (u32)&((EffDispatchEntry *)0)->callback0 == 0x10 &&
     (u32)&((EffDispatchEntry *)0)->setScale == 0x14 &&
     (u32)&((EffDispatchEntry *)0)->callback2 == 0x18 &&
     (u32)&((EffDispatchEntry *)0)->callback3 == 0x1C &&
     (u32)&((EffDispatchEntry *)0)->callback4 == 0x20 &&
     (u32)&((EffDispatchEntry *)0)->callback5 == 0x24)
        ? 1 : -1];

extern EffDispatchEntry effParamWorkFactories[];
extern EffDispatchEntry effParameterWorkOperations[];

#endif
