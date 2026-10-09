#ifndef BTL_RESOURCE_SELECTION_H
#define BTL_RESOURCE_SELECTION_H

/* Result values shared by the resource browser and name-entry selectors. */
enum BtlResourceSelectionStatus {
    BTL_RESOURCE_SELECTION_PENDING = 0,
    BTL_RESOURCE_SELECTION_ACCEPTED = 1,
    BTL_RESOURCE_SELECTION_CANCELED = 2,
};

#endif
