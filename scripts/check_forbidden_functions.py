#!/usr/bin/env python3
import bisect
import re
import subprocess
from pathlib import Path


REPORT = Path("forbidden_functions_report.md")


def run(args):
    return subprocess.check_output(args, text=True)


def rg_files(*args):
    out = run(["rg", "--files", *args])
    return [line.strip() for line in out.splitlines() if line.strip()]


def ctags_names(files):
    if not files:
        return set()
    proc = subprocess.run(
        ["ctags", "-x", "--c-kinds=fp", "--language-force=C", *files],
        text=True,
        capture_output=True,
        check=False,
    )
    names = set()
    for line in proc.stdout.splitlines():
        parts = line.split()
        if parts:
            names.add(parts[0])
    return names


def math_names_set():
    base = {
        "acos",
        "asin",
        "atan",
        "atan2",
        "cos",
        "sin",
        "tan",
        "acosh",
        "asinh",
        "atanh",
        "cosh",
        "sinh",
        "tanh",
        "exp",
        "exp2",
        "expm1",
        "log",
        "log10",
        "log1p",
        "log2",
        "logb",
        "frexp",
        "ldexp",
        "modf",
        "scalbn",
        "scalbln",
        "ilogb",
        "pow",
        "sqrt",
        "cbrt",
        "hypot",
        "erf",
        "erfc",
        "lgamma",
        "tgamma",
        "ceil",
        "floor",
        "trunc",
        "round",
        "nearbyint",
        "rint",
        "lrint",
        "llrint",
        "lround",
        "llround",
        "remainder",
        "remquo",
        "fmod",
        "copysign",
        "nan",
        "nextafter",
        "nexttoward",
        "fdim",
        "fmax",
        "fmin",
        "fma",
        "fabs",
        "j0",
        "j1",
        "jn",
        "y0",
        "y1",
        "yn",
        "gamma",
        "sincos",
    }
    extra = {
        "fpclassify",
        "isfinite",
        "isinf",
        "isnan",
        "isnormal",
        "signbit",
        "isgreater",
        "isgreaterequal",
        "isless",
        "islessequal",
        "islessgreater",
        "isunordered",
        "finite",
    }
    out = set(extra)
    for name in base:
        out.add(name)
        out.add(name + "f")
        out.add(name + "l")
    return out


def sanitize_source(src):
    lines = src.splitlines(keepends=True)
    preprocessed = []
    for ln in lines:
        if ln.lstrip().startswith("#"):
            if ln.endswith("\n"):
                preprocessed.append(" " * (len(ln) - 1) + "\n")
            else:
                preprocessed.append(" " * len(ln))
        else:
            preprocessed.append(ln)
    src = "".join(preprocessed)

    out = []
    i = 0
    n = len(src)
    in_sl = False
    in_ml = False
    in_str = False
    in_char = False
    esc = False

    while i < n:
        c = src[i]
        nxt = src[i + 1] if i + 1 < n else ""

        if in_sl:
            if c == "\n":
                in_sl = False
                out.append("\n")
            else:
                out.append(" ")
            i += 1
            continue

        if in_ml:
            if c == "*" and nxt == "/":
                in_ml = False
                out.append(" ")
                out.append(" ")
                i += 2
                continue
            if c == "\n":
                out.append("\n")
            else:
                out.append(" ")
            i += 1
            continue

        if in_str:
            if esc:
                esc = False
                out.append(" ")
            elif c == "\\":
                esc = True
                out.append(" ")
            elif c == "\"":
                in_str = False
                out.append(" ")
            elif c == "\n":
                in_str = False
                out.append("\n")
            else:
                out.append(" ")
            i += 1
            continue

        if in_char:
            if esc:
                esc = False
                out.append(" ")
            elif c == "\\":
                esc = True
                out.append(" ")
            elif c == "'":
                in_char = False
                out.append(" ")
            elif c == "\n":
                in_char = False
                out.append("\n")
            else:
                out.append(" ")
            i += 1
            continue

        if c == "/" and nxt == "/":
            in_sl = True
            out.append(" ")
            out.append(" ")
            i += 2
            continue
        if c == "/" and nxt == "*":
            in_ml = True
            out.append(" ")
            out.append(" ")
            i += 2
            continue
        if c == "\"":
            in_str = True
            out.append(" ")
            i += 1
            continue
        if c == "'":
            in_char = True
            out.append(" ")
            i += 1
            continue

        out.append(c)
        i += 1

    return "".join(out)


def main():
    target_files = rg_files(
        "-g",
        "*.c",
        "-g",
        "*.h",
        "-g",
        "!libs/**",
        "-g",
        "!maps/**",
        "-g",
        "!assets/**",
    )
    target_files = sorted(
        f
        for f in target_files
        if not (f.startswith("build/") or f.startswith("build_bonus/"))
    )

    project_names = ctags_names(target_files)
    libft_files = rg_files("libs/libft", "-g", "*.c", "-g", "*.h")
    libft_names = ctags_names(libft_files)
    mlx_files = rg_files("libs/minilibx", "-g", "*.c", "-g", "*.h")
    mlx_names = ctags_names(mlx_files)
    math_names = math_names_set()

    allowed_explicit = {
        "open",
        "close",
        "read",
        "write",
        "printf",
        "malloc",
        "free",
        "perror",
        "strerror",
        "exit",
        "gettimeofday",
    }

    always_skip = {
        "if",
        "for",
        "while",
        "switch",
        "return",
        "sizeof",
        "_Alignof",
        "__alignof__",
        "_Generic",
        "defined",
        "__attribute__",
        "__extension__",
        "__builtin_choose_expr",
    }

    allowed = set()
    allowed.update(project_names)
    allowed.update(libft_names)
    allowed.update(mlx_names)
    allowed.update(math_names)
    allowed.update(allowed_explicit)

    call_re = re.compile(r"\b([A-Za-z_][A-Za-z0-9_]*)\s*\(")

    results = {}
    for rel in target_files:
        src = Path(rel).read_text(encoding="utf-8", errors="ignore")
        clean = sanitize_source(src)

        line_starts = [0]
        for idx, ch in enumerate(clean):
            if ch == "\n":
                line_starts.append(idx + 1)

        forbidden = []
        seen = set()

        for match in call_re.finditer(clean):
            name = match.group(1)

            if name in always_skip:
                continue
            if name.startswith("__builtin_") or name.startswith("__"):
                continue
            if re.fullmatch(r"[A-Z0-9_]+", name):
                continue
            if name in allowed or name.startswith("mlx_") or name.startswith("ft_"):
                continue

            line = bisect.bisect_right(line_starts, match.start())
            key = (line, name)
            if key in seen:
                continue
            seen.add(key)
            forbidden.append(key)

        forbidden.sort()
        results[rel] = forbidden

    with REPORT.open("w", encoding="utf-8") as report_file:
        for rel in target_files:
            forb = results[rel]
            if not forb:
                report_file.write(f"{rel}: OK!\n")
            else:
                report_file.write(f"{rel}: KO:\n")
                for line, name in forb:
                    report_file.write(f"    {line}: {name}\n")

    ko_files = sum(1 for v in results.values() if v)
    ko_hits = sum(len(v) for v in results.values())
    print(f"FILES={len(target_files)} KO_FILES={ko_files} KO_HITS={ko_hits}")
    print(f"REPORT={REPORT}")


if __name__ == "__main__":
    main()
