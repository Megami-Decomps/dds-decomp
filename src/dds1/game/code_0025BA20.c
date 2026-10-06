#include "common.h"
#include "sdf.h"


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


extern SpriteSpawnNode *func_0025B888(void *, SpriteSpawnNode *, s8);
extern f32 effMiscRandUnitFloat(void *);

void func_0025BA20(void *resource, SdfList *list, void *parentData, s8 mode) {
    SpriteSpawnNode *parent = parentData;
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

