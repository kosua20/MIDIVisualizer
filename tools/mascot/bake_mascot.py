#!/usr/bin/env python3
"""Запекает OBJ-модель в поле расстояний для шейдера keys3d (маскот на щёчке корпуса).

    python bake_mascot.py model.obj out.mascot --drop WATER,mud --height 1.4 [--thick 0.012] [--res 128]

Модель — оболочка толщиной 2*thick (открытые поверхности вроде лепестков тоже работают),
расстояние без знака по плотным точкам поверхности, в ширинах белой клавиши. Файл .mascot кладётся
рядом с пресетами и выбирается в меню Mascot (там же поворот, наклон, размер, свет). Запекать лучше
в исходной ориентации (--yaw 180: лицом к зрителю), поворачивать уже в программе.
"""
import argparse, sys, numpy as np
from scipy.spatial import cKDTree
from objload import load_obj
import mascot_file


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("obj"); ap.add_argument("out")
    ap.add_argument("--drop", default="", help="материалы/объекты, которые выкинуть")
    ap.add_argument("--xmax", type=float, default=None, help="выкинуть треугольники правее (исходные координаты)")
    ap.add_argument("--move", default=None, help="xmin,dx,dy,dz: сдвинуть треугольники правее xmin (исходные координаты)")
    ap.add_argument("--roll", type=float, default=0.0, help="наклон вокруг оси z (к клавишам: отрицательный)")
    ap.add_argument("--yaw", type=float, default=180.0, help="поворот вокруг вертикали, градусы")
    ap.add_argument("--pitch", type=float, default=0.0, help="наклон вокруг x (положить цветок: 90)")
    ap.add_argument("--height", type=float, default=None, help="высота модели в ширинах белой клавиши")
    ap.add_argument("--size", type=float, default=None, help="или наибольший размер")
    ap.add_argument("--thick", type=float, default=0.012)
    ap.add_argument("--res", type=int, default=128)
    ap.add_argument("--color", action="append", default=[], help="материал=r,g,b[,свечение] поверх Kd")
    ap.add_argument("--set", action="append", default=[], help="заводская настройка: x= y= z= turn= lean= size= glow= light= lx= ly= lz= bulb= r= g= b=")
    a = ap.parse_args()

    T, TM, TO, mats = load_obj(a.obj)
    drop = [x for x in a.drop.split(",") if x]
    keep = np.array([not (m in drop or o in drop) for m, o in zip(TM, TO)])
    if a.xmax is not None:
        keep &= T[:, :, 0].mean(1) < a.xmax
    T, TM = T[keep], TM[keep]
    if a.move:
        xmin, dx, dy, dz = map(float, a.move.split(","))
        sel = T[:, :, 0].mean(1) > xmin
        T[sel] += np.array([dx, dy, dz])
    names = sorted(set(TM), key=str)
    over = {kv.split("=")[0]: tuple(map(float, kv.split("=")[1].split(","))) for kv in a.color}
    pal = []
    for n in names:
        m = mats.get(n, {})
        c = over.get(n, tuple(m.get("Kd", (0.8, 0.8, 0.8))) + (max(m.get("Ke", (0, 0, 0))),))
        pal.append(c if len(c) == 4 else c + (0.0,))
    pal = np.array(pal)
    mid = np.array([names.index(m) for m in TM])

    # Orientation: OBJ from Blender is y-up, front towards +z; the scene looks towards +z.
    V = T.reshape(-1, 3)
    p, y = np.radians(a.pitch), np.radians(a.yaw)
    Rx = np.array([[1, 0, 0], [0, np.cos(p), -np.sin(p)], [0, np.sin(p), np.cos(p)]])
    Ry = np.array([[np.cos(y), 0, np.sin(y)], [0, 1, 0], [-np.sin(y), 0, np.cos(y)]])
    r = np.radians(a.roll)
    Rz = np.array([[np.cos(r), -np.sin(r), 0], [np.sin(r), np.cos(r), 0], [0, 0, 1]])
    V = V @ (Rz @ Ry @ Rx).T
    lo, hi = V.min(0), V.max(0)
    ext = hi - lo
    scale = a.height / ext[1] if a.height else a.size / ext.max()
    V = (V - np.array([(lo[0] + hi[0]) / 2, lo[1], (lo[2] + hi[2]) / 2])) * scale
    T = V.reshape(-1, 3, 3)
    ext = ext * scale
    print(f"{len(T)} треугольников, размер {ext.round(2)} (x, y, z)")
    C = T.mean(1)
    print("центр верхней пятой:", C[C[:, 1] > 0.8 * ext[1]].mean(0).round(2),
          "центр по высоте 40-80%:", C[(C[:, 1] > 0.4 * ext[1]) & (C[:, 1] < 0.8 * ext[1])].mean(0).round(2))

    # Dense surface samples, area weighted.
    pad = 4 * a.thick + 0.03
    vox = (ext.max() + 2 * pad) / a.res
    step = vox * 0.35
    e1, e2 = T[:, 1] - T[:, 0], T[:, 2] - T[:, 0]
    area = 0.5 * np.linalg.norm(np.cross(e1, e2), axis=1)
    count = np.maximum(1, np.ceil(area / step ** 2)).astype(int)
    total = count.sum()
    print(f"точек поверхности: {total}")
    if total > 6_000_000:
        sys.exit("слишком много точек, уменьши --res")
    ti = np.repeat(np.arange(len(T)), count)
    r1, r2 = np.random.default_rng(0).random((2, total))
    flip = r1 + r2 > 1
    r1[flip], r2[flip] = 1 - r1[flip], 1 - r2[flip]
    P = T[ti, 0] + e1[ti] * r1[:, None] + e2[ti] * r2[:, None]
    P = np.concatenate([P, T.reshape(-1, 3)])
    PM = np.concatenate([mid[ti], np.repeat(mid, 3)])
    tree = cKDTree(P)

    half = ext / 2 + pad
    center = np.array([0.0, ext[1] / 2, 0.0])
    dims = np.maximum(8, np.ceil(2 * half / vox)).astype(int)
    dims += dims % 2  # Colors are stored at half resolution.
    half = dims * vox / 2
    axes = [center[i] - half[i] + vox * (np.arange(dims[i]) + 0.5) for i in range(3)]
    Z, Y, X = np.meshgrid(axes[2], axes[1], axes[0], indexing="ij")
    G = np.stack([X.ravel(), Y.ravel(), Z.ravel()], 1)
    d, idx = tree.query(G, workers=-1)
    dist = (d - a.thick).reshape(dims[2], dims[1], dims[0])
    col = pal[PM[idx]].reshape(dims[2], dims[1], dims[0], 4)
    col = col.reshape(dims[2] // 2, 2, dims[1] // 2, 2, dims[0] // 2, 2, 4).mean((1, 3, 5))
    color = np.clip(col * 255 + 0.5, 0, 255).astype(np.uint8)
    # Far from the surface the distance only has to stay a lower bound: clamping it keeps precision near it.
    mascot_file.write(a.out, dist, color, half, center, (-2.0 * a.thick, 0.5), mascot_file.parse_defaults(a.set))
    print(f"сетка {dims} (x, y, z), воксель {vox:.4f}, {a.out}")


if __name__ == "__main__":
    main()
