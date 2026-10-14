"""Validated non-owning aliases into reconstructed NOBITS objects."""

from dataclasses import dataclass
import json
import re


@dataclass(frozen=True)
class SymbolAlias:
    name: str
    owner: str
    unit: str
    section: str
    offset: int
    address: int


def load_symbol_aliases(root, version):
    """Resolve explicit owner-plus-offset aliases against native symbol facts."""
    path = root / "config" / version / "symbol_aliases.json"
    if not path.exists():
        return ()
    rows = json.loads(path.read_text())
    if not isinstance(rows, list):
        raise ValueError(f"{path}: expected a list of symbol aliases")
    symbols = {
        name: int(value, 16)
        for name, value in re.findall(
            r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;",
            (path.parent / "symbol_addrs.txt").read_text(), re.M)
    }
    aliases = []
    names = set()
    fields = {"name", "owner", "unit", "section", "offset"}
    for row in rows:
        if not isinstance(row, dict) or set(row) != fields:
            raise ValueError(f"{path}: alias fields must be {sorted(fields)}")
        name, owner = row["name"], row["owner"]
        if any(not isinstance(value, str) or not re.fullmatch(r"[A-Za-z_]\w*", value)
               for value in (name, owner)):
            raise ValueError(f"{path}: invalid alias or owner name")
        if name in names or name == owner:
            raise ValueError(f"{path}: duplicate or self alias {name}")
        names.add(name)
        unit, section, offset = row["unit"], row["section"], row["offset"]
        if not isinstance(unit, str) or not re.fullmatch(r"[A-Za-z0-9_/]+", unit) \
                or section not in (".bss", ".sbss") \
                or type(offset) is not int or offset < 0:
            raise ValueError(f"{path}: invalid NOBITS alias contract for {name}")
        owner_address = symbols.get(owner)
        native_address = symbols.get(name)
        if native_address is None and (match := re.fullmatch(r"D_([0-9A-F]{8})", name)):
            native_address = int(match[1], 16)
        if owner_address is None or native_address is None \
                or owner_address + offset != native_address:
            raise ValueError(f"{path}: {name} does not equal native {owner}+0x{offset:X}")
        aliases.append(SymbolAlias(name, owner, unit, section, offset, native_address))
    if any(alias.owner in names for alias in aliases):
        raise ValueError(f"{path}: aliases must refer directly to primary objects")
    return tuple(aliases)


def linker_aliases(text, aliases):
    """Replace absolute fallbacks with non-allocating owner-relative aliases."""
    begin = "/* source-owned symbol aliases */"
    end = "/* end source-owned symbol aliases */"
    text = re.sub(re.escape(begin) + r".*?" + re.escape(end) + r"\n?",
                  "", text, flags=re.S)
    for alias in aliases:
        text = re.sub(rf"^\s*PROVIDE\(\s*{re.escape(alias.name)}\s*=[^;]*;\s*\n?",
                      "", text, flags=re.M)
    if not aliases:
        return text
    rows = [begin, *(f"PROVIDE({alias.name} = {alias.owner} + 0x{alias.offset:X});"
                     for alias in aliases), end, ""]
    return "\n".join(rows) + text
