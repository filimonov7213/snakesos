# 🐍 SnakeGame

Versione testuale del classico gioco **Snake**, sviluppata in **C++** con libreria **ncurses/pdcurses**.  
Il progetto implementa:
- livelli multipli con velocità e limiti di tempo,
- punteggio dinamico basato sulle mele raccolte,
- wrap-around (il serpente passa da un bordo all’altro),
- classifica persistente (Top 10).

---

## 📦 Requisiti

### Linux
Assicurati di avere `ncurses` installato:
```bash
sudo apt update
sudo apt install libncurses-dev
```

### Windows
- Compilazione con MinGW e **PDCurses**.
- Scarica e includi PDCurses nella cartella `libraries/`.

---

## ⚙️ Compilazione

### Con CMake
```bash
mkdir build
cd build
cmake ..
make
```

Su Windows con MinGW:
```powershell
cmake -G "MinGW Makefiles" ..
mingw32-make
```

L’eseguibile sarà `SnakeGame` (o `SnakeGame.exe` su Windows).

---

## 🎮 Come si gioca

- **Menu principale**
    - `Nuova Partita`
    - `Visualizza Classifica`

- **Comandi di gioco**
    - `W` o Freccia Su → su
    - `S` o Freccia Giù → giù
    - `A` o Freccia Sinistra → sinistra
    - `D` o Freccia Destra → destra
    - `P` → pausa

- **Regole**
    - Mangia le mele (`@`) per guadagnare punti.
    - Non urtare te stesso → game over.
    - Puoi attraversare i bordi (wrap-around).
    - Ogni livello ha un **tempo limite**. Scaduto → game over.

- **Post-partita**
    - `n` → livello successivo
    - `b` → livello precedente
    - `q` → ritorna al menu

---

## 🏆 Classifica

- I punteggi vengono salvati in un file di testo (`scores.txt`).
- Solo i **Top 10** vengono mantenuti.
- Alla fine di ogni partita viene richiesto il nome del giocatore.

---

## 📂 Struttura del progetto

- `libraries/`
    - `Board.*` → gestione griglia di gioco e bordi
    - `Snake.*` → logica serpente (matrice booleana + array segmenti)
    - `Apple.*` → mela
    - `Drawable.*` → classe astratta, rappresenta un oggetto disegnabile sullo schermo.
    - `SnakeGame.*` → ciclo principale di gioco
    - `Livello.*` → definizione livelli (dimensioni, velocità, tempo limite, moltiplicatore punti)
    - `Game.*` → gestione della lista di livelli e progressione
    - `Menu.*` → interfaccia menu principale
    - `Leaderboard.*` → classifica e salvataggio punteggi
- `source/main.cpp` → entry point
- `scoreboard/scoreboard.txt` → classifica
- `CMakeLists.txt` → configurazione build

