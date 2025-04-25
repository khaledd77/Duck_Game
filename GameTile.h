#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

struct GameTile {
public:
    enum TileType { Ground, Cement, Tree, Weapon , Stone , Branch, Root,  None };

    GameTile();
    const sf::Sprite& getTileSprite(TileType type);

    std::map<TileType, sf::Texture> textures;
    std::map<TileType, sf::Sprite> sprites;

    void loadTexture(TileType type, const std::string& path);
};
