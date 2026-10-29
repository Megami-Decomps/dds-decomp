# Native SDK runtime names

The GCC runtime helpers below retain their native layouts and calling
conventions. DDS1 and DDS2 bodies agree after masking resolved code/data
relocations only: scalar constants, branches, registers and member offsets
are unchanged. They also pair with the corrected Nocturne SDK entries.
The GCC definitions establish the constructor/destructor, frame-registration,
packing, comparison, cleanup and integer-conversion roles.

| DDS1 | DDS2 | Previous DDS name | Runtime name |
| --- | --- | --- | --- |
| 0x2fbf58 | 0x356e08 | runAtExitList | __do_global_dtors |
| 0x2fbfe8 | 0x356e98 | initAtExit | __do_global_ctors |
| 0x2fcf50 | 0x358098 | registerAtExit | __register_frame_info |
| 0x2fd140 | 0x358288 | ctime_4 | __deregister_frame |
| 0x2fcfe8 | 0x358130 | func_002FCFE8 | __register_frame_info_table |
| 0x2fded8 | 0x359020 | func_002FDED8 | __pack_df |
| 0x2fe070 | 0x3591b8 | func_002FE070 | __fpcmp_parts_df |
| 0x2fe748 | 0x359890 | func_002FE748 | _cleanup_r |
| 0x315758 | 0x3709d8 | func_00315758 | __fixunsdfdi |
| 0x3156b0 | 0x370930 | func_003156B0 | __floatdidf |
| 0x2fc718 | 0x357860 | func_002FC718 | __make_dp |

DDS2 address-placeholder names use their own addresses; the table shows the
DDS1 spelling for those rows. Both versions now use the runtime name.

`__make_dp` stores class, sign, exponent and fraction into a temporary record
and calls `__pack_df`; its name is inferred from the corresponding GCC
four-argument packing helper. It is distinct from `dptoul`, the conversion.
See [GCC fp-bit.c](https://github.com/gcc-mirror/gcc/blob/releases/gcc-2.95.3/gcc/config/fp-bit.c)
and [Nocturne's native corrections](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/idb-corrections.md).
These are naming changes only; no SDK code or input is added.
