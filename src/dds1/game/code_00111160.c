#include "common.h"

s32 func_00111160(u32 kind) {
    s32 result = 0;

    if (kind >= 4) {
        if (kind >= 8) {
            result = kind == 8;
        }
    }
    return result;
}

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    s32 unk8;     /* 0x8 */
    u8 unkC[0xC]; /* 0xC */
    void *unk18;  /* 0x18 */
} ActionObj;

extern ActionObj *func_00110880();

ActionObj *evtSpawnActionObj2(s32 a, s32 b) {
    ActionObj *obj = func_00110880(2);

    obj->unk4 = a;
    obj->unk8 = b;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_00111160", func_001111C8);

INCLUDE_ASM(const s32, "game/code_00111160", func_00111258);

s32 dds3GetWorldSlotValue(u8 *obj, s32 index) {
    return *(s32 *)(*(u8 **)(obj + 0x18) + (index << 2));
}

INCLUDE_ASM(const s32, "game/code_00111160", func_001112C0);

INCLUDE_ASM(const s32, "game/code_00111160", func_00111388);

extern void *func_002CFEB8(s32 size);

typedef struct ObjWithWork {
    u8 unk0[0x18];
    u32 *work;
} ObjWithWork;

s32 func_00111400(ObjWithWork *obj) {
    obj->work = func_002CFEB8(0x10);
    memset(obj->work, 0, 0x10);
    return 1;
}

void dds3ReleaseWorldSlotResource(ObjWithWork *obj) {
    u32 *slot;

    slot = obj->work;
    dds3ReleaseSlotPath();
    dds3ExchangeSlot(*slot, 0, 1);
    func_002CFF98(slot);
}
