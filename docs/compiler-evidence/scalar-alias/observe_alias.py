"""Read-only gen_mem_addressof alias initialization observation; requires output parity."""
import sys, struct
from pathlib import Path
sys.path.insert(0, str(Path.cwd() / 'tools'))
import ee_gcc_observe as runner
import _ee_gcc_observer as obs
hooks = {134823336: '5589e55756538b5d', 134823419: '66c7034a00897304', 134823513: '57e80d9105008943', 134823519: '89430c89531083c4', 134823525: '83c404f6470a0175', 135188643: '8b50108b400ce9b4'}
for a, h in hooks.items():
    obs.HOOK_BYTES[a] = bytes.fromhex(h)

class AliasTracer(obs.GatedGlobalTracer):

    def bp(self, a, name):
        if name == 'target_compilation_gate':
            a = 134823336
        return super().bp(a, name)

    def raw(self, p):
        return self.r.mem(p, 20).hex()

    def alias(self, p):
        return struct.unpack('<q', self.r.mem(p + 12, 8))[0]

    def handle(self, name, r):
        if name == 'target_compilation_gate':
            if self.fn() != self.function:
                return
            self.found = True
            self.active = 'expansion'
            self.remove(134823336)
            node, decl = self.args(r, 2)
            self.node = node
            self.decl = decl
            self.log('gen_mem_addressof_enter', node=hex(node), decl=hex(decl), rtl=self.rtx(node, 2), raw=self.raw(node), alias_storage_before=self.alias(node))
            for a, n in [(134823419, 'rewrite_reg_to_mem'), (134823513, 'before_get_alias'), (134823519, 'after_get_alias'), (134823525, 'after_alias_store'), (135188643, 'get_alias_read_decl_rtl'), (134698852, 'expansion_complete')]:
                self.bp(a, n)
            return
        if name == 'expansion_complete':
            self.log(name, node=hex(self.node), raw=self.raw(self.node))
            for a in list(self.bps):
                self.remove(a)
            self.completed = True
            return
        if name == 'get_alias_read_decl_rtl':
            self.log(name, node=hex(r['eax']), same_node=r['eax'] == self.node, raw=self.raw(r['eax']), alias_value=self.alias(r['eax']))
            return
        self.log(name, node=hex(r['ebx']), raw=self.raw(r['ebx']), rtl=self.rtx(r['ebx'], 2), alias_storage=self.alias(r['ebx']), return_low=r['eax'], return_high=r['edx'])
runner.GatedGlobalTracer = AliasTracer
if __name__ == '__main__':
    runner.main()
