#pragma once
#include <Adafruit_SH110X.h>

extern Adafruit_SH1106G display;
extern int8_t dotX;
extern int8_t dotY;
extern uint8_t aktualny_indeks;
extern uint8_t zdrowieGracza;
extern uint8_t maxZdrowieGracza;
extern uint16_t ostatniAtakCzas;
extern bool atakTrafiony;
extern uint16_t atakTrafionyCzas;
extern const uint16_t CZAS_ANIMACJI_ATAKU;

extern bool graczMaRakiete;
extern uint8_t maxZdrowieKubka;
extern uint8_t zdrowieKubka;
extern uint8_t kubekX;
extern uint8_t kubekY;
extern uint8_t kubekAkcja;
extern uint8_t kubekFaza;
extern int8_t kubekDashDX;
extern int8_t kubekDashDY;
extern uint8_t kubekCelX;
extern uint8_t kubekCelY;
extern bool kubekDashPoziomy;
extern uint16_t kubekAkcjaStart;
extern uint16_t kubekNastepnaAkcja;
extern uint16_t kubekOgluszonyDo;
extern uint16_t kubekWrazliwyDo;
extern uint16_t kubekHitFlashDo;
extern bool mieczAktywny[2];
extern int16_t mieczX[2];
extern int16_t mieczY[2];
extern int8_t mieczDX[2];
extern int8_t mieczDY[2];
extern bool plamaAktywna[3];
extern uint8_t plamaX[3];
extern uint8_t plamaY[3];
extern uint16_t plamaDo[3];
extern uint16_t ostatnieObrazenieFinal;
extern bool rakietaGraczaAktywna;
extern int16_t rakietaGraczaX;
extern int16_t rakietaGraczaY;
extern int8_t rakietaGraczaDX;
extern int8_t rakietaGraczaDY;
extern uint8_t rakietaGraczaKierunek;
extern uint16_t rakietaGraczaCooldownDo;
extern bool przyciskAtakuBylWcisniety;
extern uint16_t przyciskAtakuWcisniecieCzas;
extern bool przytrzymanieZuzyteNaRakiete;

enum AkcjaKubka {
  KUBEK_IDLE = 0,
  KUBEK_DASH_TELL = 1,
  KUBEK_DASH = 2,
  KUBEK_RECOVER = 3,
  KUBEK_TELEPORT = 4,
  KUBEK_CROSS_TELL = 5,
  KUBEK_CROSS = 6
};

const uint8_t KUBEK_SZEROKOSC = 20;
const uint8_t KUBEK_WYSOKOSC = 24;
const uint8_t KUBEK_CIALO_SZER = 14;
const uint8_t KUBEK_CIALO_WYS = 16;
const uint8_t KUBEK_HITBOX_OFFSET_X = 5;
const uint8_t KUBEK_HITBOX_OFFSET_Y = 5;
const uint8_t KUBEK_HITBOX_SZER = 10;
const uint8_t KUBEK_HITBOX_WYS = 12;
const uint8_t KUBEK_KONTAKT_HITBOX_OFFSET_X = 6;
const uint8_t KUBEK_KONTAKT_HITBOX_OFFSET_Y = 6;
const uint8_t KUBEK_KONTAKT_HITBOX_SZER = 8;
const uint8_t KUBEK_KONTAKT_HITBOX_WYS = 10;
const uint8_t KUBEK_DASH_HITBOX_OFFSET_X = 7;
const uint8_t KUBEK_DASH_HITBOX_OFFSET_Y = 7;
const uint8_t KUBEK_DASH_HITBOX_SZER = 6;
const uint8_t KUBEK_DASH_HITBOX_WYS = 8;
const uint16_t KUBEK_RAKIETA_HOLD = 320U;
const uint16_t KUBEK_RAKIETA_COOLDOWN = 2000U;
const uint16_t KUBEK_DASH_TELL_MS = 760U;
const uint16_t KUBEK_DASH_MAX_MS = 1450U;
const uint16_t KUBEK_TELEPORT_UKRYCIE_MS = 420U;
const uint16_t KUBEK_TELEPORT_CALKOWITY_MS = 1220U;
const uint16_t KUBEK_TELEPORT_WRAZLIWOSC_MS = 1080U;
const uint8_t KUBEK_MELEE_DODATKOWY_ZASIEG = 10;

inline int srodekGraczaX() {
  return dotX + graczSzerokosc() / 2;
}

inline int srodekGraczaY() {
  return dotY + graczWysokosc() / 2;
}

inline int graczHitboxLewo() {
  return dotX + 5;
}

inline int graczHitboxPrawo() {
  return dotX + graczSzerokosc() - 5;
}

inline int graczHitboxGora() {
  return dotY + 4;
}

inline int graczHitboxDol() {
  return dotY + graczWysokosc() - 4;
}

inline int srodekKubkaX() {
  return kubekX + KUBEK_SZEROKOSC / 2;
}

inline int srodekKubkaY() {
  return kubekY + KUBEK_WYSOKOSC / 2;
}

inline int kubekHitboxLewo() {
  return kubekX + KUBEK_HITBOX_OFFSET_X;
}

inline int kubekHitboxPrawo() {
  return kubekHitboxLewo() + KUBEK_HITBOX_SZER;
}

inline int kubekHitboxGora() {
  return kubekY + KUBEK_HITBOX_OFFSET_Y;
}

inline int kubekHitboxDol() {
  return kubekHitboxGora() + KUBEK_HITBOX_WYS;
}

inline int kubekKontaktHitboxLewo() {
  return kubekX + KUBEK_KONTAKT_HITBOX_OFFSET_X;
}

inline int kubekKontaktHitboxPrawo() {
  return kubekKontaktHitboxLewo() + KUBEK_KONTAKT_HITBOX_SZER;
}

inline int kubekKontaktHitboxGora() {
  return kubekY + KUBEK_KONTAKT_HITBOX_OFFSET_Y;
}

inline int kubekKontaktHitboxDol() {
  return kubekKontaktHitboxGora() + KUBEK_KONTAKT_HITBOX_WYS;
}

inline int kubekDashHitboxLewo() {
  return kubekX + KUBEK_DASH_HITBOX_OFFSET_X;
}

inline int kubekDashHitboxPrawo() {
  return kubekDashHitboxLewo() + KUBEK_DASH_HITBOX_SZER;
}

inline int kubekDashHitboxGora() {
  return kubekY + KUBEK_DASH_HITBOX_OFFSET_Y;
}

inline int kubekDashHitboxDol() {
  return kubekDashHitboxGora() + KUBEK_DASH_HITBOX_WYS;
}

inline bool prostokatyNachodza(int aLewo, int aPrawo, int aGora, int aDol,
                               int bLewo, int bPrawo, int bGora, int bDol) {
  return aLewo < bPrawo && aPrawo > bLewo && aGora < bDol && aDol > bGora;
}

inline bool kubekJestOgluszony(uint16_t teraz) {
  return kubekOgluszonyDo > teraz;
}

inline bool kubekJestWrazliwy(uint16_t teraz) {
  return kubekJestOgluszony(teraz) || kubekWrazliwyDo > teraz;
}

inline bool kubekUkryty(uint16_t teraz) {
  return kubekAkcja == KUBEK_TELEPORT && (teraz - kubekAkcjaStart < KUBEK_TELEPORT_UKRYCIE_MS);
}

inline bool widocznyKubekDoKolizji(uint16_t teraz) {
  return !kubekUkryty(teraz);
}

void capuccinoIntro() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(18, 10);
  display.print(F("FINAL"));
  display.setCursor(30, 34);
  display.print(F("CUP"));
  display.display();
}

void pokazFinalneZwyciestwo() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(8, 6);
  display.print(F("YOU WIN"));
  display.setTextSize(1);
  display.setCursor(22, 30);
  display.print(F("Cup defeated"));
  display.setCursor(10, 46);
  display.print(F("Reset to play again"));
  display.display();
}

void zakonczGrePoFinale() {
  pokazFinalneZwyciestwo();
  while (true) {
    delay(50);
  }
}

void wyczyscMieczeKubka() {
  for (int i = 0; i < 2; i++) {
    mieczAktywny[i] = false;
    mieczX[i] = 0;
    mieczY[i] = 0;
    mieczDX[i] = 0;
    mieczDY[i] = 0;
  }
}

void wyczyscPlamyKawy() {
  for (int i = 0; i < 3; i++) {
    plamaAktywna[i] = false;
    plamaX[i] = 0;
    plamaY[i] = 0;
    plamaDo[i] = 0;
  }
}

void resetujFinalBossa() {
  zdrowieKubka = maxZdrowieKubka;
  kubekX = 84;
  kubekY = 24;
  kubekAkcja = KUBEK_IDLE;
  kubekFaza = 1;
  kubekDashDX = 0;
  kubekDashDY = 0;
  kubekCelX = kubekX;
  kubekCelY = kubekY;
  kubekDashPoziomy = true;
  kubekAkcjaStart = 0;
  kubekNastepnaAkcja = millis() + 800;
  kubekOgluszonyDo = 0;
  kubekWrazliwyDo = 0;
  kubekHitFlashDo = 0;
  wyczyscMieczeKubka();
  wyczyscPlamyKawy();
  rakietaGraczaAktywna = false;
  rakietaGraczaX = 0;
  rakietaGraczaY = 0;
  rakietaGraczaDX = 0;
  rakietaGraczaDY = 0;
  rakietaGraczaKierunek = 0;
  rakietaGraczaCooldownDo = 0;
  ostatnieObrazenieFinal = 0;
  przyciskAtakuBylWcisniety = false;
  przyciskAtakuWcisniecieCzas = 0;
  przytrzymanieZuzyteNaRakiete = false;
}

void zadajObrazeniaGraczowiFinal(int obrazenia, uint16_t teraz) {
  if (teraz - ostatnieObrazenieFinal < 350) return;
  ostatnieObrazenieFinal = teraz;
  odejmijZdrowie(zdrowieGracza, static_cast<uint8_t>(obrazenia));
}

void ustawFazeKubka(uint16_t teraz) {
  if (zdrowieKubka <= 30) {
    kubekFaza = 3;
  } else if (zdrowieKubka <= 70) {
    kubekFaza = 2;
  } else {
    kubekFaza = 1;
  }
}

void zadajObrazeniaKubkowi(int obrazenia, uint16_t teraz, bool rakietaSpecjalna) {
  odejmijZdrowie(zdrowieKubka, static_cast<uint8_t>(obrazenia));
  kubekHitFlashDo = teraz + 150;

  if (rakietaSpecjalna) {
    kubekOgluszonyDo = teraz + 1200;
    kubekWrazliwyDo = kubekOgluszonyDo;
    kubekAkcja = KUBEK_RECOVER;
    kubekNastepnaAkcja = kubekOgluszonyDo + 250;
  }
}

void aktywujTeleportKubka(uint16_t teraz) {
  kubekAkcja = KUBEK_TELEPORT;
  kubekAkcjaStart = teraz;
  kubekNastepnaAkcja = teraz + KUBEK_TELEPORT_CALKOWITY_MS;
  kubekCelX = random(12, SCREEN_WIDTH - KUBEK_SZEROKOSC - 12);
  kubekCelY = random(18, SCREEN_HEIGHT - KUBEK_WYSOKOSC);
}

void dodajPlameKawy(int idx, uint16_t teraz, int x, int y) {
  plamaAktywna[idx] = true;
  plamaX[idx] = constrain(x, 12, SCREEN_WIDTH - 12);
  plamaY[idx] = constrain(y, 18, SCREEN_HEIGHT - 10);
  plamaDo[idx] = teraz + 2600;
}

void aktywujPlamyKawy(uint16_t teraz) {
  dodajPlameKawy(0, teraz, random(18, 110), random(22, 56));
  dodajPlameKawy(1, teraz, random(18, 110), random(22, 56));
  if (kubekFaza >= 3) {
    dodajPlameKawy(2, teraz, random(18, 110), random(22, 56));
  }
}

void dodajMiecz(int idx, int x, int y, int dx, int dy) {
  mieczAktywny[idx] = true;
  mieczX[idx] = x;
  mieczY[idx] = y;
  mieczDX[idx] = dx;
  mieczDY[idx] = dy;
}

void aktywujRzutMieczy(uint16_t teraz) {
  int diffX = srodekGraczaX() - srodekKubkaX();
  int diffY = srodekGraczaY() - srodekKubkaY();
  int dx = diffX >= 0 ? 4 : -4;
  int dy = diffY >= 0 ? 2 : -2;

  if (abs(diffY) > abs(diffX)) {
    dx = diffX >= 0 ? 2 : -2;
    dy = diffY >= 0 ? 4 : -4;
  }

  dodajMiecz(0, srodekKubkaX(), srodekKubkaY(), dx, dy - 1);
  dodajMiecz(1, srodekKubkaX(), srodekKubkaY(), dx, dy + 1);

  kubekNastepnaAkcja = teraz + 650;
}

void aktywujDashKubka(uint16_t teraz) {
  kubekAkcja = KUBEK_DASH_TELL;
  kubekAkcjaStart = teraz;
  int diffX = srodekGraczaX() - srodekKubkaX();
  int diffY = srodekGraczaY() - srodekKubkaY();
  kubekDashPoziomy = abs(diffX) >= abs(diffY);

  if (kubekDashPoziomy) {
    kubekCelY = constrain(srodekGraczaY() - KUBEK_WYSOKOSC / 2, 16, SCREEN_HEIGHT - KUBEK_WYSOKOSC);
    kubekDashDX = diffX >= 0 ? 3 : -3;
    kubekDashDY = 0;
  } else {
    kubekCelX = constrain(srodekGraczaX() - KUBEK_SZEROKOSC / 2, 4, SCREEN_WIDTH - KUBEK_SZEROKOSC - 4);
    kubekDashDX = 0;
    kubekDashDY = diffY >= 0 ? 3 : -3;
  }
}

void aktywujCrossCut(uint16_t teraz) {
  kubekAkcja = KUBEK_CROSS_TELL;
  kubekAkcjaStart = teraz;
  kubekCelX = constrain(srodekGraczaX(), 12, SCREEN_WIDTH - 12);
  kubekCelY = constrain(srodekGraczaY(), 16, SCREEN_HEIGHT - 10);
  kubekWrazliwyDo = teraz + 450;
}

void wybierzNastepnaAkcjeKubka(uint16_t teraz) {
  if (kubekJestOgluszony(teraz) || teraz < kubekNastepnaAkcja) return;
  if (kubekAkcja != KUBEK_IDLE && kubekAkcja != KUBEK_RECOVER) return;

  kubekAkcja = KUBEK_IDLE;

  int los = random(0, 100);
  if (kubekFaza == 1) {
    if (los < 60) {
      aktywujDashKubka(teraz);
    } else {
      aktywujRzutMieczy(teraz);
    }
    return;
  }

  if (kubekFaza == 2) {
    if (los < 35) {
      aktywujTeleportKubka(teraz);
    } else if (los < 70) {
      aktywujPlamyKawy(teraz);
      kubekNastepnaAkcja = teraz + 700;
    } else {
      aktywujRzutMieczy(teraz);
    }
    return;
  }

  if (los < 35) {
    aktywujCrossCut(teraz);
  } else if (los < 65) {
    aktywujDashKubka(teraz);
  } else if (los < 82) {
    aktywujTeleportKubka(teraz);
  } else {
    aktywujPlamyKawy(teraz);
    aktywujRzutMieczy(teraz);
    kubekNastepnaAkcja = teraz + 800;
  }
}

void wystrzelRakieteGracza(uint16_t teraz) {
  rakietaGraczaAktywna = true;
  rakietaGraczaX = srodekGraczaX() - 8;
  rakietaGraczaY = srodekGraczaY() - 8;
  rakietaGraczaDX = 0;
  rakietaGraczaDY = 0;
  rakietaGraczaCooldownDo = teraz + KUBEK_RAKIETA_COOLDOWN;

  switch (aktualny_indeks) {
    case 0:
      rakietaGraczaDX = 4;
      rakietaGraczaKierunek = 0;
      break;
    case 1:
      rakietaGraczaDX = -4;
      rakietaGraczaKierunek = 1;
      break;
    case 2:
      rakietaGraczaDY = -4;
      rakietaGraczaKierunek = 3;
      break;
    default:
      rakietaGraczaDY = 4;
      rakietaGraczaKierunek = 2;
      break;
  }
}

void obsluzAtakGraczaNaKubka(uint16_t teraz) {
  bool przyciskAtaku = przyciskStartWcisniety();
  bool przyciskRakiety = przyciskRakietyWcisniety();

  if (przyciskAtaku && !przyciskAtakuBylWcisniety && teraz - ostatniAtakCzas >= 350) {
    ostatniAtakCzas = teraz;
    int atakLewo = dotX - 4;
    int atakPrawo = dotX - 4 + graczSzerokosc();
    int atakGora = dotY;
    int atakDol = dotY + graczWysokosc();

    switch (aktualny_indeks) {
      case 0:
        atakPrawo += KUBEK_MELEE_DODATKOWY_ZASIEG;
        atakGora -= 2;
        atakDol += 2;
        break;
      case 1:
        atakLewo -= KUBEK_MELEE_DODATKOWY_ZASIEG;
        atakGora -= 2;
        atakDol += 2;
        break;
      case 2:
        atakGora -= KUBEK_MELEE_DODATKOWY_ZASIEG;
        atakLewo -= 2;
        atakPrawo += 2;
        break;
      default:
        atakDol += KUBEK_MELEE_DODATKOWY_ZASIEG;
        atakLewo -= 2;
        atakPrawo += 2;
        break;
    }

    bool trafienieKubka = widocznyKubekDoKolizji(teraz) &&
                          prostokatyNachodza(
                            atakLewo, atakPrawo, atakGora, atakDol,
                            kubekHitboxLewo(), kubekHitboxPrawo(), kubekHitboxGora(), kubekHitboxDol()
                          );

    if (trafienieKubka) {
      atakTrafiony = true;
      atakTrafionyCzas = teraz;
      if (kubekJestWrazliwy(teraz) || kubekAkcja == KUBEK_CROSS_TELL) {
        zadajObrazeniaKubkowi(8, teraz, false);
      }
    }
  }

  if (przyciskRakiety && !przytrzymanieZuzyteNaRakiete &&
      graczMaRakiete && !rakietaGraczaAktywna &&
      teraz >= rakietaGraczaCooldownDo) {
    wystrzelRakieteGracza(teraz);
  }

  przyciskAtakuBylWcisniety = przyciskAtaku;
  przytrzymanieZuzyteNaRakiete = przyciskRakiety;
}

void aktualizujMieczeKubka(uint16_t teraz) {
  for (int i = 0; i < 2; i++) {
    if (!mieczAktywny[i]) continue;

    mieczX[i] += mieczDX[i];
    mieczY[i] += mieczDY[i];

    bool kolX = mieczX[i] < dotX + graczSzerokosc() && mieczX[i] + 6 > dotX;
    bool kolY = mieczY[i] < dotY + graczWysokosc() && mieczY[i] + 6 > dotY;
    if (kolX && kolY) {
      zadajObrazeniaGraczowiFinal(8, teraz);
      mieczAktywny[i] = false;
      continue;
    }

    if (mieczX[i] < -6 || mieczX[i] > SCREEN_WIDTH + 6 ||
        mieczY[i] < 8 || mieczY[i] > SCREEN_HEIGHT + 6) {
      mieczAktywny[i] = false;
    }
  }
}

void aktualizujPlamyKawy(uint16_t teraz) {
  for (int i = 0; i < 3; i++) {
    if (!plamaAktywna[i]) continue;
    if (teraz >= plamaDo[i]) {
      plamaAktywna[i] = false;
      continue;
    }

    int dx = srodekGraczaX() - plamaX[i];
    int dy = srodekGraczaY() - plamaY[i];
    if (dx * dx + dy * dy <= 144) {
      zadajObrazeniaGraczowiFinal(6, teraz);
    }
  }
}

void aktualizujRakieteGracza(uint16_t teraz) {
  if (!rakietaGraczaAktywna) return;

  rakietaGraczaX += rakietaGraczaDX;
  rakietaGraczaY += rakietaGraczaDY;

  int rakietaLewo = rakietaGraczaX;
  int rakietaPrawo = rakietaGraczaX + rakietaa[rakietaGraczaKierunek].szerokosc;
  int rakietaGora = rakietaGraczaY;
  int rakietaDol = rakietaGraczaY + rakietaa[rakietaGraczaKierunek].wysokosc;
  bool trafienieKubka = widocznyKubekDoKolizji(teraz) &&
                        prostokatyNachodza(
                          rakietaLewo, rakietaPrawo, rakietaGora, rakietaDol,
                          kubekHitboxLewo(), kubekHitboxPrawo(), kubekHitboxGora(), kubekHitboxDol()
                        );

  if (trafienieKubka) {
    rakietaGraczaAktywna = false;
    zadajObrazeniaKubkowi(16, teraz, true);
    return;
  }

  if (rakietaGraczaX < -16 || rakietaGraczaX > SCREEN_WIDTH + 16 ||
      rakietaGraczaY < 0 || rakietaGraczaY > SCREEN_HEIGHT + 16) {
    rakietaGraczaAktywna = false;
  }
}

void aktualizujDashKubka(uint16_t teraz) {
  if (kubekAkcja == KUBEK_DASH_TELL) {
    if (teraz - kubekAkcjaStart >= KUBEK_DASH_TELL_MS) {
      kubekAkcja = KUBEK_DASH;
      kubekAkcjaStart = teraz;
      if (kubekDashPoziomy) {
        kubekY = kubekCelY;
      } else {
        kubekX = kubekCelX;
      }
    }
    return;
  }

  if (kubekAkcja != KUBEK_DASH) return;

  kubekX += kubekDashDX;
  kubekY += kubekDashDY;

  bool trafienieGracza = prostokatyNachodza(
    kubekDashHitboxLewo(), kubekDashHitboxPrawo(), kubekDashHitboxGora(), kubekDashHitboxDol(),
    graczHitboxLewo(), graczHitboxPrawo(), graczHitboxGora(), graczHitboxDol()
  );
  if (trafienieGracza) {
    zadajObrazeniaGraczowiFinal(12, teraz);
  }

  bool koniecDasha = false;
  if (kubekDashPoziomy) {
    if (kubekDashDX > 0 && kubekX >= SCREEN_WIDTH - KUBEK_SZEROKOSC - 4) koniecDasha = true;
    if (kubekDashDX < 0 && kubekX <= 4) koniecDasha = true;
  } else {
    if (kubekDashDY > 0 && kubekY >= SCREEN_HEIGHT - KUBEK_WYSOKOSC) koniecDasha = true;
    if (kubekDashDY < 0 && kubekY <= 16) koniecDasha = true;
  }

  if (koniecDasha || teraz - kubekAkcjaStart > KUBEK_DASH_MAX_MS) {
    kubekAkcja = KUBEK_RECOVER;
    kubekAkcjaStart = teraz;
    kubekWrazliwyDo = teraz + 450;
    kubekNastepnaAkcja = teraz + 500;
  }
}

void aktualizujTeleportKubka(uint16_t teraz) {
  if (kubekAkcja != KUBEK_TELEPORT) return;

  if (teraz - kubekAkcjaStart >= KUBEK_TELEPORT_UKRYCIE_MS) {
    kubekX = kubekCelX;
    kubekY = kubekCelY;
    kubekWrazliwyDo = kubekAkcjaStart + KUBEK_TELEPORT_WRAZLIWOSC_MS;
  }

  if (teraz >= kubekNastepnaAkcja) {
    kubekAkcja = KUBEK_IDLE;
    kubekNastepnaAkcja = teraz + 350;
  }
}

void aktualizujCrossKubka(uint16_t teraz) {
  if (kubekAkcja == KUBEK_CROSS_TELL) {
    if (teraz - kubekAkcjaStart >= 450) {
      kubekAkcja = KUBEK_CROSS;
      kubekAkcjaStart = teraz;
    }
    return;
  }

  if (kubekAkcja != KUBEK_CROSS) return;

  bool trafionyPion = abs(srodekGraczaX() - kubekCelX) < 7;
  bool trafionyPoziom = abs(srodekGraczaY() - kubekCelY) < 7;
  if (trafionyPion || trafionyPoziom) {
    zadajObrazeniaGraczowiFinal(14, teraz);
  }

  if (teraz - kubekAkcjaStart >= 220) {
    kubekAkcja = KUBEK_IDLE;
    kubekNastepnaAkcja = teraz + 400;
  }
}

void aktualizujKubka(uint16_t teraz) {
  ustawFazeKubka(teraz);

  if (kubekJestOgluszony(teraz)) {
    kubekAkcja = KUBEK_RECOVER;
    return;
  }

  aktualizujDashKubka(teraz);
  aktualizujTeleportKubka(teraz);
  aktualizujCrossKubka(teraz);

  if (kubekAkcja == KUBEK_RECOVER && teraz >= kubekNastepnaAkcja) {
    kubekAkcja = KUBEK_IDLE;
  }

  wybierzNastepnaAkcjeKubka(teraz);
}

void sprawdzKontaktKubkaZGraczem(uint16_t teraz) {
  if (kubekUkryty(teraz)) return;
  if (kubekAkcja != KUBEK_IDLE && kubekAkcja != KUBEK_RECOVER) return;

  bool trafienieGracza = prostokatyNachodza(
    kubekKontaktHitboxLewo(), kubekKontaktHitboxPrawo(), kubekKontaktHitboxGora(), kubekKontaktHitboxDol(),
    graczHitboxLewo(), graczHitboxPrawo(), graczHitboxGora(), graczHitboxDol()
  );
  if (trafienieGracza && !kubekJestOgluszony(teraz)) {
    zadajObrazeniaGraczowiFinal(10, teraz);
  }
}

void rysujSylwetkeKubka(int x, int y, bool prawdziwy, bool guard, uint16_t teraz) {
  display.drawRect(x + 3, y + 2, KUBEK_CIALO_SZER, KUBEK_CIALO_WYS, SH110X_WHITE);
  display.drawFastHLine(x + 3, y + 6, KUBEK_CIALO_SZER, SH110X_WHITE);
  display.drawFastHLine(x + 5, y + 12, KUBEK_CIALO_SZER - 4, SH110X_WHITE);
  display.drawFastHLine(x + 6, y + 16, KUBEK_CIALO_SZER - 6, SH110X_WHITE);
  display.drawFastVLine(x + 7, y + 18, 4, SH110X_WHITE);
  display.drawFastVLine(x + 13, y + 18, 4, SH110X_WHITE);
  display.drawFastHLine(x + 5, y + 22, 5, SH110X_WHITE);
  display.drawFastHLine(x + 11, y + 22, 5, SH110X_WHITE);
  display.drawFastHLine(x + 1, y + 14, 4, SH110X_WHITE);
  display.drawFastHLine(x + 17, y + 14, 4, SH110X_WHITE);
  display.drawFastVLine(x + 1, y + 10, 4, SH110X_WHITE);
  display.drawFastVLine(x + 20, y + 10, 4, SH110X_WHITE);

  if (prawdziwy) {
    display.fillRect(x + 7, y + 8, 2, 2, SH110X_WHITE);
    display.fillRect(x + 13, y + 8, 2, 2, SH110X_WHITE);
    display.drawFastHLine(x + 6, y + 6, 4, SH110X_WHITE);
    display.drawFastHLine(x + 12, y + 6, 4, SH110X_WHITE);
    display.drawFastHLine(x + 4, y + 14, 14, SH110X_WHITE);
    if (kubekHitFlashDo > teraz) {
      display.drawRect(x + 1, y, KUBEK_SZEROKOSC, KUBEK_WYSOKOSC, SH110X_WHITE);
    }
  }

  if (guard) {
    display.drawRect(x, y + 2, KUBEK_SZEROKOSC + 2, KUBEK_CIALO_WYS, SH110X_WHITE);
  }
}

void rysujMieczeKubka() {
  for (int i = 0; i < 2; i++) {
    if (!mieczAktywny[i]) continue;
    display.drawRect(mieczX[i], mieczY[i], 4, 4, SH110X_WHITE);
  }
}

void rysujPlamyKawy(uint16_t teraz) {
  for (int i = 0; i < 3; i++) {
    if (!plamaAktywna[i]) continue;
    display.fillRect(plamaX[i] - 3, plamaY[i] - 3, 6, 6, SH110X_WHITE);
    display.drawRect(plamaX[i] - 5, plamaY[i] - 5, 10, 10, SH110X_WHITE);
    if ((teraz / 150) % 2 == 0) {
      display.drawPixel(plamaX[i] - 1, plamaY[i] - 8, SH110X_WHITE);
      display.drawPixel(plamaX[i] + 2, plamaY[i] - 10, SH110X_WHITE);
    }
  }
}

void rysujRakieteGracza(uint16_t teraz) {
  if (rakietaGraczaAktywna) {
    display.drawBitmap(
      rakietaGraczaX, rakietaGraczaY,
      rakietaa[rakietaGraczaKierunek].bitmapa,
      rakietaa[rakietaGraczaKierunek].szerokosc,
      rakietaa[rakietaGraczaKierunek].wysokosc,
      SH110X_WHITE
    );
  }
}

void rysujKubka(uint16_t teraz) {
  if (atakTrafiony && (teraz - atakTrafionyCzas >= CZAS_ANIMACJI_ATAKU)) {
    atakTrafiony = false;
  }

  if (kubekAkcja == KUBEK_DASH_TELL) {
    if (kubekDashPoziomy) {
      display.drawFastHLine(0, kubekCelY + KUBEK_WYSOKOSC / 2, SCREEN_WIDTH, SH110X_WHITE);
    } else {
      display.drawFastVLine(kubekCelX + KUBEK_SZEROKOSC / 2, 8, SCREEN_HEIGHT - 8, SH110X_WHITE);
    }
  }

  if (kubekAkcja == KUBEK_CROSS_TELL) {
    display.drawFastVLine(kubekCelX, 8, SCREEN_HEIGHT - 8, SH110X_WHITE);
    display.drawFastHLine(0, kubekCelY, SCREEN_WIDTH, SH110X_WHITE);
  } else if (kubekAkcja == KUBEK_CROSS) {
    display.fillRect(kubekCelX - 2, 8, 5, SCREEN_HEIGHT - 8, SH110X_WHITE);
    display.fillRect(0, kubekCelY - 2, SCREEN_WIDTH, 5, SH110X_WHITE);
  }

  if (!kubekUkryty(teraz)) {
    rysujSylwetkeKubka(kubekX, kubekY, true, false, teraz);
    if (atakTrafiony) {
      display.drawBitmap(kubekX + 2, kubekY + 4, atak, 16, 16, SH110X_WHITE);
    }
  }
}
