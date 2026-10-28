#ifndef BTL_ACTOR_RESOURCE_TABLE_KIND_H
#define BTL_ACTOR_RESOURCE_TABLE_KIND_H

/* The actor setup paths select the party bank with zero and the opposing
 * actor bank with one. The table providers route zero to the first bank and
 * every nonzero value to the second. */
#define BTL_ACTOR_RESOURCE_TABLE_PARTY 0
#define BTL_ACTOR_RESOURCE_TABLE_OPPOSING 1

#endif /* BTL_ACTOR_RESOURCE_TABLE_KIND_H */
