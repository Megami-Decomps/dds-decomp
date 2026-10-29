#ifndef MNU_CAMP_OPTION_INDEX_API_H
#define MNU_CAMP_OPTION_INDEX_API_H

#include "common.h"

struct evtViewerInfo_t;

/* Zero-based selections stored in the camp viewer's primary and secondary
 * option fields. The current menus expose two primary rows and three
 * secondary rows; setters preserve the packed two-bit representation. */
typedef u32 MnuCampPrimaryOptionIndex;
typedef u32 MnuCampSecondaryOptionIndex;

void mnuCampSetPrimaryOption(struct evtViewerInfo_t *scene, MnuCampPrimaryOptionIndex optionIndex);
MnuCampPrimaryOptionIndex mnuCampGetPrimaryOption(struct evtViewerInfo_t *scene);
void mnuCampSetSecondaryOption(struct evtViewerInfo_t *scene, MnuCampSecondaryOptionIndex optionIndex);
MnuCampSecondaryOptionIndex mnuCampGetSecondaryOption(struct evtViewerInfo_t *scene);

#endif /* MNU_CAMP_OPTION_INDEX_API_H */
