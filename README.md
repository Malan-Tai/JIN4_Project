# Hollow Lens (temporary name)

For now, this is more of a technical demo of a simple animation-driven game engine than an actual game.  

## How to play (controller only)

Directional arrows : move around, fast fall, throw a grabbed enemy  
Left joystick : vertically, toggle left (red) lens  
Right joystick : vertically, toggle right (blue) lens  

A : jump  
B : hold to sprint, press to roll (even in mid air)  
X : when grabbing an enemy, clone yourself. Else, switch controlled clone  
Y : change weapon size (has no effect for now)  

Right bumper : light attack  
Right trigger : heavy attack  
Left bumper : switch weapon range (melee / ranged)  
Left trigger : grab a staggered enemy  

You can stagger an enemy by hitting it fast enough (temporary implementation).

# JIN4_Project

Contains two directories:  
- HitboxParser, which contains a python program used to parse anim hit and hurt boxes  
- Game, which contains everything else needed for the actual game  

## Hitbox Parser

For now, the parser cannot parse correctly some configurations, so it might be preferrable to leave a one-pixel space between boxes.  
Examples of problematic configurations:  

```
..XXXXX
..XXXXX
..XXXXX
XXX....
XXX....
```

or

```
....XXXX
XXXXXXXX
XXXX....
```

## Game

Contains the actual game source code, with resources, .h and .cpp .
To build the project on your computer, you can use this procedure (in french !) : http://www-inf.telecom-sudparis.eu/COURS/CSC4526/new_site/Supports/Documents/OutilsCSC4526/outilsCSC4526.html

