#include "common.h"
#include "mnu_result.h"


extern s32 btlAddBaseStats(s32 *, DatPartyRecord *);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern void mnuRefreshSelectedUnitPanels(DatPartyRecord *, BrsSkillPackageWork *);

void kwlnItemUpdateDisplay(BrsSkillPackageWork *scene) {
    DatPartyRecord *item = scene->selectedRewardRow->unit;
    switch (scene->rewardMode) {
    case 4:
        btlAddBaseStats(scene->statGains, item);
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 1:
        item->hp = item->maxHp;
        item->mp = item->maxMp;
        item->status = 0;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 2:
        item->hp = item->maxHp;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 3:
        item->mp = item->maxMp;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    }
    mnuRefreshSelectedUnitPanels(item, scene);
}
