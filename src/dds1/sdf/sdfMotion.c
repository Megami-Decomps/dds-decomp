#include "common.h"

typedef struct {
    void *unk0;
    void *unk4;
} Pair;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Triple;

typedef struct {
    u8 pad[0x30];
    u8 unk30;
    u8 unk31;
} StateByte;

typedef struct {
    f32 *unk0;
    f32 *unk4;
    f32 unk8;
} BlendArg;

typedef struct VTab {
    void (*fn)(void);
} VTab;

typedef struct VObj {
    VTab *unk0;
} VObj;

typedef struct KeyOut {
    f32 *p0;
    f32 *p4;
    f32 f8;
} KeyOut;

typedef struct {
    Pair pair;
    s32 unk8;
    s32 unkC;
} TmpBuf;

typedef struct {
    s32 unk0;
    void *unk4;
} HasPtr4;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    s32 *arr;
} ArrHolder;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    ArrHolder *unkC;
} MidPtr;

typedef struct {
    s32 unk0;
    MidPtr *unk4;
} Src360;

typedef struct {
    Pair pair;
    s32 unk8;
    s32 unkC;
} Dst360;

typedef struct {
    s32 i0;
    s32 i4;
    s32 i8;
    s32 iC;
    s32 i10;
    s32 i14;
    s32 i18;
    s32 i1C;
    s32 i20;
    s32 i24;
    s32 i28;
} SubI;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    SubI *sub;
    s32 res;
} CmdI;

typedef struct {
    s32 w0;
    s32 w4;
    s32 w8;
    s32 wC;
    s32 w10;
    s32 w14;
    s32 w18;
    f32 f1C;
} SubF;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    SubF *sub;
    f32 res;
} CmdF;

typedef struct {
    u8 pad[0x14];
    u16 u14;
} SubU;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    SubU *sub;
    s8 res;
} CmdB;
typedef struct {
    u16 u0;
    u16 u2;
    u16 n;
    u16 stride;
    u16 data[1];
} Keys;

typedef struct {
    u8 pad[0x2E];
    u16 unk2E;
    u8 unk30;
    u8 unk31;
    u8 unk32;
} Ctl7;

typedef struct {
    s32 u0;
    Ctl7 *ctl;
    Keys *keys;
} Arg7;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    void *sub;
} HasSub;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    SubU *sub;
} HasSubU;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    void *sub;
    f32 f10;
} HasSubF;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    void *sub;
    f32 f10[5];
} HasArr;

typedef struct {
    s64 a;
    s64 b;
} __attribute__((packed)) P16;

typedef struct {
    P16 p;
    s32 c;
} Blk;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    void *sub;
    Blk blk;
} DstBlk;
typedef struct {
    s32 u0;
    s16 n4;
    s16 pad6;
    s32 u8;
    void **arrC;
} ArrObj;

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    s32 unkC;
    ArrObj *unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s32 unk24;
    s32 unk28;
    s16 unk2C;
    u16 unk2E;
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 pad33;
} Motion;

typedef struct {
    void *unk0;
    void *unk4;
    s32 u8;
    s32 uC;
    ArrObj *unk10;
} Obj308;

typedef struct {
    u8 pad[0x14];
    void *unk14;
} HasLink14;

typedef struct {
    u16 u0;
    u16 n;
    void **tbl;
    s32 data[1];
} KeysH;

typedef struct FuncTab {
    void *w0;
    void (*w4)(void *a0, void *a1);
    void (*w8)(void *a0, void *a1, f32 t);
    void (*wC)(void *a0);
    void (*w10)(void *a0, f32 t1, f32 t2);
} FuncTab;

void *func_002CFF68(s32 size);
void func_002CFF98(void *a0);
void *func_002CFEB8(s32 size);
ArrObj *func_002E75F0(u16 n, s32 e1, s32 e2);
s32 func_002DB1C8(void *a0, s32 a1, s32 a2);
s32 func_002D7D68(void *a0, s32 a1);
void func_002DA3C0(void *a0, s32 a1);
void func_002DA3D8(void *a0, s32 a1);
void func_002DA3F0(void *a0, s32 a1);
void func_002DA408(void *a0, s32 a1);
void func_002DA420(void *a0, f32 a1);
Blk *func_002DA490(void *a0);
void func_002DA548(void *a0, void *a1);
void func_002DA5B0(void *a0, s32 a1);
Blk *func_002DA5F8(void *a0);
void func_002DA6B0(void *a0, void *a1);
void func_002E7680(void *a0);
void func_002DB220(Pair *a0, void *a1, void *a2);
void func_002DB3D0(Motion *a0, s32 a1, s32 a2, f32 t0, f32 t1);
f32 func_002DB8D8(KeyOut *a0);
void func_002DB7C8(void *a0, KeyOut *out, f32 t);
s32 func_002DB958(KeyOut *a0);
void func_002DBA80(void *tmp, void *src, void *tbl, s32 x);
void func_002DC360(Dst360 *a0, Src360 *a1, void *a2, s32 a3);
extern s32 (*D_003981B8[])(void *a0, s32 a1);
extern s32 (*D_00398248[])(void *a0, s32 a1);
extern void *D_003981D0[];
extern void *D_003981E8[];
extern void *D_00398200[];
extern void *D_00398218[];
extern void *D_00398230[];
extern void *D_00398270[];
extern void *D_00398288[];
extern void *D_003982A0[];
extern void *D_003982B8[];
extern void *D_003982D0[];
extern void *D_003982E8[];
extern void *D_00398300[];
extern void *D_00398318[];
extern void *D_00398330[];
extern void *D_00398348[];

void func_002DB1F8(VObj *a0) {
    a0->unk0->fn();
}

void func_002DB220(Pair *a0, void *a1, void *a2) {
    a0->unk0 = a2;
    a0->unk4 = a1;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB230);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB308);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB3D0);
void func_002DB518(void *a0, s32 a1, s32 a2) {
    func_002DB3D0(a0, a1, a2, 0.0f, 0.0f);
}


INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB538);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB660);
void func_002DB768(StateByte *a0) {
    u8 t;

    t = a0->unk30;
    if (t != 6) {
        a0->unk31 = t;
        a0->unk30 = 6;
    }
}

void func_002DB788(StateByte *a0) {
    if (a0->unk30 == 6) {
        a0->unk30 = a0->unk31;
    }
}

void func_002DB7A8(void *a0) {
    func_002CFF98(a0);
}

void func_002DB7C0(Triple *a0, s32 a1) {
    a0->unk8 = a1;
}
INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB7C8);

f32 func_002DB8D8(KeyOut *a0) {
    f32 t;
    f32 a;

    t = a0->f8;
    a = *a0->p0;
    return (a + (*a0->p4 * t)) - (a * t);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB900);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB958);

void func_002DB9D8(f32 *dst, f32 *src1, f32 *src2, f32 t) {
    s32 i;
    f32 nt;

    i = 0;
    nt = 1.0f - t;
    do {
        dst[i] = src1[i] * nt + src2[i] * t;
        i++;
    } while (i != 5);
}

void func_002DBA28(KeyOut *src, f32 *dst) {
    func_002DB9D8(dst, src->p0, src->p4, src->f8);
}

s32 func_002DBA50(void *a0, s32 a1) {
    return D_003981B8[(u16)a1](a0, a1);
}

void func_002DBA80(void *tmp, void *src, void *tbl, s32 x) {
    func_002DB220(tmp, src, tbl);
    ((TmpBuf *)tmp)->unkC = func_002D7D68(((HasPtr4 *)src)->unk4, x);
}

void *func_002DBAD0(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x20);
    func_002DBA80(r, a0, D_003981D0, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DBB30);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DBBB0);

void *func_002DBC60(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x20);
    func_002DBA80(r, a0, D_003981E8, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DBCC0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DBD80);

void *func_002DBE60(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x20);
    func_002DBA80(r, a0, D_00398200, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DBEC0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DBF40);

void *func_002DBFF0(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x20);
    func_002DBA80(r, a0, D_00398218, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC050);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC0E8);

void *func_002DC1A0(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DBA80(r, a0, D_00398230, a2);
    return r;
}

void func_002DC200(HasSubU *a0, f32 t) {
    KeyOut b;
    SubU *s;

    func_002DB7C8(a0, &b, t);
    s = a0->sub;
    if (*(u8 *)b.p0 == 0) {
        s->u14 = s->u14 | 0x10;
    } else {
        s->u14 = s->u14 & 0xFFEF;
    }
}

void func_002DC258(HasSubU *a0, f32 t) {
    KeyOut b;
    SubU *s;

    func_002DB7C8(a0, &b, t);
    s = a0->sub;
    if (*(u8 *)b.p0 == 0) {
        s->u14 = s->u14 | 0x10;
    } else {
        s->u14 = s->u14 & 0xFFEF;
    }
}

void func_002DC2B0(CmdB *a0) {
    a0->res = (s8)(((a0->sub->u14 >> 4) ^ 1) & 1);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC2D0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC2F0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC310);

s32 func_002DC330(void *a0, s32 a1) {
    return D_00398248[(u16)a1](a0, a1);
}

void func_002DC360(Dst360 *a0, Src360 *a1, void *a2, s32 a3) {
    func_002DB220(&a0->pair, a1, a2);
    a0->unkC = a1->unk4->unkC->arr[a3];
}

void *func_002DC3B8(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DC360(r, a0, D_00398270, a2);
    return r;
}

void func_002DC418(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA3D8(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC458);

void func_002DC508(CmdI *a0) {
    a0->res = a0->sub->i14;
}

void *func_002DC518(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DC360(r, a0, D_00398288, a2);
    return r;
}

void func_002DC578(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA3C0(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC5B8);

void func_002DC668(CmdI *a0) {
    a0->res = a0->sub->i10;
}

void *func_002DC678(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DC360(r, a0, D_003982A0, a2);
    return r;
}

void func_002DC6D8(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA3F0(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC718);

void func_002DC7C8(CmdI *a0) {
    a0->res = a0->sub->i20;
}

void *func_002DC7D8(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DC360(r, a0, D_003982B8, a2);
    return r;
}

void func_002DC838(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA408(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC878);

void func_002DC928(CmdI *a0) {
    a0->res = a0->sub->i28;
}

void *func_002DC938(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DC360(r, a0, D_003982D0, a2);
    return r;
}

void func_002DC998(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA420(a0->sub, func_002DB8D8(&b));
}

void func_002DC9D8(HasSubF *a0, f32 t1, f32 t2) {
    KeyOut b;

    func_002DB7C8(a0, &b, t1);
    func_002DA420(a0->sub, (a0->f10 + func_002DB8D8(&b) * t2) - (a0->f10 * t2));
}

void func_002DCA30(CmdF *a0) {
    a0->res = a0->sub->f1C;
}

void *func_002DCA40(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x24);
    func_002DC360(r, a0, D_003982E8, a2);
    return r;
}

void func_002DCAA0(HasSub *a0, f32 t) {
    KeyOut b0;
    f32 b1[5];

    func_002DB7C8(a0, &b0, t);
    func_002DBA28(&b0, b1);
    func_002DA548(a0->sub, b1);
}

void func_002DCAF0(HasArr *a0, f32 t1, f32 t2) {
    KeyOut b0;
    f32 b1[5];
    f32 b2[5];

    func_002DB7C8(a0, &b0, t1);
    func_002DBA28(&b0, b2);
    func_002DB9D8(b1, a0->f10, b2, t2);
    func_002DA548(a0->sub, b1);
}

void func_002DCB68(DstBlk *a0) {
    Blk *p;

    p = func_002DA490(a0->sub);
    a0->blk = *p;
}

void *func_002DCBB8(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x24);
    func_002DC360(r, a0, D_00398300, a2);
    return r;
}

void func_002DCC18(HasSub *a0, f32 t) {
    KeyOut b0;
    f32 b1[5];

    func_002DB7C8(a0, &b0, t);
    func_002DBA28(&b0, b1);
    func_002DA6B0(a0->sub, b1);
}

void func_002DCC68(HasArr *a0, f32 t1, f32 t2) {
    KeyOut b0;
    f32 b1[5];
    f32 b2[5];

    func_002DB7C8(a0, &b0, t1);
    func_002DBA28(&b0, b1);
    func_002DB9D8(b2, a0->f10, b1, t2);
    func_002DA6B0(a0->sub, b2);
}

void func_002DCCE0(DstBlk *a0) {
    Blk *p;

    p = func_002DA5F8(a0->sub);
    a0->blk = *p;
}

void *func_002DCD30(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DC360(r, a0, D_00398318, a2);
    return r;
}

void func_002DCD90(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA5B0(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DCDD0);

void func_002DCE80(CmdI *a0) {
    a0->res = a0->sub->i18;
}

void *func_002DCE90(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x24);
    func_002DC360(r, a0, D_00398330, a2);
    return r;
}

void func_002DCEF0(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA548(a0->sub, b.p0);
}

void func_002DCF28(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA548(a0->sub, b.p0);
}

void func_002DCF60(void) {
}

void *func_002DCF68(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x24);
    func_002DC360(r, a0, D_00398348, a2);
    return r;
}

void func_002DCFC8(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA6B0(a0->sub, b.p0);
}

void func_002DD000(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA6B0(a0->sub, b.p0);
}
