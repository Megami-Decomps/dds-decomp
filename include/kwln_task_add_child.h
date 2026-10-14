#ifndef KWLN_TASK_ADD_CHILD_H
#define KWLN_TASK_ADD_CHILD_H

#include "kwln.h"

/* Append child to parent's child list, detaching it from any previous parent. */
void kwlnTaskAddChild(KwlnTask *parent, KwlnTask *child);

#endif /* KWLN_TASK_ADD_CHILD_H */
