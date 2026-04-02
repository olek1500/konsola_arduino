#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "menu.h"
#include "chodzenie.h"
#include "dane.h"
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
int dotY = 10; 
int dotX = 10;
int aktualny_indeks = 0;
int rekinX        = 90;
int rekinY        = 30;
int rekinDX       =  1;  
int rekinDY       =  0;  
int rekinIndeks   =  2;  
int predkoscRekina = 2;
int krokX         = 128; // Zaczyna za prawą krawędzią ekranu (SCREEN_WIDTH)
int krokY         = 30;
int krokDX        = -2;  // Ujemna prędkość, bo na start leci w lewo
int krokKierunek  = 0;   // Indeks 0 to grafika 'krok_lewo' w tablicy samolot
// --- Zmienne dla statycznej Bomby ---
bool bombaAktywna = false;
int bombaX = 0;
int bombaY = 0;
unsigned long czasPojawieniaBomby = 0;

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
void rekin()
{
    display.clearDisplay();
    display.drawBitmap(32,16,rekin_w_butach,64,48,SH110X_WHITE);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(15,0);
    display.print("NEXT OPPONENT :");
    display.display();
    display.display();
}
void aktualizujRekina() {
  if (rekinZlapany) return; 
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
void rysujRekina(unsigned long teraz) {
  if (atakTrafiony && (teraz - atakTrafionyCzas >= CZAS_ANIMACJI_ATAKU)) {
    atakTrafiony = false;
  }
  if (atakTrafiony) {
    if ((teraz / 80) % 2 == 0) {
      display.drawBitmap(
        rekinX, rekinY,
        rekiny[rekinIndeks].bitmapa,
        rekiny[rekinIndeks].szerokosc,
        rekiny[rekinIndeks].wysokosc,
        SH110X_WHITE
      );
    }
    display.drawBitmap(rekinX + 8, rekinY + 5, atak, 16, 16, SH110X_WHITE);
  }

  if (rekinZlapany) {
    display.drawBitmap(
      rekinX, rekinY,
      rekin_odwrocony, 
      REKIN_SIZE, REKIN_SIZE,
      SH110X_WHITE
    );
  } 
  else {
    display.drawBitmap(
      rekinX, rekinY,
      rekiny[rekinIndeks].bitmapa,
      rekiny[rekinIndeks].szerokosc,
      rekiny[rekinIndeks].wysokosc,
      SH110X_WHITE
    );
  }
}
void rysujPaskiZdrowia(int zdrowieopp) {
  display.drawRect(0, 0, 26, 4, SH110X_WHITE);
  display.drawRect(76, 0, 52, 4, SH110X_WHITE);
  int szerokoscGracz = map(zdrowieGracza, 0, 50, 0, 24);
  if (szerokoscGracz > 0) {
    display.fillRect(1, 1, szerokoscGracz, 2, SH110X_WHITE);
  }
  int szerokoscRekin = map(zdrowieopp, 0, 100, 0, 50);
  if (szerokoscRekin > 0) {
    display.fillRect(127 - szerokoscRekin, 1, szerokoscRekin, 2, SH110X_WHITE);
  }
}
void rysujSiatke() {
  if (!siatkaAktywna) return;
  display.drawBitmap(
    siatkaX, siatkaY,
    siatka_bmp,
    SIATKA_SIZE, SIATKA_SIZE,
    SH110X_WHITE
  );
}
void aktualizujSiatke(unsigned long teraz) {
  if (!siatkaAktywna && (teraz - ostatniSpawnSiatki >= SPAWN_INTERWAL)) {
int margines = 8; 
    siatkaX = random(margines, SCREEN_WIDTH - SIATKA_SIZE - margines);
    siatkaY = random(6 + margines, SCREEN_HEIGHT - SIATKA_SIZE - margines);
    siatkaAktywna = true;
  }

  if (siatkaAktywna && !rekinZlapany) {
    bool kolX = (rekinX < siatkaX + SIATKA_SIZE) && (rekinX + REKIN_SIZE > siatkaX);
    bool kolY = (rekinY < siatkaY + SIATKA_SIZE) && (rekinY + REKIN_SIZE > siatkaY);
    if (kolX && kolY) {
      ostatniSpawnSiatki = teraz;
      rekinZlapany    = true;
      rekinZlapayCzas = teraz;
      siatkaAktywna   = false;
    }
  }

  if (rekinZlapany && (teraz - rekinZlapayCzas >= CZAS_ZAMROZENIA)) {
    rekinZlapany  = false;
    
  }
}
void sprawdzKolizjeZGraczem(unsigned long teraz) {
  if (rekinZlapany) return;
  int marginesGracza = 4; 
  int marginesRekina = 6;
  int gLewo  = dotX + marginesGracza;
  int gPrawo = dotX + BMP_WIDTH - marginesGracza;
  int gGora  = dotY + marginesGracza;
  int gDol   = dotY + BMP_HEIGHT - marginesGracza;
  int rLewo  = rekinX + marginesRekina;
  int rPrawo = rekinX + REKIN_SIZE - marginesRekina;
  int rGora  = rekinY + marginesRekina;
  int rDol   = rekinY + REKIN_SIZE - marginesRekina;
  bool kolX = (rLewo < gPrawo) && (rPrawo > gLewo);
  bool kolY = (rGora < gDol) && (rDol > gGora);
  if (kolX && kolY) {
    if (teraz - ostatnieUgryzienieCzas >= 500) {
      zdrowieGracza -= 10;
      ostatnieUgryzienieCzas = teraz;
      if (zdrowieGracza < 0) zdrowieGracza = 0;
    }
  }
}
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
void stphase(bool wygrales) {
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  int x = (display.width() - SCREEN_WIDTH) / 2;
  int y = (display.height() - SCREEN_HEIGHT) / 2;
  if (wygrales) {
    display.setTextSize(1);
    display.setCursor(10, 0); 
    display.println("YOU RECEIVED A NEW ITEM!");
    display.drawBitmap(40,16,shoes,40,40,SH110X_WHITE);

  } else {
    display.setTextSize(2); 
    display.setCursor(20, 0); 
    display.println("YOU LOOSE");
  }
  display.display();
  movement_speed ++;
}
void bombardiro()
  { display.clearDisplay();
    display.drawBitmap(16,16,krokodyl_bombowiec,96,48,SH110X_WHITE);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(15,0);
    display.print("NEXT OPPONENT :");
    display.display();
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
  krokX += krokDX; // Przesuwamy bossa (dodając ujemne lub dodatnie krokDX)

  // 1. Jeśli leciał w LEWO i całkowicie wyleciał za lewą krawędź
  if (krokDX < 0 && krokX < -samolot[0].szerokosc) {
    krokDX = 2;          // Zmieniamy wektor ruchu na prawo
    krokKierunek = 1;    // Zmieniamy grafikę na 'krok_prawo' (indeks 1)
    
    // Losujemy nową wysokość dla lotu w prawo
    krokY = random(6, SCREEN_HEIGHT - samolot[1].wysokosc); 
  }
  // 2. Jeśli leciał w PRAWO i całkowicie wyleciał za prawą krawędź
  else if (krokDX > 0 && krokX > SCREEN_WIDTH) {
    krokDX = -2;         // Zmieniamy wektor ruchu na lewo
    krokKierunek = 0;    // Zmieniamy grafikę na 'krok_lewo' (indeks 0)
    
    // Losujemy nową wysokość dla lotu w lewo
    krokY = random(6, SCREEN_HEIGHT - samolot[0].wysokosc); 
  }
}
void aktualizujBombe(unsigned long teraz) {
  // 1. Jeśli nie ma bomby na ekranie, próbujemy ją zrzucić
  if (!bombaAktywna) {
    // Niewielka szansa co klatkę (zmień to '2', jeśli pojawia się za rzadko/za często)
    if (random(0, 100) < 2) { 
      bombaAktywna = true;
      bombaX = krokX + 16; // Przesunięcie względem samolotu (dopasujesz pod swoją bitmapę)
      bombaY = krokY + 16; 
      czasPojawieniaBomby = teraz; // Zapisujemy stoper
    }
  }

  // 2. Jeśli bomba już leży, odliczamy 3 sekundy
  if (bombaAktywna) {
    if (teraz - czasPojawieniaBomby >= 3000) { // 3000 ms = 3 sekundy
      bombaAktywna = false; // Czas minął, bomba znika
    }
  }
}

void rysujBombe() {
  if (bombaAktywna) {
    // Póki co kwadrat, jak prosiłeś (8x8 pikseli)
    display.fillRect(bombaX, bombaY, 8, 8, SH110X_WHITE);
    
    // Docelowo podmienisz na:
    // display.drawBitmap(bombaX, bombaY, nazwa_bitmapy, SZEROKOSC, WYSOKOSC, SH110X_WHITE);
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
  display.print("LOADING ...");
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
      obslugaWejscia(BMP_HEIGHT,SCREEN_HEIGHT,BMP_WIDTH,SCREEN_WIDTH);
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
          display.print("RUN!");
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
          
          obslugaWejscia(BMP_HEIGHT,SCREEN_HEIGHT,BMP_WIDTH,SCREEN_WIDTH);
          display.clearDisplay();
          rysujPaskiZdrowia(zdrowieKrokodyla);
          display.drawBitmap(
          dotX, dotY,
          ludziki[aktualny_indeks].bitmapa,
          ludziki[aktualny_indeks].szerokosc,
          ludziki[aktualny_indeks].wysokosc,
 SH110X_WHITE);
          
          aktualizujBK();          // Twoja funkcja samolotu
          aktualizujBombe(teraz);  // <--- DODANE
          
          rysujBK();               // Twoja funkcja samolotu
          rysujBombe();            // <--- DODANE
          
          display.display();
        }
      }
    }
    display.display();
    }
return 0;
}
