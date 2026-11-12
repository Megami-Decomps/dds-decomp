"""Positive, site-specific evidence for named rodata address expressions.

No proximity inference: unsupported metadata/listings return no provenance.
The caller must separately prove the complete production span and object extent.
"""
import io
import re
import struct
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.common.exceptions import ELFError
from elftools.elf.sections import SymbolTableSection
from elftools.elf.relocation import RelocationSection


LW = re.compile(r'\s*lw\s+\$(\d+),([A-Za-z_]\w*)(?:([+-])(0[xX][0-9a-fA-F]+|[0-9]+))?\(\$(\d+)\)\s*')


def raw_metadata_clear(blob):
    """Reject duplicate/overlapping raw relocation sites before dictionary loss."""
    try:
        elf = ELFFile(io.BytesIO(blob))
        if elf.elfclass != 32 or not elf.little_endian or elf['e_type'] != 'ET_REL' or elf['e_machine'] != 'EM_MIPS':
            return False
        for sec in elf.iter_sections():
            if sec['sh_type'] != 'SHT_NOBITS' and not 0 <= sec['sh_offset'] <= len(blob) - sec['sh_size']:
                return False
        names = [s.name for s in elf.iter_sections()]
        if len(names) != len(set(names)) or names.count('.symtab') != 1:
            return False
        if len([s for s in elf.iter_sections() if isinstance(s, SymbolTableSection)]) != 1:
            return False
        symtab = elf.get_section_by_name('.symtab')
        if symtab['sh_entsize'] != 16 or symtab['sh_size'] % 16:
            return False
        ro = names.index('.rodata') if '.rodata' in names else None
        section_symbols = [s for s in symtab.iter_symbols()
                           if s['st_shndx'] == ro and s['st_info']['type'] == 'STT_SECTION']
        if ro is not None and (len(section_symbols) != 1 or
                               section_symbols[0]['st_value'] != 0 or section_symbols[0]['st_size'] != 0):
            return False
        seen = {}
        for sec in elf.iter_sections():
            if not isinstance(sec, RelocationSection):
                continue
            if sec['sh_link'] != names.index('.symtab'):
                return False
            # ee-as emits empty RELA companion sections even for REL-only
            # objects. They carry no addends; a nonempty RELA is unsupported.
            if sec['sh_type'] == 'SHT_RELA' and sec['sh_size'] == 0 and sec['sh_entsize'] == 12:
                continue
            if sec['sh_type'] != 'SHT_REL' or sec['sh_entsize'] != 8 or sec['sh_size'] % 8:
                return False
            offsets = seen.setdefault(sec['sh_info'], [])
            for r in sec.iter_relocations():
                if r['r_info_sym'] >= symtab.num_symbols():
                    return False
                off = r['r_offset']
                if off < 0 or off + 4 > elf.get_section(sec['sh_info'])['sh_size'] or any(
                        abs(off - old) < 4 for old in offsets):
                    return False
                offsets.append(off)
        return True
    except (ELFError, ValueError, TypeError, KeyError, IndexError, struct.error):
        return False


def listing_lines(source, listing):
    """Match every original source line exactly; never infer instruction sizes."""
    lines = source.splitlines()
    records = {}
    current = 0
    for row in listing.splitlines():
        if '\t' in row:
            lhs, rhs = row.split('\t', 1)
            m = re.fullmatch(r'\s*(\d+)\s+(?:([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s*)?', lhs)
            if not m:
                return None
            n = int(m[1])
            if n != current + 1 or n > len(lines) or rhs != lines[n - 1]:
                return None
            current = n
            records[n] = [(m[2], m[3])]
        else:
            m = re.fullmatch(r'\s*(\d+)\s+([0-9a-fA-F]+)\s*', row)
            if not m or int(m[1]) != current or not current:
                return None
            records[current].append((None, m[2]))
    return records if current == len(lines) else None


def lw_flow(words, rt, rs):
    """Only the verified LUI/ADDU/LW macro, no alternative expansion guesses."""
    if len(words) != 3 or not 0 < rt < 32 or not 0 < rs < 32 or rt == rs:
        return False
    return (words[0] & 0xffff0000 == (0x0f << 26) | (rt << 16)
            and words[1] == (rt << 21) | (rs << 16) | (rt << 11) | 0x21
            and words[2] & 0xffff0000 == (0x23 << 26) | (rt << 21) | (rt << 16))


def signed_target(words):
    lo = words[2] & 0xffff
    return ((words[0] & 0xffff) << 16) + (lo - 0x10000 if lo & 0x8000 else lo)


def named_lw_sites(source, listing, original, listed):
    """Return {LO16 site: named-expression fact}, only for identical whole ELFs."""
    if original != listed or not raw_metadata_clear(original):
        return {}
    records = listing_lines(source, listing)
    if records is None or re.search(
            r'^\s*\.(?:include|macro|endm|altmacro|purgem|exitm|mri|if\w*|else\w*|endif|rept|irp\w*|endr|incbin|pushsection|popsection|sdata|sbss|lit4|lit8|rodata)\b',
            source, re.M):
        return {}
    try:
        elf = ELFFile(io.BytesIO(original))
        sections = list(elf.iter_sections())
        names = [s.name for s in sections]
        text_index, ro_index = names.index('.text'), names.index('.rodata')
        text = sections[text_index].data()
        symtab = elf.get_section_by_name('.symtab')
        symbols = list(symtab.iter_symbols())
        rels = [r for sec in sections if isinstance(sec, RelocationSection)
                and sec['sh_info'] == text_index for r in sec.iter_relocations()]
        section, previous, function, inline = None, None, None, False
        result = {}
        for n, line in enumerate(source.splitlines(), 1):
            stripped = line.strip()
            if stripped == '#APP':
                inline = True
            elif stripped == '#NO_APP':
                inline = False
            sec = re.fullmatch(r'\.(text|data|rdata|bss)(?:\s+\d+)?', stripped)
            explicit = re.match(r'\.section\s+(\.[\w.]+)(?:,.*)?$', stripped)
            if sec or explicit:
                previous, section = section, ('.rodata' if sec and sec[1] == 'rdata'
                                             else '.' + sec[1] if sec else explicit[1])
            elif stripped == '.previous':
                section, previous = previous, section
            elif re.match(r'\.(?:section|subsection|text|data|rdata|bss)\b', stripped):
                return {}
            ent = re.fullmatch(r'\.ent\s+([A-Za-z_]\w*)', stripped)
            end = re.fullmatch(r'\.end\s+([A-Za-z_]\w*)', stripped)
            if ent:
                if function is not None:
                    return {}
                function = ent[1]
            if end:
                if function != end[1]:
                    return {}
                function = None
            m = LW.fullmatch(line)
            if not m or section != '.text' or not function or inline:
                continue
            rt, name, sign, number, rs = m.groups()
            rt, rs = int(rt), int(rs)
            addend = (int(number, 16 if number.lower().startswith('0x') else 10)
                      * (-1 if sign == '-' else 1)) if number else 0
            if not -(1 << 31) <= addend < (1 << 31):
                continue
            rows = records[n]
            if len(rows) != 3 or rows[0][0] is None or any(
                    addr is not None for addr, _ in rows[1:]) or any(
                    data is None or len(data) != 8 for _, data in rows):
                continue
            off = int(rows[0][0], 16)
            payload = bytes.fromhex(''.join(data for _, data in rows))
            if off % 4 or text[off:off + 12] != payload:
                continue
            words = struct.unpack('<III', payload)
            if not lw_flow(words, rt, rs):
                continue
            owners = [s for s in symbols if s['st_shndx'] == text_index
                      and s['st_info']['type'] == 'STT_FUNC'
                      and s['st_value'] < off + 12 and s['st_value'] + s['st_size'] > off]
            # Zero-sized competing function aliases cannot disappear from the
            # owner proof merely because they contain no bytes themselves.
            if len(owners) == 1:
                owner = owners[0]
                competitors = [s for s in symbols if s['st_info']['type'] == 'STT_FUNC' and
                               (s.name == owner.name or s['st_shndx'] == text_index and
                                owner['st_value'] <= s['st_value'] < owner['st_value'] + owner['st_size'])]
                if len(competitors) != 1:
                    continue
            objs = [s for s in symbols if s.name == name]
            if len(owners) != 1 or owners[0].name != function or not (
                    owners[0]['st_value'] <= off and off + 12 <= owners[0]['st_value'] + owners[0]['st_size']):
                continue
            if len(objs) != 1 or objs[0]['st_shndx'] != ro_index or \
                    objs[0]['st_info']['type'] != 'STT_OBJECT' or \
                    objs[0]['st_info']['bind'] != 'STB_GLOBAL' or objs[0]['st_size'] <= 0:
                continue
            site_rels = [r for r in rels if off - 3 <= r['r_offset'] < off + 12]
            if len(site_rels) != 2 or [(r['r_offset'], r['r_info_type']) for r in site_rels] != [
                    (off, 5), (off + 8, 6)] or site_rels[0]['r_info_sym'] != site_rels[1]['r_info_sym']:
                continue
            target = symbols[site_rels[0]['r_info_sym']]
            if target['st_info']['type'] != 'STT_SECTION' or target['st_shndx'] != ro_index:
                continue
            value = objs[0]['st_value'] + addend
            if not 0 <= value <= 0xffffffff or signed_target(words) != value:
                continue
            # No other source row may claim bytes overlapping this expansion in
            # .text. Section-local ambiguity is checked independently below.
            result[off + 8] = (name, objs[0]['st_value'], objs[0]['st_size'], addend,
                               function, off - owners[0]['st_value'], rt, rs)
        # Listing source is one-to-one, but an assembler could still assign
        # overlapping byte claims. Rewalk contexts, rejecting any such claim.
        section = previous = None
        claimed = set()
        for n, line in enumerate(source.splitlines(), 1):
            stripped = line.strip()
            sec = re.fullmatch(r'\.(text|data|rdata|bss)(?:\s+\d+)?', stripped)
            explicit = re.match(r'\.section\s+(\.[\w.]+)(?:,.*)?$', stripped)
            if sec or explicit:
                previous, section = section, ('.rodata' if sec and sec[1] == 'rdata'
                                             else '.' + sec[1] if sec else explicit[1])
            elif stripped == '.previous':
                section, previous = previous, section
            rows = records[n]
            byte_section = previous if sec or explicit or stripped == '.previous' else section
            if byte_section == '.text' and rows[0][0] is not None:
                start = int(rows[0][0], 16)
                data = ''.join(d or '' for _, d in rows)
                if len(data) % 2:
                    return {}
                span = set(range(start, start + len(data) // 2))
                if claimed & span:
                    result = {site: fact for site, fact in result.items()
                              if not (set(range(site - 8, site + 4)) & (claimed & span))}
                claimed.update(span)
        return result
    except (ELFError, ValueError, TypeError, KeyError, IndexError, struct.error):
        return {}


def native_site_agrees(fact, code, object_address):
    """Check corresponding native macro bytes/registers and exact addend."""
    _, _, _, addend, _, offset, rt, rs = fact
    if offset < 0 or offset + 12 > len(code) or object_address is None:
        return False
    words = struct.unpack_from('<III', code, offset)
    return lw_flow(words, rt, rs) and signed_target(words) == object_address + addend


def attribute_rodata_references(tables, bounds, objects_proven, sites, native_code):
    """Classify every site before storage dedup; only ownership-proven objects enter.

    Return normalized references/bounds, original genuine storage identities and
    explicit address failures. The caller still runs payload/PAD/MERGED checks.
    """
    errors = []
    def expression_object(site, name):
        fact = sites.get(site)
        if not fact or fact[4] != name:
            return None
        found = [o for o in objects_proven if o[0] == fact[1] and
                 o[1] - o[0] == fact[2] and o[3] == fact[0]]
        if len(found) != 1:
            return None
        return found[0]

    def expression_native(site, name, obj):
        fact = sites[site]
        if not native_site_agrees(fact, native_code(name), obj[2]):
            return False
        return True

    ordinary, expressions = [], []
    for offset, native, name, site in tables:
        obj = expression_object(site, name)
        if obj is not None and not expression_native(site, name, obj):
            errors.append(f"ADDRESS rodata of {name}: named expression has wrong native instruction/address")
            obj = None
        if obj is not None and not obj[0] <= offset < obj[1]:
            fact = sites[site]
            if offset == obj[0] + fact[3] and native == obj[2] + fact[3]:
                expressions.append((obj[0], obj[2], name))
                continue
        ordinary.append((offset, native, name))
    # MERGED applies to genuine storage references, not proven prebiased address
    # expressions. A genuine table at the same numeric offset remains ordinary.
    merged_uses = {}
    for offset, native, name in ordinary:
        merged_uses.setdefault(offset, {}).setdefault(native, name)
    tables = []
    for offset, native, name in ordinary:
        objects = [o for o in objects_proven if o[0] <= offset < o[1]]
        if len(objects) == 1:
            start, end, addr, symbol = objects[0]
            if native != addr + offset - start:
                errors.append(f"ADDRESS rodata of {name}: reference to {symbol} has wrong native offset")
            else:
                offset, native = start, addr
        tables.append((offset, native, name))
    tables.extend(expressions)
    normalized_bounds = []
    for offset, name, site in bounds:
        obj = expression_object(site, name)
        if obj is not None and expression_native(site, name, obj) and offset == obj[0] + sites[site][3]:
            offset = obj[0]
        else:
            objects = [o for o in objects_proven if o[0] <= offset < o[1]]
            if len(objects) == 1:
                offset = objects[0][0]
        normalized_bounds.append(offset)
    bounds = normalized_bounds
    return tables, bounds, merged_uses, errors


def load_named_lw_sites(source, original, listing_path):
    """An absent, partial or unreadable optional evidence pair grants nothing."""
    try:
        return named_lw_sites(source, Path(listing_path).read_text(), original,
                              Path(str(listing_path) + ".o").read_bytes())
    except (OSError, UnicodeError):
        return {}
