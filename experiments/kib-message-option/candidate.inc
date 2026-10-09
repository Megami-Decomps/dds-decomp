s32 func_001A4A10(s32 window, s32 id, s32 value) {
    ItfMesBlk40 *optionBlock = &itfWindowSlots[window].mes->blk40;
    ItfMesOption *option = NULL;
    s32 index;

    if ((u32)id >= 0x10) {
        return 0;
    }
    if (id == 1 || id == 6 || id == 7) {
        return 0;
    }
    for (index = 0; index < optionBlock->optionCount; index++) {
        if (optionBlock->options[index].id == id) {
            option = &optionBlock->options[index];
            break;
        }
    }
    if (option == NULL) {
        if (optionBlock->optionCount >= 15) {
            return 0;
        }
        option = &optionBlock->options[optionBlock->optionCount];
        optionBlock->optionCount++;
    }
    option->id = id;
    option->value = value;
    return 1;
}
