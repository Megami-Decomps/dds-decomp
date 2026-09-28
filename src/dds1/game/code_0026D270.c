#include "common.h"

extern s64 func_00273750(void);

extern s32 func_00105C48(void);

extern u8 D_0037B8BC[];

extern u8 D_0037B888[];

extern u8 D_003253C8[];

extern char D_003B1140[]; /* "staffImageProc" */

extern char D_003B1168[]; /* "staffProc" */

extern u8 D_003DC578[];

extern u8 D_0037B168[];

extern u32 D_003BC62C;

extern u32 D_003BC630;

extern u16 D_003BA72C;

extern u32 *D_003BC610;

extern s32 func_002D3EE8(void);

extern s32 D_003BC5D0;

extern char D_003B1AC8[];

extern s32 func_00101A70();

extern s32 D_003BAA00;

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D270);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D480);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D510);

void func_0026D648(void) {
    s32 *temp_v0 = (s32 *)D_003BC5D0;

    temp_v0[13] = 1;
    temp_v0[5] = 0;
    temp_v0[7] = 0;
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D660);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D808);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026DC50);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026DD10);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026DD30);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026DEA8);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026DED0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026E160);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026E188);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026E240);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026E388);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026E4F0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026E5A0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026E608);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026E720);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026E798);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003AFEC8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003AFEE0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003AFEF8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003AFF18);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003AFF40);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003AFF58);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003AFF90);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003AFFC8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003AFFE0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003AFFF0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0008);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0018);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0028);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0038);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0050);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0068);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0078);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0088);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0098);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B00A8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B00C0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B00D0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B00E8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B00F8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0108);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0118);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0128);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0138);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0150);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0168);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0178);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0188);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0198);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B01B0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B01C0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B01D0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B01E8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B01F8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0208);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0218);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0230);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0240);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0250);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0260);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0270);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0288);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0298);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B02A8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B02B8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B02C8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B02D8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B02E8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B02F8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0310);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0320);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0338);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0350);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0360);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0370);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0388);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B03A0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B03B0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B03C0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B03D0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B03E0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B03F0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0408);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0420);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0438);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0450);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0460);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0470);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0480);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0490);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B04A8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B04C0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B04D0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B04E0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0500);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0518);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0538);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0548);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0558);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0568);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0578);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0588);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B05A0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B05B0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B05C0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B05D8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B05F0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0608);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0618);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0630);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0648);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0660);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0678);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0690);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B06A0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B06B0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B06C0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B06D8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B06F0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0700);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0710);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0728);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0738);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0748);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0758);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0768);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0778);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0788);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B07A0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B07B0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B07C0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B07D0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B07E0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B07F8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0808);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0818);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0828);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0840);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0850);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0860);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0870);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0880);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0898);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B08A8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B08B8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B08C8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B08D8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B08F0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0908);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0918);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0928);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0938);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0948);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0958);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0978);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0988);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B09A0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B09B8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B09C8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B09D8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B09E8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B09F8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0A08);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0A18);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0A28);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0A40);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0A50);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0A68);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0A78);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0A90);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0AA8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0AB8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0AD0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0AE0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0B00);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0B18);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0B38);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0B50);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0B78);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0B88);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0BA0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0BB0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0BC0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0BD0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0BF0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0C18);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0C30);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0C40);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0C58);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0C68);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0C80);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0C98);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0CA8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0CB8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0CC8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0CD8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0CF0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0D00);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0D20);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0D30);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0D48);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0D60);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0D70);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0D88);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0D98);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0DB0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0DC0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0DE0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0DF8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0E10);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0E20);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0E30);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0E48);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0E58);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0E70);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0E80);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0E98);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0EA8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0EC0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0ED0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0EE8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0F00);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0F18);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0F30);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0F40);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0F50);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0F60);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0F78);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0F88);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0FA0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0FB0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0FC8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0FE0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B0FF0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1008);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1018);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1028);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1050);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1060);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1078);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1088);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1098);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B10A8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B10B8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B10C8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B10E0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B10F0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1108);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1118);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026E8A0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026E8D8);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026EA70);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026EC90);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026F118);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026F230);

void func_0026F500(void) {
    func_00197348();
}

void func_0026F518(void) {
    func_00197378();
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026F530);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026F5E8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1140);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026F918);

INCLUDE_ASM(const s32, "game/code_0026D270", staffImageProc);

void func_0026FD88(void) {
    s64 temp_v0;

    D_003BA72C = 2;
    func_0026A808();
    func_0026A950();
    func_0026F518();
    do {
        temp_v0 = func_002D3EE8();
    } while (temp_v0 != 0);
    func_002D0A10(*D_003BC610);
    D_003BC610 = (u32 *)0x0;
}

void func_0026FDD8(void) {
    func_002BDD60(D_003BC610[1]);
    while (func_002D3EE8() != 0) {
    }
    func_002ECA40(0);
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026FE10);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026FE98);

u32 func_0026FF18(void) {
    func_0026FE98();
    return 0xffffffff;
}

s32 func_0026FF38(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003B1140, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003B1168, 1);
    return 0;
}

s32 movieDraw(void) {
    func_002ECCF8(D_0037B888, D_003253C8);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1168);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1178);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1198);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B11B8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B11D8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B11F8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1218);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1238);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1258);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1278);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1298);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B12B8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B12D8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B12F8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1318);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1338);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1358);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1378);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1398);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B13B8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B13D8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B13F8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1418);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1438);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1458);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1478);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1498);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B14B8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B14D8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B14F8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1518);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1538);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1558);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1578);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1598);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B15B8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B15D8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B15F8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1618);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1638);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1658);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1678);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1698);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B16B8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B16D8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B16F8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1718);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1738);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1758);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1778);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1798);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B17B8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B17D8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B17F8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1818);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1838);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1850);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1868);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1888);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B18A0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B18B8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B18D0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B18F0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1908);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1920);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1938);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1958);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1970);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1990);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B19B0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B19C8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B19E8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1A08);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1A20);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1A38);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1A58);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026FFA0);

void func_0026FFF8(s32 arg0) {
    u8 *temp_v0 = D_0037B168 + arg0 * 24;

    func_0026FFA0(*(s32 *)temp_v0, (s32)(temp_v0 + 4));
}

void func_00270030(void) {
    if (D_003BC62C == 0) {
        return;
    }
    func_002EDAE0(D_0037B888);
    kwlnTaskDestroyWithHierarchy(D_003BC62C, 0);
    D_003BC62C = 0;
}

void func_00270068(void) {
    func_002EDBB8(D_0037B888);
}

s32 func_00270088(void) {
    return D_0037B8BC[0];
}

s32 func_00270098(void) {
    func_002EC5E0(0x3c / D_003BA72C);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_002700D0);

u32 func_00270110(void) {
    s32 temp_v0;

    temp_v0 = func_0010D428(0);
    func_0026FFF8(temp_v0);
    D_003BC630 = 0;
    return 1;
}

u32 func_00270140(void) {
    func_00270030();
    D_003BC630 = 0;
    func_00106690(0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00270170);

u8 func_00270218(void) {
    s64 temp_v0;

    temp_v0 = func_00270088();
    return temp_v0 == 2;
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00270240);

INCLUDE_ASM(const s32, "game/code_0026D270", func_002702A0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00270508);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00270558);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00270738);

INCLUDE_ASM(const s32, "game/code_0026D270", movieViewer);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00270A30);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00270A80);

void func_00270AC0(void) {
    D_003DC578[2] = 1;
    D_003DC578[3] = 1;
}

void func_00270AD8(void) {
    u32 *temp_v0 = (u32 *)D_003DC578;

    temp_v0[1] = 0x10002010;
    temp_v0[2] = (u32)D_0037B888;
    func_00270AC0();
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00270B10);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1AC8);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00270BC8);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00270FB0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271020);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271098);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1AF0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1B00);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1B10);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1B20);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1B30);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1B48);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1B60);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1B78);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1B88);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1BA0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1BB8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1BC8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1BE0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1BF8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1C10);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1C28);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1C38);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1C48);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1C60);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1C78);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1C90);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1CA8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1CB8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1CC8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1CD8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1CE8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1CF8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1D08);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1D28);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1D38);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1D48);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1D58);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1D68);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1D78);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1D88);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1D98);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1DA8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1DB8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1DC8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1DD8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1DE8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1DF8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1E08);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1E18);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1E28);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1E48);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1E58);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1E68);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1E78);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1E88);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1E98);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1EA8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1EB8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1EC8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1EE0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1F00);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1F18);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1F30);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1F48);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1F60);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1F70);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1F80);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1F90);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1FA0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1FB0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1FC0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1FD0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1FE0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B1FF0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B2000);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B2010);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B2020);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271100);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271180);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271210);

INCLUDE_ASM(const s32, "game/code_0026D270", func_002712A0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271308);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271368);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271418);

void func_00271480(u32 arg0, u32 *arg1, u32 arg2, u32 arg3) {
    func_002802E0(arg0, arg3, arg1[3], 7, arg1[4], 0, *arg1, 0x11);
    func_0027FAA8(arg0, *arg1);
    func_0027FBE0(arg0, arg1 + 9);
    func_0027FC10(arg0, arg1 + 0x11);
    func_0027FC40(arg0, arg1 + 0x19);
    func_0027FF60(arg0);
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271500);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271648);

INCLUDE_ASM(const s32, "game/code_0026D270", func_002716E8);

INCLUDE_ASM(const s32, "game/code_0026D270", func_002717D8);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271948);

INCLUDE_ASM(const s32, "game/code_0026D270", func_002719F0);

void func_00271B40(void) {
}

void func_00271B48(void) {
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271B50);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271D30);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271DF8);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271E58);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271F18);

u32 func_00271FC8(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    func_00283BF8(temp_v0 + 0x914, 0x53);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00271FF8);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B20C0);

INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B20D0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_002720B0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_002721E8);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00272228);

u8 func_00272260(void) {
    s64 temp_v0;

    temp_v0 = func_00105C48();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00272280);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00272350);

INCLUDE_ASM(const s32, "game/code_0026D270", func_002723B0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00272518);

void func_00272668(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_00272518(arg0, arg1, arg2, arg3, arg4, 0, arg5);
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00272688);

void func_00272778(u32 arg0) {
    func_00272688(0, arg0);
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00272798);

INCLUDE_ASM(const s32, "game/code_0026D270", func_002728F8);

INCLUDE_ASM(const s32, "game/code_0026D270", func_002729C8);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00272A00);

u32 func_00272A58(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    func_0027E790(*(u32 *)(temp_v0 + 0x138), *(u32 *)(temp_v0 + 0x6c), 0, 1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00272A90);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00272B00);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00272B80);

u32 func_00272BB8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00272BC0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00272D50);

void func_00273020(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x90c);
    func_0027C430(*(u32 *)(temp_v0 + 8));
    func_0027C430(*(u32 *)(temp_v0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00273050);

INCLUDE_ASM(const s32, "game/code_0026D270", func_002730A0);

void func_00273200(s32 arg0) {
    func_0027C430(*(u32 *)(*(s32 *)(arg0 + 0x90c) + 0x10));
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_00273220);

void func_00273390(u32 arg0) {
    func_00271308(2, arg0);
}

void func_002733B0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_002733B8);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00273470);

INCLUDE_ASM(const s32, "game/code_0026D270", func_002734C0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00273670);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00273718);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00273750);

void func_00273838(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s64 temp_v1;

    temp_v0 = *(s32 *)(arg1 + 0x90c);
    temp_v1 = func_00273750();
    if (temp_v1 != 0) {
        *(u32 *)(*(s32 *)(*(s32 *)(*(s32 *)(temp_v0 + 8) + 0x14) + 0x1c) + 0x60) =
                  (u32)*(u8 *)(arg0 + D_003BAA00 + 0x12a0);
        *(s32 *)(temp_v0 + 0x28) = arg0;
    }
    func_00283BF0(arg1 + 0x914, 1);
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_002738A0);

INCLUDE_ASM(const s32, "game/code_0026D270", func_00273A30);









INCLUDE_RODATA(const s32, "game/code_0026D270", D_003B2100);


INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC610);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC614);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC618);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC620);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC628);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC62C);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC630);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC638);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC640);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC648);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC650);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC658);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC660);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC668);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC670);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC678);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC680);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC688);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC690);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC698);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC6A0);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC6A8);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC6B0);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC6B4);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC6B5);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC6B8);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC6C0);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC6C8);

INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC6D0);


INCLUDE_SDATA(const s32, "game/code_0026D270", D_003BC6D8);

