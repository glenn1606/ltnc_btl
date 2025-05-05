#include <bits/stdc++.h>
#include "collisions.h"

using namespace std;

bool collision(Asteroid& asteroid, int x, int y, int r)
{
    double distance = sqrt((asteroid.x + asteroid._size/2 - (double)x)*(asteroid.x + asteroid._size/2 - (double)x)
                         + (asteroid.y + asteroid._size/2 - (double)y)*(asteroid.y + asteroid._size/2 - (double)y));
    return (distance < asteroid._size/2 + r);
}

bool collision(Laser& laser, int x, int y, int r)
{
    double distance = sqrt((laser.x + laser._size/2 - (double)x)*(laser.x + laser._size/2 - (double)x)
                         + (laser.y + laser._size/2 - (double)y)*(laser.y + laser._size/2 - (double)y));
    return (distance < laser._size/2 + r);
}

bool collision(Asteroid& asteroid, Laser& laser)
{
    double laser_center_x = (double)laser.x + (double)laser._size/2;
    double laser_center_y = (double)laser.y + (double)laser._size/2;
    double asteroid_center_x = asteroid.x + asteroid._size/2;
    double asteroid_center_y = asteroid.y + asteroid._size/2;
    double distance = sqrt((laser_center_x - asteroid_center_x)*(laser_center_x - asteroid_center_x)
                         + (laser_center_y - asteroid_center_y)*(laser_center_y - asteroid_center_y));
    return (distance < asteroid._size/2);
}

bool collision(Asteroid& asteroid, Spaceship& spaceship)
{
    double spaceship_center_x = (double)spaceship.x + (double)spaceship._size/2;
    double spaceship_center_y = (double)spaceship.y + (double)spaceship._size/2;
    double asteroid_center_x = asteroid.x + asteroid._size/2;
    double asteroid_center_y = asteroid.y + asteroid._size/2;
    double distance = sqrt((spaceship_center_x - asteroid_center_x)*(spaceship_center_x - asteroid_center_x)
                         + (spaceship_center_y - asteroid_center_y)*(spaceship_center_y - asteroid_center_y));
    return (distance < asteroid._size/2 + spaceship._size/2);
}

bool collision(Asteroid& asteroid, Ultimate& ultimate)
{
    double ultimate_center_x = (double)ultimate.x + (double)ultimate._size/2;
    double ultimate_center_y = (double)ultimate.y + (double)ultimate._size/2;
    double asteroid_center_x = asteroid.x + asteroid._size/2;
    double asteroid_center_y = asteroid.y + asteroid._size/2;
    double distance = sqrt((ultimate_center_x - asteroid_center_x)*(ultimate_center_x - asteroid_center_x)
                         + (ultimate_center_y - asteroid_center_y)*(ultimate_center_y - asteroid_center_y));
    return (distance < asteroid._size/2 + ultimate._size/2);
}

bool collision(Laser& laser, Spaceship& spaceship)
{
    double spaceship_center_x = (double)spaceship.x + (double)spaceship._size/2;
    double spaceship_center_y = (double)spaceship.y + (double)spaceship._size/2;
    double laser_center_x = laser.x + laser._size/2;
    double laser_center_y = laser.y + laser._size/2;
    double distance = sqrt((spaceship_center_x - laser_center_x)*(spaceship_center_x - laser_center_x)
                         + (spaceship_center_y - laser_center_y)*(spaceship_center_y - laser_center_y));
    return (distance < laser._size/2 + spaceship._size/2);
}
