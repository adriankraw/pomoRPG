# pomoRPG
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](#license)  

The Idea of PomoRPG is to create an idle game in which the the user manages various timers. This game will provide you with vaious text- & configfiles.  
You configure timers and let your character “level up” as you work. 

---
## Features
- Manage multiple named timers
- Character stats evolve based on time spend  
- Commands-driven interface (start, pause, stop, etc.)  
- Multiple UI views
- Text-based configuration

## Commands:
```
- save                      saving both timers and characterstats
- timer [nameOfTimer]       set header timer to [nameOfTimer]
- add [newTimerName]        add a new timer to the list
- pause                     pauses everything
- pause [name]              pauses a specific timer
- stop                      same as pause
- stop [name]               same as pause [name]
- start [name]              start a specific timer
- resume [name]             same as start [name]
- exit                      exit the app (CTRL + C)
- help                      toggle help (prints all writable command)
```

## UI-Commands:
```
- bigclock
- charsettings
- stopwatches
- fight
- eventlist
- circle
```

## Configuration Files
skilltree.txt — Skills & leveling config  
timerList.txt — List of timers and defaults  
saveFile.txt — Saved timers / character state  
The program reads and writes to these files to maintain persistent state across sessions.

## Getting Started

### Prerequisites

- A C++ compiler (supporting C++11 or newer)  
- CMake  
- A terminal / shell environment

### Installation

```bash
git clone https://github.com/adriankraw/pomoRPG.git
cd pomoRPG
mkdir build
cd build
cmake ..
cmake --build .
./start.pomo
```

Let me know if you got any ideas.
Currently working on
* [ ] More Skills
* [ ] Leveling System
* [ ] Inventory & Items
* [ ] Create Encounters
