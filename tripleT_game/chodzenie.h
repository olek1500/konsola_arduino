#pragma once
#include <Adafruit_SH110X.h>
#include "dane.h"
extern int8_t dotY;
extern int8_t dotX;
extern uint8_t aktualny_indeks;
extern int8_t movement_speed;
extern bool graczMaButy;
extern bool graczMaRakiete;
extern uint16_t dashOstatniCzas;
extern bool przyciskDashaBylWcisniety;
inline int graczSzerokosc() {
  return ludziki[aktualny_indeks].szerokosc;
}
inline int graczWysokosc() {
  return ludziki[aktualny_indeks].wysokosc;
}
inline bool przyciskDashaWcisniety() {
  return !(PIND & B00010000);
}
inline uint8_t dystansDashaGracza() {
  if (graczMaButy && !graczMaRakiete) {
    return 28;
  }
  return 16;
}
inline uint16_t cooldownDashaGracza() {
  if (graczMaButy && !graczMaRakiete) {
    return 2000U;
  }
  return 450U;
}
inline void wykonajDashGracza() {
  const int dystansDasha = dystansDashaGracza();

  switch (aktualny_indeks) {
    case 0:
      dotX += dystansDasha;
      break;
    case 1:
      dotX -= dystansDasha;
      break;
    case 2:
      dotY -= dystansDasha;
      break;
    default:
      dotY += dystansDasha;
      break;
  }

  if (dotX < 0) dotX = 0;
  if (dotY < 6) dotY = 6;
  if (dotX + graczSzerokosc() > SCREEN_WIDTH) {
    dotX = SCREEN_WIDTH - graczSzerokosc();
  }
  if (dotY + graczWysokosc() > SCREEN_HEIGHT) {
    dotY = SCREEN_HEIGHT - graczWysokosc();
  }
}
void obslugaWejscia()
  {
    uint16_t teraz = millis();
    if (!(PIND & B10000000)) {
    aktualny_indeks = 2;
    dotY -= movement_speed;
      if (dotY < 6) dotY = 6; 
    }
    if (!(PINB & B00000010)) {
    aktualny_indeks = 3;
    dotY += movement_speed;
      if (dotY + graczWysokosc() > SCREEN_HEIGHT)
        dotY = SCREEN_HEIGHT - graczWysokosc();
    }
    if (!(PINB & B00000001)) {
    aktualny_indeks = 0;
    dotX += movement_speed;
      if (dotX + graczSzerokosc() > SCREEN_WIDTH)
      dotX = SCREEN_WIDTH - graczSzerokosc();
    }
    if (!(PINB & B00000100)) {
    aktualny_indeks = 1;
    dotX -= movement_speed;
      if (dotX < 0) dotX = 0;
    }

    bool dashWcisniety = przyciskDashaWcisniety();
    if (graczMaButy && !graczMaRakiete &&
        dashWcisniety && !przyciskDashaBylWcisniety &&
        (uint16_t)(teraz - dashOstatniCzas) >= cooldownDashaGracza()) {
      dashOstatniCzas = teraz;
      wykonajDashGracza();
    }
    przyciskDashaBylWcisniety = dashWcisniety;
}
void sprawdzAtakGracza(uint16_t teraz) {
  if (!(PIND & B00100000)) { 
    if (teraz - ostatniAtakCzas >= 500) {
      ostatniAtakCzas = teraz;
      bool kolX = (rekinX < dotX - 4 + graczSzerokosc()) && (rekinX + REKIN_SIZE / 2 > dotX - 4);
      bool kolY = (rekinY < dotY + graczWysokosc()) && (rekinY + REKIN_SIZE / 2 > dotY);

      if (kolX && kolY) {
        odejmijZdrowie(zdrowieRekina, 5);
        atakTrafiony      = true;
        atakTrafionyCzas  = teraz;
      }
    }
  }
}
