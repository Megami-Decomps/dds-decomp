#!/usr/bin/env python3
"""Pin how the IRX draft check reads the host compiler's result."""
import contextlib
import io
import subprocess
import sys
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_irx_nonmatching as irx


class CompilerResult(unittest.TestCase):
    def result(self, status, stderr="", stdout=""):
        result = subprocess.CompletedProcess([], status, stdout, stderr)
        with patch.object(irx.irx_config, "load", return_value={}), \
                patch.object(irx.irx_config, "path", return_value=Path("unit.c")), \
                patch.object(irx.subprocess, "run", return_value=result), \
                contextlib.redirect_stdout(io.StringIO()):
            return irx.check("test", Path("shim"))

    def test_failure_without_an_error_diagnostic_is_reported(self):
        for stderr in ("", "cc1: terminated\n", "unit.c:3: warning: unused variable\n"):
            with self.subTest(stderr=stderr):
                self.assertGreater(self.result(1, stderr), 0)

    def test_warning_with_a_successful_exit_is_allowed(self):
        self.assertEqual(self.result(0, "unit.c:3: warning: unused variable\n"), 0)

    def test_error_is_counted(self):
        self.assertEqual(self.result(1, "unit.c:3: error: syntax error\n"), 1)


if __name__ == "__main__":
    unittest.main()
