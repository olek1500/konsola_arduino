#pragma once
#include <Adafruit_SH110X.h>
extern Adafruit_SH1106G display;
extern int animFaza;
extern int animLitera;
extern int animY;
extern int animMiganie;
extern bool animNapisWyswietlony;
extern unsigned long animNapisCzas;
extern unsigned long poprzedniCzasAnimacja;
extern int zdrowieGracza;
extern unsigned int trudnosc[];

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
    static unsigned long lastMillis = 0;
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
  unsigned long teraz = millis();

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
        display.fillCircle(10,  10, 2, SH110X_WHITE);
        display.fillCircle(110, 15, 1, SH110X_WHITE);
        display.fillCircle(64,   5, 2, SH110X_WHITE);
        display.fillCircle(25,  50, 1, SH110X_WHITE);
        display.fillCircle(105, 45, 2, SH110X_WHITE);
      } else {
        display.fillCircle(15,  20, 1, SH110X_WHITE);
        display.fillCircle(115, 30, 2, SH110X_WHITE);
        display.fillCircle(50,  55, 1, SH110X_WHITE);
        display.fillCircle(80,  50, 2, SH110X_WHITE);
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
  int wybranyIndeks = 0;
  bool wybrano = false;

  unsigned long start300 = millis();
  while (millis() - start300 < 300) {}

  while (!wybrano) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(20, 5);
    display.print("Choose difficulty:");
    for (int i = 0; i < 3; i++) {
      display.setCursor(30, 27 + (i * 12));
      display.print(wybranyIndeks == i ? "> " : "  ");
      display.print(tryb[i]);
    }
    display.display();

    if (!(PIND & B10000000)) {
      wybranyIndeks--;
      if (wybranyIndeks < 0) wybranyIndeks = 2;
      unsigned long t = millis(); while (millis() - t < 150);
    }
    if (!(PINB & B00000010)) {
      wybranyIndeks++;
      if (wybranyIndeks > 2) wybranyIndeks = 0;
      unsigned long t = millis(); while (millis() - t < 150);
    }
    if (!(PIND & B00100000)) {
      wybrano = true;
      unsigned long t = millis(); while (millis() - t < 200);
      zdrowieGracza = trudnosc[wybranyIndeks];
    }
  }
  return tryb[wybranyIndeks];
}