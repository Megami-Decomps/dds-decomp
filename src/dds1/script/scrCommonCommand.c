#include "common.h"

typedef struct KwlnTask KwlnTask;

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D768);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D7A0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D7C0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D808);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D838);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D868);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D908);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D9A8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D9E8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DA28);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DA80);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DAB8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DAF0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DB48);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DBD0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DC80);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DCC8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DCF0);

/* Persona 4 scrCommand_SCR_GET_TIMER @ 00299660 (src/Script/scrCommonCommand.c), recompiled unchanged */
u32 scrCommand_SCR_GET_TIMER()
{
    KwlnTask* task;
    task = (KwlnTask*)func_0010D428(0);
    if (!func_001019C8(task))
    {
        func_0010D5F0(0);
    }
    else
    {
        func_0010D5F0(kwlnTaskGetTimer(task));
    }
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DD98);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DDD8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DDF8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DE70);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DF20);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DF90);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DFE8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E120);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E178);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E1D0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E258);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E2E0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E338);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E360);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E388);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E3B0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E498);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E520);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E578);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E5A0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E5C8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E5F0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E6D8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E760);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E7B8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E7E0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E808);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E830);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E8D0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E958);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E998);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E9C0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E9E8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EA10);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EA90);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EB18);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EB40);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EB68);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EB90);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EC10);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EC98);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010ECF0);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010ED18);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010ED40);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010ED68);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EDB8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EE08);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EE18);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EE30);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EE40);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EE80);

/* Persona 4 scrCommand_SCR_EXISTS @ 00299600 (src/Script/scrCommonCommand.c), recompiled unchanged */
u32 scrCommand_SCR_EXISTS()
{
    KwlnTask* task;
    task = (KwlnTask*)func_0010D428(0);
    if (func_001198E8(task))
    {
        func_0010D5F0(1);
    }
    else
    {
        func_0010D5F0(0);
    }
    return 1;
}
