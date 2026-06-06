# Folling Stone

A black-and-white Tetris game built with C++ and SDL2, inspired by the anime **Dr. Stone** specifically the scene where **Sai Nanami**, creates a Tetris-like game. This project is my attempt to bring that fictional game to life, though with some elements simplified or left out.

---

## Tech Stack

- C++17
- SDL2
- SDL2_ttf
- CMake
- vcpkg (Windows)

---

## Project Structure

```
folling_stone/
├── assets/
│   ├── PixelifySans-Bold.ttf
│   └── PixelifySans-Regular.ttf
├── .gitignore
├── CMakeLists.txt
├── vcpkg.json
├── game.h
├── game.cpp
├── render.h
├── render.cpp
└── main.cpp
```

---

## Controls

| Key       | Action                     |
|-----------|----------------------------|
| `←` `→`  | Move piece                 |
| `↑`       | Rotate                     |
| `↓`       | Soft drop                  |
| `Space`   | Hard drop                  |
| `Q`       | Quit                       |
| `R`       | Restart (on game over)     |

---

## Grade System

Grade is calculated based on **score** and **efficiency** (score ÷ lines cleared):

| Grade | Min Score | Min Efficiency |
|-------|-----------|----------------|
| S+    | 20000     | > 200          |
| S     | 15000     | > 180          |
| A     | 10000     | > 150          |
| B     | 6000      | > 120          |
| C     | 3000      | > 100          |
| D     | 1000      | —              |
| E     | —         | —              |

---

## Build

### Linux / macOS

```bash
# Arch Linux
sudo pacman -S sdl2 sdl2_ttf cmake

# Ubuntu / Debian
sudo apt install libsdl2-dev libsdl2-ttf-dev cmake

# macOS
brew install sdl2 sdl2_ttf cmake

# Build
cmake -B build
cmake --build build

# Run from project root (so assets/ is found)
./build/tetris
```

### Windows

**Prerequisites:**
- [CMake](https://cmake.org/download/) ≥ 3.16
- [vcpkg](https://github.com/microsoft/vcpkg)
- Visual Studio 2019/2022 with "Desktop development with C++"

```powershell
# Setup vcpkg (once)
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat

# Build
cmake -B build -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release

# Run from project root
.\build\Release\tetris.exe
```

> **Note:** Always run the executable from the project root folder, not from inside `build/`, so that `assets/` is found correctly.

---

## Notes

- This project is for **learning and fun** — it is not a full recreation of the game depicted in Dr. Stone.
- The leaderboard is **in-memory only** — scores reset when the application is closed.
- Font used: [Pixelify Sans](https://fonts.google.com/specimen/Pixelify+Sans) by Eknath Bendre, licensed under SIL Open Font License.

---
