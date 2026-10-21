#include "common.h"

void sceMcFormat(u32 arg0, s32 arg1);

void fileReqStartCardDetection(u32 port) {
    sceMcFormat(port, 0);
}
