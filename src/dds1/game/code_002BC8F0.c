#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

extern void func_002BD640(u32, u32);

extern u32 effCreateMappedResource(u32);

extern u32 sdfResourceRetainAddress(u32);

extern u32 fileGetResourceHandle(void);

extern u32 func_002BD9C0(u32, u32);

extern u32 D_003BD11C;

extern u32 D_003BD120;

extern u32 D_003BD128;

extern u32 D_003BD158;

extern u32 D_003BD15C;

extern u32 D_003BD160;

extern u32 D_003BD124;

extern s32 D_003BD118;

extern s32 D_003BD10C;

extern s32 D_003BD110;

extern s32 D_003BD098;

extern s32 D_003BD09C;

extern s32 D_003BD06C;

extern s32 D_003BD05C;

extern u32 D_003BC998;

extern s32 D_003BC99C;

extern u32 D_003BC9A0;

extern u32 D_003BC988;

extern u32 D_003BC984;

extern u32 D_003BC994;

extern u32 func_002EB028(const char *, u32 *, s32);

extern u32 D_003BD068;

extern u32 D_003BC980;

extern s32 D_003BC98C;

extern u32 D_003BC990;

extern u32 D_003BC968;

extern u32 D_003BC96C;

extern u32 D_003BC950;

extern u32 D_003BC954;

typedef struct EffectObjectFlag {
    u8 pad00[0xC];
    u32 flags;
} EffectObjectFlag;

typedef struct EffectObjectNode {
    EffectObjectFlag *object;
    struct EffectObjectNode *prev;
    struct EffectObjectNode *next;
} EffectObjectNode;

extern EffectObjectNode *D_003BC948;

typedef struct EffectRecordGroup {
    u32 unk_00;
    u32 unk_04;
    u32 count;
    u8 *records;
} EffectRecordGroup;

extern EffectRecordGroup D_0038FD88[];

extern u32 D_003BD058;

/* Reference-counted object header (layout inferred from field accesses). */
typedef struct RefObj {
    u8 pad_0x00[0x14]; // 0x00
    s32 cnt14;         // 0x14
    s32 unk18;         // 0x18
    s32 cnt1C;         // 0x1C
} RefObj; // 0x20

extern s32 D_003BC970[2];

extern RefObj *D_003BC978[2];

/* Battle/display work object (layout inferred from field accesses). */
typedef struct BdWork {
    s32 flags;           // 0x00
    s32 phase16_16;      // 0x04: clamped to [0, 0x10000]
    u8 pad_0x08[0x28];  // 0x08
    u32 materialFlags;  // 0x30
    u32 materialColor;  // 0x34
    u8 pad_0x38[0x28];  // 0x38
    void *owner;         // 0x60
    s32 slotIndex;       // 0x64
    u8 pad_0x68[0x34];  // 0x68
    s32 alternate;      // 0x9C: alternate work entry when nonzero
} BdWork; // 0xA0

/* Resource slot header: the 0x80-byte descriptions parallel 0xA0-byte work entries. */
typedef struct EffectSlotSet {
    u8 pad_00[8];        // 0x00
    u32 count;           // 0x08
    u8 pad_0C[4];        // 0x0C
    u32 descriptions;    // 0x10
    u32 workAllocation;  // 0x14
    s32 workEntries;     // 0x18
} EffectSlotSet;

extern u32 D_003BC94C;

extern void func_002BD3D8(void *, s32, void *);

extern u32 D_003BC9B0[2];

extern u32 D_003BC9B8[2];

extern u8 D_003BC9C0[2];

extern u8 D_003BC9C8[2];

extern u8 D_003BD088[];

extern u8 D_003BD090[];

extern u8 *D_003BD074;

extern u8 D_003BCA40[];

extern u8 D_003BD050[];

extern u8 D_003BD000[];

extern u8 D_003BD108;

extern u8 D_003BD109;

extern u8 D_003BD10A;

/* Set of texture handles released and cleared one by one (layout inferred). */
typedef struct TexHandleSet {
    u8 pad_0x00[0x1C]; // 0x00
    u32 count;         // 0x1C
    u8 pad_0x20[4];    // 0x20
    void **handles;    // 0x24
} TexHandleSet; // 0x28

extern void func_002BD5C8(s32);

extern u16 D_003BC944;

extern u8 *D_003BC958;

extern void sdfTexReleaseReference(void *);

extern void *func_002D03F8(u32);

extern u32 D_003BC960[2];

extern s8 D_003BC9AC;

extern f32 D_003BC9A8;

extern s32 D_003BD060;

extern s32 D_003BD060;

extern char D_003BD080[];

extern s32 func_003014F0(char *, const char *, ...);

extern u8 D_003BD078[];

extern s32 D_003BD060;

extern s32 D_003BD070;

extern u32 D_003BD114;

extern u8 D_003BCF30[];

extern s32 func_003014F0(char *, const char *, ...);

extern u32 D_003BD064;

extern void *func_002CFEB8(u32);

extern void func_002CFF98(void *);

extern char D_003BD198[];

u32 effLoadIndexedResource(const char *base, const char *name, u32 retainResource) {
    char path[0x80];
    u32 handle;
    u32 resource;
    u32 result;

    func_003014F0(path, D_003BD198, base, name);
    resource = func_002EB028(path, &handle, 0);
    result = func_002BD9C0(resource, retainResource);
    if (retainResource == 0) {
        func_002D0918(resource);
    }
    return result;
}

void func_002BC978(u64 job, u32 *out) {
    u32 resource;
    u32 instance;

    resource = fileGetResourceHandle();
    instance = func_002BD9C0(resource, 0);
    *out = instance;
    func_002D0918(resource);
    func_002887A0(job);
}

void func_002BC9D0(u64 job, u32 *out) {
    u32 resource;
    u32 instance;

    resource = fileGetResourceHandle();
    instance = func_002BD9C0(resource, 1);
    *out = instance;
    func_002887A0(job);
}

extern s32 func_003014F0(char *, const char *, ...);

extern void func_00288AD0(const char *, u32, void (*)(u64, u32 *), u32 *);

void effRequestResourceByMode(const char *base, const char *name, u32 mode, u32 *out) {
    char path[0x80];

    func_003014F0(path, D_003BD198, base, name);
    *out = 0;
    if (mode == 1) {
        func_00288AD0(path, 0, func_002BC9D0, out);
    } else {
        func_00288AD0(path, 0, func_002BC978, out);
    }
}

u32 effLoadMappedResource(const char *base, const char *name) {
    char path[0x80];
    u32 handle;
    u32 value;
    u32 resource;

    func_003014F0(path, D_003BD198, base, name);
    resource = func_002EB028(path, &handle, 0);
    value = effCreateMappedResource(handle);
    func_002D0918(resource);
    return value;
}

void func_002BCB18(u64 job, u32 *out) {
    u32 resource;
    u32 address;
    u32 mapped;

    resource = fileGetResourceHandle();
    address = sdfResourceRetainAddress(resource);
    mapped = effCreateMappedResource(address);
    *out = mapped;
    func_002D0918(resource);
    func_002887A0(job);
}

void effRequestMappedResource(const char *base, const char *name, u32 *out) {
    char path[0x80];

    func_003014F0(path, D_003BD198, base, name);
    *out = 0;
    func_00288AD0(path, 0, func_002BCB18, out);
}

void *func_002BCBD0(void *owner) {
    u32 *data = func_002CFEB8(0x44);
    memset(data, 0, 0x44);
    data[0] = (u32)owner;
    return data;
}

typedef struct EffectRecord {
    void *owner;
    s32 slot;
    struct EffectRecord *prev;
    struct EffectRecord *next;
} EffectRecord;

typedef struct EffectOwnerRecord {
    void *owner;
    EffectRecord *entries[16];
} EffectOwnerRecord;

/* Per-slot effect data (0x80 bytes each); only the bucket index is known. */
typedef struct EffectSlot {
    u8 pad_0x00[0x28]; // 0x00
    s32 bucket;        // 0x28
    u8 pad_0x2C[0x54]; // 0x2C
} EffectSlot; // 0x80

typedef struct EffectSlotOwner {
    u8 pad_0x00[0x10];  // 0x00
    EffectSlot *slots;  // 0x10
} EffectSlotOwner;

void func_002BCC20(void *owner, EffectOwnerRecord *list, EffectSlotOwner *work, s32 slot) {
    EffectSlot *entry = &work->slots[slot];
    s32 bucket = entry->bucket;
    EffectRecord *record;

    if (bucket >= 16) {
        bucket = 15;
    }
    record = func_002CFEB8(sizeof(*record));
    record->prev = 0;
    record->owner = owner;
    record->slot = slot;
    record->next = list->entries[bucket];
    list->entries[bucket] = record;
}

s32 func_002BCCA8(EffectOwnerRecord *list, EffectSlotOwner *work, s32 slot) {
    EffectSlot *slotData = &work->slots[slot];
    s32 bucket = slotData->bucket;
    EffectRecord *record = list->entries[bucket];

    while (record != 0) {
        if (record->slot == slot) {
            if (record->prev != 0) {
                record->prev->next = record->next;
            }
            if (record->next != 0) {
                record->next->prev = record->prev;
            }
            if (record == list->entries[bucket]) {
                list->entries[bucket] = record->next;
            }
            func_002CFF98(record);
            return 1;
        }
        record = record->next;
    }
    return 0;
}

void effReleaseRecordBuckets(list)
EffectOwnerRecord *list;
{
    EffectRecord **entry = list->entries;
    s32 remaining = 15;

    do {
        EffectRecord *record = *entry;
        while (record != 0) {
            if (record->prev != 0) {
                record->prev->next = record->next;
            }
            if (record->next != 0) {
                record->next->prev = record->prev;
            }
            if (record == *entry) {
                *entry = record->next;
            }
            func_002CFF98(record);
            record = record->next;
        }
        ++entry;
    } while (--remaining >= 0);
}

extern void func_002BF790(u32, u32, u32, u32, u32, s32, s32);

extern void itfGridLookupValueOrDefault(void *, s32);

u32 effDispatchRecordBuckets(u32 active, EffectOwnerRecord *list, s32 option) {
    EffectRecord **entry = list->entries;
    s32 remaining = 15;

    do {
        EffectRecord *record = *entry;
        while (record != 0) {
            func_002BF790(0, 0, 0, 0, (u32)list->owner, record->slot, option);
            if (active != 0) {
                itfGridLookupValueOrDefault(list->owner, record->slot);
            }
            record = record->next;
        }
        ++entry;
    } while (--remaining >= 0);
    return 1;
}

u32 func_002BCEA8(u32 arg0) {
    effReleaseRecordBuckets();
    func_002CFF98(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002BC8F0", effConvertParamValue);

extern u32 effConvertParamValue(u32 *, void *, void *, void *);

u32 effSumRecordStatuses(u32 *payload) {
    EffectRecordGroup *group = &D_0038FD88[payload[5]];
    u32 total = 0;
    u32 i;

    for (i = 0; i < group->count; i++) {
        total += effConvertParamValue((u32 *)(group->records + i * 0x18 + 4), 0, 0, 0);
    }
    return total;
}

INCLUDE_ASM(const s32, "game/code_002BC8F0", func_002BD028);

typedef struct {
    s32 count;
    void *records;
    void *allocation;
} EffMappedResource;

typedef struct {
    u8 pad00[0x14];
    s32 count;
} EffMappedHeader;

extern void *func_002BD028(u32 source, EffMappedHeader *header);

u32 effCreateMappedResource(u32 source) {
    EffMappedResource *work = (EffMappedResource *)func_002CFEB8(0xC);
    EffMappedHeader header;

    work->records = func_002BD028(source, &header);
    work->allocation = (void *)sdfResourceRetainAddress((u32)work->records);
    work->count = header.count;
    return (u32)work;
}

u32 *func_002BD258(u32 kind) {
    u32 *header = func_002CFEB8(0xC);
    u32 allocation;
    u32 data;
    u32 size;
    void *scratch;

    header[0] = 1;
    allocation = (u32)func_002D03F8(0x24);
    header[1] = allocation;
    data = sdfResourceRetainAddress(allocation);
    header[2] = data;
    memset((void *)data, 0, 0x24);
    {
        u32 *payload = (u32 *)header[2];
        payload[5] = kind;
        size = effSumRecordStatuses(payload);
    }
    scratch = func_002CFEB8(size);
    ((u32 *)header[2])[8] = (u32)scratch;
    memset(scratch, 0, size);
    ((u32 *)header[2])[6] = size;
    return header;
}

typedef struct PackedEffectRecord {
    u8 unk_00[0x20];
    void *storage;
} PackedEffectRecord;

typedef struct PackedEffectBatch {
    s32 count;
    u32 job;
    PackedEffectRecord *records;
} PackedEffectBatch;

u32 effDestroyPackedBatch(PackedEffectBatch *batch) {
    s32 i;
    for (i = 0; i < batch->count; i++) {
        func_002CFF98(batch->records[i].storage);
    }
    func_002D0918(batch->job);
    func_002CFF98(batch);
    return 1;
}

u32 func_002BD378(s32 arg0) {
    func_002D0918(((EffectSlotSet *)arg0)->workAllocation);
    return 1;
}

s32 func_002BD398(s32 work, s32 index) {
    s32 alternate;
    s32 entry;

    entry = index * 0xa0 + ((EffectSlotSet *)work)->workEntries;
    alternate = ((BdWork *)entry)->alternate;
    if (alternate != 0) {
        entry = alternate;
    }
    return entry;
}

void func_002BD3B8(BdWork *p) {
    if (p->flags & 1) {
        p->phase16_16 = 0;
    } else {
        p->phase16_16 = 0x10000;
    }
}

INCLUDE_ASM(const s32, "game/code_002BC8F0", func_002BD3D8);

void func_002BD5A0(s32 arg0, s32 arg1) {
    func_002BD3D8(arg0, arg1, ((EffectSlotSet *)arg0)->workEntries + arg1 * 0xa0);
}

void func_002BD5C8(s32 work) {
    u32 index;
    for (index = 0; index < ((EffectSlotSet *)work)->count; index++) {
        func_002BD640(work, index);
    }
}

void func_002BD620(void *a0, s32 a1, BdWork *p) {
    p->owner = a0;
    p->slotIndex = a1;
    func_002BD3D8(a0, a1, p);
}

void func_002BD640(u32 work, u32 index) {
    s32 entry;

    entry = ((EffectSlotSet *)work)->workEntries + (s32)index * 0xa0;
    memset(entry, 0, 0xa0);
    func_002BD620(work, index, entry);
}

u32 func_002BD6A8(TexHandleSet *set, u8 *data, s32 release, s32 only) {
    u32 i = 0;
    u8 *entry = data;

    entry += *(u32 *)(entry + 0xC);

    if (set->count != 0) {
        do {
            s32 *resource = (s32 *)(data + *(u32 *)(entry + 4));

            entry += 8;
            if (release == 0) {
                if (only == -1 || only == (s32)i) {
                    if (set->handles[i] == 0) {
                        set->handles[i] = (void *)func_002D3288(resource);
                    }
                } else {
                    set->handles[i] = 0;
                }
            } else {
                set->handles[i] = 0;
            }
            i++;
        } while (i < set->count);
    }
    return (u32)entry;
}

extern u32 func_002D3288(s32 *);

void effResolveAndReleaseResource(u32 *handle) {
    if (*handle != 0) {
        u32 data = sdfResourceRetainAddress(*handle);
        func_002BD6A8(handle, data, 0, -1);
        func_002D0A60(*handle);
    }
}

void effResolveAndReleaseSelectedResource(u32 *handle, s32 slot) {
    if (*handle != 0) {
        u32 data = sdfResourceRetainAddress(*handle);
        func_002BD6A8(handle, data, 0, slot);
        func_002D0A60(*handle);
    }
}

void func_002BD870(TexHandleSet *set) {
    u32 i;

    for (i = 0; i < set->count; i++) {
        if (set->handles[i] != 0) {
            sdfTexReleaseReference(set->handles[i]);
            set->handles[i] = 0;
        }
    }
    func_002BD5C8((s32)set);
}

u8 func_002BD8F8(s32 arg0) {
    return **(s32 **)(arg0 + 0x24) != 0;
}

u32 *effCreatePayload(u32 count) {
    u32 size = count * 0x6c;
    u32 *header = func_002CFEB8(0xC);
    u32 allocation = (u32)func_002D03F8(size);
    u32 data;

    header[1] = count;
    header[0] = allocation;
    data = sdfResourceRetainAddress(allocation);
    header[2] = data;
    memset((void *)data, 0, size);
    return header;
}

u32 func_002BD988(u32 arg0) {
    func_002D0918(*(u32 *)arg0);
    func_002CFF98(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002BC8F0", func_002BD9C0);

extern void func_002BD640(u32, u32);

u32 *effCreateResourceSlotSet(u32 *source, u32 slot, u32 count) {
    u32 *effect = (u32 *)func_002CFEB8(0x30);
    u32 index = 0;
    effect[1] = 1;
    {
        u32 mode = source[7];
        u32 size = source[9];
        effect[7] = mode;
        effect[9] = size;
    }
    effect[0] = 0;
    effect[8] = 0;
    effect[2] = count;
    effect[3] = (u32)func_002D03F8(count * 0x80);
    effect[4] = sdfResourceRetainAddress(effect[3]);
    effect[5] = (u32)func_002D03F8(effect[2] * 0xA0);
    effect[6] = sdfResourceRetainAddress(effect[5]);
    if (effect[2] != 0) {
        do {
            memcpy((void *)(effect[4] + index * 0x80),
                   (void *)(source[4] + slot * 0x80), 0x80);
            func_002BD640((u32)effect, index);
            index++;
        } while (index < effect[2]);
    }
    return effect;
}

u32 func_002BDD60(u32 arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)arg0;
    if (*piVar1 != 0) {
        func_002D0918(*piVar1);
    }
    if (piVar1[1] == 0) {
        func_002BD870(arg0);
        func_002D0918(piVar1[8]);
    }
    func_002D0918(piVar1[3]);
    func_002BD378(arg0);
    func_002CFF98(arg0);
    return 1;
}

u32 func_002BDDD0(u32 *arg0, u32 arg1, u32 arg2) {
    *arg0 = arg2;
    arg0[4] = arg1;
    if ((arg2 & 2) != 0) {
        func_002BD3B8((BdWork *)arg0);
    }
    arg0[2] = arg0[2] + 1;
    return 1;
}

u32 func_002BDE18(u32 arg0, s32 arg1, s32 arg2, u32 arg3) {
    func_002BDDD0(arg0, *(s32 *)(arg1 + 8) + arg2 * 0x24, arg3);
    return 1;
}

u32 func_002BDE50(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = 0;
    return 1;
}

u32 func_002BDE60(s32 work, s32 index, BdWork *effect) {
    if (effect->phase16_16 > 0x10000) {
        s32 flags = effect->flags;
        effect->phase16_16 = 0x10000;
        if (flags & 4) {
            if (flags & 8) {
                effect->flags = flags & ~1;
            } else {
                func_002BD5A0(work, index);
            }
            return 0;
        }
    }
    return 1;
}

u32 func_002BDEC8(s32 work, s32 index, BdWork *effect) {
    if (effect->phase16_16 < 0) {
        s32 flags = effect->flags;
        effect->phase16_16 = 0;
        if (flags & 4) {
            if (flags & 8) {
                effect->flags = flags | 1;
            } else {
                func_002BD5A0(work, index);
            }
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002BC8F0", func_002BDF28);

u32 func_002BE0C0(s32 work, s32 index, void *value) {
    s32 offset = index * 0xA0;
    if (*(void **)(offset + ((EffectSlotSet *)work)->workEntries + 0x9C) == 0) {
        func_002BD620((void *)work, index, (BdWork *)value);
    }
    *(void **)(offset + ((EffectSlotSet *)work)->workEntries + 0x9C) = value;
    return 1;
}

typedef struct EffectMaterialSlot {
    u32 value;
    u8 unk_04[0x10];
} EffectMaterialSlot;

u32 effSetMaterialSlots(s32 work, s32 index, u32 value, BdWork *asset) {
    s32 offset = index * 0xA0;
    u32 i;
    EffectMaterialSlot *slots;
    if (*(void **)(offset + ((EffectSlotSet *)work)->workEntries + 0x9C) == NULL) {
        func_002BD620((void *)work, index, asset);
    }
    *(BdWork **)(offset + ((EffectSlotSet *)work)->workEntries + 0x9C) = asset;
    slots = (EffectMaterialSlot *)((u8 *)asset + 0x30);
    for (i = 0; i < 2; i++) {
        slots[i].value = value;
    }
    return 1;
}

u32 func_002BE1C8(s32 arg0, s32 arg1) {
    ((BdWork *)(arg1 * 0xa0 + ((EffectSlotSet *)arg0)->workEntries))->alternate = 0;
    return 1;
}

extern u32 func_002BDF28(s32, s32, BdWork *);

u32 func_002BE1E8(s32 work, s32 index, u32 value, u32 flags) {
    s32 effect = ((EffectSlotSet *)work)->workEntries + index * 0xA0;
    func_002BDDD0((u32 *)(effect + 0x28), value, flags);
    func_002BDF28(work, index, (BdWork *)effect);
    return 1;
}

u32 func_002BE258(s32 work, s32 index, s32 data, s32 item, u32 flags) {
    s32 effect = ((EffectSlotSet *)work)->workEntries + index * 0xA0;
    func_002BDDD0((u32 *)(effect + 0x28), *(u32 *)(data + 8) + item * 0x24, flags);
    func_002BDF28(work, index, (BdWork *)effect);
    return 1;
}

u32 func_002BE2D8(s32 work, s32 index, s32 data, s32 item,
                  u32 flags, u32 color, u32 option) {
    s32 effect = ((EffectSlotSet *)work)->workEntries + index * 0xA0;
    func_002BDDD0((u32 *)(effect + 0x28), *(u32 *)(data + 8) + item * 0x24, option);
    func_002BDF28(work, index, (BdWork *)effect);
    ((BdWork *)effect)->materialFlags = flags;
    ((BdWork *)effect)->materialColor = color;
    return 1;
}

u32 effConfigureWithDefaultSetting(u32 effect, u32 slot, u32 kind, u32 value, u32 flags, u32 color) {
    func_002BE2D8(effect, slot, kind, value, flags, 0, color);
    return 1;
}

u32 effResetRecordRun(u8 *table, u32 first, u32 arg) {
    u32 i = 0;
    u32 index;

    do {
        func_002BDE50((first + i) * 0xA0 + ((EffectSlotSet *)table)->workEntries + 0x28, arg);
        i++;
        index = first + i;
    } while (index < ((EffectSlotSet *)table)->count && (*(u32 *)(index * 0x80 + ((EffectSlotSet *)table)->descriptions + 0x18) & 0x20));
    return 1;
}

void effSelectPresetByKind(u32 mode, u32 arg) {
    switch (mode) {
    case 0:
        func_002C0A48(0x44, arg);
        return;
    case 1:
        func_002C0A48(0x48, arg);
        return;
    case 2:
        func_002C0A48(0x42, arg);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_002BC8F0", func_002BE4B8);

INCLUDE_ASM(const s32, "game/code_002BC8F0", func_002BE728);

INCLUDE_ASM(const s32, "game/code_002BC8F0", func_002BE8A8);

void func_002BED28(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    effSelectPresetByKind(arg6, arg7);
    func_002C0F88(arg0, arg1, arg2, arg3, arg4, arg5, arg7);
    func_002C0A48(0x44, arg7);
}

