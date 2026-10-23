# Маскоты для 3D-клавиатуры

Модель (OBJ + MTL) запекается в файл `.mascot`: поле расстояний 16 бит, цвета и свечение, заводские настройки. Программа грузит его в разделе Mascot.

```
python bake_mascot.py model.obj model.mascot --height 4.2 --res 200 \
    --color stamen=0.62,0.88,1.0,1.0 --set x=1.6 --set turn=60 --set light=6 --set lx=0.2 --set ly=3.59 --set lz=-0.53
```

- `--drop` выкидывает лишние материалы или объекты сцены (пол, вода);
- `--color материал=r,g,b[,свечение]` перекрашивает материал поверх Kd;
- `--set` задаёт заводские настройки, список в `mascot_file.DEFAULTS`.

Поменять заводские настройки готового файла без перепекания: `mascot_file.rewrite(path, mascot_file.parse_defaults(["turn=45"]))`.

Нужны numpy и scipy.
