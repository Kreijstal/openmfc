#!/usr/bin/env python3
"""Decompose phase4/src translation units into per-class chunks.

The phase4 sources are flat C-shaped translation units: a preamble (includes,
macros, forward declarations) followed by a sequence of top-level definitions,
each preceded by a ``// Symbol: <mangled>`` marker that names the MSVC export it
implements.  This module splits a file into those units so callers can regroup
them one-class-per-file.

It is deliberately conservative: a file is only reported as splittable when the
preamble and every unattributed chunk contain no external definitions that
would be duplicated (and therefore multiply defined) across the outputs.
"""

import re
import os
import sys

SYMBOL_RE = re.compile(r'^// Symbol: (\S+)', re.M)
IDENT_RE = re.compile(r'^[A-Za-z_][A-Za-z_0-9]*$')


def symbol_class(sym):
    """Return the class a mangled MSVC symbol belongs to, or None for globals.

    MSVC decorates ``Class::Method`` as ``?Method@Class@@<type>`` and nested
    scopes as ``?Method@Class@Namespace@@``; a free function is ``?Func@@<type>``
    with nothing between the name and the ``@@`` scope terminator.  Special
    names (constructors ``??0``, destructors ``??1``, vftables ``??_7`` …) put
    the operator code where the name would otherwise start.
    """
    if not sym.startswith('?'):
        return None
    rest = sym[1:]
    if rest.startswith('?'):
        rest = rest[1:]
        if rest.startswith('_') and len(rest) > 1:
            rest = rest[2:]     # ??_7, ??_G, ??_E …
        elif rest:
            rest = rest[1:]     # ??0, ??1, ??4 …
    else:
        at = rest.find('@')
        if at < 0:
            return None
        rest = rest[at + 1:]
    scopes = rest.split('@')
    if not scopes or not scopes[0]:
        return None             # free function: `@@` follows the name directly
    return scopes[0] if IDENT_RE.match(scopes[0]) else None


def symbol_owner(sym):
    """Return the outermost class scope of *sym*, or None for globals.

    MSVC mangles nested scopes innermost-first, so ``COleDropSource::XDropSource``
    appears as ``?Method@XDropSource@COleDropSource@@``.  Nested interface
    implementations belong in their outer class's file, so file placement keys
    off the outermost scope while :func:`symbol_class` reports the innermost.
    """
    if not sym.startswith('?'):
        return None
    rest = sym[1:]
    if rest.startswith('?'):
        rest = rest[1:]
        if rest.startswith('_') and len(rest) > 1:
            rest = rest[2:]
        elif rest:
            rest = rest[1:]
    else:
        at = rest.find('@')
        if at < 0:
            return None
        rest = rest[at + 1:]
    scopes = []
    for tok in rest.split('@'):
        if tok == '':
            break               # `@@` terminates the scope list
        scopes.append(tok)
    while scopes and not IDENT_RE.match(scopes[-1]):
        scopes.pop()
    return scopes[-1] if scopes else None


def strip_literals(text):
    """Blank out comments and string/char literals so brace counting is sane."""
    out = []
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == '/' and i + 1 < n and text[i + 1] == '/':
            j = text.find('\n', i)
            j = n if j < 0 else j
            out.append(' ' * (j - i))
            i = j
        elif c == '/' and i + 1 < n and text[i + 1] == '*':
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
            out.append(''.join(ch if ch == '\n' else ' ' for ch in text[i:j]))
            i = j
        elif c in '"\'':
            quote = c
            j = i + 1
            while j < n:
                if text[j] == '\\':
                    j += 2
                    continue
                if text[j] == quote:
                    j += 1
                    break
                j += 1
            out.append(''.join(ch if ch == '\n' else ' ' for ch in text[i:j]))
            i = j
        else:
            out.append(c)
            i += 1
    return ''.join(out)


def top_level_line_depths(lines, blanked_lines):
    """Brace depth at the START of each line, ignoring preprocessor lines."""
    depths = []
    depth = 0
    for raw in blanked_lines:
        depths.append(depth)
        if raw.lstrip().startswith('#'):
            continue
        depth += raw.count('{') - raw.count('}')
    return depths


class Chunk:
    def __init__(self, start, end, symbols, text):
        self.start = start          # first line index (0-based, inclusive)
        self.end = end              # last line index (exclusive)
        self.symbols = symbols      # mangled symbols declared by // Symbol: markers
        self.text = text

    @property
    def classes(self):
        seen = []
        for s in self.symbols:
            c = symbol_class(s)
            if c and c not in seen:
                seen.append(c)
        return seen

    def __repr__(self):
        return f'<Chunk {self.start}:{self.end} {self.symbols}>'


def parse(path):
    """Split a translation unit into (preamble_text, [Chunk, ...]).

    A chunk starts at the first line of the comment block introducing a
    ``// Symbol:`` marker and runs until the next such comment block.
    """
    text = open(path, encoding='utf-8', errors='replace').read()
    lines = text.splitlines(keepends=True)
    blanked = strip_literals(text).splitlines(keepends=True)
    depths = top_level_line_depths(lines, blanked)

    # Locate the comment-block start for every top-level `// Symbol:` marker.
    starts = []
    for i, line in enumerate(lines):
        if not line.startswith('// Symbol:'):
            continue
        if depths[i] != 0:
            continue  # marker nested inside a function body: not a chunk head
        j = i
        while j > 0:
            prev = lines[j - 1].rstrip('\n')
            if prev.startswith('//') and not prev.startswith('//===') and \
                    not prev.startswith('//---'):
                j -= 1
            else:
                break
        starts.append((j, i))

    if not starts:
        return text, []

    # Merge markers that share one definition (several // Symbol: lines stacked
    # above a single function) and build chunk boundaries.
    heads = []
    for block_start, marker in starts:
        if heads and block_start <= heads[-1][1]:
            heads[-1][2].append(marker)
        else:
            heads.append([block_start, marker, [marker]])
    # Consecutive markers with nothing but comments between them belong together.
    merged = []
    for head in heads:
        if merged:
            prev_end = merged[-1][1]
            between = ''.join(lines[prev_end + 1:head[0]]).strip()
            if not between:
                merged[-1][1] = head[1]
                merged[-1][2].extend(head[2])
                continue
        merged.append(head)

    preamble_end = merged[0][0]
    bounds = []
    for idx, (start, _, markers) in enumerate(merged):
        limit = merged[idx + 1][0] if idx + 1 < len(merged) else len(lines)
        bounds.append([start, _definition_end(blanked, markers[-1], limit), limit,
                       markers])

    # A `#if` region may enclose several marked definitions.  Such a region can
    # not be divided, so the chunks it covers are merged into one.
    idx = 0
    while idx < len(bounds):
        start, end, limit, markers = bounds[idx]
        if _guard_balance(blanked[start:end]) == 0:
            idx += 1
            continue
        if idx + 1 == len(bounds):
            bounds[idx][1] = limit
            break
        nxt = bounds[idx + 1]
        bounds[idx] = [start, nxt[1], nxt[2], markers + nxt[3]]
        del bounds[idx + 1]

    chunks = []
    for start, end, limit, markers in bounds:
        syms = [SYMBOL_RE.match(lines[m]).group(1) for m in markers]
        chunk = Chunk(start, end, syms, ''.join(lines[start:end]))
        chunk.trailer = ''.join(lines[end:limit])
        chunks.append(chunk)

    return ''.join(lines[:preamble_end]), chunks


def _guard_balance(lines):
    """`#if` minus `#endif` over *lines* (already comment-stripped)."""
    depth = 0
    for line in lines:
        stripped = line.strip()
        if re.match(r'^#\s*if', stripped):
            depth += 1
        elif re.match(r'^#\s*endif', stripped):
            depth -= 1
    return depth


def _definition_end(blanked, marker_line, limit):
    """Return the line index just past the definition introduced at *marker_line*.

    Text between one definition and the next ``// Symbol:`` block is *not* part
    of the definition — it is usually a file-scope helper shared by whatever
    follows — so the caller can keep it with the origin rather than dragging it
    into an unrelated class file.
    """
    depth = 0
    guard = 0
    started = False
    for i in range(marker_line, limit):
        line = blanked[i]
        if line.lstrip().startswith('#'):
            # Never end a definition inside a conditional region: the `#endif`
            # would be stranded in whatever follows.
            if re.match(r'^\s*#\s*if', line):
                guard += 1
            elif re.match(r'^\s*#\s*endif', line):
                guard -= 1
            continue
        depth += line.count('{') - line.count('}')
        if line.count('{'):
            started = True
        if guard > 0:
            continue
        if started and depth <= 0:
            return i + 1
        if not started and depth == 0 and line.rstrip().endswith(';'):
            return i + 1        # a plain declaration or aliased definition
    return limit


# Namespaces that wrap classes we still want to file under the class name.
SKIP_SCOPES = {'ATL', 'std', '__gnu_cxx', 'openmfc'}


def _split_scopes(qualified):
    """Split a demangled qualified name on `::` outside template arguments."""
    parts, depth, cur = [], 0, []
    i = 0
    while i < len(qualified):
        c = qualified[i]
        if c == '<':
            depth += 1
        elif c == '>':
            depth -= 1
        elif c == ':' and depth == 0 and qualified[i + 1:i + 2] == ':':
            parts.append(''.join(cur))
            cur = []
            i += 2
            continue
        cur.append(c)
        i += 1
    parts.append(''.join(cur))
    return parts


def owner_from_demangled(demangled):
    """Return the owning class of a demangled MSVC symbol, or None.

    ``public: void __cdecl COleDropSource::XDropSource::AddRef(void)`` is owned
    by ``COleDropSource``: nested interface implementations live in their outer
    class's file.  Free functions and template free functions return None.
    """
    if not demangled:
        return None
    text = demangled
    # Drop the parameter list and anything after it.
    depth = 0
    for i, c in enumerate(text):
        if c == '<':
            depth += 1
        elif c == '>':
            depth -= 1
        elif c == '(' and depth == 0:
            text = text[:i]
            break
    text = text.strip()
    # Drop leading access/storage specifiers and the return type: the qualified
    # name is the last whitespace-separated token outside template arguments.
    depth, start = 0, 0
    for i, c in enumerate(text):
        if c == '<':
            depth += 1
        elif c == '>':
            depth -= 1
        elif c in ' \t' and depth == 0:
            start = i + 1
    qualified = text[start:]
    scopes = _split_scopes(qualified)[:-1]      # last component is the member
    for scope in scopes:
        name = scope.split('<', 1)[0].strip()
        if not name or name in SKIP_SCOPES:
            continue
        if IDENT_RE.match(name):
            return name
    return None


def demangle(symbols):
    """Return {mangled: demangled} using llvm-undname.

    llvm-undname echoes each input and prints its result underneath, but not
    every input yields exactly two lines — a name it cannot parse produces just
    the echo.  Pairing positionally would therefore slide out of step and
    attribute symbols to the wrong class, so each echo is matched against the
    set of inputs instead.
    """
    import subprocess
    symbols = list(symbols)
    if not symbols:
        return {}
    proc = subprocess.run(['llvm-undname'], input='\n'.join(symbols) + '\n',
                          capture_output=True, text=True)
    wanted = set(symbols)
    out = {}
    pending = None
    for line in proc.stdout.splitlines():
        stripped = line.strip()
        if stripped in wanted:
            pending = stripped
            out.setdefault(pending, '')
            continue
        if pending is not None:
            out[pending] = stripped
            pending = None
    return out


DEFINITION_RE = re.compile(
    r'^[A-Za-z_].*\)\s*(?:const\s*)?\{|^[A-Za-z_][^=;]*=\s*[^;]*;', re.M)


def preamble_defines_externals(preamble):
    """True when the preamble contains definitions that cannot be duplicated.

    Declarations (``extern "C" T f(...);``), macros and includes are safe to
    repeat in every output file.  Definitions of non-static objects or
    functions are not: repeating them would multiply-define the symbol.
    """
    body = strip_literals(preamble)
    offenders = []
    for m in DEFINITION_RE.finditer(body):
        line = body[m.start():body.find('\n', m.start())]
        if 'static' in line or 'extern' in line and line.rstrip().endswith(';'):
            continue
        if line.lstrip().startswith('#'):
            continue
        offenders.append(line.strip())
    return offenders


def analyse(path):
    preamble, chunks = parse(path)
    classes = {}
    orphans = []
    for ch in chunks:
        cls = ch.classes
        if len(cls) == 1:
            classes.setdefault(cls[0], []).append(ch)
        else:
            orphans.append(ch)
    return {
        'path': path,
        'preamble': preamble,
        'chunks': chunks,
        'classes': classes,
        'orphans': orphans,
        'preamble_defs': preamble_defines_externals(preamble),
    }


def main():
    paths = sys.argv[1:]
    for p in paths:
        info = analyse(p)
        n_cls = len(info['classes'])
        print(f"{os.path.basename(p)}: {len(info['chunks'])} chunks, "
              f"{n_cls} classes, {len(info['orphans'])} orphan chunks, "
              f"{len(info['preamble_defs'])} preamble defs")
        for d in info['preamble_defs'][:3]:
            print('    preamble def:', d[:100])


if __name__ == '__main__':
    main()
