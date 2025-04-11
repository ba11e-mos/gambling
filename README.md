# TDT4102 - Prosjekt

https://github.com/ba11e-mos/gambling

## Beskrivelse av programmet

Gambling++ er et program som lar brukeren spille tre populære pengespill:
- **Poker:** Spillet følger reglene for Texas Hold'em, hvor spilleren kan satse, 'checke'/stå, eller kaste seg.
- **BlackJack:** Målet er å få en håndverdi nærmest mulig 21 uten å gå over. Spilleren kan trekke kort eller stå.
- **Slots:** Et klassisk spill hvor spilleren kan spinne et hjul med kort og vinne basert på antall kort av samme sort.

## Logikk og implementasjon

Kortstokken er implementert som en `CardDeck`-klasse som inneholder en liste over `Card`-objekter. Kortene trekkes og stokkes dynamisk, og samme kortstokk gjenbrukes mellom spillene.

BlackJack og Poker bruker spilltilstander (`GameState`) for å håndtere de ulike fasene i spillene, som å trekke kort, animere trekking, avgjøre vinneren osv.

Brukergrensesnittet er bygget med SDL2 og AnimationWindow, vi bruker en klasse `GameWindow` som arver fra AnimationWindow som legger til funksjoner som sikrer at spillene ser like ut.

## Ressurser brukt i prosjektet
- **AnimationWindow:** Endret for å støtte alpha-kanalen og legge til en egen font.
- **Reddit/StackOverflow:** Brukt til å finne løsninger på problemer med SDL2 og AnimationWindow.
- **ChatGPT/Copilot:** Brukt til debugging av absurde feilmeldinger.
- **SDL2-dokumentasjon:** [https://wiki.libsdl.org/SDL2/FrontPage](https://wiki.libsdl.org/SDL2/FrontPage)

## !!OBS!!
På grunn av endringene gjort til filer i AnimationWindow må disse filene også være med for at koden skal funke. (Font.h, FontCache.cpp, FontCache.cpp, AnimationWindow.cpp, AnimationWindow.h)

Hvis grafikkene ser feil ut (for store/for små) er det mest sannsynlig at du har på skjermskalering på datamaskinen din. Sett denne ned/opp til 100% for best spilleopplevelse.

## Hvordan sette opp og bygge programmet

1. Last ned og sett opp TDT4102 biblioteket i vsCode.
2. Opprett nytt tomt prosjekt med TDT4102.
3. Pakk ut .zip mappen til prosjektet. Eller bruk `git clone https://github.com/ba11e-mos/gambling`
4. Ctrl + F5 for å bygge og kjøre programmet.

## Hvordan kjøre programmet

1. Last ned .zip mappen til ditt system fra [utgivlesene](https://github.com/ba11e-mos/gambling/releases)).
2. Pakk ut .zip filen til en mappe (spiller ingen rolle hvor så lenge alle filene er samlet)
3. Kjør .exe filen (windows) eller ELF filen (linux).
4. Kos deg!
