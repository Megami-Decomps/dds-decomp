#ifndef DDS3_BASIC_TABLES_H
#define DDS3_BASIC_TABLES_H

#include "eff_transform.h"

/* The vtables dds3BasicFuncTablePtr points at, one per basic type. */
extern dds3BasicFunction_t worldBasicTable;
extern dds3BasicFunction_t sceneBasicTable;
extern dds3BasicFunction_t AreaBasicTable;
extern dds3BasicFunction_t MoverBasicTable;
extern dds3BasicFunction_t dds3CameraObjectBasicTable;
extern dds3BasicFunction_t dds3UnitObjectBasicTable;
extern dds3BasicFunction_t dds3MatterObjectBasicTable;
extern dds3BasicFunction_t dds3EffectObjectBasicTable;
extern dds3BasicFunction_t EventObjectBasicTable;
extern dds3BasicFunction_t lightBasicTable;
extern dds3BasicFunction_t dds3PathBasicTable;
extern dds3BasicFunction_t dds3ModelBasicTable;

#endif
