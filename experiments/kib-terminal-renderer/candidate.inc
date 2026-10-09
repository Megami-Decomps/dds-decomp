extern void func_00294D50(s32, s32, s32, MenuTerminalContext *, s32, s32);
extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, u8 *, s32);

s32 func_00297320(MenuTerminalContext *scene) {
    f32 progress;
    f32 reveal;
    f32 position;
    s32 frames;

    func_00294B40(0, 0, 0, scene, 0x100, 0x53);
    switch (scene->phase) {
    case 0:
        if ((f32)scene->commandFrames > 0.0f) {
            frames = (s32)((f32)scene->commandFrames - 0.0f);
            if (frames < 0) frames = 0;
            progress = (f32)frames / 10.0f;
            reveal = 0.0f;
            if (!(progress < 0.5f)) reveal = (progress - 0.5f) * 2.0f;
        } else {
            progress = 0.0f;
            reveal = 0.0f;
        }
        position = progress;
        if (progress > 1.0f) progress = 1.0f;
        if (reveal > 1.0f) reveal = 1.0f;
        if (position > 1.0f) position = 1.0f;
        func_00294D50(0, (s32)((1.0f - position) * -48.0f) << 3, 0,
                      scene, (s32)(reveal * 256.0f), 0x53);
        func_00296D90(scene, 0xA09DC300 | (s32)(reveal * 128.0f));
        func_00297000((s32)scene, (u32)(progress * 256.0f), 4, 0x53);
        break;
    case 2:
        if ((f32)scene->commandFrames > 0.0f) {
            progress = ((f32)scene->commandFrames - 0.0f) / 10.0f;
            if (progress > 1.0f) progress = 1.0f;
        } else {
            progress = 0.0f;
        }
        position = progress;
        if (progress > 1.0f) progress = 1.0f;
        if (position > 1.0f) position = 1.0f;
        func_00294D50(0, (s32)(position * 64.0f) << 3, 0,
                      scene, (s32)((1.0f - progress) * 256.0f), 0x53);
        mnuDrawListChildrenWithCountdown(0, 0, 0, (u8 *)scene->ownedWindows[0]->list, 0x53);
        func_00296D90(scene, 0xA09DC380);
        func_00296E98((s32)scene, (u32)((1.0f - progress) * 256.0f), 4, 0x53);
        break;
    case 1:
        func_00294D50(0, 0, 0, scene, 0x100, 0x53);
        mnuDrawListChildrenWithCountdown(0, 0, 0, (u8 *)scene->ownedWindows[0]->list, 0x53);
        func_00296D90(scene, 0xA09DC380);
        func_00296E98((s32)scene, 0x100, 4, 0x53);
        break;
    case 3:
        func_00294D50(0, 0, 0, scene, 0x100, 0x53);
        mnuDrawListChildrenWithCountdown(0, 0, 0, (u8 *)scene->ownedWindows[0]->list, 0x53);
        func_00296D90(scene, 0xA09DC380);
        func_00296E98((s32)scene, 0x100, 4, 0x53);
        break;
    case 14:
        if ((f32)scene->commandFrames > 0.0f) {
            frames = (s32)((f32)scene->commandFrames - 0.0f);
            if (frames < 0) frames = 0;
            progress = (f32)frames / 10.0f;
            reveal = 0.0f;
            if (!(progress < 0.5f)) reveal = (progress - 0.5f) * 2.0f;
        } else {
            progress = 0.0f;
            reveal = 0.0f;
        }
        position = progress;
        if (reveal > 1.0f) reveal = 1.0f;
        if (position > 1.0f) position = 1.0f;
        func_00294D50(0, (s32)((1.0f - position) * -48.0f) << 3, 0,
                      scene, (s32)(reveal * 256.0f), 0x53);
        mnuDrawListChildrenWithCountdown(0, 0, 0, (u8 *)scene->ownedWindows[0]->list, 0x53);
        func_00296D90(scene, 0xA09DC380);
        func_00297000((s32)scene, 0x100, 4, 0x53);
        break;
    case 15:
        if ((f32)scene->commandFrames > 0.0f) {
            progress = ((f32)scene->commandFrames - 0.0f) / 10.0f;
            if (progress > 1.0f) progress = 1.0f;
        } else {
            progress = 0.0f;
        }
        position = progress;
        if (progress > 1.0f) progress = 1.0f;
        if (position > 1.0f) position = 1.0f;
        func_00294D50(0, (s32)(position * 64.0f) << 3, 0,
                      scene, (s32)((1.0f - progress) * 256.0f), 0x53);
        mnuDrawListChildrenWithCountdown(0, 0, 0, (u8 *)scene->ownedWindows[0]->list, 0x53);
        func_00296D90(scene, 0xA09DC380);
        func_00296E98((s32)scene, 0x100, 4, 0x53);
        break;
    }
    return 0;
}
