#pragma once
#include <Adafruit_SH110X.h>
#include "dane.h" 
extern Adafruit_SH1106G display;
extern uint8_t animFaza;
extern uint8_t animLitera;
extern int8_t animY;
extern uint8_t animMiganie;
extern bool animNapisWyswietlony;
extern uint16_t animNapisCzas;
extern uint16_t poprzedniCzasAnimacja;
extern uint8_t zdrowieGracza;
extern uint8_t maxZdrowieGracza;
extern int8_t movement_speed; 
extern bool graczMaButy;
extern const uint8_t STALE_HP_GRACZA;
extern uint8_t maxZdrowieRekina;
extern uint8_t zdrowieRekina;
extern uint8_t maxZdrowieKrokodyla;
extern uint8_t zdrowieKrokodyla;
extern uint8_t maxZdrowieKubka;
extern uint8_t zdrowieKubka;
extern const uint8_t hpRekinaTrudnosc[3];
extern const uint8_t hpKrokodylaTrudnosc[3];
extern const uint8_t hpKubkaTrudnosc[3];
enum TypNagrodyBossa {
  NAGRODA_BUTY = 0,
  NAGRODA_RAKIETA = 1
};
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#endif
void stphase(bool wygrales, TypNagrodyBossa typNagrody);
void bombardiro();
void rysujPaskiZdrowia(int zdrowieopp, int maxZdrowieOpp);
void start();
bool animacjaPoczatkowa();

void rysujIkoneRakietyNagrody() {
  display.drawBitmap(36, 16, rakietaa[3].bitmapa, rakietaa[3].szerokosc, rakietaa[3].wysokosc, SH110X_WHITE);
  display.drawBitmap(52, 16, rakietaa[3].bitmapa, rakietaa[3].szerokosc, rakietaa[3].wysokosc, SH110X_WHITE);
  display.drawBitmap(44, 32, rakietaa[3].bitmapa, rakietaa[3].szerokosc, rakietaa[3].wysokosc, SH110X_WHITE);
}

void start() {
  bool flash = true;
  while (PIND & B00100000) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(10, 10);
    display.println(F("TRIPLE T"));
    if (flash) {
      display.setTextSize(1);
      display.setCursor(15, 35);
      display.println(F("PRESS START"));
    }
    display.display();
    static uint16_t lastMillis = 0;
    if (millis() - lastMillis > 500) {
      flash = !flash;
      lastMillis = millis();
    }
  }
  display.clearDisplay();
  display.setCursor(10, 30);
  display.print(F("LOADING..."));
  display.display();
  delay(1000);
}

bool animacjaPoczatkowa() {
  const int pozX[3]   = {20, 54, 88};
  const int doceloweY = 16;
  uint16_t teraz = millis();

  if (animFaza == 0) {
    if (teraz - poprzedniCzasAnimacja >= 15) {
      poprzedniCzasAnimacja = teraz;
      display.clearDisplay();
      display.setTextSize(4);
      display.setTextColor(SH110X_WHITE);
      for (int i = 0; i < animLitera; i++) {
        display.setCursor(pozX[i], doceloweY);
        display.print("T");
      }
      display.setCursor(pozX[animLitera], animY);
      display.print("T");
      display.display();
      animY += 4;
      if (animY > doceloweY) {
        animLitera++;
        animY = -30;
        if (animLitera >= 3) {
          animFaza = 1;
          animMiganie = 0;
          poprzedniCzasAnimacja = millis();
        }
      }
    }
    return false;
  }

  if (animFaza == 1) {
    if (teraz - poprzedniCzasAnimacja >= 200) {
      poprzedniCzasAnimacja = teraz;
      display.clearDisplay();
      display.setTextSize(4);
      for (int i = 0; i < 3; i++) {
        display.setCursor(pozX[i], doceloweY);
        display.print("T");
      }
      if (animMiganie % 2 == 0) {
        display.drawPixel(10, 10, SH110X_WHITE);
        display.drawPixel(11, 10, SH110X_WHITE);
        display.drawPixel(110, 15, SH110X_WHITE);
        display.drawPixel(64, 5, SH110X_WHITE);
        display.drawPixel(64, 6, SH110X_WHITE);
        display.drawPixel(25, 50, SH110X_WHITE);
        display.drawPixel(105, 45, SH110X_WHITE);
        display.drawPixel(106, 45, SH110X_WHITE);
      } else {
        display.drawPixel(15, 20, SH110X_WHITE);
        display.drawPixel(115, 30, SH110X_WHITE);
        display.drawPixel(116, 30, SH110X_WHITE);
        display.drawPixel(50, 55, SH110X_WHITE);
        display.drawPixel(80, 50, SH110X_WHITE);
        display.drawPixel(81, 50, SH110X_WHITE);
      }
      display.display();
      animMiganie++;
      if (animMiganie >= 6) {
        animFaza = 2;
        animNapisWyswietlony = false;
      }
    }
    return false;
  }

  if (animFaza == 2) {
    if (!animNapisWyswietlony) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(16, 15);
      display.print("Triple T");
      display.setCursor(4, 40);
      display.print("Adventures");
      display.display();
      animNapisCzas = teraz;
      animNapisWyswietlony = true;
    }
    if (teraz - animNapisCzas >= 2500) {
      display.clearDisplay();
      display.display();
      animFaza = 3;
    }
    return false;
  }

  return true;
}
const char* wybierzPoziom() {
  const char* tryb[] = {"easy", "medium", "hard"};
  int8_t wybranyIndeks = 0;
  bool wybrano = false;

  uint16_t start300 = millis();
  while (millis() - start300 < 300) {}

  while (!wybrano) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(20, 5);
    display.print("Difficulty");
    for (int i = 0; i < 3; i++) {
      display.setCursor(30, 27 + (i * 12));
      display.print(wybranyIndeks == i ? "> " : "  ");
      display.print(tryb[i]);
    }
    display.display();

    if (!(PIND & B10000000)) {
      wybranyIndeks--;
      if (wybranyIndeks < 0) wybranyIndeks = 2;
      uint16_t t = millis(); while ((uint16_t)(millis() - t) < 150) {}
    }
    if (!(PINB & B00000010)) {
      wybranyIndeks++;
      if (wybranyIndeks > 2) wybranyIndeks = 0;
      uint16_t t = millis(); while ((uint16_t)(millis() - t) < 150) {}
    }
    if (!(PIND & B00100000)) {
      wybrano = true;
      uint16_t t = millis(); while ((uint16_t)(millis() - t) < 200) {}
      maxZdrowieGracza = STALE_HP_GRACZA;
      zdrowieGracza = maxZdrowieGracza;
      maxZdrowieRekina = hpRekinaTrudnosc[wybranyIndeks];
      zdrowieRekina = maxZdrowieRekina;
      maxZdrowieKrokodyla = hpKrokodylaTrudnosc[wybranyIndeks];
      zdrowieKrokodyla = maxZdrowieKrokodyla;
      maxZdrowieKubka = hpKubkaTrudnosc[wybranyIndeks];
      zdrowieKubka = maxZdrowieKubka;
    }
  }
  return tryb[wybranyIndeks];
}
void stphase(bool wygrales, TypNagrodyBossa typNagrody) {
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  if (wygrales) {
    display.setTextSize(1);
    display.setCursor(30, 0);
    display.println("NEW ITEM");
    if (typNagrody == NAGRODA_BUTY) {
      display.drawBitmap(44, 14, shoes, 40, 40, SH110X_WHITE);
      graczMaButy = true;
      movement_speed++;
    } else {
      rysujIkoneRakietyNagrody();
    }
  } else {
    display.setTextSize(2);
    display.setCursor(20, 0);
    display.println("YOU LOOSE");
  }
  display.display();
}
void bombardiro()
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(15, 6);
    display.print("NEXT BOSS");
    display.setTextSize(2);
    display.setCursor(18, 28);
    display.print("CROCODILE");
    display.display();
  }
int szerokoscPaskaZdrowia(int zdrowie, int maxZdrowie, int maxSzerokosc) {
  if (maxZdrowie <= 0) return 0;
  zdrowie = constrain(zdrowie, 0, maxZdrowie);
  return (static_cast<long>(zdrowie) * maxSzerokosc) / maxZdrowie;
}
void rysujPaskiZdrowia(int zdrowieopp, int maxZdrowieOpp) {
  display.drawRect(0, 0, 26, 4, SH110X_WHITE);
  display.drawRect(76, 0, 52, 4, SH110X_WHITE);
  int szerokoscGracz = szerokoscPaskaZdrowia(zdrowieGracza, maxZdrowieGracza, 24);
  if (szerokoscGracz > 0) {
    display.fillRect(1, 1, szerokoscGracz, 2, SH110X_WHITE);
  }
  int szerokoscRekin = szerokoscPaskaZdrowia(zdrowieopp, maxZdrowieOpp, 50);
  if (szerokoscRekin > 0) {
    display.fillRect(127 - szerokoscRekin, 1, szerokoscRekin, 2, SH110X_WHITE);
  }
}
