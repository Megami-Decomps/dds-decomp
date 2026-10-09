#ifndef EFF_RESOURCE_LIST_H
#define EFF_RESOURCE_LIST_H

struct EffectList;

/* Release the list header allocation. Entry nodes are managed separately. */
void effDestroyEffectList(struct EffectList *list);
/* Poll queued resource entries and report the number still pending. */
s32 effPollResourceList(struct EffectList *list);
u32 mnuGetValueRecordOwner(const struct EffectList *list);
#ifdef VERSION_DDS2
u32 effAppendListEntry(struct EffectList *list, u32 value, u32 length, u32 kind,
                       u32 reference);
#else
s32 effAppendListEntry(struct EffectList *list, u32 value, u32 length, u32 kind,
                       u32 reference);
#endif

#endif
