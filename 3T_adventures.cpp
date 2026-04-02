#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address  0x3c
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1
#define BMP_WIDTH      32
#define BMP_HEIGHT     24
#define BMP_WIDTH1     24
#define REKIN_SIZE     24 
#define SIATKA_SIZE    16 
#define SPAWN_INTERWAL 5000UL
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool bombaAktywna = false;
bool czyWybucha = false;
int bombaX = 0;
int bombaY = 0;
int indeks_bomba = 0;
unsigned long ostatniaZmianaKlatki = 0;

int obecnaKlatka = 0;
int dotY = 10; 
int dotX = 10;
int aktualny_indeks = 0;
int rekinX        = 90;
int rekinY        = 30;
int rekinDX       =  1;  
int rekinDY       =  0;  
int rekinIndeks   =  2;  
int predkoscRekina = 2;
int krokX         = 128;
int krokY         = 32;
int krokDX        = -2;
int krokKierunek  = 0;

unsigned long ostatnieUgryzienieCzas = 0;
unsigned long ostatniAtakCzas = 0;
bool atakTrafiony= false;
unsigned long atakTrafionyCzas = 0;
const unsigned long CZAS_ANIMACJI_ATAKU = 400UL;
int zdrowieGracza = 0;
int zdrowieRekina = 10;
int zdrowieKrokodyla = 100;
bool          siatkaAktywna      = false;
int           siatkaX            = 8;
int           siatkaY            = 8;
unsigned long ostatniSpawnSiatki = 0;
const unsigned long CZAS_ZAMROZENIA  = 3500UL;
bool          rekinZlapany       = false;
unsigned long rekinZlapayCzas    = 0;
unsigned long poprzedniCzasAnimacja = 0;
unsigned long poprzedniCzasGra      = 0;
int  animFaza             = 0;
int  animLitera           = 0;
int  animY                = -30;
int  animMiganie          = 0;
int movement_speed =2;
bool animNapisWyswietlony = false;
unsigned long animNapisCzas = 0;
int indeksBK = 0;

unsigned int trudnosc[] =
{
  50, 20, 1
};
#include "menu.h"
#include "chodzenie.h"
#include "dane.h"
#include "rekin.h"
#include "krok.h"





void sprawdzAtakGracza(unsigned long teraz) {
  if (!(PIND & B00100000)) { 
    if (teraz - ostatniAtakCzas >= 500) {
      ostatniAtakCzas = teraz;
      bool kolX = (rekinX < dotX -4 + BMP_WIDTH) && (rekinX + REKIN_SIZE/2 > dotX - 4);
      bool kolY = (rekinY < dotY + BMP_HEIGHT) && (rekinY + REKIN_SIZE/2> dotY);

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


void rysujBK() {
  display.drawBitmap(
      krokX, krokY,
      samolot[krokKierunek].bitmapa, 
      samolot[krokKierunek].szerokosc,
      samolot[krokKierunek].wysokosc,
      SH110X_WHITE);
}
void aktualizujBK() {
  krokX += krokDX; 
  if (krokDX < 0 && krokX < -samolot[0].szerokosc) {
    krokDX = 2;   
    krokKierunek = 1;   
    krokY = dotY;
  }
  else if (krokDX > 0 && krokX > SCREEN_WIDTH) {
    krokDX = -2;
    krokKierunek = 0;
    krokY = dotY;
  }
}
void aktualizujBombe(unsigned long teraz) {
  const unsigned long interwalRuchu = 400UL;
  const unsigned long czasKlatkiWybuchu = 300UL;

  if (!bombaAktywna) {
    if (random(0, 100) < 5) {
      bombaX = krokX;
      bombaY = krokY+12;
      bombaAktywna = true;
      czyWybucha = false;
      indeks_bomba = 0;
      ostatniaZmianaKlatki = teraz;
    }
    return;
  }
  if (!czyWybucha) {
    if (teraz - ostatniaZmianaKlatki >= interwalRuchu) {
      ostatniaZmianaKlatki = teraz;
      bombaY++;
      if (krokY-bombaY<16) { 
        czyWybucha = true;
        indeks_bomba = 0;
      }
    }
    display.drawBitmap(bombaX, bombaY, bomba, 8, 12, SH110X_WHITE);
  } 
  else {
    if (teraz - ostatniaZmianaKlatki >= czasKlatkiWybuchu) {
      ostatniaZmianaKlatki = teraz;
      indeks_bomba++;
    }
    if (indeks_bomba < 3) {
      if(abs(bombaX-dotX)<24 || abs(bombaY-dotY)<24)
      {
        zdrowieGracza-=1;
      }
      display.drawBitmap(
        bombaX, bombaY-4,
        buh[indeks_bomba].bitmapa,
        buh[indeks_bomba].szerokosc,
        buh[indeks_bomba].wysokosc,
        SH110X_WHITE
      );
    } else {
      bombaAktywna = false;
    }
  }
}

int main() {
  init();
  PORTD |= B10110000;
  PORTB |= B00000111;

  unsigned long startCzas = millis();
  while (millis() - startCzas < 250) {}

  display.begin(i2c_Address, true);
  display.setRotation(2);
  display.clearDisplay();
  display.display();

  start();
  while (!animacjaPoczatkowa()) {}

  const char* aktualnyPoziom = wybierzPoziom();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(15, 25);
  display.print(F("LOADING ..."));
  display.display();
  delay(500);
  rekin();
  delay(1000);
  unsigned long startLvlCzas = millis();
  while (millis() - startLvlCzas < 1500) {}

  display.clearDisplay();

  while (true) {
    unsigned long teraz = millis();
    if (teraz - poprzedniCzasGra >= 16) {
      poprzedniCzasGra = teraz;
      obslugaWejscia();
      display.clearDisplay();
      rysujPaskiZdrowia(zdrowieRekina);
      display.drawBitmap(
        dotX, dotY,
        ludziki[aktualny_indeks].bitmapa,
        ludziki[aktualny_indeks].szerokosc,
        ludziki[aktualny_indeks].wysokosc,
        SH110X_WHITE
      );
        aktualizujSiatke(teraz);   
        aktualizujRekina();
        sprawdzKolizjeZGraczem(teraz); 
        sprawdzAtakGracza(teraz);
        rysujSiatke();             
        rysujRekina(teraz);
        if (rekinZlapany && (teraz - rekinZlapayCzas >= 2000)){
          display.setTextSize(2);
          display.setTextColor(SH110X_WHITE);
          display.setCursor(64, 32); 
          display.print(F("RUN!"));
        }
        display.display();
      if (zdrowieGracza <= 0) {
        display.display();
        stphase(false); 
        return 0;
      }
      if (zdrowieRekina <= 0) {
        stphase(true);
          display.display();
          delay(2000);
          bombardiro();
          delay(2000);
          display.clearDisplay();
        while(1){
    unsigned long teraz = millis();
    static unsigned long poprzedniCzasBK = 0;
  if (teraz - poprzedniCzasBK >= 16) {
    poprzedniCzasBK = teraz;
    obslugaWejscia();
    display.clearDisplay();
    rysujPaskiZdrowia(zdrowieKrokodyla);
    display.drawBitmap(
      dotX, dotY,
      ludziki[aktualny_indeks].bitmapa,
      ludziki[aktualny_indeks].szerokosc,
      ludziki[aktualny_indeks].wysokosc,
      SH110X_WHITE
    );
    aktualizujBK();
    aktualizujBombe(teraz);
    rysujBK();
    display.display();
    if (zdrowieGracza <= 0) {
        display.display();
        stphase(false); 
        return 0;
      }
  }
}
      }
    }
    display.display();
    }
return 0;
}