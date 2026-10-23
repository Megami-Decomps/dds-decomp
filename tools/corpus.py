#!/usr/bin/env python3
"""Locating the extracted game data and script corpus.

src/<version>/data and src/<version>/scripts hold retail content, so they are
not tracked; docs/building.md covers importing them from a disc image you own.
Tests that read the corpus skip when it is absent instead of failing.
"""
from __future__ import annotations

import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def data_dir(version: str) -> Path:
    return ROOT / "src" / version / "data"


def script_dir(version: str) -> Path:
    return ROOT / "src" / version / "scripts"


def require(*paths: Path) -> None:
    """Skip the test unless every path has been extracted."""
    for path in paths:
        if not path.exists():
            raise unittest.SkipTest(f"corpus not extracted: {_label(path)}")


def require_sources(directory: Path, pattern: str) -> list[Path]:
    """Matching sources, skipping the test when the corpus holds none."""
    sources = sorted(directory.glob(pattern))
    if not sources:
        raise unittest.SkipTest(f"corpus not extracted: {_label(directory)}/{pattern}")
    return sources


def _label(path: Path) -> str:
    try:
        return str(path.relative_to(ROOT))
    except ValueError:
        return str(path)
