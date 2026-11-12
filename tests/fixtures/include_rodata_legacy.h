/* Frozen pre-fix behavior for executable regression controls. */
#define INCLUDE_RODATA(TYPE, FOLDER, NAME)                     \
    __asm__(".section .rodata\n"                               \
            "\t.include \"" ASM_ROOT FOLDER "/" #NAME ".s\"\n" \
            ".section .text\n")
