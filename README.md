# Snake Game

## Description

A modern reinterpretation of the classic “Snake” game, written in C++ using SDL2. Unlike traditional implementations, colliding with walls does not end the game — instead, play continues and special bonuses appear that either shorten the snake or reduce its speed. Navigate the snake to consume food, manage your length, and strategically use bonuses to maximize your score.

---

## 1. Prerequisites

-   **C++ compiler** with C++17 support
-   **SDL2** library (>= 2.0.0) and development headers
-   **Make**
-   **pkg-config** (on Linux/macOS) or equivalent (e.g. vcpkg/Conan on Windows)

> Ensure all dependencies are installed and discoverable via your system’s include/library paths.

---

## 2. Setup & Run

```bash
# 1. Clone repository
git clone https://github.com/YOUR_USERNAME/snake-game.git

# 2. Change directory to the project
cd snake-game

# 3. Build and run
make && ./main
```

# License

This project is licensed under the terms of the GNU GPLv3 license.

# Authors

-   **[Bartosz Kluska](github.com/kluczi)**
