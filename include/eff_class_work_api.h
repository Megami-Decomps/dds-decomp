#ifndef EFF_CLASS_WORK_API_H
#define EFF_CLASS_WORK_API_H

#include "common.h"

struct EffClassWork;

struct EffClassWork *effCreateClassWork(u16 kind, void *source);
void effDestroyClassWork(struct EffClassWork *work);
void effInitializeClassFrame(struct EffClassWork *work);

#endif
