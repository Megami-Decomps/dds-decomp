#ifndef SDF_MODEL_COPY_H
#define SDF_MODEL_COPY_H

struct DevRequest;

/* Copy destination->usedCount * 16 bytes from source->buffer into
 * destination->buffer.
 * A NULL destination is ignored; source must be valid when that count is > 0. */
void sdfModelMorphListCopyStat(struct DevRequest *destination, const struct DevRequest *source);

/* Copy runtime model and draw-node state into an existing model overlay.
 * Both models and their resource and draw-node lists must be initialized. */
struct sdfModelRootNode_t;
void sdfModelCopyStat(struct sdfModelRootNode_t *destination, struct sdfModelRootNode_t *source);

#endif /* SDF_MODEL_COPY_H */
