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
bool bombaWybuchTrafila = false;
bool rakieta = false;
bool rakietaLeci = false;
bool rakietaWybucha = false;
int8_t bombaz = 0;
int16_t bombaX = 0;
int8_t bombaY = 0;
uint8_t indeks_bomba = 0;
uint16_t ostatniaZmianaKlatki = 0;

int8_t rakietax = 0;
int8_t rakietay = 0;
uint8_t rakieta_in = 0;

int8_t rakietaWybuchX = 0;
int8_t rakietaWybuchY = 0;
uint8_t indeks_rakieta_wybuch = 0;
uint16_t ostatniaZmianaKlatkiRakiety = 0;

int8_t dotY = 10; 
int8_t dotX = 10;
uint8_t aktualny_indeks = 0;

int8_t rekinX = 90;
int8_t rekinY = 30;
uint8_t rekinIndeks = 2;
uint8_t predkoscRekina = 2;

int16_t krokX = 128;
int8_t krokY = 32;
int8_t krokDX = -2;
uint8_t krokKierunek = 0;

bool graczMaRakiete = false;
bool graczMaButy = false;
uint8_t maxZdrowieKubka = 120;
uint8_t zdrowieKubka = maxZdrowieKubka;
uint8_t kubekX = 84;
uint8_t kubekY = 24;
uint8_t kubekAkcja = 0;
uint8_t kubekFaza = 1;
int8_t kubekDashDX = 0;
int8_t kubekDashDY = 0;
uint8_t kubekCelX = 64;
uint8_t kubekCelY = 32;
bool kubekDashPoziomy = true;
uint16_t kubekAkcjaStart = 0;
uint16_t kubekNastepnaAkcja = 0;
uint16_t kubekOgluszonyDo = 0;
uint16_t kubekWrazliwyDo = 0;
uint16_t kubekHitFlashDo = 0;
bool mieczAktywny[2] = {false, false};
int16_t mieczX[2] = {0, 0};
int16_t mieczY[2] = {0, 0};
int8_t mieczDX[2] = {0, 0};
int8_t mieczDY[2] = {0, 0};
bool plamaAktywna[3] = {false, false, false};
uint8_t plamaX[3] = {0, 0, 0};
uint8_t plamaY[3] = {0, 0, 0};
uint16_t plamaDo[3] = {0, 0, 0};
uint16_t ostatnieObrazenieFinal = 0;
bool rakietaGraczaAktywna = false;
int16_t rakietaGraczaX = 0;
int16_t rakietaGraczaY = 0;
int8_t rakietaGraczaDX = 0;
int8_t rakietaGraczaDY = 0;
uint8_t rakietaGraczaKierunek = 0;
uint16_t rakietaGraczaCooldownDo = 0;
bool przyciskAtakuBylWcisniety = false;
uint16_t przyciskAtakuWcisniecieCzas = 0;
bool przytrzymanieZuzyteNaRakiete = false;

uint16_t ostatnieUgryzienieCzas = 0;
uint16_t ostatniAtakCzas = 0;
bool atakTrafiony = false;
uint16_t atakTrafionyCzas = 0;
const uint16_t CZAS_ANIMACJI_ATAKU = 400U;

uint8_t zdrowieGracza = 0;
const uint8_t STALE_HP_GRACZA = 100;
uint8_t maxZdrowieGracza = STALE_HP_GRACZA;
uint8_t maxZdrowieRekina = 20;
uint8_t zdrowieRekina = maxZdrowieRekina;
uint8_t maxZdrowieKrokodyla = 150;
uint8_t zdrowieKrokodyla = maxZdrowieKrokodyla;

bool          siatkaAktywna      = false;
uint8_t       siatkaX            = 8;
uint8_t       siatkaY            = 8;
uint16_t      ostatniSpawnSiatki = 0;
const uint16_t CZAS_ZAMROZENIA   = 3500U;
bool          rekinZlapany       = false;
uint16_t rekinZlapayCzas    = 0;

uint16_t poprzedniCzasAnimacja = 0;
uint16_t poprzedniCzasGra      = 0;

uint8_t animFaza             = 0;
uint8_t animLitera           = 0;
int8_t  animY                = -30;
uint8_t animMiganie          = 0;
int8_t movement_speed        = 2;
uint16_t dashOstatniCzas = 0;
bool przyciskDashaBylWcisniety = false;
bool animNapisWyswietlony = false;
uint16_t animNapisCzas = 0;

const uint8_t hpRekinaTrudnosc[3] = {15, 20, 30};
const uint8_t hpKrokodylaTrudnosc[3] = {120, 150, 200};
const uint8_t hpKubkaTrudnosc[3] = {100, 120, 160};

inline void odejmijZdrowie(uint8_t& zdrowie, uint8_t obrazenia) {
  if (zdrowie <= obrazenia) {
    zdrowie = 0;
  } else {
    zdrowie -= obrazenia;
  }
}

#include "menu.h"
#include "chodzenie.h"
#include "dane.h"
#include "rekin.h"
#include "krok.h"
#include "minigry.h"
#include "finalboss.h"


int main() {
  init();
  PORTD |= B10110000;
  PORTB |= B00000111;

  uint16_t startCzas = millis();
  while ((uint16_t)(millis() - startCzas) < 250) {}

  display.begin(i2c_Address, true);
  display.setRotation(2);
  display.clearDisplay();
  display.display();

  start();
  while (!animacjaPoczatkowa()) {}
  wybierzPoziom();
  randomSeed(millis());
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(15, 25);
  display.print(F("LOADING ..."));
  display.display();
  delay(500);
  display.clearDisplay();

  while (true) {
    uint16_t teraz = millis();
    if (teraz - poprzedniCzasGra >= 16) {
      poprzedniCzasGra = teraz;
      obslugaWejscia();
      display.clearDisplay();
      rysujPaskiZdrowia(zdrowieRekina, maxZdrowieRekina);
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
      
      if (rekinZlapany && (teraz - rekinZlapayCzas >= 2000)) {
        display.setTextSize(2);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(64, 32); 
        display.print(F("RUN!"));
      }
      display.display();
      
      if (zdrowieGracza <= 0) {
        display.display();
        stphase(false, NAGRODA_BUTY);
        return 0;
      }
      if (zdrowieRekina <= 0) {
        stphase(true, NAGRODA_BUTY);
        display.display();
        delay(2000);
        zagrajMiniGrePrzedKrokodylem();
        display.clearDisplay();
        
        while(1) {
          uint16_t teraz = millis();
          static uint16_t poprzedniCzasBK = 0;
          if (teraz - poprzedniCzasBK >= 16) {
            poprzedniCzasBK = teraz;
            obslugaWejscia();
            display.clearDisplay();
            rysujPaskiZdrowia(zdrowieKrokodyla, maxZdrowieKrokodyla);
            display.drawBitmap(
              dotX, dotY,
              ludziki[aktualny_indeks].bitmapa,
              ludziki[aktualny_indeks].szerokosc,
              ludziki[aktualny_indeks].wysokosc,
              SH110X_WHITE
            );
            aktualizujBK();
            aktualizujBombe(teraz);
            aktualizujRakiete(teraz);
            rysujBK();
            
            if (rakieta) {
              rysujrocka();
            }
            
            rysujWybuchRakiety(teraz);
            
            display.display();
            
            if (zdrowieGracza <= 0) {
              display.display();
              stphase(false, NAGRODA_RAKIETA);
              return 0;
            }
            if (zdrowieKrokodyla <= 0) {
              display.display();
              stphase(true, NAGRODA_RAKIETA);
              graczMaRakiete = true;
              delay(2000);
              zagrajMiniGrePrzedFinalem();
              resetujFinalBossa();
              display.clearDisplay();

              while (1) {
                uint16_t teraz = millis();
                static uint16_t poprzedniCzasFinal = 0;
                if (teraz - poprzedniCzasFinal >= 16) {
                  poprzedniCzasFinal = teraz;
                  obslugaWejscia();
                  obsluzAtakGraczaNaKubka(teraz);
                  aktualizujPlamyKawy(teraz);
                  aktualizujMieczeKubka(teraz);
                  aktualizujRakieteGracza(teraz);
                  aktualizujKubka(teraz);
                  sprawdzKontaktKubkaZGraczem(teraz);

                  display.clearDisplay();
                  rysujPaskiZdrowia(zdrowieKubka, maxZdrowieKubka);
                  display.drawBitmap(
                    dotX, dotY,
                    ludziki[aktualny_indeks].bitmapa,
                    ludziki[aktualny_indeks].szerokosc,
                    ludziki[aktualny_indeks].wysokosc,
                    SH110X_WHITE
                  );
                  rysujPlamyKawy(teraz);
                  rysujMieczeKubka();
                  rysujKubka(teraz);
                  rysujRakieteGracza(teraz);
                  display.display();

                  if (zdrowieGracza <= 0) {
                    display.display();
                    stphase(false, NAGRODA_RAKIETA);
                    return 0;
                  }
                  if (zdrowieKubka <= 0) {
                    zakonczGrePoFinale();
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return 0;
}
