/* Advance each selected entry, render its child stream, then restore the
 * instance parameters that the entry evaluator temporarily replaces. */
void func_001591D8(BillObj *obj) {
    BillChildPayload *child;
    if (obj->unk50 == 0) {
        return;
    }
    if (obj->modeFlags & 0x10000000) {
        BillData *data = obj->entryList;
        u8 *base = data->base;
        BillAnimationEntry *animation = data->entries + obj->unk58;
        BillPluralRecord *records = (BillPluralRecord *)(base + animation->offset);
        u32 pluralColor = obj->childParam;
        f32 pluralLength = obj->lengthScale;
        u32 i = 0;

        if (obj->entryCount != 0) {
            BillPluralRecord *record = records;

            do {
                BillOut *entry = (BillOut *)obj->unk60 + i;

                if (entry->frameIndex >= 0) {
                    f32 savedX;
                    f32 savedY;

                    child = func_00158F88(obj, entry);
                    savedX = child->x;
                    savedY = child->y;

                    child->x = record->x;
                    child->y = -record->y;
                    obj->childParam = effBillModulateColors(pluralColor, obj->childParam);
                    obj->lengthScale += pluralLength;
                    func_00157EA0(obj, child);
                    child->x = savedX;
                    child->y = savedY;
                } else {
                    entry->frameIndex++;
                }
                i++;
                record++;
            } while (i < (u32)obj->entryCount);
        }
        obj->childParam = pluralColor;
        obj->lengthScale = pluralLength;
    } else {
        u32 savedColor;
        f32 savedLength;

        if (obj->modeFlags & 0x40) {
            savedColor = obj->childParam;
            savedLength = obj->lengthScale;
            child = func_00158F88(obj, obj->unk60);
            obj->childParam = effBillModulateColors(savedColor, obj->childParam);
            obj->lengthScale += savedLength;
            func_00157EA0(obj, child);
            child = func_00158F88(obj, (BillOut *)obj->unk60 + 1);
            obj->childParam = effBillModulateColors(savedColor, obj->childParam);
            obj->lengthScale += savedLength;
            func_00157EA0(obj, child);
            obj->lengthScale = savedLength;
            obj->childParam = savedColor;
        } else if (obj->modeFlags & 0x80) {
            u32 flags;

            savedColor = obj->childParam;
            savedLength = obj->lengthScale;
            obj->pair.children[0] = func_00158F88(obj, obj->unk60);
            obj->pair.colors[0] = effBillModulateColors(savedColor, obj->childParam);
            obj->pair.children[1] = func_00158F88(obj, (BillOut *)obj->unk60 + 1);
            obj->pair.colors[1] = effBillModulateColors(savedColor, obj->childParam);
            flags = ((BillOut *)obj->unk60)[1].entry->flags;
            if (flags & 2) {
                obj->pair.kind = 2;
            } else if (flags & 4) {
                obj->pair.kind = 3;
            } else {
                obj->pair.kind = 1;
            }
            obj->lengthScale += savedLength;
            func_00158430(obj, &obj->pair);
            obj->lengthScale = savedLength;
            obj->childParam = savedColor;
        } else {
            savedColor = obj->childParam;
            savedLength = obj->lengthScale;
            child = func_00158F88(obj, obj->unk60);
            obj->childParam = effBillModulateColors(savedColor, obj->childParam);
            obj->lengthScale += savedLength;
            func_00157EA0(obj, child);
            obj->lengthScale = savedLength;
            obj->childParam = savedColor;
        }
    }
}
