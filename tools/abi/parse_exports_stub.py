#!/usr/bin/env python3
"""
Placeholder for Phase 1 tools.

In Phase 1, this script will parse `dumpbin /EXPORTS` output and turn it
into structured JSON (names, ordinals, RVAs, etc.) so we can later
generate .def files and stub code.

For Phase 0A we just keep this stub here to lock in the path.
"""

import sys


def main():
    print("parse_exports_stub.py: not implemented yet", file=sys.stderr)
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
