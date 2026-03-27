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
int dotY = 0;
int dotX = 0;
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
const unsigned char ludzik_z_kijem_gora[] PROGMEM = {
  // Szerokość 24 px (3 bajty), Wysokość 24 px
  0x00, 0x00, 0x38, // Wiersz 0: Kij (gruby)
  0x00, 0x00, 0x38, // Wiersz 1: Kij
  0x00, 0x00, 0x38, // Wiersz 2: Kij
  0x00, 0x00, 0x38, // Wiersz 3: Kij
  0x00, 0x00, 0x38, // Wiersz 4: Kij
  0x00, 0x00, 0x38, // Wiersz 5: Kij
  0x00, 0xff, 0x38, // Wiersz 6: Prostokątna głowa + Kij
  0x00, 0xff, 0x38, // Wiersz 7: Głowa + Kij
  0x00, 0xff, 0x38, // Wiersz 8: Głowa + Kij
  0x00, 0xff, 0x38, // Wiersz 9: Głowa + Kij
  0x3e, 0xff, 0x78, // Wiersz 10: Lewe ramię + Głowa + Prawe ramię z kijem (1px pustej przestrzeni wokół głowy!)
  0x7e, 0xff, 0x78, // Wiersz 11: Barki + Głowa
  0x7e, 0xff, 0x78, // Wiersz 12: Barki + Głowa
  0x7e, 0xff, 0x78, // Wiersz 13: Barki + Głowa
  0x7e, 0x00, 0x78, // Wiersz 14: Przerwa za głową (odcina głowę od pleców, daje efekt 3D)
  0x3e, 0xff, 0x70, // Wiersz 15: Plecy i łokcie
  0x1c, 0xff, 0x20, // Wiersz 16: Dolne plecy
  0x00, 0x7e, 0x00, // Wiersz 17: Pas
  0x00, 0x3c, 0x00, // Wiersz 18: Nogi z tyłu
  0x00, 0x3c, 0x00, // Wiersz 19: Nogi
  0x00, 0x24, 0x00, // Wiersz 20: Stopy
  0x00, 0x24, 0x00, // Wiersz 21: Stopy
  0x00, 0x00, 0x00, // Wiersz 22
  0x00, 0x00, 0x00  // Wiersz 23
};
const unsigned char ludzik_z_kijem_dol[] PROGMEM = {
  0x00, 0x00, 0x00, // Wiersz 0: Pusty
  0x00, 0x00, 0x00, // Wiersz 1: Pusty
  0x00, 0x24, 0x00, // Wiersz 2: Stopy
  0x00, 0x24, 0x00, // Wiersz 3: Stopy
  0x00, 0x3c, 0x00, // Wiersz 4: Nogi
  0x00, 0x3c, 0x00, // Wiersz 5: Nogi
  0x00, 0x7e, 0x00, // Wiersz 6: Pas
  0x04, 0xff, 0x38, // Wiersz 7: Dolne plecy
  0x0e, 0xff, 0x7c, // Wiersz 8: Plecy i łokcie
  0x1e, 0x00, 0x7e, // Wiersz 9: Przerwa za głową (efekt 3D)
  0x1e, 0xff, 0x7e, // Wiersz 10: Barki + Głowa
  0x1e, 0xff, 0x7e, // Wiersz 11: Barki + Głowa
  0x1e, 0xff, 0x7e, // Wiersz 12: Barki + Głowa
  0x1e, 0xff, 0x7c, // Wiersz 13: Lewe ramię + Głowa + Prawe ramię z kijem
  0x1c, 0xff, 0x00, // Wiersz 14: Kij + Głowa
  0x1c, 0xff, 0x00, // Wiersz 15: Kij + Głowa
  0x1c, 0xff, 0x00, // Wiersz 16: Kij + Głowa
  0x1c, 0xff, 0x00, // Wiersz 17: Kij + Prostokątna głowa
  0x1c, 0x00, 0x00, // Wiersz 18: Kij (gruby)
  0x1c, 0x00, 0x00, // Wiersz 19: Kij
  0x1c, 0x00, 0x00, // Wiersz 20: Kij
  0x1c, 0x00, 0x00, // Wiersz 21: Kij
  0x1c, 0x00, 0x00, // Wiersz 22: Kij
  0x1c, 0x00, 0x00  // Wiersz 23: Kij
};
// Płetwa rekina (Szerokość: 16 px, Wysokość: 8 px)
const unsigned char pletwa_rekina[] PROGMEM = {
  0x01, 0x80, //        **
  0x03, 0xc0, //       ****
  0x07, 0xe0, //      ******
  0x0f, 0xf0, //     ********
  0x1f, 0xf8, //    **********
  0x3f, 0xfc, //   ************
  0x7f, 0xfe, //  **************
  0xff, 0xff  // ****************
};
void start()
{ while(PIND & B00100000)
    display.clearDisplay();
    display.display();
}
struct Postac {
  const unsigned char* bitmapa;
  int szerokosc;
  int wysokosc;
};

void animacjaPoczatkowa() {
  display.setTextSize(4); // Bardzo duża czcionka dla głównych liter
  display.setTextColor(SH110X_WHITE);
  
  int pozX[3] = {20, 54, 88}; 
  int doceloweY = 16; 
  
  // 1. Spadające litery "T" (zostawiamy, bo robią super wrażenie)
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
  
  // 2. Efekt migotania z GWIAZDKAMI
  for (int miganie = 0; miganie < 6; miganie++) { // 6 klatek migania
    display.clearDisplay();
    
    // Rysujemy nasze 3 litery T
    display.setTextSize(4);
    for (int i = 0; i < 3; i++) {
      display.setCursor(pozX[i], doceloweY);
      display.print("T");
    }
    
    // Rysujemy "gwiazdki" (małe kółka), zmieniając ich układ co drugą klatkę
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
    delay(150); // Czas trwania jednej klatki migania gwiazdek
  }
  
  // 3. Wyczyszczenie ekranu i wyświetlenie pełnego tytułu
  display.clearDisplay();
  
  // Ustawiamy rozmiar czcionki na 2 (szerokość litery to ok. 12 pikseli)
  display.setTextSize(2); 
  
  // Napis "Triple T" (wyśrodkowany: ekran ma 128px, napis 8 liter * 12px = 96px. Zostaje 32px, czyli po 16px marginesu)
  display.setCursor(16, 15);
  display.print("Triple T");
  
  // Napis "Adventures" (wyśrodkowany: 10 liter * 12px = 120px. Margines: 4px)
  display.setCursor(4, 40);
  display.print("Adventures");
  
  display.display();
  
  // Zatrzymujemy napis na ekranie na 2.5 sekundy, żeby gracz zdążył przeczytać
  delay(2500); 
  
  // 4. Ostatnie czyszczenie przed wczytaniem ludzika
  display.clearDisplay();
  display.display();
  delay(200); 
}
int wybierzPoziom() {
  int wybranyPoziom = 1; // Zaczynamy z kursorem na poziomie 1
  bool wybrano = false;

  // Czekamy chwilę, żeby użytkownik nie przeklikał ekranu z rozpędu po wciśnięciu "START"
  delay(300);

  while (!wybrano) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);

    // Nagłówek menu
    display.setCursor(20, 5);
    display.print("Wybierz poziom:");

    // Wyświetlanie opcji
    for (int i = 1; i <= 3; i++) {
      display.setCursor(30, 15 + (i * 12));
      
      // Jeśli opcja jest aktualnie zaznaczona, rysujemy "strzałkę"
      if (wybranyPoziom == i) {
        display.print("> ");
      } else {
        display.print("  ");
      }
      
      display.print("Poziom ");
      display.print(i);
    }
    
    display.display();

    // OBSŁUGA PRZYCISKÓW MENU
    // Góra (Pin 7)
    if (!(PIND & B10000000)) {
      wybranyPoziom--;
      if (wybranyPoziom < 1) wybranyPoziom = 3; // Zapętlenie na dół
      delay(150); // Debounce - opóźnienie, by kursor nie skakał jak szalony
    }
    
    // Dół (Pin 9)
    if (!(PINB & B00000010)) {
      wybranyPoziom++;
      if (wybranyPoziom > 3) wybranyPoziom = 1; // Zapętlenie na górę
      delay(150); // Debounce
    }

    // Zatwierdzenie (Pin 5 - ten sam co w start() )
    if (!(PIND & B00100000)) {
      wybrano = true;
      delay(200); // Chwila przerwy na "odskoczenie" przycisku
    }
  }

  // Zwracamy numer poziomu (1, 2 lub 3), żeby go później użyć
  return wybranyPoziom;
}
int main() {
  init();

  PORTD |= B10110000; // pull-up na pinie 7,5,4
  PORTB |= B00000111; // pull-up na pinach 8,9,10
  
  int aktualny_indeks = 0;
  delay(250); // Czas na ustabilizowanie się zasilania OLEDa

  // UWAGA: Inicjalizacja ekranu musi być przed użyciem jakichkolwiek funkcji rysowania!
  display.begin(i2c_Address, true);
  display.setRotation(2); 
  display.clearDisplay();
  display.display();

  // Oczekiwanie na przycisk START
  start();

  // ODPALAMY NASZĄ NOWĄ ANIMACJĘ INTRO!
  animacjaPoczatkowa();
int aktualnyPoziom = wybierzPoziom();

  // Opcjonalnie: Krótki komunikat, żeby gracz wiedział, co wybrał
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(15, 25);
  display.print("START Lvl ");
  display.print(aktualnyPoziom);
  display.display();
  delay(1500); // Wyświetlamy napis przez 1.5 sekundy
  display.clearDisplay();

  Postac ludziki[4] = {
    {ludzik_z_kijem, BMP_WIDTH, BMP_HEIGHT},       // Indeks 0 (Prawo)
    {ludzik_z_kijem_lewo, BMP_WIDTH, BMP_HEIGHT},  // Indeks 1 (Lewo)
    {ludzik_z_kijem_gora, BMP_WIDTH1, BMP_HEIGHT},
    {ludzik_z_kijem_dol, BMP_WIDTH1, BMP_HEIGHT},    
  };
// Zmienne dla rekina (Poziom 1)
  int rekinX = 90;       // Zaczyna po prawej stronie
  int rekinY = 45;       // Wysokość na ekranie (woda)
  int rekinKierunek = -1; // -1 oznacza ruch w lewo, 1 w prawo
  int predkoscRekina = 1;
  while (true) {
    // ... Tutaj zostawiasz swoją resztę kodu ze sterowaniem postacią ...
    if (!(PIND & B10000000)) { // góra - pin 7
      dotY -= 2;
      if (dotY < 0) dotY = 0;
      aktualny_indeks = 2;
    }
    if (!(PINB & B00000010)) { // dół - pin 9
      dotY += 2;
      if (dotY + BMP_HEIGHT > SCREEN_HEIGHT) dotY = SCREEN_HEIGHT - BMP_HEIGHT;
      aktualny_indeks = 3;
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
    display.clearDisplay();
    display.drawBitmap(
        dotX, 
        dotY, 
        ludziki[aktualny_indeks].bitmapa, 
        ludziki[aktualny_indeks].szerokosc, 
        ludziki[aktualny_indeks].wysokosc, 
        SH110X_WHITE
    );
    // --- AKCJE DLA POZIOMU 1 ---
    if (aktualnyPoziom == 1) {
      // 1. Aktualizacja pozycji rekina
      rekinX += (rekinKierunek * predkoscRekina);

      // 2. Odbijanie rekina od niewidzialnych ścian (patroluje prawą stronę)
      if (rekinX <= 60) {
        rekinKierunek = 1; // Zmień kierunek na prawo
      } else if (rekinX >= 110) {
        rekinKierunek = -1; // Zmień kierunek na lewo
      }

      // 3. Rysowanie "wody" (linia pod rekinem)
      display.drawLine(60, rekinY + 8, 128, rekinY + 8, SH110X_WHITE);

      // 4. Rysowanie płetwy rekina
      display.drawBitmap(rekinX, rekinY, pletwa_rekina, 16, 8, SH110X_WHITE);
    }
    display.display();
    
    delay(16); // ~60 FPS
  }

  return 0;
}

  return 0;
}
