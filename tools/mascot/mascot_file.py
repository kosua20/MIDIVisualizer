"""Формат .mascot (читает Renderer::loadMascot): заголовок, затем zlib(расстояния uint16, x быстрее всех;
цвета rgba uint8 на сетке вдвое реже). Расстояния в единицах модели, отображены на dist_range.
В заголовке ещё заводские настройки модели (DEFAULTS), их программа ставит при выборе модели."""
import struct, zlib, numpy as np

VERSION = 3
# Same order as in Renderer::loadMascot; values are those of a new mascot in State.
DEFAULTS = dict(x=0.0, y=0.0, z=0.0, turn=0.0, lean=0.0, size=1.0, glow=1.0, light=0.0,
                lx=0.0, ly=1.0, lz=-0.5, bulb=0.0, r=0.72, g=0.86, b=1.0)
HEADER = struct.Struct("<4sI3I3I3f3f2f%dfI" % len(DEFAULTS))
HEADER_V2 = struct.Struct("<4sI3I3I3f3f2fI")


def parse_defaults(items):
    """["x=1.6", "turn=60", ...] -> полный словарь настроек."""
    out = dict(DEFAULTS)
    for item in items:
        k, v = item.split("=")
        assert k in out, "неизвестная настройка %s, есть: %s" % (k, " ".join(DEFAULTS))
        out[k] = float(v)
    return out


def write(path, dist, color, half, center, dist_range, defaults=None):
    """dist: (z, y, x) float; color: (z/2, y/2, x/2, 4) uint8."""
    lo, hi = dist_range
    # 16 bits: with 8, the steps of the distance show as grain in the normals of thin petals.
    q = np.clip(np.round((dist - lo) / (hi - lo) * 65535), 0, 65535).astype("<u2")
    data = zlib.compress(q.tobytes() + np.ascontiguousarray(color).tobytes(), 9)
    dz, dy, dx = dist.shape
    cz, cy, cx = color.shape[:3]
    with open(path, "wb") as f:
        d = dict(DEFAULTS, **(defaults or {}))
        f.write(HEADER.pack(b"MSCT", VERSION, dx, dy, dz, cx, cy, cz, *half, *center, lo, hi,
                            *[d[k] for k in DEFAULTS], len(data)))
        f.write(data)


def read(path):
    raw = open(path, "rb").read()
    # Version 2 had no defaults, read it too so that rewrite() can upgrade old files.
    header = HEADER_V2 if struct.unpack_from("<I", raw, 4)[0] == 2 else HEADER
    h = header.unpack_from(raw)
    assert h[0] == b"MSCT" and h[1] in (2, VERSION)
    dx, dy, dz, cx, cy, cz = h[2:8]
    half, center, rng = np.array(h[8:11]), np.array(h[11:14]), h[14:16]
    defaults = dict(zip(DEFAULTS, h[16:-1])) or dict(DEFAULTS)
    data = zlib.decompress(raw[header.size:header.size + h[-1]])
    n = dx * dy * dz
    dist = np.frombuffer(data[:2 * n], "<u2").reshape(dz, dy, dx)
    color = np.frombuffer(data[2 * n:], np.uint8).reshape(cz, cy, cx, 4)
    return dict(dist=dist, color=color, half=half, center=center, dist_range=rng, defaults=defaults)


def rewrite(path, defaults):
    """Поменять заводские настройки без перепекания (расстояния уже квантованы, пишутся как есть)."""
    m = read(path)
    lo, hi = m["dist_range"]
    write(path, lo + m["dist"].astype(np.float64) / 65535 * (hi - lo), m["color"], m["half"], m["center"],
          m["dist_range"], defaults)
