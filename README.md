SnakeProject/
├── sources/
│   └── main.cpp
├── libraries/
│   ├── same.h / game.cpp
│   ├── snake.h / snake.cpp
│   ├── grid.h / grid.cpp
│   ├── livello.h / livello.cpp
│   └── menu.h / menu.cpp
├── scoreboard/
│   └── scoreboard.txt
├── CMakeLists.txt
└── README.md


Funzionalità principali
Interfaccia testuale (ASCII) usando ncurses

Controllo del serpente tramite tastiera (freccette, wasd)

Gestione multi-livello, con velocità crescente

Navigazione tra i livelli con i tasti:

n → livello successivo

b → livello precedente

Menù iniziale per avviare una nuova partita

Struttura modulare con classi per:

Game → motore del gioco

Grid → gestione della griglia

Snake → movimento e corpo del serpente

Livello → struttura dei livelli (lista doppiamente collegata)

(altre classi in futuro)
