# Triple T Adventures

## Opis gry

`Triple T Adventures` to gra akcji na `Arduino Uno` z ekranem `SH1106 128x64`.
Gracz przechodzi przez trzy walki z bossami, zdobywa ulepszenia i konczy gre po pokonaniu finalowego przeciwnika.

Aktualny przebieg gry:

1. ekran startowy,
2. animacja tytulu,
3. wybor trudnosci,
4. walka z rekinem,
5. nagroda po rekinie: buty,
6. mini-gra przed krokodylem,
7. walka z krokodylem-samolotem,
8. nagroda po krokodylu: rakieta,
9. mini-gra przed finalem,
10. walka z finalowym bossem,
11. ekran zwyciestwa albo przegranej.

## Sterowanie

Sterowanie jest czytane bezposrednio z portow mikrokontrolera:

- `Gora` - ruch gracza w gore
- `Dol` - ruch gracza w dol
- `Lewo` - ruch gracza w lewo
- `Prawo` - ruch gracza w prawo
- `START` - wybor w menu i zwykly atak melee
- `PD4` - przycisk alternatywny

Zachowanie przycisku `START`:

- na ekranie startowym uruchamia gre,
- w menu trudnosci zatwierdza wybor,
- w kazdej walce wykonuje zwykly atak melee.

Zachowanie przycisku `PD4`:

- po zdobyciu butow i przed zdobyciem rakiety uruchamia mocny dash gracza,
- cooldown dasha wynosi `2000 ms`,
- po zdobyciu rakiety `PD4` przestaje robic dash i w finale odpala rakiete.

## Trudnosc

Po animacji startowej pojawia sie wybor trudnosci:

- gracz ma stale `100 HP`,
- trudnosc zmienia zdrowie przeciwnikow.

Aktualne wartosci:

- `easy`
  - rekin: `15 HP`
  - krokodyl: `120 HP`
  - Cup: `100 HP`
- `medium`
  - rekin: `20 HP`
  - krokodyl: `150 HP`
  - Cup: `120 HP`
- `hard`
  - rekin: `30 HP`
  - krokodyl: `200 HP`
  - Cup: `160 HP`

## Etap 1: Rekin

### Cel

Pokonac rekina i przejsc do kolejnego etapu.

### Przebieg walki

- rekin goni gracza po planszy,
- gracz atakuje go z bliska przyciskiem `START`,
- co pewien czas pojawia sie siatka,
- jesli rekin wejdzie w siatke, zostaje na chwile unieruchomiony.

### Dodatkowe zasady

- rekin zadaje obrazenia przy kontakcie,
- obrazenia kontaktowe maja cooldown, wiec nie naliczaja sie co klatke,
- po trafieniu rekina pojawia sie animacja uderzenia i miganie przeciwnika,
- paski zdrowia sa liczone z prawdziwych maksymalnych HP.

### Jak wygrac

- unikac kontaktu,
- lapac rekina w siatke,
- podchodzic i uderzac go melee.

### Nagroda

Po wygranej pojawia sie ekran `NEW ITEM` z bitmapa butow.

Efekt:

- `movement_speed` rosnie o `1`,
- od tego momentu gracz ma dostep do mocnego dasha na `PD4`.

## Mini-gra przed krokodylem

To mini-gra zbierania.

### Cel

Zebrac `3` cele w limicie czasu.

### Zasady

- gracz steruje malym kwadratem,
- na planszy pojawia sie cel do zebrania,
- po zebraniu celu losowany jest nastepny,
- trzeba zebrac `3` cele,
- limit czasu wynosi `9000 ms`,
- porazka powoduje powtorke mini-gry.

### Efekt sukcesu

- pozycja gracza jest resetowana,
- startuje walka z krokodylem.

## Etap 2: Krokodyl-samolot

### Cel

Pokonac drugiego bossa i zdobyc rakiete potrzebna w finale.

### Przebieg walki

- boss lata przez ekran w lewo i w prawo,
- dopasowuje wysokosc lotu do pozycji gracza,
- zrzuca bomby,
- na planszy pojawiaja sie rakiety do podniesienia.

### Bomby

- bomba spada z bossa,
- po chwili przechodzi w wybuch,
- wybuch ma `2` ticki i zwieksza promien zagrozenia,
- realny promien obrazen jest wiekszy niz sam sprite bomby,
- dash z butow ma pomagac odskoczyc od wybuchu.

### Dash gracza w tym etapie

- dash jest dostepny po zdobyciu butow,
- uruchamia sie przyciskiem `PD4`,
- jest mocniejszy niz zwykle przesuniecie,
- cooldown dasha wynosi `2 s`.

### Rakiety etapu

- rakiety spawnia sie tylko w obszarze, z ktorego da sie je podniesc,
- po podniesieniu rakieta leci w strone bossa,
- po trafieniu uruchamia sie stara bitmapowa animacja wybuchu rakiety.

### Aktualny tryb testowy

Obecnie testowy one-shot jest wylaczony:

- `TEST_KROKODYL_JEDNA_RAKIETA = false`

Skutek:

- krokodyl nie ginie juz od jednej rakiety,
- rakieta zadaje mu normalne obrazenia.

### Nagroda

Po wygranej pojawia sie ekran `NEW ITEM` z ikona rakiet.

Efekt:

- `graczMaRakiete = true`,
- od tej chwili `PD4` jest zarezerwowane pod rakiete w finale.

## Mini-gra przed finalem

To mini-gra refleksowa.

### Cel

Nacisnac `START` w odpowiednim momencie.

### Zasady

- po pasku porusza sie marker,
- na pasku jest strefa trafienia,
- trzeba zatrzymac marker wewnatrz tej strefy,
- porazka powoduje powtorke mini-gry.

### Efekt sukcesu

- pozycja gracza jest resetowana,
- startuje finalowa walka.

## Etap 3: Finalowy boss - Cup

### Cel

Pokonac ostatniego bossa i zakonczyc gre zwyciestwem.

### Podstawy walki

- boss ma zmienne HP zalezne od trudnosci:
  - `easy`: `100 HP`
  - `medium`: `120 HP`
  - `hard`: `160 HP`,
- walka ma `3` fazy zalezne od pozostalego zdrowia,
- `START` robi zwykly atak melee,
- `PD4` odpala rakiete,
- cooldown rakiety wynosi `2000 ms`.

### Melee gracza w finale

- zwykly atak melee ma ten sam sposob liczenia zasiegu co atak z pierwszej walki,
- po trafieniu pojawia sie ta sama bitmapowa animacja `atak`,
- boss przyjmuje zwykle obrazenia tylko w oknach podatnosci.

### Faza 1

Aktywna powyzej `70 HP`.

Boss uzywa glownie:

- dasha,
- rzutu mieczy.

#### Dash

- boss najpierw pokazuje linie ostrzegawcza,
- potem wykonuje przelot poziomy albo pionowy,
- po zakonczeniu dasha ma okno podatnosci,
- hitbox obrazen przy dashu jest mniejszy i oparty o srodek bossa, nie o obramowanie sprite'a.

#### Rzut mieczy

- boss rzuca dwa pociski w strone gracza,
- trafienie mieczem zadaje obrazenia.

### Faza 2

Aktywna od `70 HP` do `31 HP`.

Dochodzi:

- teleport,
- plamy kawy.

#### Teleport

- boss znika na chwile,
- pojawia sie w nowym miejscu,
- po wyjsciu z teleportu ma krotkie okno podatnosci.

#### Plamy kawy

- na planszy pojawiaja sie strefy zagrozenia,
- wejscie na plame zadaje obrazenia,
- plamy znikaja po czasie.

### Faza 3

Aktywna przy `30 HP` i mniej.

Boss dalej uzywa poprzednich atakow i dodatkowo wykonuje `Cross Cut`.

#### Cross Cut

- najpierw pojawia sie pionowa i pozioma linia ostrzegawcza,
- ta cienka linia jest tylko indykatorem i sama nie zadaje obrazen,
- po chwili odpalany jest wlasciwy atak po osiach krzyza,
- podczas zapowiedzi boss moze dostac obrazenia.

### Kiedy boss przyjmuje obrazenia

Boss jest najbardziej podatny:

- po dashu,
- po teleportacji,
- podczas zapowiedzi `Cross Cut`,
- po trafieniu rakieta.

### Rakieta w finale

Rakieta:

- zadaje wieksze obrazenia niz zwykly atak,
- oglusza bossa,
- otwiera dluzsze okno podatnosci.

### Kontakt z bossem

- zwykly kontakt z bossem zadaje obrazenia tylko centralnym hitboxem,
- dash bossa takze zadaje obrazenia centralnym, jeszcze mniejszym hitboxem.

## Zakonczenie gry

### Przegrana

Jesli zdrowie gracza spadnie do `0` podczas dowolnej walki:

- pojawia sie ekran przegranej,
- rozgrywka sie konczy.

### Wygrana

Jesli zdrowie finalowego bossa spadnie do `0`:

- pojawia sie ekran koncowy,
- wyswietlane sa komunikaty:
  - `YOU WIN`
  - `Cup defeated`
  - `Reset to play again`
- gra zatrzymuje sie na tym ekranie i czeka na reset urzadzenia.

## Paski zdrowia

Na ekranie walki sa dwa paski:

- po lewej stronie zdrowie gracza,
- po prawej stronie zdrowie aktualnego bossa.

Paski sa skalowane wedlug rzeczywistych maksymalnych wartosci HP dla danego etapu.

## Losowosc

Gra uzywa `randomSeed(millis())`, wiec:

- spawn siatki,
- bomby,
- rakiety,
- czesc zachowan finalowego bossa

nie powinny powtarzac sie identycznie po kazdym restarcie.

## Uwagi techniczne

- gra jest zoptymalizowana pod `Arduino Uno`,
- kod uzywa malych typow (`uint8_t`, `int8_t`, `uint16_t`), zeby oszczedzac RAM,
- odejmowanie HP jest saturujace, wiec zdrowie nie zawija sie juz ponizej zera.

## Szybkie streszczenie

1. Uruchom gre przyciskiem `START`.
2. Wybierz trudnosc.
3. Pokonaj rekina.
4. Odbierz buty i korzystaj z dasha na `PD4`.
5. Wygrywaj mini-gre zbierania.
6. Pokonaj krokodyla.
7. Odbierz rakiete.
8. Wygrywaj mini-gre refleksowa.
9. Pokonaj finalowego bossa `Cup`, atakujac melee `START` i rakieta na `PD4`.
10. Obejrzyj ekran zwyciestwa i zresetuj urzadzenie, jesli chcesz zagrac ponownie.
