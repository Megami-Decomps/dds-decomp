#ifndef EFF_EXPANDED_LIST_H
#define EFF_EXPANDED_LIST_H

/* Kind-7 resource wrapper; its retained RefObj children stay distinct. */
struct EffExpandedList;

struct EffExpandedList *effReferenceObjectRetain(struct EffExpandedList *obj);
void effReleaseReferenceHolder(struct EffExpandedList *holder);

#endif
