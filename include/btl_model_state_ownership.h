#ifndef BTL_MODEL_STATE_OWNERSHIP_H
#define BTL_MODEL_STATE_OWNERSHIP_H

/* BtlUnit.modelBorrowed stores whether its shared actor model state is owned
 * by this unit or inherited from another unit. Every observed write is 0/1.
 */
enum {
    BTL_MODEL_STATE_OWNED = 0,
    BTL_MODEL_STATE_BORROWED = 1
};

#endif
