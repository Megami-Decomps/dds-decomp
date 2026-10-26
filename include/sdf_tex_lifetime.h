#ifndef SDF_TEX_LIFETIME_H
#define SDF_TEX_LIFETIME_H

struct sdfTexHandle_t;

/* Destroy a live texture: unlink it and release its buffers and allocations.
 * This does not decrement the ordinary texture reference count. */
void sdfTexDestroy(struct sdfTexHandle_t *texture);

#endif /* SDF_TEX_LIFETIME_H */
