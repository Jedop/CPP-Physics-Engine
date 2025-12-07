# 2D Verlet Physics Engine

![Physics Simulation Demo](demo.gif)
*(A simulation of 500+ particles with collision resolution and boundary constraints)*

## Overview
A high-performance 2D physics simulation built from scratch in **C++** using the **SFML** library. 

This engine moves away from standard Euler integration, utilizing **Verlet Integration** to handle particle trajectories. This ensures greater numerical stability and energy conservation, making it ideal for simulating complex systems like particle piles, cloth, and soft bodies.

## Key Features
- **Verlet Integration:** Implements position-based dynamics ($x_{n+1} = 2x_n - x_{n-1} + a \cdot dt^2$) for stable trajectory calculation.
- **Collision Resolution:** Handles particle-particle collision detection and resolution.
- **Constraint Solving:** Implements rigid boundary constraints (walls/floor) with coefficient of restitution (energy loss).
- **Dynamic Instancing:** Real-time object spawning with randomized properties.
- **Render Pipeline:** Custom rendering loop using SFML shapes.

## The Math
The core solver relies on **Verlet Integration**, which calculates the next position based on the current position and the previous position, rather than storing explicit velocity. 

The update logic follows:
```cpp
velocity = position - last_position
new_position = position + velocity + acceleration * (dt * dt)
```
This approach implicitly handles velocity, making the system robust against the errors often found in simple Euler integration.

## Build Instructions

### Prerequisites
- C++ Compiler (G++ / Clang)
- CMake (Version 3.10+)
- SFML Library

**Installing Dependencies (Arch/EndeavourOS):**
```bash
sudo pacman -S base-devel cmake sfml
```

**Installing Dependencies (Ubuntu/Debian):**
```bash
sudo apt install build-essential cmake libsfml-dev
```

### Compilation & Running
```bash
# 1. Clone the repository
git clone https://github.com/Jedop/CPP-Physics-Engine.git
cd CPP-Physics-Engine

# 2. Create build directory
mkdir build && cd build

# 3. Compile
cmake ..
make

# 4. Run
./PhysicsEngine
```

## Future Roadmap
- [ ] **Spatial Partitioning:** Implement a Grid or Quadtree to optimize collision detection from $O(N^2)$ to $O(N)$.
- [ ] **Link Constraints:** Implement distance constraints to simulate cloth and rope dynamics.
- [ ] **Interactive Forces:** Add mouse interaction to apply external forces to the system.

## License
MIT License
