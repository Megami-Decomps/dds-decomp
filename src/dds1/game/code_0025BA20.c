#include "common.h"
#include "sdf.h"
#include "mnu_sprite_resource.h"



extern SpriteSpawnNode *mnuCreateSpriteSpawnNode(MnuSpriteResourceGroup *, SpriteSpawnNode *, s8);
extern f32 effMiscRandUnitFloat(void *);

void func_0025BA20(MnuSpriteResourceGroup *resource, SdfList *list, void *parentData, s8 mode) {
    SpriteSpawnNode *parent = parentData;
    SpriteSpawnNode *node;
    f32 chance;
    s32 i;

    switch (mode) {
    case 0:
        node = mnuCreateSpriteSpawnNode(resource, NULL, 0);
        node->mode = 3;
        sdfListAppend(list, list->count, node);
        break;
    case 1:
        chance = effMiscRandUnitFloat(NULL);
        if (chance < 0.8f) {
            node = mnuCreateSpriteSpawnNode(resource, parent, 0);
            node->mode = mode;
            sdfListAppend(list, list->count, node);
        }
        /* The same roll can admit both branches. */
        if (chance < 0.9f) {
            node = mnuCreateSpriteSpawnNode(resource, parent, 1);
            node->mode = 2;
            sdfListAppend(list, list->count, node);
        }
        break;
    case 2:
        if (effMiscRandUnitFloat(NULL) < 0.8f) {
            node = mnuCreateSpriteSpawnNode(resource, parent, 0);
            node->mode = mode;
            sdfListAppend(list, list->count, node);
        }
        break;
    case 3:
        for (i = 0; i < 6; i++) {
            node = mnuCreateSpriteSpawnNode(resource, parent, i);
            node->mode = 1;
            sdfListAppend(list, list->count, node);
        }
        break;
    }
}

