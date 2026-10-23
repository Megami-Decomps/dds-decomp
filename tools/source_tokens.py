#!/usr/bin/env python3
"""Tokenize one line of a data tool's assembler source.

`shlex` is the reference, and for 90% of the corpus it has nothing to do: a
line with no quote, escape or comment character splits on whitespace, which is
50 times cheaper. Quoting of any kind still goes through shlex.
"""
from __future__ import annotations

import re
import shlex

QUOTING = re.compile(r"""["'\\#]""")


def split(line: str) -> list[str]:
    """shlex.split(line, comments=True, posix=True), fast when nothing is quoted."""
    if QUOTING.search(line):
        return shlex.split(line, comments=True, posix=True)
    return line.split()
