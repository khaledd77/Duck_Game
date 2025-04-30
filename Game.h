#pragma once
#include <iostream>
#include <cmath>
#include <algorithm>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/Config.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>


using namespace std;
using namespace sf;
typedef long long ll;

struct mapTile {
    Sprite tileSprite;
    bool isCollidable;
    FloatRect bounds;
};

mapTile mapTiles[1000];
int tileCount = 0;
int weapCount = 0;
Vector2f weapPos;
Texture ground;
Texture ice;
Texture snow;
Texture wood;
Sprite map;
const int TILE_SIZEE = 32;