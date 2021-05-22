# JIN4_Project

Contains two directories:
HitboxParser, which contains a python program used to parse anim hit and hurt boxes
Game, which contains everything else needed for the actual game

## Hitbox Parser

For now, the parser cannot parse correctly some configurations, so it might be preferrable to leave a one-pixel space between boxes.
Examples of problematic configurations:

..XXXXX         ....XXXX
..XXXXX         XXXXXXXX
..XXXXX         XXXX....
XXX....
XXX....
