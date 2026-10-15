#!/usr/bin/env python3
"""Explain a near-miss's register differences against retail, pseudo by pseudo.

    python3 tools/ee_gcc_retail_regs.py src/dds2/game/code_002DE248.c \
        --source /path/to/unit_with_draft.c --func billAdvanceCellInstances

The source is a complete unit file in which the target function is C (the
same input `check_unit.py --source` takes). The tool

  1. compares the draft with retail (check_unit.py) and rebuilds both word
     streams from the function's nonmatching assembly,
  2. compiles the draft with `-dp` so every emitted instruction carries its
     RTL insn uid,
  3. runs `cc1probe.sh --alloc` for the pass-19/20 dumps (pseudo use counts,
     live lengths, call crossings, conflicts, global order, dispositions),
  4. for every operand register that differs from retail, names the pseudo
     that produced it, its allocator priority, its defining RTL, and each
     conflicting pseudo that holds the register retail used, with the live
     length or reference count that would reverse the decision.

Priorities use global.c's formula, floor_log2(refs) * refs * 10000 / live
(local-alloc ranks qtys with the same terms over block-local lifetimes).
Reference counts are loop-depth weighted, as flow.c records them. A result
of "no conflicting holder" means order or preference decided it, or reload
chose the register; read the probe's decisions.log for those.
"""
import argparse
import collections
import difflib
import os
import re
import struct
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ABI = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
       's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7', 't8', 't9', 'k0', 'k1', 'gp', 'sp', 's8', 'ra']
CALL_CLOBBERED = set(range(1, 16)) | {24, 25, 31}
OBJDUMP = os.path.join(ROOT, 'tools', 'bin', 'mips-ps2-decompals-objdump')
MACROS = {'subu': ['addiu'], 'move': ['daddu'], 'li': ['addiu'], 'b': ['beq'], 's.s': ['swc1'],
          'l.s': ['lwc1'], 'la': ['lui', 'addiu'], 'li.s': ['lui', 'mtc1']}


def regnum(tok):
    tok = tok.strip().lstrip('$')
    if tok in ABI:
        return ABI.index(tok)
    if tok == 'fp':
        return 30
    m = re.fullmatch(r'f(\d+)', tok)
    if m:
        return 32 + int(m.group(1))
    return None


def regname(n):
    return ABI[n] if n < 32 else f'f{n - 32}' if n < 64 else f'#{n}'


def priority(d):
    if not d or not d['live']:
        return 0
    return (d['refs'].bit_length() - 1) * d['refs'] * 10000 // d['live']


def disassemble(words):
    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as f:
        f.write(b''.join(struct.pack('<I', w) for w in words))
    try:
        out = subprocess.run([OBJDUMP, '-D', '-b', 'binary', '-m', 'mips:5900', '-EL', '-M', 'no-aliases',
                              f.name], capture_output=True, text=True).stdout
    finally:
        os.remove(f.name)
    text = {}
    for m in re.finditer(r'^\s*([0-9a-f]+):\s+[0-9a-f]+\s+(.*)$', out, re.M):
        text[int(m.group(1), 16) // 4] = ' '.join(m.group(2).split())
    return [text.get(i, '?') for i in range(len(words))]


def operands(text):
    parts = text.split(None, 1)
    if len(parts) < 2:
        return parts[0] if parts else '', []
    return parts[0], [x.strip() for x in re.split(r'[,()]', parts[1]) if x.strip()]


def function_dump(path, func):
    text = open(path, errors='ignore').read()
    start = text.find(f';; Function {func}\n')
    if start < 0:
        sys.exit(f'{path}: no dump for {func}')
    end = text.find(';; Function ', start + 10)
    return text[start:end if end > 0 else None]


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('unit')
    ap.add_argument('--source', required=True, help='complete unit file with the draft in place')
    ap.add_argument('--func', required=True)
    ap.add_argument('--out', help='work directory (default: a new temporary directory)')
    ap.add_argument('--max', type=int, default=30, help='mismatches to report')
    args = ap.parse_args()
    version = args.unit.split('/')[1]
    out = args.out or tempfile.mkdtemp(prefix='retail-regs-')
    os.makedirs(out, exist_ok=True)
    source = os.path.abspath(args.source)

    # Retail words from the nonmatching assembly; draft words differ only where check_unit says.
    asm = subprocess.run(['find', f'asm/{version}/nonmatchings', '-name', f'{args.func}.s'],
                         capture_output=True, text=True, cwd=ROOT).stdout.split()
    if not asm:
        sys.exit(f'no nonmatching assembly for {args.func}')
    # The comments print each word's bytes in memory (little-endian) order.
    retail = [int.from_bytes(bytes.fromhex(w), 'little')
              for w in re.findall(r'/\* [0-9A-F]+ [0-9A-F]+ ([0-9A-F]{8}) \*/',
                                  open(os.path.join(ROOT, asm[0])).read())]
    check = subprocess.run([sys.executable, 'tools/check_unit.py', args.unit, '--source', source,
                            '--func', args.func, '-v'], capture_output=True, text=True, cwd=ROOT).stdout
    mine = list(retail)
    for off, a, _ in re.findall(r'\+0x(\w+) mine (\w+) retail (\w+)', check):
        i = int(off, 16) // 4
        while i >= len(mine):
            mine.append(0)
        mine[i] = int(a, 16)
    summary = re.search(rf'{args.func}.*', check)
    print(summary.group(0) if summary else check.strip().splitlines()[-1:])
    mine_text, retail_text = disassemble(mine), disassemble(retail + [0] * (len(mine) - len(retail)))

    # Instruction -> insn uid from an -dp compile.
    env = {**os.environ, 'DDS_VERSION': version, 'DDS_AS_UNIT': args.unit, 'DDS_KEEP_S': f'{out}/draft.s'}
    subprocess.run(['tools/cc.sh', '-dp', source, '-o', f'{out}/draft.o'], capture_output=True, cwd=ROOT, env=env)
    lines, inside, uid, in_asm = [], False, None, False
    for line in open(f'{out}/draft.s', errors='ignore'):
        if re.match(rf'^{re.escape(args.func)}:', line):
            inside = True
            continue
        if not inside:
            continue
        if re.match(r'^\s*\.end\s', line):
            break
        t = line.strip()
        if t.startswith('#APP'):
            in_asm = True
            continue
        if t.startswith('#NO_APP'):
            in_asm = False
            continue
        if not t or t[0] in '#.' or re.match(r'^\$?\w+:$', t):
            continue
        m = re.search(r'#\s*(\d+)\s+\S+.*\[length = \d+\]', line)
        if m:
            uid = int(m.group(1))
        mnem = t.split()[0]
        owner = None if (in_asm and not m) or (mnem == 'nop' and not m) else uid
        lines += [(x, owner) for x in MACROS.get(mnem, [mnem])]
    sm = difflib.SequenceMatcher(None, [x for x, _ in lines], [t.split()[0] for t in mine_text], autojunk=False)
    uid_at = [None] * len(mine_text)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag in ('equal', 'replace'):
            for k in range(min(i2 - i1, j2 - j1)):
                uid_at[j1 + k] = lines[i1 + k][1]

    # Allocator dumps.
    probe = f'{out}/probe'
    penv = {**os.environ}
    penv.setdefault('TMPDIR', out)
    subprocess.run(['tools/cc1probe.sh', source, '--alloc', '--func', args.func, '--as-unit', args.unit,
                    '--version', version, '--out', probe, '--quiet'], capture_output=True, cwd=ROOT, env=penv)
    lreg = function_dump(f'{probe}/rtl.19.lreg', args.func)
    greg = function_dump(f'{probe}/rtl.20.greg', args.func)
    info = {}
    for m in re.finditer(r'^Register (\d+) used (\d+) times across (\d+) insns(.*)$', lreg, re.M):
        calls = re.search(r'crosses (\d+) call', m.group(4))
        info[int(m.group(1))] = dict(refs=int(m.group(2)), live=int(m.group(3)),
                                     calls=int(calls.group(1)) if calls else 0, local=' in block ' in m.group(4))
    block = greg[greg.find('Register dispositions'):].split('\n\n')[0]
    disp = {int(a): int(b) for a, b in re.findall(r'(\d+) in (\d+)', block)}
    conflicts = {int(a): {int(x) for x in b.split()}
                 for a, b in re.findall(r'^;; (\d+) conflicts: ([\d ]*)$', greg, re.M)}
    m = re.search(r'^;; \d+ regs to allocate: ([\d ]*)$', greg, re.M)
    order = {int(r): k for k, r in enumerate(m.group(1).split())} if m else {}
    insn_regs, defs = {}, {}
    for chunk in re.split(r'\n(?=\((?:insn|jump_insn|call_insn|note|code_label|barrier))', lreg):
        m = re.match(r'\((?:insn|jump_insn|call_insn)\S* (\d+) ', chunk)
        if not m:
            continue
        insn_regs[int(m.group(1))] = {int(x) for x in re.findall(r'\(reg(?:/\w+)?:\w+ (\d+)\)', chunk)
                                      if int(x) in info}
        m = re.match(r'\(insn\S* \d+ \d+ \d+ \(set \(reg(?:/\w+)?:\w+ (\d+)\)\s+(.*)', chunk, re.S)
        if m and int(m.group(1)) not in defs:
            defs[int(m.group(1))] = re.sub(r'\s+', ' ', m.group(2))[:96]

    def describe(r):
        d = info.get(r, {})
        return (f'r{r} pri={priority(d)} refs={d.get("refs")} live={d.get("live")} calls={d.get("calls")}'
                f' {"local" if d.get("local") else "global#" + str(order.get(r, "?"))}')

    seen, reported, swapped = set(), 0, []
    for i, (a, b) in enumerate(zip(mine_text, retail_text)):
        if a == b:
            continue
        near = mine_text[max(0, i - 8):i + 9]
        if b in near:
            # Retail's instruction exists nearby in the draft: an order decision, not allocation.
            swapped.append(i)
            continue
        oa, ra = operands(a)
        ob, rb = operands(b)
        if oa != ob or len(ra) != len(rb):
            continue
        for x, y in zip(ra, rb):
            hx, hy = regnum(x), regnum(y)
            if hx is None or hy is None or hx == hy:
                continue
            owners = sorted(r for r in insn_regs.get(uid_at[i], ()) if disp.get(r) == hx)
            key = (tuple(owners), hx, hy)
            if key in seen or not owners:
                continue
            seen.add(key)
            reported += 1
            if reported > args.max:
                break
            print(f'\n+0x{i * 4:03X} mine {a:34} retail {b}')
            for r in owners:
                d = info[r]
                print(f'  {describe(r)}: {regname(hx)}, retail {regname(hy)}')
                if r in defs:
                    print(f'    def {defs[r]}')
                if hy in CALL_CLOBBERED and d['calls']:
                    print(f'    {regname(hy)} is call-clobbered and r{r} crosses {d["calls"]} calls')
                blockers = sorted((z for z in conflicts.get(r, ()) if z != r and disp.get(z) == hy),
                                  key=lambda z: order.get(z, 1 << 30))
                for z in blockers:
                    dz = info.get(z, {})
                    print(f'    blocker {describe(z)} holds {regname(hy)}')
                    if z in defs:
                        print(f'      def {defs[z]}')
                    if order.get(z, 1 << 30) < order.get(r, 1 << 30) and dz:
                        lg = d['refs'].bit_length() - 1
                        need = priority(dz) + 1
                        live = lg * d['refs'] * 10000 // need if lg else 0
                        zlive = (dz['refs'].bit_length() - 1) * dz['refs'] * 10000 // max(priority(d), 1)
                        print(f'      to outrank it: r{r} live <= {live} at refs {d["refs"]},'
                              f' or r{z} live >= {zlive + 1}')
                if not blockers and not d['local']:
                    print(f'    no conflicting holder of {regname(hy)}: order, preference or reload decided')
        if reported > args.max:
            break
    if swapped:
        print('\norder-only differences (retail instruction present within 8 words): '
              + ' '.join(f'+0x{i * 4:03X}' for i in swapped[:16]) + ' -- see cc1probe --sched')
    print(f'\n{min(reported, args.max)} register decisions differ from retail; probe and dumps in {out}')


if __name__ == '__main__':
    main()
