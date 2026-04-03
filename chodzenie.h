#pragma once
#include <Adafruit_SH110X.h>
extern int dotY;
extern int dotX;
extern int aktualny_indeks;
extern int movement_speed;
void obslugaWejscia()
  {
    if (!(PIND & B10000000)) {
    dotY -= movement_speed;
      if (dotY < 6) dotY = 6; 
    aktualny_indeks = 2;
    }
    if (!(PINB & B00000010)) {
    dotY += movement_speed;
      if (dotY + BMP_HEIGHT > SCREEN_HEIGHT) 
        dotY = SCREEN_HEIGHT - BMP_HEIGHT;
    aktualny_indeks = 3;
    }
    if (!(PINB & B00000001)) {
    dotX += movement_speed;
      if (dotX + BMP_WIDTH> SCREEN_WIDTH) 
      dotX = SCREEN_WIDTH-BMP_WIDTH;
    aktualny_indeks = 0;
    }
    if (!(PINB & B00000100)) {
    dotX -= movement_speed;
      if (dotX < 0) dotX = 0;
    aktualny_indeks = 1;
  }
}
void sprawdzAtakGracza(unsigned long teraz) {
  if (!(PIND & B00100000)) { 
    if (teraz - ostatniAtakCzas >= 500) {
      ostatniAtakCzas = teraz;
      bool kolX = (rekinX < dotX - 4 + BMP_WIDTH) && (rekinX + REKIN_SIZE / 2 > dotX - 4);
      bool kolY = (rekinY < dotY + BMP_HEIGHT) && (rekinY + REKIN_SIZE / 2 > dotY);

      if (kolX && kolY) {
        zdrowieRekina -= 5;
        if (zdrowieRekina < 0) {
          zdrowieRekina = 0;
        }
        atakTrafiony      = true;
        atakTrafionyCzas  = teraz;
      }
    }
  }
}