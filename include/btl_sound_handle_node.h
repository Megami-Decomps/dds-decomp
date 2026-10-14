#ifndef BTL_SOUND_HANDLE_NODE_H
#define BTL_SOUND_HANDLE_NODE_H

struct FileQueue;

typedef struct SoundHandleNode {
    struct FileQueue *queue;
    void *actor;
} SoundHandleNode;

#endif
