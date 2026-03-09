# ✨ Particle Simulator

An interactive real-time particle simulator built with **C++** and **SFML 2.x**. Spawn and manipulate up to 60,000 particles with different visual effects, physics, and mouse interactions.

---

## ✨ Features

**5 Particle Modes**
- 🔥 Fire — warm rising flames that fade to dark embers
- 💧 Water — scattered droplets with fluid-like motion
- ⚡ Sparks — fast short-lived electric sparks
- 💨 Smoke — slow expanding smoke clouds
- 🌈 Rainbow — colorful particles with HSV color cycling

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
