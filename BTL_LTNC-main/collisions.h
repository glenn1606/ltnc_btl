#ifndef COLLISIONS_H_INCLUDED
#define COLLISIONS_H_INCLUDED

#include "SDL.h"
#include "objects.h"
using namespace std;

bool collision(Asteroid& asteroid, int x, int y, int r);

bool collision(Laser& laser, int x, int y, int r);

bool collision(Asteroid& asteroid, Laser& laser);

bool collision(Asteroid& asteroid, Spaceship& spaceship);

bool collision(Asteroid& asteroid, Ultimate& ultimate);

bool collision(Laser& laser, Spaceship& spaceship);


#endif // COLLISIONS_H_INCLUDED
