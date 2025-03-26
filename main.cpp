#include <iostream>
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

RenderWindow window(VideoMode(1280, 720), "screen", Style::Default);
Texture ground;
Sprite background;
ll x;


const float gravity = 0.5f;
const float jumpSpeed = -14.f;
float velocityX = 5.f;
float scalex, scaley;

struct ducks {
    Texture skin;
    Clock myclock;
    Sprite myduck;
    int frame = 0;
    bool facingRight = true;
    bool isJumping = false;
    Keyboard::Key up;
    Keyboard::Key right;
    Keyboard::Key left;
    float velocityY = 0.f;
}duck1, duck2;

void init() {
    window.setFramerateLimit(90);

    duck1.skin.loadFromFile("img/duck1.png");
    duck1.myduck.setTexture(duck1.skin);
    duck1.myduck.setPosition(0.f, 600.f);
    duck1.myduck.setTextureRect(IntRect(0, 0, 32, 32));
    duck1.myduck.setScale(3.f, 3.f);
    duck1.up = Keyboard::W;
    duck1.right = Keyboard::D;
    duck1.left = Keyboard::A;

    duck2.skin.loadFromFile("img/duck2.png");
    duck2.myduck.setTexture(duck2.skin);
    duck2.myduck.setPosition(1200.f, 600.f);
    duck2.myduck.setTextureRect(IntRect(0, 0, 32, 32));
    duck2.myduck.setScale(-3.f, 3.f);
    duck2.up = Keyboard::Up;
    duck2.right = Keyboard::Right;
    duck2.left = Keyboard::Left;
    duck2.facingRight = false;
    duck2.myduck.setOrigin(32.f, 0.f);

    ground.loadFromFile("img/map.png");
    background.setTexture(ground);
    scalex = window.getSize().x * 1.0 / ground.getSize().x;
    scaley = window.getSize().y * 1.0 / ground.getSize().y;
    background.setScale(scalex, scaley);
    background.setPosition(0.f, 0.f);

    x = 1;
}

void update_duck(ducks& duck) {
    bool moving = false;
    float duckWidth = duck.myduck.getGlobalBounds().width / 3;

    if (Keyboard::isKeyPressed(duck.right)) {
        moving = true;

        if (!duck.facingRight) {
            duck.facingRight = true;
            duck.myduck.setScale(3.f, 3.f);
            duck.myduck.setOrigin(0.f, 0.f);
        }

        if (duck.myduck.getPosition().x + velocityX <= 1280 - duckWidth * 2)
            duck.myduck.move(velocityX, 0.f);

    }
    if (Keyboard::isKeyPressed(duck.left)) {
        moving = true;
        if (duck.myduck.getPosition().x - velocityX >= -duckWidth)
            duck.myduck.move(-velocityX, 0.f);
        if (duck.facingRight) {
            duck.facingRight = false;
            duck.myduck.setScale(-3.f, 3.f);
            duck.myduck.setOrigin(32.f, 0.f);
        }
    }

    if (!duck.isJumping && Keyboard::isKeyPressed(duck.up)) {
        duck.isJumping = true;
        duck.velocityY = jumpSpeed;
    }

    if (duck.isJumping) {
        duck.velocityY += gravity;
        duck.myduck.move(0.f, duck.velocityY);

        if (duck.velocityY < 0) {
            duck.myduck.setTextureRect(IntRect(0, 32, 32, 32));
        }
        else {
            duck.myduck.setTextureRect(IntRect(32, 32, 32, 32));
        }

        if (duck.myduck.getPosition().y >= 600.f) {
            duck.myduck.setPosition(duck.myduck.getPosition().x, 600.f);
            duck.isJumping = false;
            duck.velocityY = 0.f;
        }
    }
    else if (moving) {
        if (duck.myclock.getElapsedTime().asMilliseconds() > 120) {
            duck.frame = (duck.frame + 1) % 6;
            duck.myduck.setTextureRect(IntRect(duck.frame * 32, 0, 32, 32));
            duck.myclock.restart();
        }
    }
    else {
        duck.myduck.setTextureRect(IntRect(0, 0, 32, 32));
    }
}


void update() {
    update_duck(duck1);
    update_duck(duck2);
}

void draw() {
    window.clear();
    window.draw(background);
    window.draw(duck1.myduck);
    window.draw(duck2.myduck);
    window.display();
}

int main() {
    init();
    while (window.isOpen()) {
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) {
                window.close();
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                window.close();
            }
        }
        update();
        draw();
    }
}