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
---
## How to play

### Goal
Navigate through systems to find the wormhole to a lost civilisation. Infected ships will try to stop you. Taking damage increases your infection level—reach full infection and you'll be dragged back to the infected's home system.
### Credits & Upgrades
Earn credits by destroying infected ships. Spend them in the space station to restock Ammo, Missiles, Health, Armor, and Quantum Fuel (needed to jump to the next system). After 5 jumps, you'll reach the wormhole.
### Difficulty
Enemy strength and numbers scale with the difficulty shown in the top-left corner. Prices rise too, but you'll earn more credits per kill. *Tip: You can shoot down enemy missiles!*
### Shop & Resources
Manage your resources carefully, once a wave starts, the shop closes. There's a 30-second grace period between waves for repairs and restocking (press "U" to skip). If you’re caught without ammo or missiles, it’s game over.
### Controls
`W,A,S,D` - Movement <br>
`LMB` - Main Gun <br>
`RMB` - Missile Barrage <br>
`U` - Skip Wave Cooldown (Will be kicked out of shop) <br>
`M` - Mute Background Music <br>

**Cheat Keys** <br>
`;` - Kill all enemies <br>
`'` - Add 1000 credits + 1 Enemy Death <br>
`[` - Good Ending <br> 
`]` - Bad Ending <br> 

## Images
### Combat
![image](https://github.com/user-attachments/assets/afa28acf-64e5-45f5-8960-ca2b7d16142d)
### Shop
![image](https://github.com/user-attachments/assets/b42cdab4-c654-410b-8348-e7132d7ba51b)


