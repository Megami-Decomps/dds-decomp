#ifndef MNU_CAMP_OPTION_INDEX_API_H
#define MNU_CAMP_OPTION_INDEX_API_H

#include "common.h"

struct EvtRuntime;

/* Zero-based selections stored in the camp viewer's primary and secondary
 * option fields. The current menus expose two primary rows and three
 * secondary rows; setters preserve the packed two-bit representation. */
typedef u32 MnuCampPrimaryOptionIndex;
typedef u32 MnuCampSecondaryOptionIndex;

void mnuCampSetPrimaryOption(struct EvtRuntime *scene, MnuCampPrimaryOptionIndex optionIndex);
MnuCampPrimaryOptionIndex mnuCampGetPrimaryOption(struct EvtRuntime *scene);
void mnuCampSetSecondaryOption(struct EvtRuntime *scene, MnuCampSecondaryOptionIndex optionIndex);
MnuCampSecondaryOptionIndex mnuCampGetSecondaryOption(struct EvtRuntime *scene);

#endif /* MNU_CAMP_OPTION_INDEX_API_H */
