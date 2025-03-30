# TDT4102 - Prosjekt

## Hvordan sette opp på datamaskinen

### Prerequisites 
* TDT4102 biblioteket i Vscode
* Git

### Set-up
1. Lag en mappe til prosjektet.
2. I VSCode trykk "Shift + Ctrl + P" og kjør "TDT4102: Create Project from TDT4102 template." -> "Blank Project"
3. Pull eller Clone github repoet i samme mappe som du lagde i trinn 1.
4. Du er good to go ":)"

## Github basics


### Set-up
Letteste måten å hente ett repo er å klone det hvis det er offentlig. Men i dette tilfellet må du bruke din egen bruker. 

Det letteste er å bruke SSH. For å sette opp dette åpner man terminalen (Linux) og gjør følgende.

1. `ssh-keygen -t ed25519 -b 4096 -C "din-epost@eksempel.com"`
2. `eval "$(ssh-agent -s)"`
3. `ssh-add ~/.ssh/id_ed25519`
4. `cat ~/.ssh/id_ed25519.pub`
5. Kopier outputten fra steg 4. (hele greia) og åpne brukerinnstillingene på github.
6. Gå til "SSH and GPG keys" og trykk "New SSH key"
7. Lim inn teksten fra seg 4. 
8. Du kan nå pushe/pulle.

### Hvordan bruke github.

Anbefaler å sette opp origins.
1. I mappen du skal kjøre prosjektet i, kjør `git init`
2. `git remote add origin git@github.com:ba11e-mos/gambling.git`
3. `git branch -M main`

Nå kan du kjøre "git pull origin main" for å hente repoet. 

### Hvordan pushe til dette prosjektet

Siden TDT4102 biblioteket kjører ulikt på ulike maskiner er det viktig å ikke laste opp filer som inneholder "environment variables" feks. meson.build og lignende.

Når man pusher gjør man derfor:
1. Legg kun til filene man vi pushe `git add 'filer'`
2. Legg til hjelpsom commit message `git commit -m 'Denne teksten har mening'`
3. Nå er du klar til å pushe. `git push origin main`

Hvis man vil pushe til en annen branch for sammenligning endrer man bare til `git push origin 'branch'`






Her ligger kun c++ kodefiler, build og backend lages på din pc for å sikre at det ikke skjer noe fucky.