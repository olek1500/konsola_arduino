#pragma once
#include <Adafruit_SH110X.h>
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

void aktualizujBombe(unsigned long teraz) {
  const unsigned long interwalRuchu = 400UL;
  const unsigned long czasKlatkiWybuchu = 300UL;
  int dbomb;
  if (!bombaAktywna) {
    if (random(0, 90) < 5) {
      bombaX = krokX;
      bombaY = krokY ;
      bombaAktywna = true;
      czyWybucha = false;
      indeks_bomba = 0;
      ostatniaZmianaKlatki = teraz;
      bombaz = bombaY;
    }
    return;
  }
  
  if (!czyWybucha) {
    if (teraz - ostatniaZmianaKlatki >= interwalRuchu) {
      ostatniaZmianaKlatki = teraz;
      bombaY += 4; 
      dbomb = bombaY - bombaz;
      if (dbomb > 12) { 
        czyWybucha = true;
        indeks_bomba = 0;
      }
      
      if (abs(bombaX - dotX-12) < 16 && abs(bombaY - dotY-12) < 16) {
         zdrowieGracza -= 10; 
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
      display.drawBitmap(
        bombaX, bombaY,
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

bool aktualizujRakiete(unsigned long teraz) {
  if (!rakieta) {
    int rant = random(0, 128);
    if (rant < 100 && rant > 20 && random(0, 100) < 5) {
      rakietax = rant;
      rakietay = random(20, 56); 
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
    zdrowieKrokodyla -= 10; 
    
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

void rysujWybuchRakiety(unsigned long teraz) {
  if (!rakietaWybucha) return;

  const unsigned long czasKlatkiWybuchu = 300UL;

  if (teraz - ostatniaZmianaKlatkiRakiety >= czasKlatkiWybuchu) {
    ostatniaZmianaKlatkiRakiety = teraz;
    indeks_rakieta_wybuch++;
  }

  if (indeks_rakieta_wybuch < 3) {
    display.drawBitmap(
      rakietaWybuchX, rakietaWybuchY,
      buh[indeks_rakieta_wybuch].bitmapa,
      buh[indeks_rakieta_wybuch].szerokosc,
      buh[indeks_rakieta_wybuch].wysokosc,
      SH110X_WHITE
    );
  } else {
    rakietaWybucha = false; 
  }
}
