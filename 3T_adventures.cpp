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

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int dotY = 0;
int dotX = 0;

// Gdy chodzi w prawo (Kij grubszy na dłuższym odcinku)
const unsigned char ludzik_z_kijem[] PROGMEM = {
  // Szerokość 32 px (4 bajty), Wysokość 24 px
  0x00, 0x1f, 0xf8, 0x00, 
  0x00, 0x1f, 0xf8, 0x00, 
  0x00, 0x1d, 0xd8, 0x07, // Oczy + kij (3px)
  0x00, 0x1d, 0xd8, 0x0e, // Oczy + kij (3px)
  0x00, 0x3f, 0xfc, 0x1c, // Twarz + kij (3px)
  0x00, 0x3d, 0xbc, 0x38, // Uśmiech + kij (3px)
  0x00, 0x1e, 0x70, 0x70, // Kij (3px)
  0x00, 0x1f, 0xf0, 0xe0, // Kij (3px)
  0x00, 0x0f, 0xe1, 0xc0, // Tułów + kij (3px)
  0x00, 0x0f, 0xe3, 0x80, // Tułów + kij (3px)
  0x00, 0x0f, 0xe7, 0x00, // Tułów + kij (3px)
  0x00, 0x0f, 0xee, 0x00, // Tułów + kij (3px)
  0x00, 0x0f, 0xf8, 0x00, // Koniec kija schowany przy ciele (zwężenie do 2px)
  0x00, 0x0f, 0xe0, 0x00, 
  0x00, 0x0f, 0xe0, 0x00, 
  0x00, 0x0c, 0x60, 0x00, // Nogi
  0x00, 0x0c, 0x60, 0x00,
  0x00, 0x0c, 0x60, 0x00,
  0x00, 0x0c, 0x60, 0x00,
  0x00, 0x0c, 0x60, 0x00,
  0x00, 0x0c, 0x60, 0x00,
  0x00, 0x1e, 0xf0, 0x00, // Stopy
  0x00, 0x1e, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x00
};

// Gdy chodzi w lewo (Kij grubszy na dłuższym odcinku)
const unsigned char ludzik_z_kijem_lewo[] PROGMEM = {
  0x00, 0x1f, 0xf8, 0x00, 
  0x00, 0x1f, 0xf8, 0x00, 
  0xe0, 0x1b, 0xb8, 0x00, // Oczy + kij (3px)
  0x70, 0x1b, 0xb8, 0x00, // Oczy + kij (3px)
  0x38, 0x3f, 0xfc, 0x00, // Twarz + kij (3px)
  0x1c, 0x3d, 0xbc, 0x00, // Uśmiech + kij (3px)
  0x0e, 0x0e, 0x78, 0x00, // Kij (3px)
  0x07, 0x0f, 0xf8, 0x00, // Kij (3px)
  0x03, 0x87, 0xf0, 0x00, // Tułów + kij (3px)
  0x01, 0xc7, 0xf0, 0x00, // Tułów + kij (3px)
  0x00, 0xe7, 0xf0, 0x00, // Tułów + kij (3px)
  0x00, 0x77, 0xf0, 0x00, // Tułów + kij (3px)
  0x00, 0x1f, 0xf0, 0x00, // Koniec kija schowany przy ciele
  0x00, 0x07, 0xf0, 0x00, 
  0x00, 0x07, 0xf0, 0x00, 
  0x00, 0x06, 0x30, 0x00, // Nogi
  0x00, 0x06, 0x30, 0x00,
  0x00, 0x06, 0x30, 0x00,
  0x00, 0x06, 0x30, 0x00,
  0x00, 0x06, 0x30, 0x00,
  0x00, 0x06, 0x30, 0x00,
  0x00, 0x0f, 0x78, 0x00, // Stopy
  0x00, 0x0f, 0x78, 0x00,
  0x00, 0x00, 0x00, 0x00
};

struct Postac {
  const unsigned char* bitmapa;
  int szerokosc;
  int wysokosc;
};

int main() {
  init();

  DDRD = B10000000;  // pin 7 jako wyjście (góra?), reszta wejścia
  DDRB = B00000000;  // piny 8,9,10 jako wejścia (przyciski)
  PORTD |= B10000000; // pull-up na pinie 7
  PORTB |= B00000111; // pull-up na pinach 8,9,10
  
  int aktualny_indeks = 0;
  delay(250); // Czas na ustabilizowanie się zasilania OLEDa
  
  display.begin(i2c_Address, true);
  display.setRotation(2); 
  display.clearDisplay();
  display.display();
  
  Postac ludziki[2] = {
    {ludzik_z_kijem, BMP_WIDTH, BMP_HEIGHT},       // Indeks 0 (Prawo)
    {ludzik_z_kijem_lewo, BMP_WIDTH, BMP_HEIGHT}   // Indeks 1 (Lewo)
  };
  
  while (true) {
    // Odczyt przycisków
    if (!(PIND & B10000000)) { // góra - pin 7
      dotY -= 2;
      if (dotY < 0) dotY = 0;
    }
    if (!(PINB & B00000010)) { // dół - pin 9
      dotY += 2;
      if (dotY + BMP_HEIGHT > SCREEN_HEIGHT) dotY = SCREEN_HEIGHT - BMP_HEIGHT;
    }
    if (!(PINB & B00000001)) { // prawy - pin 8
      dotX += 2;
      if (dotX + BMP_WIDTH > SCREEN_WIDTH) dotX = SCREEN_WIDTH - BMP_WIDTH;
      aktualny_indeks = 0;
    }
    if (!(PINB & B00000100)) { // lewy - pin 10
      dotX -= 2;
      if (dotX < 0) dotX = 0;
      aktualny_indeks = 1;
    }

    // Renderowanie klatki
    display.clearDisplay();
    display.drawBitmap(
        dotX, 
        dotY, 
        ludziki[aktualny_indeks].bitmapa, 
        ludziki[aktualny_indeks].szerokosc, 
        ludziki[aktualny_indeks].wysokosc, 
        SH110X_WHITE
    );
    display.display();
    
    delay(16); // ~60 FPS
  }

  return 0;
}