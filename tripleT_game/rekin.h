#pragma once
#include <Adafruit_SH110X.h>
void rekin()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(15, 6);
    display.print("NEXT BOSS");
    display.setTextSize(2);
    display.setCursor(30, 28);
    display.print("SHARK");
    display.display();
}

uint8_t poziomRageRekina() {
  if (maxZdrowieRekina == 0) return 0;
  if ((uint16_t)zdrowieRekina * 4 <= maxZdrowieRekina) return 2;
  if ((uint16_t)zdrowieRekina * 2 <= maxZdrowieRekina) return 1;
  return 0;
}

uint8_t szybkoscRekinaWBiegu() {
  return poziomRageRekina() == 1 ? predkoscRekina + 1 : predkoscRekina;
}

uint8_t opoznienieRuchuRekina() {
  uint8_t rage = poziomRageRekina();
  if (rage >= 2) return 1;
  if (rage == 1) return 2;
  return 3;
}

void aktualizujRekina() {
  if (rekinZlapany) return; 
  static int opoznienieRuchu = 0;
  uint8_t opoznienieDocelowe = opoznienieRuchuRekina();
  opoznienieRuchu++;
  if(opoznienieRuchu < opoznienieDocelowe) return; 
  opoznienieRuchu = 0;
  uint8_t szybkosc = szybkoscRekinaWBiegu();
  
  int diffX = dotX - rekinX;
  int diffY = dotY - rekinY;

  if (abs(diffX) > abs(diffY)) {
    if (diffX > 0) {
      rekinX += szybkosc;
      rekinIndeks = 2; 
    } else if (diffX < 0) {
      rekinX -= szybkosc;
      rekinIndeks = 3; 
    }
  } else {
    if (diffY > 0) {
      rekinY += szybkosc;
      rekinIndeks = 1; 
    } else if (diffY < 0) {
      rekinY -= szybkosc;
      rekinIndeks = 0; 
    }
  }
  if (rekinY < 6) rekinY = 6;
}
void rysujRekina(uint16_t teraz) {
  if (atakTrafiony && (teraz - atakTrafionyCzas >= CZAS_ANIMACJI_ATAKU)) {
    atakTrafiony = false;
  }
  bool ukryjRekina = atakTrafiony && ((teraz / 80) % 2 != 0);

  if (!ukryjRekina) {
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

  if (atakTrafiony) {
    display.drawBitmap(rekinX + 8, rekinY + 5, atak, 16, 16, SH110X_WHITE);
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
void aktualizujSiatke(uint16_t teraz) {
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
void sprawdzKolizjeZGraczem(uint16_t teraz) {
  if (rekinZlapany) return;
  int marginesGracza = 4; 
  int marginesRekina = 6;
  int gLewo  = dotX + marginesGracza;
  int gPrawo = dotX + graczSzerokosc() - marginesGracza;
  int gGora  = dotY + marginesGracza;
  int gDol   = dotY + graczWysokosc() - marginesGracza;
  int rLewo  = rekinX + marginesRekina;
  int rPrawo = rekinX + REKIN_SIZE - marginesRekina;
  int rGora  = rekinY + marginesRekina;
  int rDol   = rekinY + REKIN_SIZE - marginesRekina;
  bool kolX = (rLewo < gPrawo) && (rPrawo > gLewo);
  bool kolY = (rGora < gDol) && (rDol > gGora);
  if (kolX && kolY) {
    if (teraz - ostatnieUgryzienieCzas >= 500) {
      odejmijZdrowie(zdrowieGracza, 10);
      ostatnieUgryzienieCzas = teraz;
    }
  }
}
