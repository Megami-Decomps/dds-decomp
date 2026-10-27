#ifndef SDF_MODEL_COPY_H
#define SDF_MODEL_COPY_H

struct DevRequest;

/* Copy destination->usedCount * 16 bytes from source->buffer into
 * destination->buffer.
 * A NULL destination is ignored; source must be valid when that count is > 0. */
void sdfModelMorphListCopyStat(struct DevRequest *destination, const struct DevRequest *source);

/* Copy runtime model and draw-node state into an existing model overlay.
 * Both models and their resource/draw-node lists must be initialized. Only
 * the destination is updated; the source graph is read-only during the copy. */
struct sdfModelRootNode_t;
void sdfModelCopyStat(
    struct sdfModelRootNode_t *destination, const struct sdfModelRootNode_t *source);

#endif /* SDF_MODEL_COPY_H */
