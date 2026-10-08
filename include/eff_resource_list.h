#ifndef EFF_RESOURCE_LIST_H
#define EFF_RESOURCE_LIST_H

struct EffectList;

/* Release the list header allocation. Entry nodes are managed separately. */
void effDestroyEffectList(struct EffectList *list);

#endif
