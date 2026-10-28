#ifndef BTL_COMMAND_RESULT_KIND_API_H
#define BTL_COMMAND_RESULT_KIND_API_H

#include "btl_action_press_result.h"
#include "btl_command_id.h"
#include "btl_scene_counter_mode.h"

/* Classify the aggregate action and press outcomes for the scene counter. */
#if defined(VERSION_DDS1)
BtlSceneCounterMode btlGetCommandResultKindFromFlags(
    u32, BtlActionPressResult);
#elif defined(VERSION_DDS2)
BtlSceneCounterMode btlGetCommandResultKindFromFlags(
    u32, BtlActionPressResult, BtlCommandId);
#else
#error "btl_command_result_kind_api.h requires a DDS title version"
#endif

#endif /* BTL_COMMAND_RESULT_KIND_API_H */
