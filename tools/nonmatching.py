"""Select the normal build's source for text scans, retaining source offsets."""
import re


def matching_source(text: str) -> str:
    """Blank NON_MATCHING branches and their directives; keep fallback code.

    Other conditionals are left for the real preprocessor. Tracking their
    nesting prevents an inner #else from ending a NON_MATCHING branch.
    Whitespace preserves offsets used by the data-placement tools.
    """
    stack = []
    out = []
    for line in text.splitlines(keepends=True):
        directive = re.match(r"\s*#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)", line)
        hide = any(target and first for target, first in stack)
        control = False
        if directive:
            kind, argument = directive.groups()
            if kind in ("if", "ifdef", "ifndef"):
                target = kind == "ifdef" and argument.strip() == "NON_MATCHING"
                stack.append((target, True))
                control = target
            elif stack:
                target, first = stack[-1]
                control = target
                if kind == "else":
                    stack[-1] = (target, False)
                elif kind == "endif":
                    stack.pop()
        out.append(re.sub(r"[^\r\n]", " ", line) if hide or control else line)
    return "".join(out)
