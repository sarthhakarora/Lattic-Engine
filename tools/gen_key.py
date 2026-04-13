# Extracts KeyboardKey enum from raylib.h
# Generates generated/keyboard_keys.def
# Run only when updating raylib

from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RAYLIB_H = ROOT / "include" / "raylib.h"
OUT_FILE = ROOT / "generated" / "keyboard_keys.def"

out = []
inside = False

with open(RAYLIB_H, "r", encoding="utf-8") as f:
    for line in f:
        line = line.strip()

        # enter enum
        if line == "typedef enum {":
            inside = True
            continue

        # exit enum
        if inside and line == "} KeyboardKey;":
            break

        if not inside:
            continue

        if not line or line.startswith("//"):
            continue

        if line.startswith("KEY_"):
            name = line.split("=")[0].strip()
            out.append(f"X({name})")

OUT_FILE.parent.mkdir(parents=True, exist_ok=True)

with open(OUT_FILE, "w", encoding="utf-8") as f:
    f.write("\n".join(out))

print("keys found:", len(out))