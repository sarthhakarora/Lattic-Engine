# Lattic Engine

Lattic engine is a real time **3D orbital mechanics simulator** written in **C** using **raylib**.  
This project simulates gravitational interaction between celestial bodies using Newtonian physics and numerical integration.

The goal of this project was **not ultra realism or NASA level accuracy**, but to:
- understand vector math
- apply classical mechanics (`F = ma`)
- build a stable physics update loop
- understand optimization techniques with run-time rendering 
- explore multi-threading
- visualize orbital motion in 3D
- **and most importantly** have a fun engine to play around with!

---

## Demo

![Orbit demo](assets/output.gif)

---

## Screenshots

### Simulation UI
![UI](assets/controls.png)

### Main Menu
![Menu](assets/titlescreen.png)

---

## Features

- Real time **N-body gravity simulation**
- Stable orbits using **Verlet integration**
- **Custom planets** with **custom textures** that can be added by the user
- **Lua scripting** layer for advanced modifications
- **Save/load system** for different simulations
- Runtime UI controls using **raygui**
- Freefly 3D camera
- Multiple gravity sources supported
- Native windows support and linux support (via wine)
- Real-time UI/UX implementation
- Multi-threaded asset loading for faster loading times
- Cross-platform builds and cross completion
- Grid and debug rendering tools
- Skybox

---

## Features I plan on adding

- Orbit trails visual clarity
- Improved graphics through support for features such as Normal maps, Bloom and Custom Shaders
- Multi-threaded or hardware accelerated physics
- Support for O<sup>n</sup> physics
- Support for SI units for research
- Graphic scaling options for low end systems

---

## Features that are out of scope

- Einstein orbital physics / General relativity
- Ray or path tracing
- GUI based creation simulations (Engine relies on its Lua API for simulations creation)

---

## How does it work?

The simulation is based on **Newtonian gravity** model:

**F = G * m1 * m2 / r²**

From this:

Each frame:
1. Compute gravitational force from all gravity sources
2. Convert force → acceleration
3. Integrate velocity
4. Integrate position

This produces **elliptical orbits** when the initial velocity is set correctly.

---

## Integration Method

The simulator uses a **velocity aware Verlet / semi-implicit integrator**:

**x(t+dt) = x(t) + v(t)·dt + ½a(t)·dt²**  
**v(t+dt) = v(t) + ½(a(t) + a(t+dt))·dt**

In simpler terms, velocity is updated using the **average acceleration over the timestep**, rather than assuming constant acceleration as in Euler integration due to there being a limited timestep (in other words fps or calculation per second) and IEEE-754 causing small numerical errors **but this is not the primary cause** of instability unstable orbits.  
This is more stable than **naive Euler integration** and helps **reduce energy drift**.

---

## Scripts & Simulations

This engine supports loading simulations via Lua scripts.

On startup, the engine scans the scripts folder and allows selection a simulation through the user interface. Each script defines a world, planets and initial conditions.

Several example simulations are included to demonstrate performance, stability and orbital behavior. Feel free to try them out and have fun!

Note: lower values in the scripts if you notice bad performance (more optimizations are on the way but hardware limitations can also cause this the values have been tuned for my high end pc with a 7800xt, 32gb ddr5 and ryzen 5 7600x)

---

## Optimizations

- Lazy-loaded textures lowering VRAM usage, RAM usage and massively program load times

---

## Controls/Keybinds

| Key | Action |
|----|-------|
| WASD | Move camera |
| Mouse | Look around |
| SPACE / SHIFT | Vertical movement |
| ENTER | Enable cursor |
| RSHIFT | Disable cursor |
| M | Toggle Menu |
| P | Toggle Time |
| ESC | Back to menu |

---

## Building the project

## WINDOWS
# Requirements **IMPORTANT** 
- MSYS2 (MinGW64 environment)
- MinGW-w64 GCC compiler
- Cmake

# Build Instructions 

```
git clone https://github.com/sarthhakarora/Orbital-Physics-Simulator.git
cd Orbital-Physics-Simulator
mkdir build
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build --target run
```

## LINUX
# Requirements **IMPORTANT** 
```
- MinGW-w64 GCC compiler
- Cmake
- Wine
```

# Build Instructions (wine)

**WARNING:** This is not native linux support this is running on linux via the translation layer wine
```
git clone https://github.com/sarthhakarora/Orbital-Physics-Simulator.git
cd Orbital-Physics-Simulator

mkdir build
cmake -S . -B build \
-DCMAKE_SYSTEM_NAME=Windows \
-DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
-DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++

cmake --build build
```

---

## Optional runtime flags:

orbitalPhysics.exe --showlogs

---

## Development (VS Code)

```
This repo includes optional VS Code configs for:
- CMake
- GCC
- GDB debugging (F5)

They are not required to build the project.
```

---

## Performance / Scaling

The engine is designed to scale across different hardware depending on simulation size.

Performance benchmarks will be added after testing across multiple systems.

## Tech Used

- I have tried to stay minimal with libraries used and have only kept essential ones.

- **Language:** C (C11)
- **Compiler:** GCC (No msvc port yet)
- **Graphics:** raylib
- **Math:** raymath
- **UI:** raygui
- **Miscellaneous:** Win32 API, Lua API
- **Platform:** Windows, Linux (wine)
