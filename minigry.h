#pragma once
#include <Adafruit_SH110X.h>

extern Adafruit_SH1106G display;
extern int8_t dotX;
extern int8_t dotY;
extern uint8_t aktualny_indeks;

inline bool przyciskGoraWcisniety() {
  return !(PIND & B10000000);
}

inline bool przyciskDolWcisniety() {
  return !(PINB & B00000010);
}

inline bool przyciskPrawoWcisniety() {
  return !(PINB & B00000001);
}

inline bool przyciskLewoWcisniety() {
  return !(PINB & B00000100);
}

inline bool przyciskStartWcisniety() {
  return !(PIND & B00100000);
}

inline bool przyciskRakietyWcisniety() {
  return !(PIND & B00010000);
}

inline bool jakikolwiekPrzyciskWcisniety() {
  return przyciskGoraWcisniety() || przyciskDolWcisniety() ||
         przyciskPrawoWcisniety() || przyciskLewoWcisniety() ||
         przyciskStartWcisniety() || przyciskRakietyWcisniety();
}

void czekajNaPuszczeniePrzyciskow() {
  while (jakikolwiekPrzyciskWcisniety()) {
    delay(10);
  }
}

void pokazKomunikatMiniGry(const __FlashStringHelper* linia1,
                           const __FlashStringHelper* linia2,
                           unsigned long czasMs) {
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(18, 20);
  display.print(linia1);
  display.setCursor(18, 34);
  display.print(linia2);
  display.display();
  delay(czasMs);
}

bool miniGraRefleks() {
  const int pasekX = 10;
  const int pasekY = 38;
  const int pasekW = 108;
  const int pasekH = 8;
  const int strefaX = 72;
  const int strefaW = 18;
  const int markerW = 5;
  int markerX = pasekX;
  int kierunek = 2;
  unsigned long ostatniRuch = 0;

  czekajNaPuszczeniePrzyciskow();

  while (true) {
    unsigned long teraz = millis();
    if (teraz - ostatniRuch >= 16) {
      ostatniRuch = teraz;
      markerX += kierunek;
      if (markerX <= pasekX || markerX + markerW >= pasekX + pasekW) {
        kierunek = -kierunek;
        markerX += kierunek;
      }
    }

    if (przyciskStartWcisniety()) {
      czekajNaPuszczeniePrzyciskow();
      bool sukces = markerX >= strefaX && markerX + markerW <= strefaX + strefaW;
      pokazKomunikatMiniGry(sukces ? F("PERFECT!") : F("TOO EARLY!"),
                            sukces ? F("Gate opened") : F("Try again"),
                            700);
      return sukces;
    }

    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(8, 8);
    display.print(F("MINI GAME"));
    display.setCursor(8, 20);
    display.print(F("Hit zone"));
    display.drawRect(pasekX, pasekY, pasekW, pasekH, SH110X_WHITE);
    display.fillRect(strefaX, pasekY + 1, strefaW, pasekH - 2, SH110X_WHITE);
    display.fillRect(markerX, pasekY - 2, markerW, pasekH + 4, SH110X_WHITE);
    display.display();
  }
}

void losujCelMiniGry(int& celX, int& celY) {
  celX = random(10, SCREEN_WIDTH - 10);
  celY = random(18, SCREEN_HEIGHT - 8);
}

bool miniGraZbieranie() {
  const int graczRozmiar = 6;
  const int rozmiarCelu = 4;
  const int limitCzasuMs = 9000;
  int graczX = 10;
  int graczY = 28;
  int celX = 0;
  int celY = 0;
  int zebrane = 0;
  unsigned long startCzas = millis();
  unsigned long ostatniRuch = 0;

  losujCelMiniGry(celX, celY);
  czekajNaPuszczeniePrzyciskow();

  while (zebrane < 3) {
    unsigned long teraz = millis();
    if (teraz - startCzas >= limitCzasuMs) {
      pokazKomunikatMiniGry(F("TIME UP"), F("Try again"), 700);
      return false;
    }

    if (teraz - ostatniRuch >= 16) {
      ostatniRuch = teraz;

      if (przyciskGoraWcisniety()) graczY -= 2;
      if (przyciskDolWcisniety()) graczY += 2;
      if (przyciskPrawoWcisniety()) graczX += 2;
      if (przyciskLewoWcisniety()) graczX -= 2;

      if (graczX < 0) graczX = 0;
      if (graczY < 14) graczY = 14;
      if (graczX + graczRozmiar > SCREEN_WIDTH) graczX = SCREEN_WIDTH - graczRozmiar;
      if (graczY + graczRozmiar > SCREEN_HEIGHT) graczY = SCREEN_HEIGHT - graczRozmiar;

      bool kolX = graczX < celX + rozmiarCelu && graczX + graczRozmiar > celX - rozmiarCelu;
      bool kolY = graczY < celY + rozmiarCelu && graczY + graczRozmiar > celY - rozmiarCelu;
      if (kolX && kolY) {
        zebrane++;
        if (zebrane < 3) {
          losujCelMiniGry(celX, celY);
        }
      }
    }

    int pozostalo = (limitCzasuMs - (teraz - startCzas)) / 1000;

    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(8, 2);
    display.print(F("MINI GAME"));
    display.setCursor(8, 12);
    display.print(F("Collect 3"));
    display.setCursor(96, 2);
    display.print(pozostalo);
    display.drawRect(0, 13, SCREEN_WIDTH, SCREEN_HEIGHT - 13, SH110X_WHITE);
    display.fillRect(graczX, graczY, graczRozmiar, graczRozmiar, SH110X_WHITE);
    display.drawRect(celX - rozmiarCelu, celY - rozmiarCelu,
                     rozmiarCelu * 2, rozmiarCelu * 2, SH110X_WHITE);
    display.setCursor(8, 55);
    display.print(F("Got: "));
    display.print(zebrane);
    display.print(F("/3"));
    display.display();
  }

  pokazKomunikatMiniGry(F("READY"), F("GO"), 700);
  return true;
}

void resetujGraczaPoMiniGrze() {
  dotX = 10;
  dotY = 10;
  aktualny_indeks = 0;
}

void zagrajMiniGrePrzedKrokodylem() {
  pokazKomunikatMiniGry(F("Croc mini"), F("Win mini"), 1200);
  while (!miniGraZbieranie()) {}
  resetujGraczaPoMiniGrze();
  pokazKomunikatMiniGry(F("CROC FIGHT"), F("Ready"), 900);
}

void zagrajMiniGrePrzedFinalem() {
  pokazKomunikatMiniGry(F("Final mini"), F("Win mini"), 1200);
  while (!miniGraRefleks()) {}
  resetujGraczaPoMiniGrze();
  pokazKomunikatMiniGry(F("FINAL FIGHT"), F("Ready"), 900);
}
