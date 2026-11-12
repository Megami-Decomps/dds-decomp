#!/usr/bin/env python3
"""Load the per-module config in config/irx/<module>/<module>.yaml."""
from __future__ import annotations

from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / "config" / "irx"


def modules() -> list[str]:
    """Every module that has a config directory, in a stable order."""
    out = []
    if not CONFIG.is_dir():
        return out
    for path in sorted(CONFIG.iterdir()):
        if path.is_dir() and (path / f"{path.name}.yaml").exists():
            out.append(path.name)
    return out


def load(module: str) -> dict:
    # Pure config-path helpers and mocked checker tests need no YAML parser.
    import yaml

    cfg = yaml.safe_load((CONFIG / module / f"{module}.yaml").read_text())
    cfg["module_dir"] = module
    for key in ("name", "sha1", "module", "paths"):
        if key not in cfg:
            raise SystemExit(f"config/irx/{module}/{module}.yaml: missing {key!r}")
    return cfg


def path(cfg: dict, key: str) -> Path:
    """A path from the config, relative to the repository root."""
    return ROOT / cfg["paths"][key]
