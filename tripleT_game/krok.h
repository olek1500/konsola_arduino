#pragma once
#include <Adafruit_SH110X.h>

const bool TEST_KROKODYL_JEDNA_RAKIETA = false; // Ustaw true tylko do szybkich testow.

void rysujProstyWybuch(int srodekX, int srodekY, int promien) {
  int rozmiar = promien * 2;
  int startX = srodekX - promien;
  int startY = srodekY - promien;
  display.drawRect(startX, startY, rozmiar, rozmiar, SH110X_WHITE);
  display.fillRect(srodekX - promien / 2, srodekY - promien / 2, promien, promien, SH110X_WHITE);
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
    krokDX = 3;   
    krokKierunek = 1;   
    krokY = dotY-12;
  }
  else if (krokDX > 0 && krokX > SCREEN_WIDTH) {
    krokDX = -3;
    krokKierunek = 0;
    krokY = dotY;
  }
}

void aktualizujBombe(uint16_t teraz) {
  const uint16_t interwalRuchu = 280U;
  const uint16_t czasKlatkiWybuchu = 220U;
  const uint8_t promienieWybuchu[2] = {20, 30};
  int dbomb;
  if (!bombaAktywna) {
    if (random(0, 90) < 5) {
      bombaX = krokX;
      bombaY = krokY ;
      bombaAktywna = true;
      czyWybucha = false;
      bombaWybuchTrafila = false;
      indeks_bomba = 0;
      ostatniaZmianaKlatki = teraz;
      bombaz = bombaY;
    }
    return;
  }
  
  if (!czyWybucha) {
    if (teraz - ostatniaZmianaKlatki >= interwalRuchu) {
      ostatniaZmianaKlatki = teraz;
      bombaY += 5; 
      dbomb = bombaY - bombaz;
      if (dbomb > 12) { 
        czyWybucha = true;
        indeks_bomba = 0;
        ostatniaZmianaKlatki = teraz;
      }
    }
    display.drawBitmap(bombaX, bombaY, bomba, 8, 12, SH110X_WHITE);
  } 
  else {
    if (teraz - ostatniaZmianaKlatki >= czasKlatkiWybuchu) {
      ostatniaZmianaKlatki = teraz;
      indeks_bomba++;
    }
    
    if (indeks_bomba < 2) {
      int bombaSrodekX = bombaX + 4;
      int bombaSrodekY = bombaY + 6;
      int graczSrodekX = dotX + graczSzerokosc() / 2;
      int graczSrodekY = dotY + graczWysokosc() / 2;
      int dx = bombaSrodekX - graczSrodekX;
      int dy = bombaSrodekY - graczSrodekY;
      int promienWybuchu = promienieWybuchu[indeks_bomba];

      if (!bombaWybuchTrafila && dx * dx + dy * dy <= promienWybuchu * promienWybuchu) {
        odejmijZdrowie(zdrowieGracza, 10);
        bombaWybuchTrafila = true;
      }

      rysujProstyWybuch(bombaSrodekX, bombaSrodekY, promienWybuchu);
    } else {
      bombaAktywna = false;
    }
  }
}

bool aktualizujRakiete(uint16_t teraz) {
  if (!rakieta) {
    const int zasiegPodniesienia = 13;
    const int minSpawnX = 6;
    const int maxSpawnX = (SCREEN_WIDTH - 32) + zasiegPodniesienia;
    const int minSpawnY = 18;
    const int maxSpawnY = (SCREEN_HEIGHT - 24) + zasiegPodniesienia;
    if (random(0, 100) < 5) {
      rakietax = random(minSpawnX, maxSpawnX);
      rakietay = random(minSpawnY, maxSpawnY);
      rakieta = true;
      rakietaLeci = false; 
      rakieta_in = 0;      
    }
    return false;
  }
  
  if (!rakietaLeci) {
    if (abs(dotX - rakietax) < 14 && abs(dotY - rakietay) < 14) {
      rakietaLeci = true; 
    }
    return false; 
  }

  int rdx = krokX - rakietax+12; 
  int rdy = krokY - rakietay+12;
  
  if (abs(rdx) < 16 && abs(rdy) < 16) {
    rakieta = false;       
    rakietaLeci = false;   
    if (TEST_KROKODYL_JEDNA_RAKIETA) {
      zdrowieKrokodyla = 0;
    } else {
      odejmijZdrowie(zdrowieKrokodyla, 10);
    }
    
    rakietaWybucha = true;
    rakietaWybuchX = rakietax;
    rakietaWybuchY = rakietay;
    indeks_rakieta_wybuch = 0;
    ostatniaZmianaKlatkiRakiety = teraz;
    
    return false;    
  }
  
  int predkoscRakiety = 3;

  if (abs(rdx) > abs(rdy)) {
    if (rdx > 0) { 
      rakietax += predkoscRakiety; 
      rakieta_in = 0; 
    } else { 
      rakietax -= predkoscRakiety; 
      rakieta_in = 1; 
    }
  } else {
    if (rdy > 0) { 
      rakietay += predkoscRakiety; 
      rakieta_in = 2; 
    } else { 
      rakietay -= predkoscRakiety; 
      rakieta_in = 3; 
    }
  }
  return true; 
}

void rysujrocka() {
  display.drawBitmap(
      rakietax, rakietay,
      rakietaa[rakieta_in].bitmapa, 
      rakietaa[rakieta_in].szerokosc,
      rakietaa[rakieta_in].wysokosc,
      SH110X_WHITE);
}

void rysujWybuchRakiety(uint16_t teraz) {
  if (!rakietaWybucha) return;

  const uint16_t czasKlatkiWybuchu = 300U;

  if (teraz - ostatniaZmianaKlatkiRakiety >= czasKlatkiWybuchu) {
    ostatniaZmianaKlatkiRakiety = teraz;
    indeks_rakieta_wybuch++;
  }

  if (indeks_rakieta_wybuch < 3) {
    const unsigned char* bitmapa = explosion_f1;
    uint8_t szerokosc = 24;
    uint8_t wysokosc = 24;

    if (indeks_rakieta_wybuch == 1) {
      bitmapa = explosion_f2;
      szerokosc = 32;
      wysokosc = 32;
    } else if (indeks_rakieta_wybuch == 2) {
      bitmapa = explosion_f3;
    }

    display.drawBitmap(rakietaWybuchX, rakietaWybuchY, bitmapa, szerokosc, wysokosc, SH110X_WHITE);
  } else {
    rakietaWybucha = false; 
  }
}
