# Mascots for the 3D keyboard

A model (OBJ + MTL) is baked into a `.mascot` file: a 16-bit distance field, colors and glow, default settings. The application loads it from the Mascot section.

```
python bake_mascot.py model.obj model.mascot --height 4.2 --res 200 \
    --color stamen=0.62,0.88,1.0,1.0 --set x=1.6 --set turn=60 --set light=6 --set lx=0.2 --set ly=3.59 --set lz=-0.53
```

- `--drop` leaves out extra materials or scene objects (a floor, water);
- `--color material=r,g,b[,glow]` recolors a material over its Kd;
- `--set` stores default settings, listed in `mascot_file.DEFAULTS`.

To change the defaults of a baked file without baking it again: `mascot_file.rewrite(path, mascot_file.parse_defaults(["turn=45"]))`.

Requires numpy and scipy.
