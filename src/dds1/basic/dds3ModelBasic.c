#include "common.h"

typedef struct {
    u8 pad[0x18];
    void *unk18;
} ModelObj;

void sdfReleaseChipBlock(void *arg);

void dds3ReleaseModelAllocation(ModelObj *model) {
    sdfReleaseChipBlock(model->unk18);
}
