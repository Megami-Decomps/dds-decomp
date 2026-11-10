#include "common.h"
#include "sdk_libgcc.h"
/* DDS's global work keeps the script variables at SMT3's offsets. */
typedef struct
{
    u8 unk0[0x40];
    s32 integers[256];
    f32 floats[256];
} dds3GlobalWork_t;
extern dds3GlobalWork_t *DDS3_GBWK;
#include "scr.h"

enum code_operation
{
    CODE_OPERATION_ADD = 0,
    CODE_OPERATION_SUB = 1,
    CODE_OPERATION_MUL = 2,
    CODE_OPERATION_DIV = 3,
    CODE_OPERATION_OR = 4,
    CODE_OPERATION_AND = 5,
    CODE_OPERATION_EQ = 6,
    CODE_OPERATION_NEQ = 7,
    CODE_OPERATION_S = 8,
    CODE_OPERATION_L = 9,
    CODE_OPERATION_SE = 10,
    CODE_OPERATION_LE = 11,
    CODE_OPERATION_MAX = 12
};

extern scrProcessWork_s *nowpwork;
extern const scrCommandTable_t scrCommandTable[];
extern s32 (*codefunc[])(scrProcessWork_s *pw);

extern s32 PopInt(scrProcessWork_s *pwork);
extern f32 PopFloat(scrProcessWork_s *pwork);
extern void CodeFunc_2Operation(scrProcessWork_s *pwork, s32 type);

void PushInt(scrProcessWork_s *pwork, s32 data)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_INT;
    pwork->StackData[pwork->SP].i = data;
    pwork->SP++;
}

void PushFloat(scrProcessWork_s *pw, f32 value)
{
    pw->StackType[pw->SP] = SCR_STACK_TYPE_FLOAT;
    pw->StackData[pw->SP].f = value;
    pw->SP++;
}

void PushStr(scrProcessWork_s *pwork, u8 *data)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_STRING;
    pwork->StackData[pwork->SP].i = (s32)data;
    pwork->SP++;
}

void PushRet(scrProcessWork_s *pwork, s32 cp)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_RET;
    pwork->StackData[pwork->SP].i = cp;
    pwork->SP++;
}


s32 PopInt(scrProcessWork_s *pwork)
{
    s32 stackIndex = pwork->SP;

    pwork->SP = stackIndex - 1;
    switch ((s8)pwork->StackType[stackIndex - 1]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_RET:
        return pwork->StackData[pwork->SP].i;
    case SCR_STACK_TYPE_FLOAT:
        return pwork->StackData[pwork->SP].f;
    case SCR_STACK_TYPE_INTVAR:
        return DDS3_GBWK->integers[pwork->StackData[pwork->SP].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->floats[pwork->StackData[pwork->SP].i];
    }
    return 0;
}

f32 PopFloat(scrProcessWork_s *pwork)
{
    s32 stackIndex = pwork->SP;

    pwork->SP = stackIndex - 1;
    switch ((s8)pwork->StackType[stackIndex - 1]) {
    case SCR_STACK_TYPE_INT:
        return (f32)pwork->StackData[pwork->SP].i;
    case SCR_STACK_TYPE_RET:
        return 0.0f;
    case SCR_STACK_TYPE_FLOAT:
        return pwork->StackData[pwork->SP].f;
    case SCR_STACK_TYPE_INTVAR:
        return (f32)DDS3_GBWK->integers[pwork->StackData[pwork->SP].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->floats[pwork->StackData[pwork->SP].i];
    }
    return 0.0f;
}

s32 CodeFunc_PushI(scrProcessWork_s *pwork)
{
    pwork->CP++;
    PushInt(pwork, pwork->Code[pwork->CP].i);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushIS(scrProcessWork_s *pwork)
{
    PushInt(pwork, pwork->Code[pwork->CP].s.s1);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushF(scrProcessWork_s *pwork)
{
    pwork->CP++;
    PushFloat(pwork, pwork->Code[pwork->CP].f);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushIX(scrProcessWork_s *pwork)
{
    PushInt(pwork, DDS3_GBWK->integers[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushFX(scrProcessWork_s *pwork)
{
    PushFloat(pwork, DDS3_GBWK->floats[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushLIX(scrProcessWork_s *pwork)
{
    PushInt(pwork, pwork->LocalInt[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushLFX(scrProcessWork_s *pwork)
{
    PushFloat(pwork, pwork->LocalFloat[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushSTR(scrProcessWork_s *pwork)
{
    PushStr(pwork, pwork->StringData + pwork->Code[pwork->CP].s.s1);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushReg(scrProcessWork_s *pwork)
{
    pwork->StackType[pwork->SP] = pwork->StackType[27];
    pwork->StackData[pwork->SP].i = pwork->StackData[27].i;
    pwork->SP++;
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PopIX(scrProcessWork_s *pwork)
{
    DDS3_GBWK->integers[pwork->Code[pwork->CP].s.s1] = PopInt(pwork);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PopFX(scrProcessWork_s *pwork)
{
    f32 value = PopFloat(pwork);

    DDS3_GBWK->floats[pwork->Code[pwork->CP].s.s1] = value;
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PopLIX(scrProcessWork_s *pwork)
{
    pwork->LocalInt[pwork->Code[pwork->CP].s.s1] = PopInt(pwork);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PopLFX(scrProcessWork_s *pwork)
{
    pwork->LocalFloat[pwork->Code[pwork->CP].s.s1] = PopFloat(pwork);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Proc(scrProcessWork_s *pwork)
{
    pwork->CP++;
    return CODE_RET_NEXT;
}

/* A pending command yields without consuming its arguments. */
s32 CodeFunc_Comm(scrProcessWork_s *pwork)
{
    s32 commandCP = pwork->CP;
    s32 commandIndex = pwork->Code[commandCP].s.s1;

    nowpwork = pwork;
    if (scrCommandTable[commandIndex].comm() == 0) {
        return CODE_RET_SYNC;
    }
    pwork->SP -= scrCommandTable[commandIndex].paranum;
    if (commandCP == pwork->CP) {
        pwork->CP = commandCP + 1;
    }
    return CODE_RET_NEXT;
}

s32 CodeFunc_End(scrProcessWork_s *pwork)
{
    if (pwork->SP == 0) {
        return CODE_RET_END;
    }

    pwork->CP = PopInt(pwork) + 1;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Jump(scrProcessWork_s *pwork)
{
    pwork->CP = pwork->ProcTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Call(scrProcessWork_s *pwork)
{
    PushRet(pwork, pwork->CP);
    pwork->CP = pwork->ProcTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Run(scrProcessWork_s *pwork)
{
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Goto(scrProcessWork_s *pwork)
{
    pwork->CP = pwork->LabelTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}

extern s32 CodeFunc_2Operation_i1;
extern s32 CodeFunc_2Operation_i2;
extern s32 CodeFunc_2Operation_ri;
extern f32 CodeFunc_2Operation_f1;
extern f32 CodeFunc_2Operation_f2;
extern f32 CodeFunc_2Operation_rf;
extern s32 CodeFunc_2Operation_t1;
extern s32 CodeFunc_2Operation_t2;

void CodeFunc_2Operation(scrProcessWork_s *pwork, s32 type)
{
    CodeFunc_2Operation_t1 = (s8)pwork->StackType[pwork->SP - 1];
    CodeFunc_2Operation_t2 = (s8)pwork->StackType[pwork->SP - 2];
    if ((CodeFunc_2Operation_t1 == 0 || CodeFunc_2Operation_t1 == 2) && (CodeFunc_2Operation_t2 == 0 || CodeFunc_2Operation_t2 == 2)) {
        CodeFunc_2Operation_i1 = PopInt(pwork);
        CodeFunc_2Operation_i2 = PopInt(pwork);
        switch (type) {
        case 0:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 + CodeFunc_2Operation_i2;
            break;
        case 1:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 - CodeFunc_2Operation_i2;
            break;
        case 2:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 * CodeFunc_2Operation_i2;
            break;
        case 3:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 / CodeFunc_2Operation_i2;
            break;
        case 4:
            if (CodeFunc_2Operation_i1 != 0 || CodeFunc_2Operation_i2 != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 5:
            if (CodeFunc_2Operation_i1 != 0 && CodeFunc_2Operation_i2 != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 6:
            if (CodeFunc_2Operation_i1 == CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 7:
            if (CodeFunc_2Operation_i1 != CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 8:
            if (CodeFunc_2Operation_i1 < CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 9:
            if (CodeFunc_2Operation_i1 > CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 10:
            if (CodeFunc_2Operation_i1 <= CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 11:
            if (CodeFunc_2Operation_i1 >= CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        }
        PushInt(pwork, CodeFunc_2Operation_ri);
    } else {
        CodeFunc_2Operation_f1 = PopFloat(pwork);
        CodeFunc_2Operation_f2 = PopFloat(pwork);
        switch (type) {
        case 0:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 + CodeFunc_2Operation_f2;
            break;
        case 1:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 - CodeFunc_2Operation_f2;
            break;
        case 2:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 * CodeFunc_2Operation_f2;
            break;
        case 3:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 / CodeFunc_2Operation_f2;
            break;
        case 4:
            if (dpcmp(fptodp(CodeFunc_2Operation_f1), 0.0) != 0 ||
                dpcmp(fptodp(CodeFunc_2Operation_f2), 0.0) != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 5:
            if (dpcmp(fptodp(CodeFunc_2Operation_f1), 0.0) != 0 &&
                dpcmp(fptodp(CodeFunc_2Operation_f2), 0.0) != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 6:
            if (CodeFunc_2Operation_f1 == CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 7:
            if (CodeFunc_2Operation_f1 != CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 8:
            if (CodeFunc_2Operation_f1 < CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 9:
            if (CodeFunc_2Operation_f1 > CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 10:
            if (CodeFunc_2Operation_f1 <= CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 11:
            if (CodeFunc_2Operation_f1 >= CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        }
        PushFloat(pwork, CodeFunc_2Operation_rf);
    }
}


s32 CodeFunc_Add(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_ADD);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Sub(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_SUB);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Mul(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_MUL);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Div(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_DIV);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Minus(scrProcessWork_s *pwork)
{
    switch ((s8)pwork->StackType[pwork->SP - 1]) {
    case SCR_STACK_TYPE_INT:
        pwork->StackData[pwork->SP - 1].i = -pwork->StackData[pwork->SP - 1].i;
        break;
    case SCR_STACK_TYPE_FLOAT:
        pwork->StackData[pwork->SP - 1].f = -pwork->StackData[pwork->SP - 1].f;
        break;
    case SCR_STACK_TYPE_INTVAR:
        DDS3_GBWK->integers[pwork->StackData[pwork->SP - 1].i] =
            -DDS3_GBWK->integers[pwork->StackData[pwork->SP - 1].i];
        break;
    case SCR_STACK_TYPE_FLOATVAR:
        DDS3_GBWK->floats[pwork->StackData[pwork->SP - 1].i] =
            -DDS3_GBWK->floats[pwork->StackData[pwork->SP - 1].i];
        break;
    case SCR_STACK_TYPE_RET:
        break;
    }
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Not(scrProcessWork_s *pwork)
{
    switch ((s8)pwork->StackType[pwork->SP - 1]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_INTVAR:
        PushInt(pwork, PopInt(pwork) == 0);
        break;
    case SCR_STACK_TYPE_FLOAT:
    case SCR_STACK_TYPE_FLOATVAR:
        PushInt(pwork, PopFloat(pwork) == 0.0f);
        break;
    case SCR_STACK_TYPE_RET:
        break;
    }

    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Or(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_OR);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_And(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_AND);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_EQ(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_EQ);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_NEQ(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_NEQ);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_S(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_S);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_L(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_L);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_SE(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_SE);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_LE(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_LE);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_IF(scrProcessWork_s *pwork)
{
    s32 cond;

    switch ((s8)pwork->StackType[pwork->SP - 1]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_INTVAR:
        cond = PopInt(pwork);
        break;
    case SCR_STACK_TYPE_FLOAT:
    case SCR_STACK_TYPE_FLOATVAR:
        cond = PopFloat(pwork) != 0.0f;
        break;
    case SCR_STACK_TYPE_RET:
    default:
        cond = 0;
        break;
    }

    if (cond) {
        pwork->CP++;
    } else {
        pwork->CP = pwork->LabelTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    }
    return CODE_RET_NEXT;
}

/* Continue until an instruction completes the procedure or yields a tick. */
s32 scrTraceCode(scrProcessWork_s *pwork)
{
    s32 result;

    while (1) {
        result = codefunc[pwork->Code[pwork->CP].s.s0](pwork);
        if (result == CODE_RET_END) {
            pwork->CommandTimer = 0;
            return SCR_RESULT_END;
        }
        if (result == CODE_RET_SYNC) {
            break;
        }
        if (result == CODE_RET_NEXT) {
            pwork->CommandTimer = 0;
        }
    }
    pwork->CommandTimer++;
    pwork->ScriptTimer++;
    return SCR_RESULT_CONTINUE;
}

s32 scrGetIntPara(s32 n)
{
    scrProcessWork_s *pw = nowpwork;
    s32 stackIndex = pw->SP - n - 1;

    switch ((s8)pw->StackType[stackIndex]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_RET:
        return pw->StackData[stackIndex].i;
    case SCR_STACK_TYPE_FLOAT:
        return pw->StackData[stackIndex].f;
    case SCR_STACK_TYPE_INTVAR:
        return DDS3_GBWK->integers[pw->StackData[stackIndex].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->floats[pw->StackData[stackIndex].i];
    }
    return 0;
}

f32 scrGetFloatPara(s32 n)
{
    scrProcessWork_s *pw = nowpwork;
    s32 stackIndex = pw->SP - n - 1;

    switch ((s8)pw->StackType[stackIndex]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_RET:
        return (f32)pw->StackData[stackIndex].i;
    case SCR_STACK_TYPE_FLOAT:
        return pw->StackData[stackIndex].f;
    case SCR_STACK_TYPE_INTVAR:
        return (f32)DDS3_GBWK->integers[pw->StackData[stackIndex].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->floats[pw->StackData[stackIndex].i];
    }
    return 0.0f;
}

u8 *scrGetStrPara(s32 n)
{
    scrProcessWork_s *pw = nowpwork;
    s32 stackIndex = pw->SP - n - 1;
    s32 stackType = (s8)pw->StackType[stackIndex];

    if (stackType >= 0) {
        if (stackType >= SCR_STACK_TYPE_STRING) {
            if (stackType == SCR_STACK_TYPE_STRING) {
                return (u8 *)pw->StackData[stackIndex].i;
            }
        }
    }
    return NULL;
}

void scrSetIntReturnValue(s32 i)
{
    nowpwork->StackType[27] = SCR_STACK_TYPE_INT;
    nowpwork->StackData[27].i = i;
}

void scrSetFloatReturnValue(f32 f)
{
    nowpwork->StackType[27] = SCR_STACK_TYPE_FLOAT;
    nowpwork->StackData[27].f = f;
}

s32 scrGetProcedureCodeAddress(s32 procindex)
{
    return nowpwork->ProcTable[procindex].CodeAddress;
}

s32 scrGetLabelCodeAddress(s32 labelindex)
{
    return nowpwork->LabelTable[labelindex].CodeAddress;
}

s32 scrGetCodePointer(void)
{
    return nowpwork->CP;
}

void scrSetCodePointer(s32 cp)
{
    nowpwork->CP = cp;
}

s32 scrGetScriptTimer(void)
{
    return nowpwork->ScriptTimer;
}

s32 scrGetCommandTimer(void)
{
    return nowpwork->CommandTimer;
}

s32 scrGetMessageHandle(void)
{
    return nowpwork->MesHandle;
}

scrProcessWork_s *scrGetScriptHandle(void)
{
    return nowpwork;
}

void *scrGetData(void)
{
    return nowpwork->data;
}
