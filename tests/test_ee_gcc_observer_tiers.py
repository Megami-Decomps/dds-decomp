"""Comparator decoding tests using synthetic i386 state; no vendor execution."""
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
import _ee_gcc_observer as observer


NEW_HOOK_BYTES = {
    0x0814100c: '5589e583ec185756',
    0x081410f5: '39d8750b8b45ec29',
    0x08141106: '8d65dc5b5e5f89ec',
    0x0816c702: '833da41427080075',
    0x0816c723: '8b55fca1c89f2608',
    0x0816c784: '8b55e083c2108b04',
    0x0816c7d1: '8b4433048b5c1304',
    0x0816c7f0: '8b0de8e22408894d',
    0x0816ca55: '8b45fc8b155ce224',
    0x0816caae: '8b4df08d04b68d14',
}


class FakeReader:
    """Fail on unprepared reads; deliberately provides no execution method."""

    def __init__(self):
        self.memory = {}
        self.breakpoints = []
        self.reads = []

    def put(self, address, data):
        self.memory.update((address + index, byte) for index, byte in enumerate(data))

    def put_u32(self, address, value):
        self.put(address, struct.pack('<I', value & 0xffffffff))

    def mem(self, address, size):
        self.reads.append((address, size))
        return bytes(self.memory[address + index] for index in range(size))

    def u32(self, address):
        return struct.unpack('<I', self.mem(address, 4))[0]

    def ints(self, address, count):
        return list(struct.unpack('<' + 'i' * count, self.mem(address, count * 4)))

    def shorts(self, address, count):
        return list(struct.unpack('<' + 'h' * count, self.mem(address, count * 2)))

    def bp(self, address, enabled):
        self.breakpoints.append((address, enabled))


class DecisionTierTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.path = Path(self.temp.name) / 'events.jsonl'
        self.reader = FakeReader()
        self.tracer = observer.GlobalTracer(self.reader, self.path, 'synthetic_target')
        self.addCleanup(self.tracer.f.close)
        self.tracer.active = 'schedule_prereload'
        self.registers = {'esp': 0x1000, 'eax': 0, 'ebx': 0}
        self.reader.put_u32(0x1000, 0x08001000)
        self.reader.put_u32(136778916, 0)  # reload_completed
        self.reader.put_u32(136748988, 0)  # max_regno
        self.reader.put_u32(136767156, 0)  # reg_renumber
        self.reader.put_u32(136635112, 0)  # last_scheduled_insn
        self.reader.put_u32(136634972, 0x9000)  # scheduler instruction data
        self.reader.put_u32(136615404, 3)  # allocno count
        self.reader.put_u32(136615412, 0x3000)  # allocno array
        self.reader.put_u32(0x08269fb0, 0x4000)  # reg_info array
        for index, pseudo in enumerate((87, 198, 197)):
            self.reader.put(0x3000 + index * 100,
                            struct.pack('<5i', pseudo, 1, 3, 7 - index, 126 - index)
                            + bytes(80))
            self.reader.put_u32(0x4000 + 16 + 4 * pseudo, 0x5000 + index * 64)
            self.reader.put(0x5000 + index * 64 + 0x24, bytes([0]))
        for index, uid in enumerate((11, 22, 33, 44)):
            insn = bytearray(40)
            struct.pack_into('<i', insn, 4, uid)
            self.reader.put(0x6000 + index * 40, insn)
            self.reader.put(0x9000 + uid * 40,
                            struct.pack('<8i3hH', 0, 0, uid + 100, 2,
                                        0, 0, 1, 0, 1, 7, 1, 0))

    def events(self):
        return [json.loads(line) for line in self.path.read_text().splitlines()]

    def enter_rank(self, left=0, right=1):
        self.reader.put_u32(0x1004, 0x2000)
        self.reader.put_u32(0x1008, 0x2004)
        self.reader.put_u32(0x2000, 0x6000 + left * 40)
        self.reader.put_u32(0x2004, 0x6000 + right * 40)
        self.tracer.handle('rank_enter', self.registers)
        return self.events()[-1]['call_id']

    def enter_allocno(self, left=0, right=1):
        self.tracer.active = 'global_alloc'
        self.reader.put_u32(0x1004, 0x2100)
        self.reader.put_u32(0x1008, 0x2104)
        self.reader.put_u32(0x2100, left)
        self.reader.put_u32(0x2104, right)
        self.tracer.handle('allocno_compare_enter', self.registers)
        return self.events()[-1]['call_id']

    def test_ten_new_hooks_have_exact_eight_byte_guards(self):
        self.assertEqual(len(observer.HOOK_BYTES), 45)
        hooks = dict(observer.RANK_STAGE_HOOKS + observer.ALLOCNO_COMPARE_HOOKS)
        self.assertEqual(set(hooks), set(NEW_HOOK_BYTES))
        for address, expected in NEW_HOOK_BYTES.items():
            with self.subTest(address=hex(address)):
                self.assertEqual(observer.HOOK_BYTES[address], bytes.fromhex(expected))
                self.assertEqual(len(observer.HOOK_BYTES[address]), 8)

    def test_new_guards_fail_closed_then_cache_successful_verification(self):
        for address, expected in NEW_HOOK_BYTES.items():
            with self.subTest(address=hex(address)):
                self.reader.put(address, bytes(8))
                with self.assertRaisesRegex(RuntimeError, 'opcode mismatch'):
                    self.tracer.verify_hook(address)
                self.assertNotIn(address, self.tracer.checked_hooks)
                self.reader.put(address, bytes.fromhex(expected))
                self.tracer.verify_hook(address)
                reads = len(self.reader.reads)
                self.tracer.verify_hook(address)
                self.assertEqual(len(self.reader.reads), reads)
        self.assertEqual(self.events(), [])

    def test_rank_hooks_install_in_both_scheduler_passes_and_remove_at_exit(self):
        for phase in ('schedule_prereload', 'schedule_postreload'):
            with self.subTest(phase=phase):
                self.tracer.setup_pass(phase, self.registers)
                for address, name in observer.RANK_STAGE_HOOKS:
                    self.assertEqual(self.tracer.bps[address], name)
                for address, _ in observer.ALLOCNO_COMPARE_HOOKS:
                    self.assertNotIn(address, self.tracer.bps)
                self.tracer.handle('pass_exit', self.registers)
                self.assertEqual(self.tracer.bps, {})
                self.assertIsNone(self.tracer.active)

    def test_allocno_hooks_install_only_in_global_pass_and_remove_at_exit(self):
        self.tracer.setup_pass('global_alloc', self.registers)
        for address, name in observer.ALLOCNO_COMPARE_HOOKS:
            self.assertEqual(self.tracer.bps[address], name)
        for address, _ in observer.RANK_STAGE_HOOKS:
            self.assertNotIn(address, self.tracer.bps)
        self.tracer.handle('pass_exit', self.registers)
        self.assertEqual(self.tracer.bps, {})

    def test_rank_exit_maps_all_eight_tiers_and_signed_results(self):
        expected = ('priority', 'reg_weight', 'interblock_bb', 'spec_val',
                    'prob_val', 'last_insn_class', 'dependents', 'luid')
        for stage, tier in enumerate(expected):
            with self.subTest(stage=stage):
                call_id = self.enter_rank()
                if stage:
                    self.tracer.handle(f'rank_stage_{stage}', self.registers)
                self.tracer.handle('rank_exit', {'eax': 0xffffffda})
                self.assertEqual(self.events()[-1], {
                    'seq': self.tracer.seq, 'event': 'rank_exit',
                    'phase': 'schedule_prereload', 'call_id': call_id,
                    'result': -38, 'stage': stage, 'deciding_tier': tier,
                })
                self.assertEqual(self.tracer.rank, [])

    def test_rank_stage_records_actual_hits_and_preserves_furthest_stage(self):
        call_id = self.enter_rank()
        for stage in (1, 2, 5, 7, 6):
            self.tracer.handle(f'rank_stage_{stage}', self.registers)
        self.tracer.handle('rank_exit', {'eax': 0})
        stages = [event for event in self.events() if event['event'] == 'rank_stage']
        self.assertEqual([event['stage'] for event in stages], [1, 2, 5, 7, 6])
        for event in stages:
            self.assertEqual((event['call_id'], event['left_uid'], event['right_uid']),
                             (call_id, 11, 22))
        self.assertEqual(self.events()[-1]['stage'], 7)
        self.assertEqual(self.events()[-1]['deciding_tier'], 'luid')

    def test_nested_rank_calls_keep_distinct_pairs_stages_and_call_ids(self):
        outer = self.enter_rank()
        self.tracer.handle('rank_stage_2', self.registers)
        inner = self.enter_rank(2, 3)
        self.tracer.handle('rank_stage_7', self.registers)
        self.tracer.handle('rank_exit', {'eax': 0xffffffff})
        self.tracer.handle('rank_stage_5', self.registers)
        self.tracer.handle('rank_exit', {'eax': 1})
        self.assertNotEqual(outer, inner)
        stages = [event for event in self.events() if event['event'] == 'rank_stage']
        self.assertEqual([(event['call_id'], event['left_uid'], event['right_uid'])
                          for event in stages],
                         [(outer, 11, 22), (inner, 33, 44), (outer, 11, 22)])
        exits = [event for event in self.events() if event['event'] == 'rank_exit']
        self.assertEqual([(event['call_id'], event['stage']) for event in exits],
                         [(inner, 7), (outer, 5)])
        self.assertEqual(self.tracer.rank, [])

    def test_rank_enter_retains_existing_snapshot_and_event_sequence(self):
        call_id = self.enter_rank()
        event = self.events()[0]
        self.assertEqual(event['seq'], call_id)
        self.assertEqual(event['left']['uid'], 11)
        self.assertEqual(event['right']['uid'], 22)
        self.assertEqual(event['left']['luid'], 111)
        self.assertIsNone(event['last'])
        self.assertNotIn('stage', event)
        self.tracer.handle('rank_stage_1', self.registers)
        self.tracer.handle('rank_exit', {'eax': 2})
        self.assertEqual([event['seq'] for event in self.events()], [1, 2, 3])

    def test_orphan_comparator_events_fail_without_emitting_events(self):
        names = [name for _, name in observer.RANK_STAGE_HOOKS]
        names += ['rank_exit', 'allocno_compare_priorities', 'allocno_compare_exit']
        for name in names:
            with self.subTest(name=name), self.assertRaisesRegex(RuntimeError, 'without an active'):
                self.tracer.handle(name, self.registers)
        self.assertEqual(self.events(), [])

    def test_unknown_rank_stage_name_does_not_mutate_active_call(self):
        self.enter_rank()
        for name in ('rank_stage_0', 'rank_stage_8', 'rank_stage_bad'):
            with self.subTest(name=name), self.assertRaises(RuntimeError):
                self.tracer.handle(name, self.registers)
        self.assertNotIn('stage', self.tracer.rank[-1])
        self.assertEqual(len(self.events()), 1)

    def test_allocno_enter_decodes_pointer_arguments_and_range_copy_bit_only(self):
        self.reader.put(0x5000 + 0x24, bytes([0xfd]))
        self.reader.put(0x5040 + 0x24, bytes([0x02]))
        self.enter_allocno(1, 0)
        event = self.events()[-1]
        self.assertEqual([item['allocno'] for item in event['pair']], [1, 0])
        self.assertEqual([item['pseudo'] for item in event['pair']], [198, 87])
        self.assertEqual([item['range_copy'] for item in event['pair']], [1, 0])
        self.assertEqual(event['pair'][0]['live_length'], 125)
        self.assertEqual(event['pair'][0]['preferences'], [])
        self.assertIsNone(event['priorities'])
        self.assertEqual(event['call_id'], event['seq'])

    def test_allocno_range_copy_exit_preserves_null_priorities(self):
        self.reader.put(0x5000 + 0x24, bytes([2]))
        call_id = self.enter_allocno()
        self.tracer.handle('allocno_compare_exit', {'eax': 0xffffffff})
        event = self.events()[-1]
        self.assertEqual(event['call_id'], call_id)
        self.assertEqual(event['result'], -1)
        self.assertEqual(event['deciding_tier'], 'range_copy')
        self.assertIsNone(event['priorities'])
        self.assertEqual([item['range_copy'] for item in event['pair']], [1, 0])

    def test_allocno_priority_checkpoint_reads_signed_left_ebx_right_eax(self):
        call_id = self.enter_allocno()
        self.tracer.handle('allocno_compare_priorities',
                           {'ebx': 0x80000000, 'eax': 0xffffffff})
        checkpoint = self.events()[-1]
        self.assertEqual(checkpoint['call_id'], call_id)
        self.assertEqual(checkpoint['priorities'], [-2147483648, -1])
        self.tracer.handle('allocno_compare_exit', {'eax': 7})
        event = self.events()[-1]
        self.assertEqual(event['priorities'], [-2147483648, -1])
        self.assertEqual(event['deciding_tier'], 'priority')
        self.assertEqual(event['result'], 7)

    def test_equal_allocno_scores_use_number_tier_including_zero(self):
        for score in (0, 1111, 0xffffffff):
            with self.subTest(score=score):
                self.enter_allocno()
                self.tracer.handle('allocno_compare_priorities', {'ebx': score, 'eax': score})
                self.tracer.handle('allocno_compare_exit', {'eax': 0xffffffda})
                event = self.events()[-1]
                self.assertEqual(event['deciding_tier'], 'allocno_number')
                self.assertEqual(event['result'], -38)
                self.assertEqual(event['priorities'], [observer.signed(score)] * 2)

    def test_nested_allocno_calls_keep_pair_and_priority_association(self):
        outer = self.enter_allocno(0, 1)
        self.tracer.handle('allocno_compare_priorities', {'ebx': 1111, 'eax': 1111})
        inner = self.enter_allocno(2, 0)
        self.tracer.handle('allocno_compare_exit', {'eax': 1})
        self.tracer.handle('allocno_compare_exit', {'eax': 0xffffffff})
        exits = [event for event in self.events() if event['event'] == 'allocno_compare_exit']
        self.assertEqual([event['call_id'] for event in exits], [inner, outer])
        self.assertEqual([[item['allocno'] for item in event['pair']] for event in exits],
                         [[2, 0], [0, 1]])
        self.assertEqual([event['priorities'] for event in exits], [None, [1111, 1111]])
        self.assertEqual([event['deciding_tier'] for event in exits],
                         ['range_copy', 'allocno_number'])
        self.assertEqual(self.tracer.allocno_comparisons, [])

    def test_allocno_exit_retains_entry_snapshot_after_reader_state_changes(self):
        self.enter_allocno()
        pair = self.events()[-1]['pair']
        self.reader.put_u32(0x3000, 999)
        self.reader.put(0x5000 + 0x24, bytes([2]))
        self.tracer.handle('allocno_compare_exit', {'eax': 0})
        self.assertEqual(self.events()[-1]['pair'], pair)
        self.assertNotIn('range_copy', self.tracer.allocnos()[0])

    def test_scheduler_and_allocator_contexts_do_not_share_stacks(self):
        rank_call = self.enter_rank()
        allocno_call = self.enter_allocno()
        self.tracer.handle('allocno_compare_exit', {'eax': 1})
        self.assertEqual(self.tracer.rank[-1]['call_id'], rank_call)
        self.tracer.active = 'schedule_prereload'
        self.tracer.handle('rank_exit', {'eax': 0})
        self.assertEqual([event['call_id'] for event in self.events()
                          if event['event'].endswith('_exit')], [allocno_call, rank_call])
        self.assertEqual(self.tracer.rank, [])
        self.assertEqual(self.tracer.allocno_comparisons, [])


if __name__ == '__main__':
    unittest.main()
