#include "common.h"

typedef struct {
    s32 allocation;
    u8 pad04[0x2C];
    s32 lifetime;
    s32 owner;
    u8 pad38[0x0C];
    u8 variant;
    u8 sprite;
    u8 pad46[2];
} MovieSpriteResource;

typedef struct {
    s32 x;
    s32 y;
    s32 framesLeft;
    s32 duration;
    u8 direction;
    s8 mode;
    u8 generations;
    u8 variant;
} SpriteSpawnNode;

typedef struct SdfListNode {
    u32 index;
    s32 key;
    struct SdfListNode *next;
    struct SdfListNode *prev;
    void *value;
} SdfListNode;

typedef struct SdfList {
    u32 pad00;
    u32 count;
    SdfListNode *head;
    SdfListNode *tail;
    u32 pad10;
    void (*onRemove)(u32, void *);
} SdfList;

extern SpriteSpawnNode *func_0025B888(MovieSpriteResource *, SpriteSpawnNode *, s8);
extern SdfListNode *sdfListAppend(SdfList *, s32, void *);
extern f32 effMiscRandUnitFloat(void *);

void func_0025BA20(s32 resourceAddress, s32 listAddress, u8 *parentData, s8 mode) {
    MovieSpriteResource *resource = (MovieSpriteResource *)resourceAddress;
    SdfList *list = (SdfList *)listAddress;
    SpriteSpawnNode *parent = (SpriteSpawnNode *)parentData;
    SpriteSpawnNode *node;
    f32 chance;
    s32 i;

    switch (mode) {
    case 0:
        node = func_0025B888(resource, NULL, 0);
        node->mode = 3;
        sdfListAppend(list, list->count, node);
        break;
    case 1:
        chance = effMiscRandUnitFloat(NULL);
        if (chance < 0.8f) {
            node = func_0025B888(resource, parent, 0);
            node->mode = mode;
            sdfListAppend(list, list->count, node);
        }
        /* The same roll can admit both branches. */
        if (chance < 0.9f) {
            node = func_0025B888(resource, parent, 1);
            node->mode = 2;
            sdfListAppend(list, list->count, node);
        }
        break;
    case 2:
        if (effMiscRandUnitFloat(NULL) < 0.8f) {
            node = func_0025B888(resource, parent, 0);
            node->mode = mode;
            sdfListAppend(list, list->count, node);
        }
        break;
    case 3:
        for (i = 0; i < 6; i++) {
            node = func_0025B888(resource, parent, i);
            node->mode = 1;
            sdfListAppend(list, list->count, node);
        }
        break;
    }
}

