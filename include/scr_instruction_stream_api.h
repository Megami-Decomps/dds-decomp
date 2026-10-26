#ifndef DDS_SCR_INSTRUCTION_STREAM_API_H
#define DDS_SCR_INSTRUCTION_STREAM_API_H

#include "scr.h"

/* The caller lends immutable decoded instruction data for the process lifetime. */
scrProcessWork_s *bfContextCreate(struct BfFlw0Header *header, ScrSection *sections,
    ScrLabel *procedures, ScrLabel *labels, const ScrInstr *instructions,
    struct ItfMesSub *auxiliaryData, const char *strings, ScrProcedureIndex procedureIndex);

dds3ProcessID_t *scrRunScriptEx(u32 priority, struct BfFlw0Header *header,
    ScrSection *sections, ScrLabel *procedures, ScrLabel *labels,
    const ScrInstr *instructions, struct ItfMesSub *auxiliaryData,
    const char *strings, ScrProcedureIndex procedureIndex);

#endif /* DDS_SCR_INSTRUCTION_STREAM_API_H */
