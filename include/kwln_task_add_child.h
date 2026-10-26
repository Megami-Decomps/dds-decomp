#ifndef KWLN_TASK_ADD_CHILD_H
#define KWLN_TASK_ADD_CHILD_H

#include "kwln.h"

/*
 * Append child to parent's child list, detaching it from any previous parent.
 * This hierarchy controls teardown: parent finalization calls
 * dds3KillProcess(child, 0) for linked children before its destroy callback.
 * It does not change scheduler queue membership.
 */
void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);

#endif /* KWLN_TASK_ADD_CHILD_H */
