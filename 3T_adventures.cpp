#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BMP_WIDTH  32
#define BMP_HEIGHT 24
#define BMP_WIDTH1  24

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ── Globalne zmienne stanu gry ──────────────────────────────────────────────
int dotY = 0;
int dotX = 0;
int aktualny_indeks = 0;

// Rekin (Poziom 1)
int rekinX = 90;
int rekinY = 45;
int rekinKierunek = -1;
int predkoscRekina = 1;

// ── Bitmapy ─────────────────────────────────────────────────────────────────
const unsigned char ludzik_z_kijem[] PROGMEM = {
  0x00, 0x1f, 0xf8, 0x00, 
  0x00, 0x1f, 0xf8, 0x00, 
  0x00, 0x1d, 0xd8, 0x07,
  0x00, 0x1d, 0xd8, 0x0e,
  0x00, 0x3f, 0xfc, 0x1c,
  0x00, 0x3d, 0xbc, 0x38,
  0x00, 0x1e, 0x70, 0x70,
  0x00, 0x1f, 0xf0, 0xe0,
  0x00, 0x0f, 0xe1, 0xc0,
  0x00, 0x0f, 0xe3, 0x80,
  0x00, 0x0f, 0xe7, 0x00,
  0x00, 0x0f, 0xee, 0x00,
  0x00, 0x0f, 0xf8, 0x00,
  0x00, 0x0f, 0xe0, 0x00, 
  0x00, 0x0f, 0xe0, 0x00, 
  0x00, 0x0c, 0x60, 0x00,
  0x00, 0x0c, 0x60, 0x00,
  0x00, 0x0c, 0x60, 0x00,
  0x00, 0x0c, 0x60, 0x00,
  0x00, 0x0c, 0x60, 0x00,
  0x00, 0x0c, 0x60, 0x00,
  0x00, 0x1e, 0xf0, 0x00,
  0x00, 0x1e, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x00
};
const unsigned char ludzik_z_kijem_lewo[] PROGMEM = {
  0x00, 0x1f, 0xf8, 0x00, 
  0x00, 0x1f, 0xf8, 0x00, 
  0xe0, 0x1b, 0xb8, 0x00,
  0x70, 0x1b, 0xb8, 0x00,
  0x38, 0x3f, 0xfc, 0x00,
  0x1c, 0x3d, 0xbc, 0x00,
  0x0e, 0x0e, 0x78, 0x00,
  0x07, 0x0f, 0xf8, 0x00,
  0x03, 0x87, 0xf0, 0x00,
  0x01, 0xc7, 0xf0, 0x00,
  0x00, 0xe7, 0xf0, 0x00,
  0x00, 0x77, 0xf0, 0x00,
  0x00, 0x1f, 0xf0, 0x00,
  0x00, 0x07, 0xf0, 0x00, 
  0x00, 0x07, 0xf0, 0x00, 
  0x00, 0x06, 0x30, 0x00,
  0x00, 0x06, 0x30, 0x00,
  0x00, 0x06, 0x30, 0x00,
  0x00, 0x06, 0x30, 0x00,
  0x00, 0x06, 0x30, 0x00,
  0x00, 0x06, 0x30, 0x00,
  0x00, 0x0f, 0x78, 0x00,
  0x00, 0x0f, 0x78, 0x00,
  0x00, 0x00, 0x00, 0x00
};
const unsigned char ludzik_z_kijem_gora[] PROGMEM = {
  0x00, 0x00, 0x38,
  0x00, 0x00, 0x38,
  0x00, 0x00, 0x38,
  0x00, 0x00, 0x38,
  0x00, 0x00, 0x38,
  0x00, 0x00, 0x38,
  0x00, 0xff, 0x38,
  0x00, 0xff, 0x38,
  0x00, 0xff, 0x38,
  0x00, 0xff, 0x38,
  0x3e, 0xff, 0x78,
  0x7e, 0xff, 0x78,
  0x7e, 0xff, 0x78,
  0x7e, 0xff, 0x78,
  0x7e, 0x00, 0x78,
  0x3e, 0xff, 0x70,
  0x1c, 0xff, 0x20,
  0x00, 0x7e, 0x00,
  0x00, 0x3c, 0x00,
  0x00, 0x3c, 0x00,
  0x00, 0x24, 0x00,
  0x00, 0x24, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00
};
const unsigned char ludzik_z_kijem_dol[] PROGMEM = {
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x24, 0x00,
  0x00, 0x24, 0x00,
  0x00, 0x3c, 0x00,
  0x00, 0x3c, 0x00,
  0x00, 0x7e, 0x00,
  0x04, 0xff, 0x38,
  0x0e, 0xff, 0x7c,
  0x1e, 0x00, 0x7e,
  0x1e, 0xff, 0x7e,
  0x1e, 0xff, 0x7e,
  0x1e, 0xff, 0x7e,
  0x1e, 0xff, 0x7c,
  0x1c, 0xff, 0x00,
  0x1c, 0xff, 0x00,
  0x1c, 0xff, 0x00,
  0x1c, 0xff, 0x00,
  0x1c, 0x00, 0x00,
  0x1c, 0x00, 0x00,
  0x1c, 0x00, 0x00,
  0x1c, 0x00, 0x00,
  0x1c, 0x00, 0x00,
  0x1c, 0x00, 0x00
};
const unsigned char pletwa_rekina[] PROGMEM = {
  0x01, 0x80,
  0x03, 0xc0,
  0x07, 0xe0,
  0x0f, 0xf0,
  0x1f, 0xf8,
  0x3f, 0xfc,
  0x7f, 0xfe,
  0xff, 0xff
};

struct Postac {
  const unsigned char* bitmapa;
  int szerokosc;
  int wysokosc;
};

Postac ludziki[4] = {
  {ludzik_z_kijem,      BMP_WIDTH,  BMP_HEIGHT},  // 0: Prawo
  {ludzik_z_kijem_lewo, BMP_WIDTH,  BMP_HEIGHT},  // 1: Lewo
  {ludzik_z_kijem_gora, BMP_WIDTH1, BMP_HEIGHT},  // 2: Góra
  {ludzik_z_kijem_dol,  BMP_WIDTH1, BMP_HEIGHT},  // 3: Dół
};

// ── Funkcje pomocnicze ───────────────────────────────────────────────────────

void start() {
  while (PIND & B00100000)
    display.clearDisplay();
    display.display();
}

void animacjaPoczatkowa() {
  display.setTextSize(4);
  display.setTextColor(SH110X_WHITE);
  
  int pozX[3] = {20, 54, 88}; 
  int doceloweY = 16; 
  
  for (int litera = 0; litera < 3; litera++) {
    for (int y = -30; y <= doceloweY; y += 4) {
      display.clearDisplay();
      for (int i = 0; i < litera; i++) {
        display.setCursor(pozX[i], doceloweY);
        display.print("T");
      }
      display.setCursor(pozX[litera], y);
      display.print("T");
      display.display();
      delay(15); 
    }
  }
  
  for (int miganie = 0; miganie < 6; miganie++) {
    display.clearDisplay();
    display.setTextSize(4);
    for (int i = 0; i < 3; i++) {
      display.setCursor(pozX[i], doceloweY);
      display.print("T");
    }
    if (miganie % 2 == 0) {
      display.fillCircle(10, 10, 2, SH110X_WHITE);
      display.fillCircle(110, 15, 1, SH110X_WHITE);
      display.fillCircle(64, 5, 2, SH110X_WHITE);
      display.fillCircle(25, 50, 1, SH110X_WHITE);
      display.fillCircle(105, 45, 2, SH110X_WHITE);
    } else {
      display.fillCircle(15, 20, 1, SH110X_WHITE);
      display.fillCircle(115, 30, 2, SH110X_WHITE);
      display.fillCircle(50, 55, 1, SH110X_WHITE);
      display.fillCircle(80, 50, 2, SH110X_WHITE);
    }
    display.display();
    delay(150);
  }
  
  display.clearDisplay();
  display.setTextSize(2); 
  display.setCursor(16, 15);
  display.print("Triple T");
  display.setCursor(4, 40);
  display.print("Adventures");
  display.display();
  delay(2500); 
  display.clearDisplay();
  display.display();
  delay(200); 
}

int wybierzPoziom() {
  int wybranyPoziom = 1;
  bool wybrano = false;
  delay(300);

  while (!wybrano) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(20, 5);
    display.print("Wybierz poziom:");

    for (int i = 1; i <= 3; i++) {
      display.setCursor(30, 15 + (i * 12));
      display.print(wybranyPoziom == i ? "> " : "  ");
      display.print("Poziom ");
      display.print(i);
    }
    display.display();

    if (!(PIND & B10000000)) { wybranyPoziom--; if (wybranyPoziom < 1) wybranyPoziom = 3; delay(150); }
    if (!(PINB & B00000010)) { wybranyPoziom++; if (wybranyPoziom > 3) wybranyPoziom = 1; delay(150); }
    if (!(PIND & B00100000)) { wybrano = true; delay(200); }
  }
  return wybranyPoziom;
}

// ── Funkcje wydzielone z pętli głównej ──────────────────────────────────────

void obslugaWejscia() {
  if (!(PIND & B10000000)) {  // Góra – pin 7
    dotY -= 2;
    if (dotY < 0) dotY = 0;
    aktualny_indeks = 2;
  }
  if (!(PINB & B00000010)) {  // Dół – pin 9
    dotY += 2;
    if (dotY + BMP_HEIGHT > SCREEN_HEIGHT) dotY = SCREEN_HEIGHT - BMP_HEIGHT;
    aktualny_indeks = 3;
  }
  if (!(PINB & B00000001)) {  // Prawo – pin 8
    dotX += 2;
    if (dotX + BMP_WIDTH > SCREEN_WIDTH) dotX = SCREEN_WIDTH - BMP_WIDTH;
    aktualny_indeks = 0;
  }
  if (!(PINB & B00000100)) {  // Lewo – pin 10
    dotX -= 2;
    if (dotX < 0) dotX = 0;
    aktualny_indeks = 1;
  }
}

void aktualizujRekina() {
  rekinX += rekinKierunek * predkoscRekina;
  if (rekinX <= 60)  rekinKierunek =  1;
  if (rekinX >= 110) rekinKierunek = -1;
}

void rysujPoziom1() {
  display.drawLine(60, rekinY + 8, 128, rekinY + 8, SH110X_WHITE);
  display.drawBitmap(rekinX, rekinY, pletwa_rekina, 16, 8, SH110X_WHITE);
}

// ── Main ─────────────────────────────────────────────────────────────────────

int main() {
  init();

  PORTD |= B10110000;
  PORTB |= B00000111;

  delay(250);
  display.begin(i2c_Address, true);
  display.setRotation(2); 
  display.clearDisplay();
  display.display();

  start();
  animacjaPoczatkowa();

  int aktualnyPoziom = wybierzPoziom();

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(15, 25);
  display.print("START Lvl ");
  display.print(aktualnyPoziom);
  display.display();
  delay(1500);
  display.clearDisplay();

  while (true) {
    obslugaWejscia();

    display.clearDisplay();
    display.drawBitmap(dotX, dotY,
      ludziki[aktualny_indeks].bitmapa,
      ludziki[aktualny_indeks].szerokosc,
      ludziki[aktualny_indeks].wysokosc,
      SH110X_WHITE);

    if (aktualnyPoziom == 1) {
      aktualizujRekina();
      rysujPoziom1();
    }

    display.display();
    delay(16);
  }

  return 0;
}
