#!/usr/bin/env python3
"""Write a generated batch into the per-subsystem files of phase4/src.

phase4/src is organised one class per file under subsystem directories, and the
generated metadata tables sit alongside the classes they describe:

    phase4/src/core/gdi/RuntimeClasses.cpp
    phase4/src/featurepack/ribbon/MessageMaps.cpp

A harvest batch is chosen by ABI convenience, not by subsystem, so its classes
usually belong to several directories.  :func:`emit` splits a batch accordingly
and appends to whichever files it touches, keeping each file's preamble intact.
"""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
import source_layout as sl

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC = os.path.join(ROOT, 'phase4', 'src')


def group_by_directory(classes):
    """{subsystem directory: [class, ...]} preserving the given order."""
    groups = {}
    for cls in classes:
        groups.setdefault(sl.directory_for_class(cls), []).append(cls)
    return groups


def emit(directory, filename, preamble, section):
    """Create or extend ``phase4/src/<directory>/<filename>``.

    *preamble* (includes and macro definitions) is written only when the file
    is new; *section* is appended either way.  Returns the path written.
    """
    path = os.path.join(SRC, directory, filename)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    exists = os.path.exists(path)
    with open(path, 'a' if exists else 'w', encoding='utf-8') as fh:
        if not exists:
            fh.write(preamble)
        fh.write(section)
    return path


def drop_sections(marker):
    """Remove previously generated sections tagged with *marker*.

    Regenerating a batch must not append a second copy, so every section is
    fenced with ``// <<< marker`` / ``// >>> marker`` and replaced in place.
    """
    for root, _dirs, files in os.walk(SRC):
        for fname in files:
            if not fname.endswith('.cpp'):
                continue
            path = os.path.join(root, fname)
            with open(path, encoding='utf-8') as fh:
                text = fh.read()
            start = f'// <<< {marker}\n'
            end = f'// >>> {marker}\n'
            if start not in text:
                continue
            while start in text and end in text:
                head, _, rest = text.partition(start)
                _, _, tail = rest.partition(end)
                text = head + tail
            with open(path, 'w', encoding='utf-8') as fh:
                fh.write(text)


def fence(marker, body):
    return f'// <<< {marker}\n{body}\n// >>> {marker}\n'
