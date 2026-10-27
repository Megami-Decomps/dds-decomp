#ifndef FLD_SCENE_SEQUENCE_API_H
#define FLD_SCENE_SEQUENCE_API_H

#include "common.h"

struct fldSceneParam_t;

/* The packet is borrowed for this call. Mode zero copies its fields into
 * persistent scene work before asynchronous tasks start. */
void fldProcTrigSequence(const struct fldSceneParam_t *record, u32 mode);

#endif /* FLD_SCENE_SEQUENCE_API_H */
