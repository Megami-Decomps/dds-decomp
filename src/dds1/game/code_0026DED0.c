#include "common.h"
#include "sdf.h"
#include "mnu_sprite_resource.h"



extern SpriteSpawnNode *func_0026DD30(MnuSpriteResourceGroup *, SpriteSpawnNode *, s8);
extern f32 effMiscRandUnitFloat(void *);
extern u32 effMiscRandMod(void *, u32);

void func_0026DED0(MnuSpriteResourceGroup *resource, SdfList *list, void *parentData, s8 mode) {
    SpriteSpawnNode *parent = parentData;
    SpriteSpawnNode *node;
    f32 chance;
    s32 i;

    switch (mode) {
    case 0:
        node = func_0026DD30(resource, NULL, 0);
        node->mode = 3;
        node->variant = effMiscRandMod(NULL, 2);
        sdfListAppend(list, list->count, node);
        break;
    case 1:
        chance = effMiscRandUnitFloat(NULL);
        if (chance < 0.8f) {
            node = func_0026DD30(resource, parent, 0);
            node->mode = mode;
            node->variant = effMiscRandMod(NULL, 2);
            sdfListAppend(list, list->count, node);
        }
        /* The same roll can admit both branches. */
        if (chance < 0.9f) {
            node = func_0026DD30(resource, parent, 1);
            node->mode = 2;
            node->variant = effMiscRandMod(NULL, 2);
            sdfListAppend(list, list->count, node);
        }
        break;
    case 2:
        if (effMiscRandUnitFloat(NULL) < 0.8f) {
            node = func_0026DD30(resource, parent, 0);
            node->mode = mode;
            node->variant = effMiscRandMod(NULL, 2);
            sdfListAppend(list, list->count, node);
        }
        break;
    case 3:
        for (i = 0; i < 6; i++) {
            node = func_0026DD30(resource, parent, i);
            node->mode = 1;
            node->variant = effMiscRandMod(NULL, 2);
            sdfListAppend(list, list->count, node);
        }
        break;
    }
}

