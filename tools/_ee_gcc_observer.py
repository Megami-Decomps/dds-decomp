"""Pinned i386 EE GCC observers, shared by ee_gcc_observe.py.

The decision hooks and field decoders are retained from the qualified observer.
They are valid only for the compiler SHA-256 guarded by the public runner.
GCC vendor rtunion fields occupy eight bytes; this is not host pointer width.
"""
import json
import struct

# First eight text bytes at every static hook; verified on its first trap.
HOOK_BYTES = {
    0x08075764: bytes.fromhex('5589e583ec285756'),
    0x08098840: bytes.fromhex('5589e583ec045756'),
    0x08098e10: bytes.fromhex('5589e583ec085756'),
    0x080c3650: bytes.fromhex('5589e581ecc80000'),
    0x080c38a2: bytes.fromhex('8b75148b55108955'),
    0x080c38ae: bytes.fromhex('837d20040f84e701'),
    0x080c5806: bytes.fromhex('8da52cffffff5b5e'),
    0x0813d048: bytes.fromhex('5589e583ec105756'),
    0x0813e96c: bytes.fromhex('5589e583ec185756'),
    0x0813ea52: bytes.fromhex('8d65dc5b5e5f89ec'),
    0x0813eccc: bytes.fromhex('5589e583ec645756'),
    0x0813f1e7: bytes.fromhex('85c00f84f7000000'),
    0x0813f214: bytes.fromhex('85c00f84ca000000'),
    0x0813f2f5: bytes.fromhex('8d65905b5e5f89ec'),
    0x0813f614: bytes.fromhex('5589e583ec5c5756'),
    0x0813f930: bytes.fromhex('8b45e08b55e48b5d'),
    0x0813f9ed: bytes.fromhex('a8010f8526010000'),
    0x0813fbfd: bytes.fromhex('8d65985b5e5f89ec'),
    0x08140060: bytes.fromhex('5589e583ec0c5756'),
    0x08141e54: bytes.fromhex('5589e583ec305756'),
    0x081420c4: bytes.fromhex('8d65c45b5e5f89ec'),
    0x081420d0: bytes.fromhex('5589e581ec800000'),
    0x081424ce: bytes.fromhex('8da574ffffff5b5e'),
    0x081424dc: bytes.fromhex('5589e581ec240100'),
    0x0814296e: bytes.fromhex('a8010f852d010000'),
    0x08142bb3: bytes.fromhex('a8010f84eb010000'),
    0x08142ea3: bytes.fromhex('a8010f8401020000'),
    0x08143517: bytes.fromhex('66890c428b15fc95'),
    0x08143846: bytes.fromhex('8da5d0feffff5b5e'),
    0x08144144: bytes.fromhex('5589e583ec345756'),
    0x0814461b: bytes.fromhex('5b5e5f89ec5dc389'),
    0x0816c6ac: bytes.fromhex('5589e583ec205756'),
    0x0816cac1: bytes.fromhex('8d65d45b5e5f89ec'),
    0x0816cacc: bytes.fromhex('5589e583ec2c5756'),
    0x0817160c: bytes.fromhex('5589e583ec405756'),
}

def signed(v):
    return v if v < 2147483648 else v - 4294967296

class Tracer:

    def __init__(self, r, output, function):
        self.r = r
        self.f = open(output, 'w')
        self.function = function
        self.bps = {}
        self.active = None
        self.counter = 0
        self.ffr = []
        self.compare = []
        self.rank = []
        self.combines = []
        self.seq = 0
        self.static = {}
        self.pass_return = None
        self.checked_hooks = set()

    def log(self, event, **kw):
        self.seq += 1
        data = {'seq': self.seq, 'event': event, 'phase': self.active, **kw}
        self.f.write(json.dumps(data, separators=(',', ':')) + '\n')
        self.f.flush()

    def bp(self, addr, name):
        expected = HOOK_BYTES.get(addr)
        if name != 'pass_exit' and expected is None:
            raise RuntimeError(f'Unknown static hook: {addr:x}')
        if addr not in self.bps:
            self.r.bp(addr, True)
        self.bps[addr] = name

    def verify_hook(self, addr):
        # Initial QEMU stop is in the loader; cc1 text is mapped only later.
        expected = HOOK_BYTES.get(addr)
        if expected is not None and addr not in self.checked_hooks:
            if self.r.mem(addr, len(expected)) != expected:
                raise RuntimeError(f'Pinned hook opcode mismatch at {addr:x}')
            self.checked_hooks.add(addr)

    def remove(self, addr):
        if addr in self.bps:
            self.r.bp(addr, False)
            del self.bps[addr]

    def fn(self):
        d = self.r.u32(136656572)
        if not d:
            return ''
        n = self.r.u32(d + 48)
        return self.r.cstr(self.r.u32(n + 16)) if n else ''

    def args(self, r, n, frame=False):
        return self.r.ints(r['ebp'] + 8 if frame else r['esp'] + 4, n)

    def mode(self, n):
        k = ('mode', n)
        if k not in self.static:
            self.static[k] = self.r.cstr(self.r.u32(136369952 + n * 4))
        return self.static[k]

    def qty(self, n):
        p = self.r.u32(136615248) + 40 * n
        b = self.r.mem(p, 40)
        v = struct.unpack('<9ihBB', b)
        out = dict(zip(['n_refs', 'birth', 'death', 'size', 'calls_crossed', 'first_reg', 'min_class', 'alternate_class', 'mode', 'phys_reg', 'changes_mode', 'pad'], v))
        out.pop('pad')
        out['qty'] = n
        out['mode_name'] = self.mode(out['mode'])
        ptr = self.r.u32(136615268)
        regs = []
        reg = out['first_reg']
        while 0 < reg < self.r.u32(136748988) and reg not in regs and (len(regs) < 100):
            regs.append(reg)
            reg = signed(self.r.u32(ptr + 4 * reg))
        out['pseudos'] = regs
        for sym, label in [(136615252, 'copy_suggestions'), (136615256, 'suggestions')]:
            addr = self.r.u32(sym) + n * 16
            out[label] = self.bitset(addr)
        return out

    def bitset(self, addr):
        b = self.r.mem(addr, 16)
        return [i for i in range(128) if b[i // 8] & 1 << i % 8]

    def allocation(self):
        count = self.r.u32(136748988)
        ptr = self.r.u32(136767156)
        return dict(enumerate(self.r.shorts(ptr, count))) if ptr else {}

    def insn(self, p):
        if not p:
            return None
        b = self.r.mem(p, 40)
        uid = struct.unpack_from('<i', b, 4)[0]
        out = {'ptr': hex(p), 'uid': uid, 'pattern': self.rtx(self.r.u32(p + 28), 3)}
        h = self.r.u32(136634972)
        if h and 0 <= uid < 100000:
            data = self.r.mem(h + 40 * uid, 40)
            v = struct.unpack('<8i3hH', data)
            out.update(dict(zip(['depend', 'line_note', 'luid', 'priority', 'dep_count', 'blockage', 'ref_count', 'tick', 'cost', 'units', 'reg_weight', 'flags'], v)))
        return out

    def rtx(self, p, depth=3):
        if not p:
            return None
        b = self.r.mem(p, 4)
        code, mode, flags = struct.unpack('<HBB', b)
        if code >= 256:
            return {'invalid': hex(p), 'code': code}
        key = ('rtx', code)
        if key not in self.static:
            self.static[key] = (self.r.cstr(self.r.u32(136367760 + 4 * code)), self.r.cstr(self.r.u32(136371812 + 4 * code)))
        name, fmt = self.static[key]
        out = {'code': name, 'mode': self.mode(mode)}
        if name == 'reg':
            out['regno'] = self.r.u32(p + 4)
            return out
        if name == 'const_int':
            out['value'] = struct.unpack('<q', self.r.mem(p + 4, 8))[0]
            return out
        if depth <= 0:
            return out
        fields = []
        for i, ch in enumerate(fmt):
            a = p + 4 + 8 * i
            if ch in 'eu':
                fields.append(self.rtx(self.r.u32(a), depth - 1))
            elif ch in 'isw':
                fields.append(self.r.ints(a, 1)[0] if ch == 'i' else self.r.cstr(self.r.u32(a)) if ch == 's' else struct.unpack('<q', self.r.mem(a, 8))[0])
            elif ch == 'E':
                v = self.r.u32(a)
                n = self.r.u32(v) if v else 0
                fields.append([self.rtx(self.r.u32(v + 4 + 4 * j), depth - 1) for j in range(min(n, 12))])
            elif ch == '0':
                fields.append(None)
        out['fields'] = fields
        return out

    def setup_pass(self, phase, r):
        self.active = phase
        ret = self.r.u32(r['esp'])
        self.pass_return = ret
        self.bp(ret, 'pass_exit')
        self.log('pass_enter', function=self.function, return_pc=hex(ret), reload_completed=self.r.u32(136778916))
        if phase.startswith('schedule'):
            for a, n in [(135710380, 'rank_enter'), (135711425, 'rank_exit'), (135711436, 'schedule_insn')]:
                self.bp(a, n)
        elif phase == 'local_alloc':
            for a, n in [(135525908, 'free_enter'), (135526704, 'free_masks'), (135526893, 'free_candidate'), (135527421, 'free_exit'), (135522668, 'compare_enter'), (135522898, 'compare_exit'), (135523532, 'combine_enter'), (135525109, 'combine_exit'), (135524839, 'combine_death_note'), (135524884, 'combine_class')]:
                self.bp(a, n)
            self.log('allocation_order', registers=self.r.ints(136569952, 79))
        elif phase == 'global_alloc':
            self.log('allocation_before_global', reg_renumber=self.allocation())

    def handle(self, name, r):
        if name in ('schedule_insns', 'local_alloc', 'global_alloc'):
            fn = self.fn()
            if fn == self.function:
                phase = name
                if name == 'schedule_insns':
                    phase = 'schedule_postreload' if self.r.u32(136778916) else 'schedule_prereload'
                self.setup_pass(phase, r)
            return
        if name == 'pass_exit':
            self.log('pass_exit', reg_renumber=self.allocation() if self.active in ('local_alloc', 'global_alloc') else None)
            for a in list(self.bps):
                if self.bps[a] not in ('schedule_insns', 'local_alloc', 'global_alloc'):
                    self.remove(a)
            self.active = None
            return
        if name == 'free_enter':
            a = self.args(r, 7)
            ctx = dict(zip(['class', 'mode', 'qtyno', 'accept_call_clobbered', 'just_try_suggested', 'born_index', 'dead_index'], a))
            ctx['quantity'] = self.qty(a[2])
            ctx['call_id'] = self.seq + 1
            self.ffr.append(ctx)
            self.log('find_free_reg_enter', **ctx)
            return
        if name == 'free_masks':
            ctx = self.ffr[-1]
            ptr = self.r.u32(136615296)
            live = {str(reg): [i for i in range(ctx['born_index'], ctx['dead_index']) if reg in self.bitset(ptr + i * 16)] for reg in (2, 3)}
            self.log('find_free_reg_masks', call_id=self.ffr[-1]['call_id'], used=self.bitset(r['ebp'] - 16), first_used=self.bitset(r['ebp'] - 32), gpr_live_indices=live)
            return
        if name == 'free_candidate':
            self.log('find_free_reg_candidate', call_id=self.ffr[-1]['call_id'], candidate=r['edi'], allocation_index=signed(self.r.u32(r['ebp'] - 36)), blocked=bool(r['eax'] & 1))
            return
        if name == 'free_exit':
            ctx = self.ffr.pop()
            self.log('find_free_reg_exit', call_id=ctx['call_id'], result=signed(r['eax']), qtyno=ctx['qtyno'], pseudos=ctx['quantity']['pseudos'])
            return
        if name == 'combine_enter':
            a = self.args(r, 6)
            used = self.rtx(a[0])
            dest = self.rtx(a[1])
            ctx = {'used': used, 'set': dest, 'may_save_copy': a[2], 'insn_number': a[3], 'insn_uid': self.r.u32(a[4] + 4), 'already_dead': a[5], 'call_id': self.seq + 1}
            self.combines.append(ctx)
            self.log('combine_regs_enter', **ctx)
            return
        if name in ('combine_death_note', 'combine_class'):
            self.log(name, call_id=self.combines[-1]['call_id'], result=r['eax'], used_reg=signed(self.r.u32(r['ebp'] - 8)), set_reg=signed(self.r.u32(r['ebp'] - 12)))
            return
        if name == 'combine_exit':
            ctx = self.combines.pop()
            data = {'call_id': ctx['call_id'], 'result': signed(r['eax'])}
            dest = ctx['set']
            if r['eax'] and dest.get('code') == 'reg':
                q = signed(self.r.u32(self.r.u32(136615272) + 4 * dest['regno']))
                if q >= 0:
                    data['quantity_after'] = self.qty(q)
                    data['quantity_after'].pop('phys_reg', None)
            self.log('combine_regs_exit', **data)
            return
        if name == 'compare_enter':
            a = self.args(r, 2)
            q = [self.r.u32(x) for x in a]
            ctx = {'left': self.qty(q[0]), 'right': self.qty(q[1]), 'call_id': self.seq + 1}
            self.compare.append(ctx)
            self.log('qty_compare_enter', **ctx)
            return
        if name == 'compare_exit':
            ctx = self.compare.pop()
            self.log('qty_compare_exit', call_id=ctx['call_id'], result=signed(r['eax']), cost_difference=signed(r['ecx']))
            return
        if name == 'rank_enter':
            a = self.args(r, 2)
            ctx = {'left': self.insn(self.r.u32(a[0])), 'right': self.insn(self.r.u32(a[1])), 'last': self.insn(self.r.u32(136635112)), 'call_id': self.seq + 1}
            self.rank.append(ctx)
            self.log('rank_enter', **ctx)
            return
        if name == 'rank_exit':
            ctx = self.rank.pop()
            self.log('rank_exit', call_id=ctx['call_id'], result=signed(r['eax']))
            return
        if name == 'schedule_insn':
            a = self.args(r, 4)
            self.log('schedule_insn', selected=self.insn(a[0]), clock=a[3], ready=[self.insn(x) for x in self.r.ints(a[1], a[2])] if a[2] > 0 else [])
            return
        raise RuntimeError(name)

    def run(self):
        self.r.cmd('qSupported')
        self.r.cmd('?')
        for a, n in [(135730700, 'schedule_insns'), (135516232, 'local_alloc'), (135528544, 'global_alloc')]:
            self.bp(a, n)
        while True:
            stop = self.r.cmd('c')
            if stop.startswith(('W', 'X')):
                self.log('exit', status=stop)
                break
            if not stop.startswith(('T05', 'S05')):
                raise RuntimeError(f'Unexpected stop {stop}')
            regs = self.r.regs()
            pc = regs['eip']
            name = self.bps.get(pc)
            if not name:
                raise RuntimeError(f'Unexpected trap at {pc:x}: {stop}')
            self.verify_hook(pc)
            self.handle(name, regs)
            retained = self.bps.get(pc)
            if retained:
                self.r.bp(pc, False)
            step = self.r.cmd('s')
            if not step.startswith(('T05', 'S05')):
                raise RuntimeError(step)
            if retained:
                self.r.bp(pc, True)
        self.f.close()

def bits(raw):
    return [i for i in range(128) if raw[i // 8] & 1 << i % 8]

class GlobalTracer(Tracer):

    def __init__(self, *args, **kw):
        super().__init__(*args, **kw)
        self.global_calls = []
        self.pref_calls = []
        self.transforms = []

    def allocnos(self):
        n = self.r.u32(136615404)
        p = self.r.u32(136615412)
        if not n < 10000:
            raise RuntimeError('Pinned compiler state invariant failed')
        raw = self.r.mem(p, n * 100)
        result = {}
        for i in range(n):
            b = raw[i * 100:(i + 1) * 100]
            d = dict(zip(['pseudo', 'size', 'calls_crossed', 'n_refs', 'live_length'], struct.unpack('<5i', b[:20])))
            d['allocno'] = i
            for off, name in [(20, 'hard_conflicts'), (36, 'preferences'), (52, 'copy_preferences'), (68, 'full_preferences'), (84, 'someone_prefers')]:
                d[name] = bits(b[off:off + 16])
            result[i] = d
        return result

    def setup_pass(self, phase, r):
        super().setup_pass(phase, r)
        if phase == 'global_alloc':
            hooks = [(135537884, 'global_find_enter'), (135539054, 'global_candidate'), (135539635, 'global_copy_preference_candidate'), (135540387, 'global_preference_candidate'), (135542039, 'global_commit'), (135542854, 'global_find_exit'), (135545156, 'preference_enter'), (135546395, 'preference_exit'), (135536212, 'expand_preferences_enter'), (135536836, 'expand_preferences_exit'), (135536848, 'prune_preferences_enter'), (135537870, 'prune_preferences_exit')]
            for a, n in hooks:
                self.bp(a, n)

    def handle(self, name, r):
        if name == 'global_find_enter':
            a = self.args(r, 5)
            snap = self.allocnos()
            ctx = dict(zip(['num', 'losers_ptr', 'alt_regs_p', 'accept_call_clobbered', 'retrying'], a))
            ctx['allocno'] = snap[a[0]]
            ctx['call_id'] = self.seq + 1
            self.global_calls.append(ctx)
            self.log(name, **ctx)
            return
        if name == 'global_candidate':
            ctx = self.global_calls[-1]
            mode = self.r.u32(r['ebp'] - 100)
            reg = r['edi']
            self.log(name, call_id=ctx['call_id'], pseudo=ctx['allocno']['pseudo'], candidate=reg, blocked_by_used_mask=bool(r['eax'] & 1), mode_eligible=bool(self.r.mem(136779616 + 79 * mode + reg, 1)[0]), search_pass=self.r.u32(r['ebp'] - 212), used=self.bitset(r['ebp'] - 16))
            return
        if name in ('global_copy_preference_candidate', 'global_preference_candidate'):
            if r['eax'] & 1:
                self.log(name, call_id=self.global_calls[-1]['call_id'], candidate=self.r.u32(r['ebp'] - 88), fallback_best=signed(self.r.u32(r['ebp'] - 92)))
            return
        if name == 'global_commit':
            self.log(name, call_id=self.global_calls[-1]['call_id'], pseudo=r['eax'], hard_register=r['ecx'] & 65535)
            return
        if name == 'global_find_exit':
            ctx = self.global_calls.pop()
            pseudo = ctx['allocno']['pseudo']
            home = self.r.shorts(self.r.u32(136767156) + 2 * pseudo, 1)[0]
            self.log(name, call_id=ctx['call_id'], pseudo=pseudo, hard_register=home, allocno_after=self.allocnos()[ctx['num']])
            return
        if name == 'preference_enter':
            a = self.args(r, 2)
            ctx = {'before': self.allocnos(), 'call_id': self.seq + 1, 'dest': self.rtx(a[0]), 'src': self.rtx(a[1])}
            self.pref_calls.append(ctx)
            self.log(name, call_id=ctx['call_id'], dest=ctx['dest'], src=ctx['src'])
            return
        if name == 'preference_exit':
            ctx = self.pref_calls.pop()
            after = self.allocnos()
            changes = [{'before': ctx['before'][k], 'after': v} for k, v in after.items() if v != ctx['before'][k]]
            self.log(name, call_id=ctx['call_id'], changes=changes)
            return
        if name in ('expand_preferences_enter', 'prune_preferences_enter'):
            ctx = {'before': self.allocnos(), 'call_id': self.seq + 1, 'kind': name}
            self.transforms.append(ctx)
            self.log(name, call_id=ctx['call_id'], allocnos=list(ctx['before'].values()))
            return
        if name in ('expand_preferences_exit', 'prune_preferences_exit'):
            ctx = self.transforms.pop()
            after = self.allocnos()
            changes = [{'before': ctx['before'][k], 'after': v} for k, v in after.items() if v != ctx['before'][k]]
            self.log(name, call_id=ctx['call_id'], changes=changes)
            return
        if name == 'free_masks':
            ctx = self.ffr[-1]
            ptr = self.r.u32(136615296)
            live = {str(reg): [i for i in range(ctx['born_index'], ctx['dead_index']) if reg in self.bitset(ptr + i * 16)] for reg in (32, 33, 34, 44)}
            self.log('fpr_live_intervals', call_id=ctx['call_id'], pseudos=ctx['quantity']['pseudos'], hard_register_live_indices=live)
        return super().handle(name, r)

class GatedGlobalTracer(GlobalTracer):

    def __init__(self, *args, **kw):
        super().__init__(*args, **kw)
        self.found = False
        self.completed = False

    def handle(self, name, r):
        if name == 'target_compilation_gate':
            if self.fn() == self.function:
                self.found = True
                self.remove(134698852)
                for a, n in [(135730700, 'schedule_insns'), (135516232, 'local_alloc'), (135528544, 'global_alloc')]:
                    self.bp(a, n)
            return
        done = name == 'pass_exit' and self.active == 'schedule_postreload'
        super().handle(name, r)
        if done:
            for a in list(self.bps):
                self.remove(a)
            self.completed = True

    def run(self):
        self.r.cmd('qSupported')
        self.r.cmd('?')
        self.bp(134698852, 'target_compilation_gate')
        while True:
            stop = self.r.cmd('c')
            if stop.startswith(('W', 'X')):
                self.log('exit', status=stop)
                break
            if not stop.startswith(('T05', 'S05')):
                raise RuntimeError(stop)
            regs = self.r.regs()
            pc = regs['eip']
            name = self.bps.get(pc)
            if not name:
                raise RuntimeError((hex(pc), stop))
            self.verify_hook(pc)
            self.handle(name, regs)
            retained = self.bps.get(pc)
            if retained:
                self.r.bp(pc, False)
            step = self.r.cmd('s')
            if not step.startswith(('T05', 'S05')):
                raise RuntimeError(step)
            if retained:
                self.r.bp(pc, True)
        self.f.close()
        if not (self.found and self.completed):
            raise RuntimeError('Pinned compiler state invariant failed')

class OperandTracer(Tracer):

    def __init__(self, *args, **kw):
        super().__init__(*args, **kw)
        self.binops = []
        self.finished = False

    def binop(self, r, frame=False):
        a = self.args(r, 7, frame)
        code = self.r.u32(a[1])
        return {'mode': self.mode(a[0]), 'operation': self.r.cstr(self.r.u32(136367760 + 4 * code)), 'op0': self.rtx(a[2]), 'op1': self.rtx(a[3]), 'target': self.rtx(a[4]), 'op0_ptr': hex(a[2]), 'op1_ptr': hex(a[3]), 'target_ptr': hex(a[4]), 'target_is_op0': a[4] == a[2], 'target_is_op1': a[4] == a[3], 'unsignedp': a[5], 'methods': a[6]}

    def handle(self, name, r):
        if name == 'expand_function_start':
            if self.fn() == self.function:
                self.active = 'expansion'
                self.log('expansion_enter', function=self.function)
                for a, n in [(134843920, 'expand_function_end'), (135018064, 'binop_enter'), (135018658, 'binop_swap'), (135018670, 'binop_canonicalized'), (135026694, 'binop_exit')]:
                    self.bp(a, n)
            return
        if name == 'expand_function_end':
            if not not self.binops:
                raise RuntimeError('Pinned compiler state invariant failed')
            self.log('expansion_exit', function=self.function)
            for a in list(self.bps):
                self.remove(a)
            self.finished = True
            self.active = None
            return
        if name == 'binop_enter':
            a = self.args(r, 7)
            record = self.mode(a[0]) == 'SF'
            ctx = {'record': record, 'call_id': self.seq + 1}
            self.binops.append(ctx)
            if record:
                self.log(name, call_id=ctx['call_id'], **self.binop(r))
            return
        if name in ('binop_swap', 'binop_canonicalized'):
            if self.binops[-1]['record']:
                self.log(name, call_id=self.binops[-1]['call_id'], **self.binop(r, True))
            return
        if name == 'binop_exit':
            ctx = self.binops.pop()
            if ctx['record']:
                self.log(name, call_id=ctx['call_id'], result=self.rtx(r['eax']))
            return
        raise RuntimeError(name)

    def run(self):
        self.r.cmd('qSupported')
        self.r.cmd('?')
        self.bp(134842432, 'expand_function_start')
        while True:
            stop = self.r.cmd('c')
            if stop.startswith(('W', 'X')):
                self.log('exit', status=stop)
                break
            if not stop.startswith(('T05', 'S05')):
                raise RuntimeError(stop)
            regs = self.r.regs()
            pc = regs['eip']
            name = self.bps.get(pc)
            if not name:
                raise RuntimeError((hex(pc), stop))
            self.verify_hook(pc)
            self.handle(name, regs)
            retained = self.bps.get(pc)
            if retained:
                self.r.bp(pc, False)
            step = self.r.cmd('s')
            if not step.startswith(('T05', 'S05')):
                raise RuntimeError(step)
            if retained:
                self.r.bp(pc, True)
        self.f.close()
        if not self.finished:
            raise RuntimeError('Pinned compiler state invariant failed')
