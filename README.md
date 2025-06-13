# Warpbound 🚀

Warpbound is a wave-based resource survival game developed using **SDL2** and **Hornet Engine** (In-house) for the *Games Programming 2* module at Northumbria University.
I achieved a mark of **85/100**, highlighting my strong system architecture and my ability to implement scalable and maintainable code.
## Highlights
- Written in C++ with a clean, modular architecture. [Class Diagram](https://drive.google.com/file/d/1Wa8vY5ho-f2eOyLEkibAxh_GCkdgCWga/view?usp=sharing)
- Built using CMake, tested primarily on MSVC (Windows)
- **Custom Spritesheet Animator** efficiently animates sprites using just a few parameters.
- **GUI Shop System** allows the player to spend credits between waves on health, ammo, armour and missiles — or warp to the next galaxy. Prices scale with progression.
- **Modular HUD System** with minimal coupling only to the SpaceShip class.
- **Difficulty Scaling** that affects shop prices, enemy health, and enemy count.

## How to Build

### Required
- CMake 3.10+  
- Tested on Windows(MSVC)

### Building

```
mkdir build
cd build
cmake ..
```
## Images
### Combat
![image](https://github.com/user-attachments/assets/afa28acf-64e5-45f5-8960-ca2b7d16142d)
### Shop
![image](https://github.com/user-attachments/assets/b42cdab4-c654-410b-8348-e7132d7ba51b)


