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

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int dotY = 10; 
int dotX = 10;
int aktualny_indeks = 0;
int rekinX        = 90;
int rekinY        = 30;
int rekinDX       =  1;  
int rekinDY       =  0;  
int rekinIndeks   =  2;  
int predkoscRekina = 3;

// --- ZDROWIE ---
int zdrowieGracza = 50;
int zdrowieRekina = 100;
// ---------------

unsigned long poprzedniCzasAnimacja = 0;
unsigned long poprzedniCzasGra      = 0;
int  animFaza             = 0;
int  animLitera           = 0;
int  animY                = -30;
int  animMiganie          = 0;
bool animNapisWyswietlony = false;
unsigned long animNapisCzas = 0;

const unsigned char ludzik_z_kijem[] PROGMEM = {
  0x00, 0x1f, 0xf8, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x1d, 0xd8, 0x07, 0x00, 0x1d, 0xd8, 0x0e,
  0x00, 0x3f, 0xfc, 0x1c, 0x00, 0x3d, 0xbc, 0x38, 0x00, 0x1e, 0x70, 0x70, 0x00, 0x1f, 0xf0, 0xe0,
  0x00, 0x0f, 0xe1, 0xc0, 0x00, 0x0f, 0xe3, 0x80, 0x00, 0x0f, 0xe7, 0x00, 0x00, 0x0f, 0xee, 0x00,
  0x00, 0x0f, 0xf8, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x0c, 0x60, 0x00,
  0x00, 0x0c, 0x60, 0x00, 0x00, 0x0c, 0x60, 0x00, 0x00, 0x0c, 0x60, 0x00, 0x00, 0x0c, 0x60, 0x00,
  0x00, 0x0c, 0x60, 0x00, 0x00, 0x1e, 0xf0, 0x00, 0x00, 0x1e, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char ludzik_z_kijem_lewo[] PROGMEM = {
  0x00, 0x1f, 0xf8, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0xe0, 0x1b, 0xb8, 0x00, 0x70, 0x1b, 0xb8, 0x00,
  0x38, 0x3f, 0xfc, 0x00, 0x1c, 0x3d, 0xbc, 0x00, 0x0e, 0x0e, 0x78, 0x00, 0x07, 0x0f, 0xf8, 0x00,
  0x03, 0x87, 0xf0, 0x00, 0x01, 0xc7, 0xf0, 0x00, 0x00, 0xe7, 0xf0, 0x00, 0x00, 0x77, 0xf0, 0x00,
  0x00, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x06, 0x30, 0x00,
  0x00, 0x06, 0x30, 0x00, 0x00, 0x06, 0x30, 0x00, 0x00, 0x06, 0x30, 0x00, 0x00, 0x06, 0x30, 0x00,
  0x00, 0x06, 0x30, 0x00, 0x00, 0x0f, 0x78, 0x00, 0x00, 0x0f, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char ludzik_z_kijem_gora[] PROGMEM = {
  0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38,
  0x00, 0xff, 0x38, 0x00, 0xff, 0x38, 0x00, 0xff, 0x38, 0x00, 0xff, 0x38, 0x3e, 0xff, 0x78, 0x7e, 0xff, 0x78,
  0x7e, 0xff, 0x78, 0x7e, 0xff, 0x78, 0x7e, 0x00, 0x78, 0x3e, 0xff, 0x70, 0x1c, 0xff, 0x20, 0x00, 0x7e, 0x00,
  0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x24, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char ludzik_z_kijem_dol[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x24, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00,
  0x00, 0x7e, 0x00, 0x04, 0xff, 0x38, 0x0e, 0xff, 0x7c, 0x1e, 0x00, 0x7e, 0x1e, 0xff, 0x7e, 0x1e, 0xff, 0x7e,
  0x1e, 0xff, 0x7e, 0x1e, 0xff, 0x7c, 0x1c, 0xff, 0x00, 0x1c, 0xff, 0x00, 0x1c, 0xff, 0x00, 0x1c, 0xff, 0x00,
  0x1c, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x1c, 0x00, 0x00
};

const unsigned char rekin_gora[] PROGMEM = {
  0x00, 0x18, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7e, 0x00, 0x00, 0xff, 0x00,
  0x01, 0xff, 0x80, 0x01, 0xff, 0x80, 0x03, 0xff, 0xc0, 0x07, 0xff, 0xe0,
  0x0f, 0xff, 0xf0, 0x1f, 0xff, 0xf8, 0x3e, 0xff, 0x7c, 0x3c, 0x7e, 0x3c,
  0x18, 0x7e, 0x18, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00,
  0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x3c, 0x00,
  0x00, 0x7e, 0x00, 0x00, 0xff, 0x00, 0x01, 0xc3, 0x80, 0x01, 0x81, 0x80
};

const unsigned char rekin_dol[] PROGMEM = {
  0x01, 0x81, 0x80, 0x01, 0xc3, 0x80, 0x00, 0xff, 0x00, 0x00, 0x7e, 0x00,
  0x00, 0x3c, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00,
  0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x18, 0x7e, 0x18,
  0x3c, 0x7e, 0x3c, 0x3e, 0xff, 0x7c, 0x1f, 0xff, 0xf8, 0x0f, 0xff, 0xf0,
  0x07, 0xff, 0xe0, 0x03, 0xff, 0xc0, 0x01, 0xff, 0x80, 0x01, 0xff, 0x80,
  0x00, 0xff, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x18, 0x00
};

const unsigned char rekin_lewo[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x3e, 0x00,
  0x01, 0xfe, 0x00, 0x0f, 0xff, 0x60, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0xf8,
  0xff, 0xff, 0xfc, 0x3f, 0xff, 0xf8, 0x0f, 0xff, 0x70, 0x03, 0x7e, 0x00,
  0x00, 0x1e, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char rekin_prawo[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x78, 0x00, 0x00, 0x7c, 0x00,
  0x00, 0x7f, 0x80, 0x06, 0xff, 0xf0, 0x0f, 0xff, 0xfc, 0x1f, 0xff, 0xff,
  0x3f, 0xff, 0xff, 0x1f, 0xff, 0xfc, 0x0e, 0xff, 0xf0, 0x00, 0x7e, 0xc0,
  0x00, 0x78, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// ── Struktury Postac ─────────────────────────────────────────────────────────
struct Postac {
  const unsigned char* bitmapa;
  int szerokosc;
  int wysokosc;
};
Postac ludziki[4] = {
  {ludzik_z_kijem,      BMP_WIDTH,  BMP_HEIGHT},
  {ludzik_z_kijem_lewo, BMP_WIDTH,  BMP_HEIGHT},
  {ludzik_z_kijem_gora, BMP_WIDTH1, BMP_HEIGHT},
  {ludzik_z_kijem_dol,  BMP_WIDTH1, BMP_HEIGHT},
};
Postac rekiny[4] = {
  {rekin_gora,  REKIN_SIZE, REKIN_SIZE},
  {rekin_dol,   REKIN_SIZE, REKIN_SIZE},
  {rekin_prawo, REKIN_SIZE, REKIN_SIZE},
  {rekin_lewo,  REKIN_SIZE, REKIN_SIZE},
};

// ── Start ────────────────────────────────────────────────────────────────────
void start() {
  while (PIND & B00100000) {
    display.clearDisplay();
    display.display();
  }
}

// ── Animacja początkowa ───────────────────────────────────────
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
      unsigned long t = millis(); while (millis() - t < 150) {}
    }
    if (!(PINB & B00000010)) {
      wybranyIndeks++;
      if (wybranyIndeks > 2) wybranyIndeks = 0;
      unsigned long t = millis(); while (millis() - t < 150) {}
    }
    if (!(PIND & B00100000)) {
      wybrano = true;
      unsigned long t = millis(); while (millis() - t < 200) {}
    }
  }
  return tryb[wybranyIndeks];
}

void obslugaWejscia() {
  if (!(PIND & B10000000)) {
    dotY -= 2;
    if (dotY < 6) dotY = 6; // Ograniczenie dla paska zdrowia
    aktualny_indeks = 2;
  }
  if (!(PINB & B00000010)) {
    dotY += 2;
    if (dotY + BMP_HEIGHT > SCREEN_HEIGHT) dotY = SCREEN_HEIGHT - BMP_HEIGHT;
    aktualny_indeks = 3;
  }
  if (!(PINB & B00000001)) {
    dotX += 2;
    if (dotX + BMP_WIDTH > SCREEN_WIDTH) dotX = SCREEN_WIDTH - BMP_WIDTH;
    aktualny_indeks = 0;
  }
  if (!(PINB & B00000100)) {
    dotX -= 2;
    if (dotX < 0) dotX = 0;
    aktualny_indeks = 1;
  }
}

void aktualizujRekina() {
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

void rysujRekina() {
  display.drawBitmap(
    rekinX, rekinY,
    rekiny[rekinIndeks].bitmapa,
    rekiny[rekinIndeks].szerokosc,
    rekiny[rekinIndeks].wysokosc,
    SH110X_WHITE
  );
}
void rysujPaskiZdrowia() {
  display.drawRect(0, 0, 26, 4, SH110X_WHITE);
  display.drawRect(76, 0, 52, 4, SH110X_WHITE);
  int szerokoscGracz = map(zdrowieGracza, 0, 50, 0, 24);
  if (szerokoscGracz > 0) {
    display.fillRect(1, 1, szerokoscGracz, 2, SH110X_WHITE);
  }
  int szerokoscRekin = map(zdrowieRekina, 0, 100, 0, 50);
  if (szerokoscRekin > 0) {
    display.fillRect(127 - szerokoscRekin, 1, szerokoscRekin, 2, SH110X_WHITE);
  }
}

// ── Main ─────────────────────────────────────────────────────────────────────
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
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(15, 25);
  display.print("START: ");
  display.print(aktualnyPoziom);
  display.display();

  unsigned long startLvlCzas = millis();
  while (millis() - startLvlCzas < 1500) {}

  display.clearDisplay();

  while (true) {
    unsigned long teraz = millis();
    if (teraz - poprzedniCzasGra >= 16) {
      poprzedniCzasGra = teraz;

      obslugaWejscia();

      display.clearDisplay();
      rysujPaskiZdrowia();

      // Rysuj Gracza
      display.drawBitmap(
        dotX, dotY,
        ludziki[aktualny_indeks].bitmapa,
        ludziki[aktualny_indeks].szerokosc,
        ludziki[aktualny_indeks].wysokosc,
        SH110X_WHITE
      );

      // Logika gry w zależności od poziomu
      if (strcmp(aktualnyPoziom, "easy") == 0) {
        aktualizujRekina();
        rysujRekina();
      }

      display.display();
    }
  }

  return 0;
}
