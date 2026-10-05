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

extern FldFileResource *D_003BD7B8;
extern u32 D_003BD7BC;
extern void *dds3GetWorldSecondaryObject(void);
extern void *dds3FindIndexedObjectChainNodeByName(void *world, s32 index, const char *name);
extern u32 dds3AdvanceWorldCounter(void);
extern void *dds3SpawnInnerVecObj6(s32, u32 *, u32 *);
extern void dds3SetWorldEntryCallbackTarget(void *, const char *);
extern char D_0039FD50[]; /* "FLD_DMY_MATTER" */
extern s32 fldGetRecordValueById(s32 id);
extern void fldSetRecordValueById(s32 id, s32 value);
extern s32 strcmp(const char *left, const char *right);

void *fldCreateDummyMatter(void) {
    u32 args[8];
    void *matter;
    args[0] = 0;
    args[1] = 0;
    args[2] = 0;
    args[3] = 0;
    args[4] = 0;
    args[5] = 0;
    args[6] = 0;
    args[7] = 0;
    matter = dds3SpawnInnerVecObj6(dds3AdvanceWorldCounter(), args, args + 4);
    dds3SetWorldEntryCallbackTarget(matter, D_0039FD50);
    return matter;
}

void *func_00126200(u32 id) {
    u32 i = 0;
    void *world = dds3GetWorldSecondaryObject();
    FldFileResource *resource = D_003BD7B8;

    for (; i < D_003BD7BC; i++, resource++) {
        if (id == resource->id) {
            if (resource->names != NULL) {
                FldFileNameEntry *entry = resource->names->entries;
                u32 j = 0;

                for (; j < resource->names->count; j++, entry++) {
                    void *object = dds3FindIndexedObjectChainNodeByName(world, 6, entry->name);
                    if (object != NULL) {
                        return object;
                    }
                }
            } else {
                void *object = (void *)fldGetRecordValueById(resource->id);
                if (object == NULL) {
                    object = fldCreateDummyMatter();
                    fldSetRecordValueById(resource->id, (s32)object);
                }
                return object;
            }
        }
    }
    return NULL;
}

void *func_00126310(const char *name) {
    u32 i = 0;
    void *world = dds3GetWorldSecondaryObject();
    FldFileResource *resource = D_003BD7B8;

    for (; i < D_003BD7BC; i++, resource++) {
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

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_0039FD50);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_001263F0);
