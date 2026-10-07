#include "common.h"
#include "mnu.h"
#include "mnu_movie.h"
#include "sdf.h"

extern void mnuDrawSprite(s32, s32, s32, s32, s32, s32, s32);

extern MnuStaffMovieWork *mnuMovieWork;

extern s32 sdfAllocGeneralBlock(s32);

extern void *sdfMemoryGetBlockAddress(s32);

extern f32 effMiscRandUnitFloat(s32);

extern void *memset(void *, s32, u32);

extern u8 D_003BC620[];

void func_0026E160(s32 a, s32 b, s32 c, s32 d, s32 value) {
    mnuDrawSprite(a, b, c, d, 0x60, 0x1b, value);
}


extern void func_0026DED0(void *, SdfList *, void *, s8);

typedef struct SpriteSpawnNode {
    s32 x;
    s32 y;
    s32 framesLeft;
    s32 duration;
    u8 direction;
    s8 mode;
    u8 generations;
    u8 variant;
} SpriteSpawnNode;
typedef char SpriteSpawnNode_size[(sizeof(SpriteSpawnNode) == 0x14) ? 1 : -1];

typedef struct MovieResourceGroup {
    s32 allocation;
    SdfList *tasks[10];
    s32 activeCount;
    s32 spawnCountdown;
} MovieResourceGroup;

extern void func_0026DEA8();

SdfList *mnuTickMovieGroup(MovieResourceGroup *owner, SdfList *group) {
    SdfListNode *list = group->head;
    SpriteSpawnNode *node;

    if (list == NULL) {
        sdfDestroyTaskWork(group);
        return 0;
    }
    do {
        node = list->value;
        node->framesLeft = node->framesLeft - 1;
        if (node->framesLeft == node->duration - 5 && node->generations != 0) {
            func_0026DED0(owner, group, node, node->mode);
        }
        if (node->framesLeft == 0) {
            list = sdfListRemoveNode(group, list);
        } else {
            list = list->next;
        }
    } while (list != NULL);
    return group;
}

void func_0026E240(MovieResourceGroup *resources) {
    SdfList **slot;
    s32 i;
    SdfList *group;

    if (resources->spawnCountdown == 0) {
        if (resources->activeCount < 10) {
            group = sdfCreateTaskHeader(0);
            group->onRemove = func_0026DEA8;
            func_0026DED0(resources, group, NULL, 0);
            for (i = 0; i < 10; i++) {
                if (resources->tasks[i] == 0) {
                    resources->tasks[i] = group;
                    resources->activeCount++;
                    break;
                }
            }
        }
        resources->spawnCountdown = (s32)(effMiscRandUnitFloat(0) * 20.0f + 1.0f);
    } else {
        resources->spawnCountdown--;
    }
    slot = resources->tasks;
    for (i = 0; i < 10; i++, slot++) {
        group = *slot;
        if (group != 0) {
            *slot = mnuTickMovieGroup(resources, group);
            if (*slot == 0) {
                resources->activeCount--;
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E388);

typedef struct {
    s32 allocation;    /* 0x00 */
    u8 pad04[0x2C];
    s32 lifetime;      /* 0x30 */
    s32 owner;         /* 0x34 */
    u8 pad38[0xC];
    u8 variant;       /* 0x44 */
    u8 sprite;        /* 0x45 */
    u8 pad46[2];
} MovieSpriteResource;

void *mnuCreateMovieSpriteResource(s32 owner, u8 sprite, u8 variant) {
    s32 allocation = sdfAllocGeneralBlock(0x48);
    MovieSpriteResource *resource = sdfMemoryGetBlockAddress(allocation);
    memset(resource, 0, 0x48);
    resource->allocation = allocation;
    resource->owner = owner;
    resource->sprite = sprite;
    resource->variant = variant;
    resource->lifetime = (s32)(effMiscRandUnitFloat(0) * 30.0f + 10.0f);
    return resource;
}


void mnuReleaseMovieResourceGroup(MovieResourceGroup *resources) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (resources->tasks[i] != 0) {
            sdfDestroyTaskWork(resources->tasks[i]);
        }
    }
    sdfReleaseResourceAllocation(resources->allocation);
}


extern MovieMenuState *mnuMovieMenuState;
extern void func_0026E240(MovieResourceGroup *);
extern void func_0026E388(s32, s32, s32, s32, void *, s32);
extern void sdfSubmitGsTestOneRegisterPacket();
extern void sdfSubmitGsAlphaOneRegisterPacket(u32, u32);
extern void uiDrawUniformColorRect(u32, u32, u32, u32, u32, u32, u32);
extern void uiDrawActiveSurfaceRegion(s32);
extern void sdfDispatchSurfaceWithPreparedTexturePacket(s32);

void func_0026E608(s32 alpha) {
    func_0026E240(mnuMovieMenuState->resources);
    sdfSubmitGsTestOneRegisterPacket(0x30000, 0x53);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, 0x53);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, 0x53);
    uiDrawActiveSurfaceRegion(0x53);
    mnuDrawSprite(0, 0, 0xFFFF, 0x80, 0x20, 0x1D, 0x53);
    sdfDispatchSurfaceWithPreparedTexturePacket(0x53);
    sdfSubmitGsAlphaOneRegisterPacket(0x48, 0x53);
    sdfSubmitGsTestOneRegisterPacket(0x50000, 0x53);
    func_0026E388(0, 0, 0, alpha, mnuMovieMenuState->resources, 0x53);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, 0x53);
    sdfSubmitGsTestOneRegisterPacket(0x30000, 0x53);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, 0x53);
    sdfSubmitGsTestOneRegisterPacket(0x5100DL, 0x53);
}

extern void func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);

void mnuDrawIconAlphaSprite(s32 x, s32 y, s32 z, s32 alpha, s32 sprite, s32 mode, s32 flag, s32 param) {
    func_002BF4E0(x << 4, y << 3, z, (u32)((f32)(alpha << 8) * 0.0078125f), flag, sprite, mode, param);
}

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E798);

void mnuLoadMovieRollSprite(void) {
    mnuMovieWork->spriteSet = effLoadIndexedResource(D_003BC620, "roll.spr", 0);
}

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E8D8);

extern s32 effMiscRandMod(s32, s32);

void mnuUpdateMovieRollEntry(MnuMovieRollEntry *entry) {
    entry->y += entry->velocity;
    entry->timer--;
    if (entry->active && entry->y < -256.0f) {
        entry->active = 0;
        entry->duration = effMiscRandMod(0, 200) + 1;
        entry->timer = entry->duration;
    } else if (entry->timer < 0) {
        if (entry->active) {
            entry->active = 0;
        } else if (mnuMovieWork->scrollTicks < 17000) {
            entry->active = 1;
            switch (effMiscRandMod(0, 8)) {
            case 0:
            case 1:
                entry->sprite = 2;
                break;
            case 2:
                entry->sprite = 1;
                break;
            default:
                entry->sprite = 0;
                break;
            }
            entry->duration = effMiscRandMod(0, 800) + 4000;
            entry->timer = entry->duration;
            entry->velocity = -(effMiscRandUnitFloat(0) + 0.5f) * 0.5f;
            entry->x = effMiscRandUnitFloat(0) * 768.0f / 6.0f + (-256.0f);
            if (entry->column >= 2) {
                entry->x += 128.0f;
            }
            if (entry->column >= 3) {
                entry->x += 128.0f;
            }
            if (entry->column >= 4) {
                entry->x += 128.0f;
            }
            if (entry->column >= 5) {
                entry->x += 128.0f;
            }
            if (entry->column >= 6) {
                entry->x += 128.0f;
            }
            entry->y = effMiscRandUnitFloat(0) * 112.0f + 448.0f;
        }
    }
}

INCLUDE_SDATA(const s32, "game/code_0026E160", mnuMovieWork);

