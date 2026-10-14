#ifndef EFF_RESOURCE_BANK_SLOT_H
#define EFF_RESOURCE_BANK_SLOT_H

#include "common.h"

typedef struct EffResourceBankSlot {
    char fullName[0xC8]; /* Formatted path or selected built-in name. */
    char name[0x34];     /* Selected resource name without its extension. */
    s32 type;            /* Resource category returned by the formatter. */
    s32 state;           /* Browser selection status. */
    s32 count;           /* Category-specific metadata from the selected row. */
    u8 opaque108[8];     /* In caller-sized buffers; not written by this provider. */
} EffResourceBankSlot;

typedef char EffResourceBankSlot_size_must_be_0x110[
    sizeof(EffResourceBankSlot) == 0x110 ? 1 : -1];
typedef char EffResourceBankSlot_fullName_must_be_0xC8[
    sizeof(((EffResourceBankSlot *)0)->fullName) == 0xC8 ? 1 : -1];
typedef char EffResourceBankSlot_name_must_be_0x34[
    sizeof(((EffResourceBankSlot *)0)->name) == 0x34 ? 1 : -1];
typedef char EffResourceBankSlot_type_must_be_at_0xFC[
    sizeof(((EffResourceBankSlot *)0)->fullName) +
    sizeof(((EffResourceBankSlot *)0)->name) == 0xFC ? 1 : -1];
typedef char EffResourceBankSlot_state_must_be_at_0x100[
    sizeof(((EffResourceBankSlot *)0)->fullName) +
    sizeof(((EffResourceBankSlot *)0)->name) +
    sizeof(((EffResourceBankSlot *)0)->type) == 0x100 ? 1 : -1];
typedef char EffResourceBankSlot_count_must_be_at_0x104[
    sizeof(((EffResourceBankSlot *)0)->fullName) +
    sizeof(((EffResourceBankSlot *)0)->name) +
    sizeof(((EffResourceBankSlot *)0)->type) +
    sizeof(((EffResourceBankSlot *)0)->state) == 0x104 ? 1 : -1];
typedef char EffResourceBankSlot_opaque_must_be_at_0x108[
    sizeof(((EffResourceBankSlot *)0)->fullName) +
    sizeof(((EffResourceBankSlot *)0)->name) +
    sizeof(((EffResourceBankSlot *)0)->type) +
    sizeof(((EffResourceBankSlot *)0)->state) +
    sizeof(((EffResourceBankSlot *)0)->count) == 0x108 ? 1 : -1];

#endif
