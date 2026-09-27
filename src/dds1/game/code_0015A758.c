#include "common.h"

extern s32 D_003BB014;

extern s32 D_003BB010;

void func_0015A758(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xf0) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015A760);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015A7A0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015A7E0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015A8C8);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015A968);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015A9A0);

void func_0015ACF0(void) {
    func_0015A608();
}

void func_0015AD08(float arg0, s32 arg1) {
    func_0015A658();
    *(float *)(arg1 + 0x8c) = *(float *)(arg1 + 0x8c) * arg0;
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015AD48);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015AD68);

void func_0015AD78(void) {
    func_0015A6F8();
}

void func_0015AD90(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xf0) = arg1;
}

void func_0015AD98(u32 arg0, u8 arg1) {
    func_0015A760(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015ADB0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015ADD0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015AE40);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015AED8);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015AF70);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B058);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B148);

u32 func_0015B220(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B228);

void func_0015B250(void) {
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B258);

void func_0015B3D8(u16 *arg0) {
    *arg0 = 1;
    func_002DAA68(*(u32 *)(arg0 + 0x20));
    func_002D0918(*(u32 *)(arg0 + 8));
}

void func_0015B410(s32 arg0) {
    *(s32 *)(arg0 + 0x54) = D_003BB010;
    D_003BB010 = arg0;
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B420);

void func_0015B648(s32 arg0) {
    func_002DA438(*(u32 *)(arg0 + 0x40));
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B660);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B6A0);

void func_0015B8B8(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x20));
    func_002D0918(*(u32 *)(arg0 + 0x10));
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B8E8);

void func_0015B918(s32 arg0) {
    *(s32 *)(arg0 + 0x24) = D_003BB014;
    D_003BB014 = arg0;
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B928);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B9E0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BA38);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BB00);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BB90);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BCE8);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BF78);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BFD8);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C128);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C2F0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C360);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C618);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C728);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C7A0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C8C0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CA40);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CAA0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CB58);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CC58);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CCD0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CDF0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CEF8);

void func_0015D078(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 2) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015D080);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015D0C0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015D710);

void func_0015D7B8(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x10));
    func_002D0918(*(u32 *)(arg0 + 0x14));
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015D7E8);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015D910);
