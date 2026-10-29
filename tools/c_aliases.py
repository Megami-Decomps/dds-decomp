"""C function identifiers that explicitly bind a different assembler name."""
import re


def function_aliases(source):
    return dict(re.findall(
        r'\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*(?:__asm__|asm)\s*\(\s*"([A-Za-z_]\w*)"\s*\)\s*;',
        source))
