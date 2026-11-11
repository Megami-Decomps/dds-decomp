#ifndef DDS_SCR_INSTRUCTION_STREAM_API_H
#define DDS_SCR_INSTRUCTION_STREAM_API_H

#include "scr.h"

/* The VM borrows its source tables and reads them without modifying them.
 * Message auxiliary data stays mutable because window initialization relocates it. */
scrProcessWork_s *scrPrintProcedureStart(const struct scrHeader_t *header,
    const scrTypeTable_t *sections, const scrProcTable_t *procedures, const scrLabelTable_t *labels,
    const scrCode_u *instructions,
    struct ItfMesSub *auxiliaryData, const char *strings, ScrProcedureIndex procedureIndex);

dds3ProcessID_t *scrRunScriptEx(u32 priority, const struct scrHeader_t *header,
    const scrTypeTable_t *sections, const scrProcTable_t *procedures, const scrLabelTable_t *labels,
    const scrCode_u *instructions, struct ItfMesSub *auxiliaryData,
    const char *strings, ScrProcedureIndex procedureIndex);

#endif /* DDS_SCR_INSTRUCTION_STREAM_API_H */
