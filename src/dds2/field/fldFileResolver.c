#include "common.h"

typedef struct FldFileNameEntry {
    u32 word00;
    const char *name;
    u32 word08;
} FldFileNameEntry;

typedef struct FldFileNameTable {
    u32 count;
    FldFileNameEntry entries[1];
} FldFileNameTable;

typedef struct FldFileResource {
    u32 id;
    u32 word04;
    const char *name;
    u32 word0C;
    f32 *transform;
    u32 word14;
    FldFileNameTable *names;
    u32 word1C;
    void *data;
} FldFileResource;

extern FldFileResource *D_00438EC0;
extern u32 D_00438EC4;
extern void *dds3GetWorldSecondaryObject(void);
extern void *dds3FindIndexedObjectChainNodeByName(void *world, s32 index, const char *name);
extern s32 strcmp(const char *left, const char *right);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_001287B8);

void *func_001288C8(const char *name) {
    u32 i = 0;
    void *world = dds3GetWorldSecondaryObject();
    FldFileResource *resource = D_00438EC0;

    for (; i < D_00438EC4; i++, resource++) {
        if (resource->names != NULL) {
            FldFileNameEntry *entry = resource->names->entries;
            u32 j = 0;

            for (; j < resource->names->count; j++, entry++) {
                if (strcmp(name, entry->name) == 0) {
                    void *object = dds3FindIndexedObjectChainNodeByName(world, 11, resource->name);
                    if (object != NULL) {
                        return object;
                    }
                }
            }
        }
    }
    return NULL;
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_001289A8);
