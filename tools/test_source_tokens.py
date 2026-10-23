"""The fast whitespace split must agree with shlex on every line it claims."""
import shlex
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import source_tokens  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
SUFFIXES = ("*.fldasm", "*.bfasm", "*.infasm", "*.wapasm", "*.ambasm")


def corpus_sources(*suffixes):
    """Imported data and script sources, skipping the test when there are none."""
    files = sorted(path for suffix in suffixes
                   for path in (ROOT / "src").rglob(suffix))
    if not files:
        raise unittest.SkipTest("corpus not imported: src/*/{data,scripts}")
    return files


class SourceTokenTests(unittest.TestCase):
    def assertAgrees(self, line):
        self.assertEqual(source_tokens.split(line),
                         shlex.split(line, comments=True, posix=True), repr(line))

    def test_plain_lines_agree(self):
        for line in ("", "   ", "object f001_001 0x12", "  tabs\tand   spaces  ",
                     "unicode café 0x1", "trailing   "):
            self.assertAgrees(line)

    def test_quoted_and_commented_lines_agree(self):
        for line in ('name "a b" 3', "name 'a b' 3", r"path a\ b", "code 0x1 # tail",
                     "# whole line", 'empty ""', r'escaped \" quote', "hash#inside",
                     '"leading quote" rest', "mixed 'a' \"b\" # c"):
            self.assertAgrees(line)

    def test_every_tracked_data_source_agrees(self):
        files = corpus_sources(*SUFFIXES)
        self.assertGreater(len(files), 100)
        quoted = 0
        for path in files[::7]:
            for line in path.read_text(encoding="utf-8").splitlines():
                if source_tokens.QUOTING.search(line):
                    quoted += 1
                    continue
                self.assertEqual(source_tokens.split(line), line.split(), repr(line))
        self.assertGreater(quoted, 0)

    def test_quoted_lines_of_the_corpus_agree(self):
        for suffix in SUFFIXES:
            for path in corpus_sources(suffix)[::11]:
                for line in path.read_text(encoding="utf-8").splitlines():
                    if source_tokens.QUOTING.search(line):
                        self.assertAgrees(line)


if __name__ == "__main__":
    unittest.main()
