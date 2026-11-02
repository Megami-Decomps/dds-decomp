#!/usr/bin/env python3
"""Rewrite sources as HEAD's text plus a rename map, dropping configure's spacing rewrites.

    python3 tools/sdf_restage.py build/sdf_rename_refs.tsv src/dds1/... [more]

Reports a file whose result differs from the working copy in more than blank lines.
"""
import re, subprocess, sys
ren = dict(l.split('\t') for l in open(sys.argv[1]).read().splitlines())
pat = re.compile(r'"(?:\\.|[^"\\\n])*"|\b(' + '|'.join(map(re.escape, ren)) + r')\b')
strip = lambda s: [l for l in s.splitlines() if l.strip()]
for f in sys.argv[2:]:
    head = subprocess.run(['git', 'show', 'HEAD:' + f], capture_output=True, text=True, check=True).stdout
    new = pat.sub(lambda m: ren[m[1]] if m[1] else m[0], head)
    cur = open(f).read()
    if strip(new) != strip(cur):
        print("DIFFERS", f)
    open(f, 'w').write(new)
