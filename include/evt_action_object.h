#ifndef EVT_ACTION_OBJECT_H
#define EVT_ACTION_OBJECT_H

#include "common.h"

struct dds3Basic_t;

/* Appends a linked kind-2 world node; both inputs remain generic words. */
struct dds3Basic_t *dds3AddAreaBasic(s32 key, s32 value);

#endif /* EVT_ACTION_OBJECT_H */
