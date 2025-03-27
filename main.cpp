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
Clock Game_clock;


const float gravity = 0.5f;
const float jumpSpeed = -14.f;
float velocityX = 5.f;
float scalex, scaley;

struct arm {
    Texture skin;
    Sprite arm;
};

struct weapons {
    Texture skin;
    Clock myclock;
    Sprite weapon;
    ll bullets;
    bool empty;
    float posx, posy;
}pistol;

struct ducks {
    Texture skin;
    Clock myclock;
    Sprite myduck;
    int frame = 0;
    bool facingRight = true;
    bool isJumping = false;
    bool haveWeapon = false;
    weapons myweap;
    arm myarm;
    Keyboard::Key up;
    Keyboard::Key right;
    Keyboard::Key left;
    Keyboard::Key hold;
    Keyboard::Key fire;
    float velocityY = 0.f;
} duck1, duck2;

vector<weapons> weaps;

void init() {
    window.setFramerateLimit(90);

    //init the first duck
    duck1.skin.loadFromFile("img/duck1.png");
    duck1.myduck.setTexture(duck1.skin);
    duck1.myduck.setPosition(0.f, 600.f);
    duck1.myduck.setTextureRect(IntRect(0, 0, 32, 32));
    duck1.myduck.setScale(3.f, 3.f);
    duck1.up = Keyboard::W;
    duck1.right = Keyboard::D;
    duck1.left = Keyboard::A;
    duck1.hold = Keyboard::C;
    duck1.fire = Keyboard::V;
    //init the first duck's arm
    duck1.myarm.skin.loadFromFile("img/duck1arms.png");
    duck1.myarm.arm.setTexture(duck1.myarm.skin);
    duck1.myarm.arm.setPosition(
        duck1.myduck.getPosition().x + duck1.myduck.getGlobalBounds().width / 6,
        duck1.myduck.getPosition().y + duck1.myduck.getGlobalBounds().height / 2 - 5.f
    );
    duck1.myarm.arm.setTextureRect(IntRect(0, 0, 16, 16));
    duck1.myarm.arm.setScale(3.f, 3.f);


    //init the second duck
    duck2.skin.loadFromFile("img/duck2.png");
    duck2.myduck.setTexture(duck2.skin);
    duck2.myduck.setPosition(1200.f, 600.f);
    duck2.myduck.setTextureRect(IntRect(0, 0, 32, 32));
    duck2.myduck.setScale(-3.f, 3.f);
    duck2.up = Keyboard::Up;
    duck2.right = Keyboard::Right;
    duck2.left = Keyboard::Left;
    duck2.hold = Keyboard::K;
    duck2.fire = Keyboard::L;
    duck2.facingRight = false;
    duck2.myduck.setOrigin(32.f, 0.f);
    //init the second duck's arm
    duck2.myarm.skin.loadFromFile("img/duck2arms.png");
    duck2.myarm.arm.setTexture(duck2.myarm.skin);
    duck2.myarm.arm.setPosition(
        duck2.myduck.getPosition().x + duck2.myduck.getGlobalBounds().width / 6,
        duck2.myduck.getPosition().y + duck2.myduck.getGlobalBounds().height / 2 - 5.f
    );
    duck2.myarm.arm.setTextureRect(IntRect(0, 0, 16, 16));
    duck2.myarm.arm.setScale(-3.f, 3.f);
    duck2.myarm.arm.setOrigin(22.f, 0.f);


    //init pistol
    pistol.skin.loadFromFile("img/pistol.png");
    pistol.weapon.setTexture(pistol.skin);
    pistol.weapon.setPosition(600.f, 660.f);
    pistol.posx = 600;
    pistol.posy = 660;
    pistol.weapon.setTextureRect(IntRect(0, 0, 16, 10));
    pistol.weapon.setScale(2.5f, 2.5f);
    pistol.empty = false;
    pistol.bullets = 10;
    weaps.push_back(pistol);

    //init the background
    ground.loadFromFile("img/map.png");
    background.setTexture(ground);
    scalex = window.getSize().x * 1.0 / ground.getSize().x;
    scaley = window.getSize().y * 1.0 / ground.getSize().y;
    background.setScale(scalex, scaley);
    background.setPosition(0.f, 0.f);

    x = 1;
}

void get_weapon(ducks& duck) {
    for (auto& weap : weaps) {
        if (duck.myduck.getGlobalBounds().intersects(weap.weapon.getGlobalBounds())) {
            duck.haveWeapon = true;
            duck.myarm.arm.setTextureRect(IntRect(0, 16*4, 16, 16));
            if (!duck.facingRight) {
                FloatRect bounds = weap.weapon.getLocalBounds();
                weap.weapon.setOrigin(bounds.width-5.f, 0.f);
                weap.weapon.setScale(-2.5f, 2.5f);
            }
            duck.myweap = weap;
            cout << "did" << endl;
            weaps.clear();
            break;
        }
    }
}

void update_weapon(ducks& duck) {
    duck.myweap.weapon.setPosition(
        duck.myduck.getPosition().x + duck.myduck.getGlobalBounds().width / 2 - 15.f,
        duck.myduck.getPosition().y + 55.f
    );
}

void update_duck(ducks& duck) {
    bool moving = false;
    float duckWidth = duck.myduck.getGlobalBounds().width / 3;

    if (duck.haveWeapon) {
        update_weapon(duck);
    }

    if (Keyboard::isKeyPressed(duck.right)) {
        moving = true;

        if (!duck.facingRight) {
            duck.facingRight = true;
            duck.myduck.setScale(3.f, 3.f);
            duck.myduck.setOrigin(0.f, 0.f);
            duck.myarm.arm.setScale(3.f, 3.f);
            duck.myarm.arm.setOrigin(0.f, 0.f);
            if (duck.haveWeapon) {
                duck.myweap.weapon.setScale(2.5f, 2.5f);
                duck.myweap.weapon.setOrigin(0.f, 0.f);
            }
        }

        if (duck.myduck.getPosition().x + velocityX <= 1280 - duckWidth * 2) {
            duck.myduck.move(velocityX, 0.f);
            duck.myarm.arm.move(velocityX, 0.f);
        }

    }
    if (Keyboard::isKeyPressed(duck.left)) {
        moving = true;
        if (duck.facingRight) {
            duck.facingRight = false;
            duck.myduck.setScale(-3.f, 3.f);
            duck.myduck.setOrigin(32.f, 0.f);
            duck.myarm.arm.setScale(-3.f, 3.f);
            duck.myarm.arm.setOrigin(22.f,0.f);
            if (duck.haveWeapon) {
                duck.myweap.weapon.setScale(-2.5f, 2.5f);
                duck.myweap.weapon.setOrigin(12, 0.f);
            }
        }
        if (duck.myduck.getPosition().x - velocityX >= -duckWidth) {
            duck.myduck.move(-velocityX, 0.f);
            duck.myarm.arm.move(-velocityX, 0.f);
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

    if (Keyboard::isKeyPressed(duck1.hold)) {
        get_weapon(duck1);
    }
    if (Keyboard::isKeyPressed(duck2.hold)) {
        get_weapon(duck2);
    }
}

void draw() {
    window.clear();
    window.draw(background);
    window.draw(duck1.myduck);
    window.draw(duck2.myduck);
    if (duck1.haveWeapon) {
        window.draw(duck1.myweap.weapon);
    }
    if (duck2.haveWeapon) {
        window.draw(duck2.myweap.weapon);
    }
    for (auto weap : weaps) {
        window.draw(weap.weapon);
    }
    window.draw(duck1.myarm.arm);
    window.draw(duck2.myarm.arm);
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
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }
        update();
        draw();
    }
}