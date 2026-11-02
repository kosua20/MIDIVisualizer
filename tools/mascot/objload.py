import os, numpy as np
def load_mtl(path):
    mats, cur = {}, None
    if not os.path.exists(path): return mats
    for line in open(path, errors='ignore'):
        t = line.split()
        if not t: continue
        if t[0] == 'newmtl': cur = ' '.join(t[1:]); mats[cur] = {'Kd': (0.8,0.8,0.8)}
        elif cur and t[0] in ('Kd','Ke'): mats[cur][t[0]] = tuple(map(float, t[1:4]))
    return mats
def load_obj(path):
    """Returns triangles (N,3,3), material name per triangle, object name per triangle, materials."""
    V, tris, tm, to = [], [], [], []
    mats, mat, obj = {}, None, None
    for line in open(path, errors='ignore'):
        if line.startswith('v '):
            V.append(tuple(map(float, line.split()[1:4])))
        elif line.startswith('f '):
            idx = [int(x.split('/')[0]) for x in line.split()[1:]]
            idx = [i - 1 if i > 0 else len(V) + i for i in idx]
            for k in range(1, len(idx) - 1):
                tris.append((idx[0], idx[k], idx[k+1])); tm.append(mat); to.append(obj)
        elif line.startswith('usemtl'): mat = line.split(None, 1)[1].strip()
        elif line.startswith('o ') or line.startswith('g '): obj = line.split(None, 1)[1].strip()
        elif line.startswith('mtllib'):
            mats.update(load_mtl(os.path.join(os.path.dirname(path), line.split(None, 1)[1].strip())))
    V = np.array(V, 'f8')
    return V[np.array(tris)], np.array(tm, object), np.array(to, object), mats
