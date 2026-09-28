#include "common.h"

extern u32 D_00438B88;

extern u32 D_00438B8C;

extern u64 func_0032C138(u32);

extern u64 func_00343ED0(u64, u32 *, u64);

extern u64 func_00343F38(u32);

extern u32 D_00438D0C;

extern s32 D_00438D10;

extern s32 D_00438D14;

extern s32 D_00438D18;

extern s32 D_00438D1C;

extern u32 D_00438D20;

extern u32 effMiscRand(void *state);

extern s8 D_004391E0;

typedef struct CmdPacket {
    /* 0x0 */ u32 unk0;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 unk8;
    /* 0xA */ u16 unkA;
    /* 0xC */ u32 unkC;
} CmdPacket;

u32 func_00341650(u32 arg0, u32 arg1, void *arg2, u32 arg3);

void func_003421E8(s32 arg0);

u32 func_003417A8(u32 arg0, u32 arg1, void *arg2, u32 arg3);

typedef struct FE250Entry {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ u8 unk6;
    /* 0x7 */ u8 unk7;
} FE250Entry;

extern FE250Entry D_0047ABD0[];

extern u32 D_00438B84;

extern s32 func_00342168(s32 id);

extern s32 D_00438B80;

extern void func_00329A00(void *out);

extern void func_0033DAD8(char *fmt, ...);

typedef struct MidiChannel {
    u8 pad00[0x19];
    u8 index;
    u8 enabled;
    u8 pad1B[0xD];
    u32 entries[8];
} MidiChannel;

INCLUDE_ASM(const s32, "game/code_00341240", func_00341240);

u32 func_003412A0(void *arg0, u32 arg1) {
    return effMiscRand(arg0) % arg1;
}

INCLUDE_ASM(const s32, "game/code_00341240", func_003412D8);

INCLUDE_ASM(const s32, "game/code_00341240", func_00341348);

INCLUDE_ASM(const s32, "game/code_00341240", func_003413F0);

void func_003414D0(void) {
    sceSifAllocIopHeap();
}

INCLUDE_ASM(const s32, "game/code_00341240", func_003414E8);

void func_00341570(void) {
    for (;;) {
        do {
            SleepThread();
        } while (D_004391E0 != 0);
        while (func_003414E8() == 0) {
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00341240", func_003415A8);

INCLUDE_ASM(const s32, "game/code_00341240", func_00341650);

void func_003417A0(void) {
}

INCLUDE_ASM(const s32, "game/code_00341240", func_003417A8);

INCLUDE_ASM(const s32, "game/code_00341240", func_003417E0);

INCLUDE_ASM(const s32, "game/code_00341240", func_00341878);

INCLUDE_ASM(const s32, "game/code_00341240", func_00341958);

INCLUDE_ASM(const s32, "game/code_00341240", func_00341A00);

INCLUDE_ASM(const s32, "game/code_00341240", func_00341AD8);

void func_00341BB8(s32 arg0) {
    CmdPacket packet;

    func_003421E8(arg0);
    packet.unk0 = arg0;
    packet.unk4 = 0;
    packet.unk8 = 0x7F;
    func_00341650(0x20, 0, &packet, 0x10);
}

void func_00341C00(u32 arg0) {
    u32 temp_v0 [4];

    temp_v0[0] = arg0;
    func_00341650(0x30, 0, temp_v0, 0x10);
}

void func_00341C30(s32 arg0) {
    CmdPacket packet;

    func_003421E8(arg0);
    packet.unk0 = arg0;
    packet.unk8 = 0x7F;
    func_00341650(0x130, 0, &packet, 0x10);
}

void func_00341C78(u32 arg0) {
    u32 temp_v0 [4];

    temp_v0[0] = arg0;
    func_00341650(0x30, 0, temp_v0, 0x10);
}

void func_00341CA8(void) {
    func_00341650(0x1a0, 0, 0, 0);
}

void func_00341CD0(void) {
    func_00341650(0x210, 0, 0, 0);
}

void func_00341CF8(void) {
    func_00341650(0x40, 0, 0, 0);
}

void func_00341D20(u32 arg0) {
    func_00341650(arg0 | 0x50, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00341240", func_00341D48);

INCLUDE_ASM(const s32, "game/code_00341240", func_00341D90);

INCLUDE_ASM(const s32, "game/code_00341240", func_00341DD8);

void func_00341E20(s32 id, s32 volume, s32 pan) {
    CmdPacket packet;
    func_003421E8(id);
    packet.unk0 = id;
    packet.unk8 = volume;
    packet.unkA = (u8)pan;
    func_00341650(0x90, 0, &packet, 0xC);
}

INCLUDE_ASM(const s32, "game/code_00341240", func_00341E80);

void func_00341FE8(s32 arg0, s32 arg1, f32 x, f32 y, f32 z) {
    u32 packet[8];

    packet[0] = arg0;
    packet[1] = arg1;
    packet[2] = (s32)(x * 0.1f);
    packet[3] = (s32)(y * 0.1f);
    packet[4] = (s32)(z * 0.1f);
    func_003417A8(0x170, 0, packet, 0x20);
}

INCLUDE_ASM(const s32, "game/code_00341240", func_00342040);

INCLUDE_ASM(const s32, "game/code_00341240", func_00342168);

INCLUDE_ASM(const s32, "game/code_00341240", func_003421E8);

INCLUDE_ASM(const s32, "game/code_00341240", func_003422C0);

INCLUDE_ASM(const s32, "game/code_00341240", func_003422F8);

INCLUDE_ASM(const s32, "game/code_00341240", func_00342358);

INCLUDE_ASM(const s32, "game/code_00341240", func_00342388);

INCLUDE_ASM(const s32, "game/code_00341240", func_003423B8);

void func_003423E8(s32 id) {
    u32 packet[4];
    if (func_00342168(id) != 0) {
        packet[0] = id;
        func_003417A8(0xB0, 0, packet, 0x10);
        id >>= 16;
        if (D_00438B80 == id) {
            D_00438B80 = -1;
        }
    }
}

u8 func_00342440(s32 arg0) {
    return D_0047ABD0[arg0].unk4;
}

u8 func_00342458(s32 arg0) {
    return D_0047ABD0[arg0].unk5;
}

s32 func_00342470(s32 arg0) {
    FE250Entry *entry = &D_0047ABD0[arg0];
    s32 diff = entry->unk4 - entry->unk5;

    if (diff <= 0) {
        diff = 0;
    }
    return diff;
}

INCLUDE_ASM(const s32, "game/code_00341240", func_00342498);

INCLUDE_ASM(const s32, "game/code_00341240", func_003424A8);

INCLUDE_ASM(const s32, "game/code_00341240", func_003424B8);

INCLUDE_ASM(const s32, "game/code_00341240", func_003424D8);

void func_00342538(s32 arg0) {
    CmdPacket packet;

    func_003421E8(arg0);
    packet.unk0 = arg0;
    packet.unk4 = 0;
    packet.unk8 = 0x17F;
    func_00341650(0x20, 0, &packet, 0x10);
}

void func_00342580(u32 arg0) {
    u32 temp_v0 [4];

    temp_v0[0] = arg0;
    func_00341650(0xd0, 0, temp_v0, 0x10);
}

void func_003425B0(void) {
    func_00341650(0x180, 0, 0, 0);
}

void func_003425D8(void) {
    func_00341650(400, 0, 0, 0);
}

void func_00342600(s32 arg0) {
    func_00341650(((arg0 + 1U) & 0xf) | 0xe0, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00341240", func_00342630);

u32 func_00342688(void) {
    return D_00438B88;
}

void func_00342690(void) {
    func_003417A8(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00341240", func_003426B8);

void func_003426E8(u32 arg0) {
    if (0x10 < arg0) {
        arg0 = 0x10;
    }
    if (arg0 == 0) {
        arg0 = 1;
    }
    func_003417A8((arg0 - 1) | 0x1d0, 0, 0, 0);
}

u32 func_00342728(u32 arg0) {
    if (D_00438B8C != 0) {
        return 0;
    }
    D_00438B84 = arg0;
    return arg0;
}

INCLUDE_ASM(const s32, "game/code_00341240", func_00342748);

u32 func_00342790(void) {
    return D_00438B8C;
}

void func_00342798(void) {
    func_003417A8(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00341240", func_003427C0);

INCLUDE_ASM(const s32, "game/code_00341240", func_003427F0);

INCLUDE_ASM(const s32, "game/code_00341240", func_00342848);

INCLUDE_ASM(const s32, "game/code_00341240", func_00342A90);

INCLUDE_ASM(const s32, "game/code_00341240", func_00342B28);

INCLUDE_ASM(const s32, "game/code_00341240", func_00342E58);

INCLUDE_ASM(const s32, "game/code_00341240", func_00343188);

INCLUDE_ASM(const s32, "game/code_00341240", func_003432F0);

INCLUDE_RODATA(const s32, "game/code_00341240", D_0042E5B0);

INCLUDE_RODATA(const s32, "game/code_00341240", D_0042E5C8);

INCLUDE_RODATA(const s32, "game/code_00341240", D_0042E5F0);

INCLUDE_RODATA(const s32, "game/code_00341240", D_0042E610);

INCLUDE_RODATA(const s32, "game/code_00341240", jtbl_0042E630);

INCLUDE_ASM(const s32, "game/code_00341240", func_00343468);

INCLUDE_ASM(const s32, "game/code_00341240", func_00343B40);

INCLUDE_ASM(const s32, "game/code_00341240", func_00343C30);

INCLUDE_RODATA(const s32, "game/code_00341240", D_0042EAC8);

INCLUDE_RODATA(const s32, "game/code_00341240", D_0042EB40);

void func_00343D20(void) {
    s32 info[6];
    func_00329A00(info);
    func_0033DAD8(" <<< memory information >>>\n             total : 0x%06X\n        free total : 0x%06X\n     max free size : 0x%06X\n     min free size : 0x%06X\n      handle total : %d\n free handle count : %d\n\n",
                    info[0], info[1], info[2], info[3], info[4], info[5]);
}

INCLUDE_ASM(const s32, "game/code_00341240", func_00343D60);

INCLUDE_ASM(const s32, "game/code_00341240", func_00343E18);

INCLUDE_ASM(const s32, "game/code_00341240", func_00343ED0);

u64 func_00343EE8(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00343ED0(arg0, temp_v2, 0);
    temp_v1 = func_0032C138(temp_v2[0]);
    func_003297C8(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00341240", func_00343F38);

u64 func_00343FC0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00343ED0(arg0, temp_v2, 0);
    temp_v1 = func_00343F38(temp_v2[0]);
    func_003297C8(temp_v0);
    return temp_v1;
}

s32 func_00344010(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0 + 0x20;
    func_00344120(temp_v0, temp_v0, temp_v0 + *(s32 *)(arg0 + 0x10), *(u32 *)(arg0 + 0x14));
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00341240", func_00344050);

s32 func_00344098(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0 + 0x20;
    func_00344120(temp_v0, temp_v0, temp_v0 + *(s32 *)(arg0 + 0x10), *(u32 *)(arg0 + 0x14));
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00341240", func_003440D8);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344120);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344208);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344298);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344338);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344378);

INCLUDE_ASM(const s32, "game/code_00341240", func_003443B8);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344420);

INCLUDE_ASM(const s32, "game/code_00341240", func_003444F8);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344768);

INCLUDE_ASM(const s32, "game/code_00341240", func_003447D8);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344870);

INCLUDE_ASM(const s32, "game/code_00341240", func_003448D0);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344A08);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344B40);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344D60);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344E30);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344E98);

INCLUDE_ASM(const s32, "game/code_00341240", func_00344F08);

INCLUDE_ASM(const s32, "game/code_00341240", func_003450D8);

INCLUDE_ASM(const s32, "game/code_00341240", func_00345198);

INCLUDE_ASM(const s32, "game/code_00341240", func_00345268);

INCLUDE_ASM(const s32, "game/code_00341240", func_00345298);

INCLUDE_ASM(const s32, "game/code_00341240", func_00345330);

u32 func_00345358(MidiChannel *channel) {
    u32 result = 0;
    if (channel->enabled != 0) {
        result = channel->entries[channel->index];
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00341240", func_00345380);

INCLUDE_ASM(const s32, "game/code_00341240", func_00345408);

INCLUDE_ASM(const s32, "game/code_00341240", func_00345488);

INCLUDE_ASM(const s32, "game/code_00341240", func_003455F0);

INCLUDE_ASM(const s32, "game/code_00341240", func_00345628);

INCLUDE_ASM(const s32, "game/code_00341240", func_003456C0);

INCLUDE_ASM(const s32, "game/code_00341240", func_003456F8);

INCLUDE_ASM(const s32, "game/code_00341240", func_003457A8);

void func_003457F8(void) {
    func_0035B7D8();
}

INCLUDE_ASM(const s32, "game/code_00341240", func_00345810);

void func_003458E8(u32 arg0) {
    D_00438D0C = arg0;
}

void func_003458F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4) {
    D_00438D10 = arg0 * 0x10 + 0x7000;
    D_00438D14 = arg1 * 8 + 0x7900;
    D_00438D18 = D_00438D10 + arg2 * 0x10;
    D_00438D1C = D_00438D14 + arg3 * 8;
    D_00438D20 = arg4;
}

INCLUDE_ASM(const s32, "game/code_00341240", func_00345928);

INCLUDE_ASM(const s32, "game/code_00341240", func_00345BA0);
INCLUDE_SDATA(const s32, "game/code_00341240", D_00438B78);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438B79);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438B80);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438B84);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438B88);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438B8C);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438B90);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438B94);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438B98);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438BA0);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438BA8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438BB0);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438BB8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438BC0);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438BC8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438BD0);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438BD8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438BE0);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438BE8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438BF0);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438BF8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C00);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C08);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C10);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C18);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C20);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C28);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C30);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C38);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C40);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C48);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C50);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C58);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C60);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C68);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C70);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C78);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C80);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C88);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C90);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438C98);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438CA0);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438CA8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438CB0);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438CB8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438CC0);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438CC8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438CD0);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438CD8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438CE0);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438CE8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438CF0);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438CF8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438D00);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438D04);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438D08);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438D0C);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438D10);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438D14);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438D18);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438D1C);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438D20);


INCLUDE_SDATA(const s32, "game/code_00341240", D_00438D23);

