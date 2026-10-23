#ifndef BTL_SOUND_HANDLE_NODE_H
#define BTL_SOUND_HANDLE_NODE_H

struct FileQueue;
struct dds3ModelHandle_t;

typedef struct SoundHandleNode {
    struct FileQueue *queue;
    struct dds3ModelHandle_t *actor;
} SoundHandleNode;

#endif
