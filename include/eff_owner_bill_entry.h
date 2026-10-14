#ifndef EFF_OWNER_BILL_ENTRY_H
#define EFF_OWNER_BILL_ENTRY_H

#include "common.h"

/* Access view used when forwarding an owner's bill entry. */
typedef struct EffOwnerBillPayloadView {
    u8 pad00[8];
    u32 state;  /* +0x08: nonzero suppresses forwarding. */
    void *params; /* +0x0C: points to the nested parameter view. */
} EffOwnerBillPayloadView;

typedef char EffOwnerBillPayloadView_size_must_be_0x10[
    (sizeof(EffOwnerBillPayloadView) == 0x10) ? 1 : -1];
typedef char EffOwnerBillPayloadView_state_at_0x08[
    ((u32)&((EffOwnerBillPayloadView *)0)->state == 0x08) ? 1 : -1];
typedef char EffOwnerBillPayloadView_params_at_0x0C[
    ((u32)&((EffOwnerBillPayloadView *)0)->params == 0x0C) ? 1 : -1];

/* Only the SDF model argument at +0x18 is read; the prefix stays opaque. */
typedef struct EffOwnerBillParamView {
    u8 pad00[0x18];
    struct SdfModel *model; /* +0x18: argument to sdfLoadMapRecordLookAtBasis. */
} EffOwnerBillParamView;

typedef char EffOwnerBillParamView_size_must_be_0x1C[
    (sizeof(EffOwnerBillParamView) == 0x1C) ? 1 : -1];
typedef char EffOwnerBillParamView_model_at_0x18[
    ((u32)&((EffOwnerBillParamView *)0)->model == 0x18) ? 1 : -1];

#endif /* EFF_OWNER_BILL_ENTRY_H */
