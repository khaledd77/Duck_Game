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
Texture duck1,duck2,ground;
Sprite myduck1,myduck2,background;
ll x;
RectangleShape shape;
Clock myclock;
int frame = 0;
bool facingRight = true;

const float gravity = 0.5f;
const float jumpSpeed = -16.f;
float velocityY = 0.f;
float velocityX = 5.f;
bool isJumping = false;
float scalex, scaley;

void init() {
    window.setFramerateLimit(90);

    duck1.loadFromFile("img/duck1.png");
    myduck1.setTexture(duck1);
    myduck1.setPosition(0.f, 600.f);
    myduck1.setTextureRect(IntRect(0, 0, 32, 32));
    myduck1.setScale(3.f, 3.f);

    duck2.loadFromFile("img/duck2.png");
    myduck2.setTexture(duck2);
    myduck2.setPosition(1200.f, 600.f);
    myduck2.setTextureRect(IntRect(0, 0, 32, 32));
    myduck2.setScale(3.f, 3.f);

    ground.loadFromFile("img/map.png");
    background.setTexture(ground);
    scalex = window.getSize().x*1.0 / ground.getSize().x;
    scaley = window.getSize().y*1.0 / ground.getSize().y;
    background.setScale(scalex, scaley);
    background.setPosition(0.f, 0.f);
    
    x = 1;
}

void update_duck(Sprite& myduck1) {
    bool moving = false;
    float duckWidth = myduck1.getGlobalBounds().width / 3;

    if (Keyboard::isKeyPressed(Keyboard::D)) {
        moving = true;
        if (myduck1.getPosition().x + velocityX <= 1280 - duckWidth * 2)
            myduck1.move(velocityX, 0.f);

        if (!facingRight) {
            facingRight = true;
            myduck1.setScale(3.f, 3.f);
            myduck1.setOrigin(0.f, 0.f);
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        moving = true;
        if (myduck1.getPosition().x - velocityX >= -duckWidth)
            myduck1.move(-velocityX, 0.f);
        if (facingRight) {
            facingRight = false;
            myduck1.setScale(-3.f, 3.f);
            myduck1.setOrigin(32.f, 0.f);
        }
    }

    if (!isJumping && Keyboard::isKeyPressed(Keyboard::W)) {
        isJumping = true;
        velocityY = jumpSpeed;
    }

    if (isJumping) {
        velocityY += gravity;
        myduck1.move(0.f, velocityY);

        if (velocityY < 0) {
            myduck1.setTextureRect(IntRect(0, 32, 32, 32));
        }
        else {
            myduck1.setTextureRect(IntRect(32, 32, 32, 32));
        }

        if (myduck1.getPosition().y >= 600.f) {
            myduck1.setPosition(myduck1.getPosition().x, 600.f);
            isJumping = false;
            velocityY = 0.f;
        }
    }
    else if (moving) {
        if (myclock.getElapsedTime().asMilliseconds() > 120) {
            frame = (frame + 1) % 6;
            myduck1.setTextureRect(IntRect(frame * 32, 0, 32, 32));
            myclock.restart();
        }
    }
    else {
        myduck1.setTextureRect(IntRect(0, 0, 32, 32));
    }
}

void update() {
    update_duck(myduck1);
    update_duck(myduck2);
}

void draw() {
    window.clear();
    window.draw(background);
    window.draw(myduck2);
    window.draw(myduck1);
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
