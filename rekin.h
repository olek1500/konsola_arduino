#pragma once
#include <Adafruit_SH110X.h>
void rekin()
{
    display.clearDisplay();
    display.drawBitmap(32,16,rekin_w_butach,64,48,SH110X_WHITE);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(15,0);
    display.print("NEXT OPPONENT :");
    display.display();
    display.display();
}
void aktualizujRekina() {
  if (rekinZlapany) return; 
  static int opoznienieRuchu = 0;
  opoznienieRuchu++;
  if(opoznienieRuchu < 3) return; 
  opoznienieRuchu = 0;
  
  int diffX = dotX - rekinX;
  int diffY = dotY - rekinY;

  if (abs(diffX) > abs(diffY)) {
    if (diffX > 0) {
      rekinX += predkoscRekina;
      rekinIndeks = 2; 
    } else if (diffX < 0) {
      rekinX -= predkoscRekina;
      rekinIndeks = 3; 
    }
  } else {
    if (diffY > 0) {
      rekinY += predkoscRekina;
      rekinIndeks = 1; 
    } else if (diffY < 0) {
      rekinY -= predkoscRekina;
      rekinIndeks = 0; 
    }
  }
  if (rekinY < 6) rekinY = 6;
}
void rysujRekina(unsigned long teraz) {
  if (atakTrafiony && (teraz - atakTrafionyCzas >= CZAS_ANIMACJI_ATAKU)) {
    atakTrafiony = false;
  }
  if (atakTrafiony) {
    if ((teraz / 80) % 2 == 0) {
      display.drawBitmap(
        rekinX, rekinY,
        rekiny[rekinIndeks].bitmapa,
        rekiny[rekinIndeks].szerokosc,
        rekiny[rekinIndeks].wysokosc,
        SH110X_WHITE
      );
    }
    display.drawBitmap(rekinX + 8, rekinY + 5, atak, 16, 16, SH110X_WHITE);
  }

  if (rekinZlapany) {
    display.drawBitmap(
      rekinX, rekinY,
      rekin_odwrocony, 
      REKIN_SIZE, REKIN_SIZE,
      SH110X_WHITE
    );
  } 
  else {
    display.drawBitmap(
      rekinX, rekinY,
      rekiny[rekinIndeks].bitmapa,
      rekiny[rekinIndeks].szerokosc,
      rekiny[rekinIndeks].wysokosc,
      SH110X_WHITE
    );
  }
}
void rysujSiatke() {
  if (!siatkaAktywna) return;
  display.drawBitmap(
    siatkaX, siatkaY,
    siatka_bmp,
    SIATKA_SIZE, SIATKA_SIZE,
    SH110X_WHITE
  );
}
void aktualizujSiatke(unsigned long teraz) {
  if (!siatkaAktywna && (teraz - ostatniSpawnSiatki >= SPAWN_INTERWAL)) {
    int margines = 8; 
    siatkaX = random(margines, SCREEN_WIDTH - SIATKA_SIZE - margines);
    siatkaY = random(6 + margines, SCREEN_HEIGHT - SIATKA_SIZE - margines);
    siatkaAktywna = true;
  }

  if (siatkaAktywna && !rekinZlapany) {
    bool kolX = (rekinX < siatkaX + SIATKA_SIZE) && (rekinX + REKIN_SIZE > siatkaX);
    bool kolY = (rekinY < siatkaY + SIATKA_SIZE) && (rekinY + REKIN_SIZE > siatkaY);
    if (kolX && kolY) {
      ostatniSpawnSiatki = teraz;
      rekinZlapany    = true;
      rekinZlapayCzas = teraz;
      siatkaAktywna   = false;
    }
  }

  if (rekinZlapany && (teraz - rekinZlapayCzas >= CZAS_ZAMROZENIA)) {
    rekinZlapany  = false;
    
  }
}
void sprawdzKolizjeZGraczem(unsigned long teraz) {
  if (rekinZlapany) return;
  int marginesGracza = 4; 
  int marginesRekina = 6;
  int gLewo  = dotX + marginesGracza;
  int gPrawo = dotX + BMP_WIDTH - marginesGracza;
  int gGora  = dotY + marginesGracza;
  int gDol   = dotY + BMP_HEIGHT - marginesGracza;
  int rLewo  = rekinX + marginesRekina;
  int rPrawo = rekinX + REKIN_SIZE - marginesRekina;
  int rGora  = rekinY + marginesRekina;
  int rDol   = rekinY + REKIN_SIZE - marginesRekina;
  bool kolX = (rLewo < gPrawo) && (rPrawo > gLewo);
  bool kolY = (rGora < gDol) && (rDol > gGora);
  if (kolX && kolY) {
    if (teraz - ostatnieUgryzienieCzas >= 500) {
      zdrowieGracza -= 10;
      ostatnieUgryzienieCzas = teraz;
      if (zdrowieGracza < 0) zdrowieGracza = 0;
    }
  }
}