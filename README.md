# Asteroid-Shooter-2Players

## Project Aim

This project uses C++ to create a windows application using Visual Studio Community 2022. It is inspired by a youtuber named "javidx9" (https://www.youtube.com/c/javidx9). I have borrowed some code (olcConsoleGameEngine.h) from him to generate a simple multiplayer shooter game named Asteorids. Shout out to his support and excellent tutorial videos on youtube. Link to his GitHub repository here (https://github.com/OneLoneCoder/olcPixelGameEngine).

### Technologies and Algorithms
* C++ 
* Visual C++ Desktop Application
* 2D Gaming Graphics

## Overview
### Basic setup
With thanks to "javidx9", the olcConsoleGameEngine.h C++ header fiel is a single-file prototyping and game-engine framework created in C++. Quoting from his repo description: "olcConsoleGameEngine.h allows you to rapidly develop prototypes and games. It does this by creating a window, and rapidly drawing to that window. It is sensitive to keyboard and mouse input. The Screen is considered to be an 2D array of Pixels. Pixels have a defined width and height in real screen-pixels. This means your application can be low or high resolution depending on the look and sophistication you are going for."

### sSpaceObject struct
Each object in space is formed of a struct named sSpaceObject that has attributes including the x-y position, x-y velocity, size and orientation. This allows a combination of identities including the spaceships, asteroids and bullets to be eaily constructed and manipulated through direct instantiation.

### Linear Algebra: Affine Transformation
To simulate the movement of the spaceships, we utilise basic linear algebra to perform affine transformation of the ship models. This allows rotation, scaling and translation of objects to create the visual effect of the ships floating and moving in space. By applying basic kinematics laws, the motion of spaceships could be manipulated through steering of and thrust provided by the ships.


## Demo Videos

## Future improvements
Future work could include implementing a cooperative mode on top of the challenger mode allowing more interesting play styles. Features such as levelling up and special equipments could also be included to add a bit of spice to the game.

