#!/usr/bin/env python3
"""Export a module's types, prototypes and data symbols to include/irx/<module>.h.

    python tools/irx_headers.py [MODULE ...]

Read-only on the IDB, like tools/irx_symbols.py: the database holds the
recovered types, so the header is generated from it rather than hand-edited.
"""
from __future__ import annotations

import argparse
import json
import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(Path(__file__).resolve().parent))
import irx_config  # noqa: E402

IDA_SCRIPTS = Path(
    os.environ.get("IDA_SKILL_SCRIPTS", Path.home() / ".claude/skills/ida-instances/scripts")
)

# irx.h defines these, and IDA renders them with MSVC spellings like __int8.
BUILTIN = {"u8", "u16", "u32", "u64", "s8", "s16", "s32", "s64",
           "BOOL", "va_list", "__builtin_va_list", "_BOOL1", "_BOOL4"}

PLACEHOLDER = re.compile(r"^(sub_|nullsub_|loc_|locret_|def_|unk_|byte_|word_|"
                         r"dword_|qword_|asc_|algn_|jpt_|jtbl_|off_|stru_)")

# An import stub lives in the module's own .text but belongs to another library,
# and include/irx/iop.h declares it with the signature the SDK gives it.
IMPORT_PREFIX = re.compile(r"^(libsd|intrman|loadcore|stdio|sysclib|thbase|thsemap|"
                           r"sifcmd|sifman|timrman|ioman|sysmem|modload|excepman|"
                           r"heaplib|sdfiopfl)_")

# IDA's spellings for things the IOP toolchain has its own names for, or no name
# at all: MIPS has one calling convention.
IDAISM = [
    (re.compile(r"\b__(fastcall|cdecl|stdcall|usercall|noreturn|spoils)\b\s*"), ""),
    # The signed spellings come first: otherwise __int8 is rewritten on its own
    # and leaves "signed s8", which does not compile.
    (re.compile(r"\bsigned __int8\b"), "s8"),
    (re.compile(r"\bsigned __int16\b"), "s16"),
    (re.compile(r"\bsigned __int32\b"), "s32"),
    (re.compile(r"\bsigned __int64\b"), "s64"),
    (re.compile(r"\bsigned char\b"), "s8"),
    (re.compile(r"\bunsigned char\b"), "u8"),
    (re.compile(r"\bunsigned short\b"), "u16"),
    (re.compile(r"\bunsigned __int8\b"), "u8"),
    (re.compile(r"\bunsigned __int16\b"), "u16"),
    (re.compile(r"\bunsigned __int32\b"), "u32"),
    (re.compile(r"\bunsigned __int64\b"), "u64"),
    (re.compile(r"\b__int8\b"), "s8"),
    (re.compile(r"\b__int16\b"), "s16"),
    (re.compile(r"\b__int32\b"), "s32"),
    (re.compile(r"\b__int64\b"), "s64"),
    (re.compile(r"\b_BYTE\b"), "u8"),
    (re.compile(r"\b_WORD\b"), "u16"),
    (re.compile(r"\b_DWORD\b"), "u32"),
    (re.compile(r"\b_QWORD\b"), "u64"),
    (re.compile(r"\b_BOOL1\b"), "u8"),
    (re.compile(r"\b_BOOL4\b"), "s32"),
    (re.compile(r"\bBOOL\b"), "s32"),
]


def detype(text: str) -> str:
    for pattern, repl in IDAISM:
        text = pattern.sub(repl, text)
    return text

PAYLOAD_TEMPLATE = r'''
def export():
    import json
    import ida_bytes, ida_funcs, ida_nalt, ida_segment, ida_typeinf, idautils, idc

    class Sink(ida_typeinf.text_sink_t):
        def __init__(self):
            ida_typeinf.text_sink_t.__init__(self)
            self.parts = []

        def _print(self, text):
            self.parts.append(text)
            return 0

    skip = set(%s)
    til = ida_typeinf.get_idati()
    types, ordinals = [], []
    for i in range(1, ida_typeinf.get_ordinal_limit()):
        t = ida_typeinf.tinfo_t()
        if not t.get_numbered_type(None, i):
            continue
        name = t.get_type_name()
        if not name:
            continue
        types.append([name, t.get_size(), t.is_struct() or t.is_union()])
        ordinals.append(i)

    sink = Sink()
    ordinals = [o for o, t in zip(ordinals, types) if t[0] not in skip]
    if ordinals:
        ida_typeinf.print_decls(sink, til, ordinals,
                                ida_typeinf.PDF_INCL_DEPS | ida_typeinf.PDF_DEF_FWD)

    protos, data = [], []
    flags = ida_typeinf.PRTYPE_1LINE | ida_typeinf.PRTYPE_SEMI
    funcs = set()
    for ea in idautils.Functions():
        s = ida_segment.getseg(ea)
        if not s or ida_segment.get_segm_name(s) != ".text":
            continue
        funcs.add(ea)
        name = idc.get_func_name(ea)
        # A function IDA has not named is func_<address> here, the same name
        # tools/irx_symbols.py gives it, because the module's own C calls it by
        # that name while it is still assembly and needs the prototype.
        if name.startswith(("sub_", "nullsub_")):
            name = "func_%08X" % ea
        tif = ida_typeinf.tinfo_t()
        got = ida_nalt.get_tinfo(tif, ea)
        protos.append([ea, name, tif._print(name, flags) if got else None])

    keep = (".rodata", ".data", ".sdata", ".sbss", ".bss")
    for ea, name in idautils.Names():
        if ea in funcs:
            continue
        s = ida_segment.getseg(ea)
        if not s or ida_segment.get_segm_name(s) not in keep:
            continue
        tif = ida_typeinf.tinfo_t()
        got = ida_nalt.get_tinfo(tif, ea)
        data.append([ea, name, tif._print(name, flags) if got else None,
                     ida_bytes.get_item_size(ea)])

    return json.dumps({"types": types, "decls": "".join(sink.parts),
                       "protos": sorted(protos), "data": sorted(data)})

RESULT = export()
'''


def collect(instance: str, ports: str | None, skip: set[str]) -> dict:
    payload = PAYLOAD_TEMPLATE.replace('%s', repr(sorted(skip)), 1)
    sys.path.insert(0, str(IDA_SCRIPTS))
    import ida

    instances = ida.get_instances(ida.parse_ports(ports or ida.DEFAULT_PORTS), refresh=False)
    port = ida.resolve_target(instance, instances)
    out = ida.call_tool(port, "py_eval", {"code": payload, "new_locals": True})
    if out.get("stderr"):
        raise SystemExit(f"IDAPython failed:\n{out['stderr']}")
    return json.loads(ida.call_tool(port, "py_eval", {"code": "RESULT"})["result"])


def clean_decls(text: str) -> str:
    """Drop what irx.h already provides and what iop-gcc 2.8.1 cannot read."""
    out = []
    for line in text.splitlines():
        s = line.strip()
        if s.startswith("#pragma pack"):
            # gcc 2.8 predates it. The size checks below catch any layout this
            # costs us, and these structs are naturally aligned anyway.
            continue
        if s.startswith("/*") and s.endswith("*/"):
            continue  # the ordinal comment print_decls puts before each type
        m = re.match(r"^typedef\s+.*\b(\w+);$", s)
        if m and m.group(1) in BUILTIN:
            continue
        out.append(line.rstrip())
    text = detype("\n".join(out))
    return re.sub(r"\n{3,}", "\n\n", text).strip()


def name_anonymous_structs(decls: str) -> tuple[str, dict]:
    """Give an anonymous struct the name its typedef uses.

    IDA names a struct with no tag after a hash of its layout, so the header
    ends up with `struct $4ABAB...` bodies that nothing readable refers to and a
    typedef beside each one. Fold the two together.
    """
    pairs = re.findall(r"^typedef struct (\$[0-9A-Fa-f]+) (\w+);$", decls, re.M)
    rename = {h: n for h, n in pairs if not n.startswith("$")}
    for h, n in rename.items():
        decls = decls.replace(h, n)
    # the bodies now carry their real names, and render() emits one forward
    # typedef per struct, so the aliases here are redundant
    out = [l for l in decls.splitlines()
           if not re.match(r"^typedef struct (\w+) \1;$", l)]
    return "\n".join(out), rename


def render(module: str, blob: dict, iop_declared: set[str]) -> str:
    L = [f"#ifndef IRX_{module}_H",
         f"#define IRX_{module}_H",
         "",
         f"/* Generated by tools/irx_headers.py from the {module}.IRX IDB; do not",
         " * hand-edit. The database is where the recovered types live, so a change",
         " * belongs there and comes back through this file.",
         " *",
         " * What the module does is in docs/irx-modules.md.",
         " */",
         "",
         '#include "iop.h"',
         ""]

    decls = clean_decls(blob["decls"])
    decls, renamed = name_anonymous_structs(decls)
    named = [t for t in blob["types"]
             if t[0] not in BUILTIN and f"struct {renamed.get(t[0], t[0])}" in decls]
    named = [(renamed.get(n, n), sz, udt) for n, sz, udt in named]
    seen = set()                      # the hash struct and its typedef are one type
    named = [t for t in named if not (t[0] in seen or seen.add(t[0]))]
    if decls:
        L += ["/* Types ------------------------------------------------------------ */",
              ""]
        # The bare name has to work as a type, which a struct tag alone does not.
        fwd = [f"typedef struct {n} {n};" for n, _, is_udt in named if is_udt]
        if fwd:
            L += fwd + [""]
        L += [decls, ""]
        checks = [f"typedef char __{module}_sizeof_{n}[(sizeof({n}) == {sz}) ? 1 : -1];"
                  for n, sz, is_udt in named if is_udt and sz > 0]
        if checks:
            L += ["/* A layout the compiler reads differently is a build error. */"] + checks + [""]

    protos = [p for p in blob["protos"]
              if p[2] and not PLACEHOLDER.match(p[1])
              and p[1] not in iop_declared and not IMPORT_PREFIX.match(p[1])]
    if protos:
        L += ["/* Functions -------------------------------------------------------- */",
              ""]
        L += [detype(p[2]) for p in protos] + [""]

    data = [d for d in blob["data"] if not PLACEHOLDER.match(d[1])]
    if data:
        L += ["/* Data ------------------------------------------------------------- */",
              ""]
        for ea, name, decl, size in data:
            if decl:
                L.append("extern " + detype(decl))
            else:
                L.append(f"extern u8 {name}[];" if size != 4 else f"extern u32 {name};")
        L.append("")

    L += [f"#endif /* IRX_{module}_H */", ""]
    return drop_orphan_structs("\n".join(L))


def drop_orphan_structs(text: str) -> str:
    """Remove a layout-hash struct that nothing in the header refers to.

    An anonymous struct whose only alias is a type iop.h already declares has
    its typedef skipped, which leaves the body behind under IDA's hash with
    nothing pointing at it. Drop the body, its forward typedef and its size
    check rather than emit a name no one can use.
    """
    for h in sorted(set(re.findall(r"\$[0-9A-Fa-f]{32}", text))):
        body = re.search(r"^struct %s\n\{.*?^\};\n" % re.escape(h), text, re.M | re.S)
        fwd = f"typedef struct {h} {h};\n"
        chk = re.search(r"^typedef char __\w+?_sizeof_%s\[[^\]]*\];\n" % re.escape(h),
                        text, re.M)
        spent = len(re.findall(re.escape(h), (body.group(0) if body else "")
                               + fwd + (chk.group(0) if chk else "")))
        if len(re.findall(re.escape(h), text)) != spent:
            continue                      # something really uses it; leave it alone
        if body:
            text = text.replace(body.group(0), "", 1)
        text = text.replace(fwd, "", 1)
        if chk:
            text = text.replace(chk.group(0), "", 1)
    return text


def iop_h_names() -> tuple[set[str], set[str]]:
    """What include/irx/iop.h already declares: functions, then types."""
    text = (ROOT / "include" / "irx" / "iop.h").read_text()
    funcs = set(re.findall(r"\b(\w+)\s*\(", text))
    types = set(re.findall(r"^\}\s*(\w+);", text, re.M))
    types |= set(re.findall(r"^typedef\s+[\w\s*]+?(\w+);", text, re.M))
    return funcs, types


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("modules", nargs="*")
    ap.add_argument("--ports", default=None)
    args = ap.parse_args()

    declared, iop_types = iop_h_names()
    for module in args.modules or irx_config.modules():
        cfg = irx_config.load(module)
        if "idb" not in cfg:
            continue
        blob = collect(cfg["idb"], args.ports, BUILTIN | iop_types)
        path = ROOT / "include" / "irx" / f"{module}.h"
        text = render(module, blob, declared)
        path.write_text(text)
        named = [t for t in blob["types"] if f"struct {t[0]}" in text]
        protos = [p for p in blob["protos"]
                  if p[2] and not PLACEHOLDER.match(p[1])
                  and not IMPORT_PREFIX.match(p[1])]
        print(f"{path.relative_to(ROOT)}: {len(named)} types, {len(protos)} prototypes, "
              f"{len(blob['data'])} data symbols")


if __name__ == "__main__":
    main()
