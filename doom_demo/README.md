# UNO DOOM Demo

Minimalny szkic `DOOM-like` pod `Arduino Uno` i `SH1106 128x64`.

Założenia:
- ekran obrócony przez `display.setRotation(2)`,
- prosty raycasting zamiast pełnego silnika DOOM,
- jeden cel do trafienia,
- osobny folder, niezależny od głównej gry.

Sterowanie:
- `GORA` - ruch do przodu
- `DOL` - ruch do tyłu
- `LEWO` / `PRAWO` - obrót
- `START` - strzał
- `PD4` - reset celu

To jest punkt startowy pod dalszą rozbudowę: kolejne ściany, przeciwnicy, pickupy, broń i lepszy renderer.
