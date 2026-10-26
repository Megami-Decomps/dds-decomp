#ifndef DDS_SCR_INSTRUCTION_STREAM_API_H
#define DDS_SCR_INSTRUCTION_STREAM_API_H

#include "scr.h"

/* The VM borrows its source tables and reads them without modifying them.
 * Message auxiliary data stays mutable because window initialization relocates it. */
scrProcessWork_s *bfContextCreate(const struct BfFlw0Header *header,
    const ScrSection *sections, const ScrLabel *procedures, const ScrLabel *labels,
    const ScrInstr *instructions,
    struct ItfMesSub *auxiliaryData, const char *strings, ScrProcedureIndex procedureIndex);

dds3ProcessID_t *scrRunScriptEx(u32 priority, const struct BfFlw0Header *header,
    const ScrSection *sections, const ScrLabel *procedures, const ScrLabel *labels,
    const ScrInstr *instructions, struct ItfMesSub *auxiliaryData,
    const char *strings, ScrProcedureIndex procedureIndex);

#endif /* DDS_SCR_INSTRUCTION_STREAM_API_H */
