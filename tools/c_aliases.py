"""C function identifiers that bind a different assembler name."""
import re

ASM_LABEL = re.compile(
    r'\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*(?:__asm__|asm|FAKEMATCH_ASM_NAME)\s*\(\s*"([A-Za-z_]\w*)"\s*\)\s*;')
# A shared unit names a DDS function by SMT3's name: `#define sdf_2C0510 effMiscQuatLengthSqVU`.
MACRO_ALIAS = re.compile(r"^[ \t]*#[ \t]*define[ \t]+([A-Za-z_]\w*)[ \t]+([A-Za-z_]\w*)[ \t]*$", re.M)


def function_aliases(source):
    aliases = dict(MACRO_ALIAS.findall(source))
    aliases.update(ASM_LABEL.findall(source))
    return aliases
