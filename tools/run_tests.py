#!/usr/bin/env python3
"""Run the script and data tooling tests across every core.

    python tools/run_tests.py                 # every module
    python tools/run_tests.py test_fld        # one module

Each module's tests are dealt round-robin into small batches, and every batch
runs as its own `python tools/test_*.py Class.method ...` process, so a module
with one slow corpus test does not hold the whole run.
"""
from __future__ import annotations

import importlib.util
import os
import subprocess
import sys
import unittest
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
MODULES = [
    "test_flw0",
    "test_fld",
    "test_battle_tbl",
    "test_wap",
    "test_inf",
    "test_production_report",
    "test_configure",
    "test_check_unit",
    "test_dev_elf",
    "test_ee_gcc_diagnostics",
    "test_ee_gcc_aslr",
    "test_include_sdata",
    "test_include_rodata",
    "test_lb",
    "test_m2ctx",
    "test_source_tokens",
]
BATCH = 4


def test_ids(module: str) -> list[str]:
    """Test names of a module, as its own `unittest.main` spells them."""
    spec = importlib.util.spec_from_file_location(f"_collect_{module}", TOOLS / f"{module}.py")
    loaded = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = loaded
    spec.loader.exec_module(loaded)

    def walk(suite):
        for test in suite:
            if isinstance(test, unittest.TestSuite):
                yield from walk(test)
            else:
                yield test.id().split(".", 1)[1]

    return list(walk(unittest.defaultTestLoader.loadTestsFromModule(loaded)))


def batches(module: str, ids: list[str]) -> list[tuple[str, list[str]]]:
    """Deal the ids round-robin, so slow tests of one module land in different batches."""
    count = max(1, -(-len(ids) // BATCH))
    return [(module, ids[i::count]) for i in range(count) if ids[i::count]]


def run(task: tuple[str, list[str]]) -> tuple[str, list[str], subprocess.CompletedProcess]:
    module, ids = task
    result = subprocess.run([sys.executable, str(TOOLS / f"{module}.py"), *ids],
                            capture_output=True, text=True)
    return module, ids, result


def workers() -> int:
    """Cores this process may use; os.process_cpu_count is Python 3.13."""
    count = getattr(os, "process_cpu_count", os.cpu_count)()
    return count or 4


def main() -> None:
    modules = sys.argv[1:] or MODULES
    unknown = [m for m in modules if not (TOOLS / f"{m}.py").is_file()]
    if unknown:
        raise SystemExit(f"no such test module: {' '.join(unknown)}")
    sys.path.insert(0, str(TOOLS))
    tasks = [batch for module in modules for batch in batches(module, test_ids(module))]
    failures = []
    with ThreadPoolExecutor(max_workers=workers()) as pool:
        for module, ids, result in pool.map(run, tasks):
            if result.returncode:
                failures.append(module)
                print(f"FAIL {module}: {' '.join(ids)}\n{result.stdout}{result.stderr}", flush=True)
    tests = sum(len(ids) for _, ids in tasks)
    if failures:
        raise SystemExit(f"{len(failures)} of {len(tasks)} batches failed: {' '.join(sorted(set(failures)))}")
    print(f"{tests} tests in {len(tasks)} batches: OK")


if __name__ == "__main__":
    main()
