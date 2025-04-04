# 🧨 Tank Battle - Maze War Game

A real-time two-player tank battle game played inside a maze. Each player controls a tank using separate keyboard controls to manage each track independently. The goal is to destroy the opponent's tank using movement, strategy, and precise cannon fire.

---

## 📘 Project Instructions
This project was created as part of the **C++ and Object-Oriented Programming course**  
at MTA College, Year 2025, Semester B.
This game follows detailed functional and gameplay specifications provided in a Google Docs document.

📄 **Full Game Specs & Requirements**:  
👉 [View the Instructions Document](https://docs.google.com/document/d/1D0PUaVaaIaBdFTpSsuRcAUAmZFRctlOgsvb4OKACP9g/edit?usp=sharing)


---

## 🎮 Gameplay Overview

- Two players control tanks inside a maze.
- Tanks move automatically in the last selected direction.
- Movement is based on separate controls for each tank track.
- Use strategy to rotate, navigate the maze, and fire accurately.
- Avoid mines, outsmart your opponent, and shoot to win.

---

## 🕹️ Controls

### Player 1:
| Action                | Key  |
|-----------------------|------|
| Left track forward    | `Q`  |
| Left track backward   | `A`  |
| Right track forward   | `E`  |
| Right track backward  | `D`  |
| Stay (stop movement)  | `S`  |
| Shoot                 | `W`  |

### Player 2:
| Action                | Key  |
|-----------------------|------|
| Left track forward    | `U`  |
| Left track backward   | `J`  |
| Right track forward   | `O`  |
| Right track backward  | `L`  |
| Stay (stop movement)  | `K`  |
| Shoot                 | `I`  |

> 🔠 Input is **case-insensitive**.

---

## 🔁 Tank Movement & Rotation

- Rotate by running one track while stopping the other.
- Four rotation methods:
  - Right track forward ONLY → counter-clockwise (8 steps)
  - Right forward + Left back → counter-clockwise (4 steps)
  - Left track forward ONLY → clockwise (8 steps)
  - Left forward + Right back → clockwise (4 steps)

---

## 🧱 Game Elements

| Element     | Symbol | Description                                   |
|-------------|--------|-----------------------------------------------|
| Tank        | `O`    | Tank's main body                              |
| Cannon      | `| / \\ -` | Directional, shown relative to the tank   |
| Wall        | `#`    | Static obstacle, breaks after 2 hits         |
| Mine        | `@`    | Hidden trap, destroys tank on contact        |
| Shell       | `*`    | Fired by tanks, destroys other tanks/shells  |

- Cannons disappear if hit by a shell, but tanks survive.
- Mines remain under cannons and reappear when cannon moves away.

---

## 🌍 Game Board & Rules

- The game board wraps around (teleport from one edge to opposite edge).
- Tanks cannot aim into walls.
- Shells can collide with other shells or walls.
- After a tank shoots, it cannot shoot again for 5 game-loop cycles.
- The game ends when one or both tanks are destroyed.

---

## ⏸️ Pausing the Game

- Press `ESC` to pause the game.
- When paused:
  - Press `ESC` again to **resume**
  - Press `X` or `x` to **return to the main menu**
- Game state is preserved during pause.

---

## 📋 Game Over

- A tank is destroyed if:
  - Hit directly by a shell
  - Steps on a mine
- If both tanks are destroyed simultaneously: **It's a tie**
- After game over, a message is shown and the game returns to the main menu.

---

## ✅ To Do / Improvements

- Add ASCII tank art
- Add a splash screen and game logo
- Create support for color and no-color display
- Sound support (optional)
- Save high scores and stats

---

## 🛠️ Built With

- Language: C++
- Platform: Windows Console
- Libraries: `<conio.h>`, `<windows.h>`

---

## 👨‍💻 Author

- [Your Name] – Developer

---

## 📜 License

This project is for educational/non-commercial purposes. Feel free to use and extend.
