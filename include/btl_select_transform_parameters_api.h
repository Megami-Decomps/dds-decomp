#ifndef BTL_SELECT_TRANSFORM_PARAMETERS_API_H
#define BTL_SELECT_TRANSFORM_PARAMETERS_API_H

#include "btl_actor_transform_parameters.h"
#include "btl_actor_resource_table_kind.h"
#include "btl_actor_resource_table_kind_type.h"

BtlActorTransformParameters *btlSelectSharedOrIndexedTransformParameters(
    BtlActorResourceTableKind resourceTableKind, s32 index);

#endif /* BTL_SELECT_TRANSFORM_PARAMETERS_API_H */
