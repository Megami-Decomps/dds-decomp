#ifndef EVT_ACTION_OBJECT_H
#define EVT_ACTION_OBJECT_H

#include "common.h"

struct dds3Basic_t;

/* Appends a linked kind-2 world node whose value is an object name. */
struct dds3Basic_t *dds3AddAreaBasic(s32 key, const char *objectName);

#endif /* EVT_ACTION_OBJECT_H */
