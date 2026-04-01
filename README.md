# ✨ Particle Simulator

An interactive real-time particle simulator built with **C++** and **SFML 2.x**. Spawn and manipulate up to 60,000 particles with different visual effects, physics, and mouse interactions.

---

## ✨ Features

**5 Particle Modes**
- 🔥 Fire — warm rising flames that fade to dark embers
  <img width="579" height="457" alt="image" src="https://github.com/user-attachments/assets/69320fd6-4e74-4b2e-93ca-9a00b3744d5a" />

- 💧 Water — scattered droplets with fluid-like motion
  <img width="1094" height="557" alt="image" src="https://github.com/user-attachments/assets/d9092dae-e172-4800-ac7e-cd669bb576ca" />

- ⚡ Sparks — fast short-lived electric sparks
  <img width="525" height="313" alt="image" src="https://github.com/user-attachments/assets/804b91d9-ce76-4395-affa-08c5d08cd565" />

- 💨 Smoke — slow expanding smoke clouds
  <img width="759" height="675" alt="image" src="https://github.com/user-attachments/assets/d5fc691e-0647-4208-a0fb-762013e602de" />

- 🌈 Rainbow — colorful particles with HSV color cycling
<img width="495" height="388" alt="image" src="https://github.com/user-attachments/assets/2ec6177d-d5c3-4e10-8142-3e20924d1dd4" />

**Physics & Interactions**
- Toggle **gravity** on/off
- **Attract** particles toward the cursor (right click)
- **Repel** particles away from the cursor (middle click)
- Particles **bounce** off the bottom of the screen
- Adjustable **spawn rate** (1–300 particles/second)
- Optional **motion blur / trail effect**

---

## ⌨️ Controls

| Input | Action |
|---|---|
| Left Click (hold) | Spawn particles |
| Right Click (hold) | Attract particles to cursor |
| Middle Click (hold) | Repel particles from cursor |
| `1` – `5` | Switch particle mode |
| `G` | Toggle gravity |
| `C` | Clear all particles |
| `R` | Toggle trails (motion blur) |
| `+` / `-` | Increase / decrease spawn rate |
| `Esc` | Quit |

---

## 🛠️ Built With

- **C++**
- **SFML 2.x** — Simple and Fast Multimedia Library

---

## 🚀 Getting Started

### Prerequisites
- A C++ compiler (MSVC, GCC, or Clang)
- [SFML 2.x](https://www.sfml-dev.org/download.php) installed

### Build & Run

1. Clone the repository
   ```bash
   git clone https://github.com/YOUR_USERNAME/Particle-Simulator.git
   ```

2. Link SFML to your project and compile, for example with g++:
   ```bash
   g++ main.cpp -o ParticleSimulator -lsfml-graphics -lsfml-window -lsfml-system
   ```

3. Run the executable:
   ```bash
   ./ParticleSimulator
   ```

> If you're on Windows using Visual Studio, add the SFML include and lib directories to your project properties and link `sfml-graphics.lib`, `sfml-window.lib`, and `sfml-system.lib`.

---

## 📖 What I Learned

- Real-time graphics rendering using SFML's `VertexArray` with quads for performance
- Particle system design — lifetime, color interpolation, size fading
- HSV to RGB color conversion for smooth rainbow effects
- Physics simulation — gravity, attraction/repulsion, bounce, drag
- Using a `RenderTexture` with a semi-transparent overlay to achieve motion blur trails
- Managing large numbers of objects efficiently (up to 60,000 particles)

---

*Made as a learning project — 2nd year, Automatică și Informatică Aplicată, Universitatea Transilvania Brașov*
