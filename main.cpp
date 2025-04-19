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
Texture grave;
Sprite Grave;

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
    Sprite weapon;
    float velocityX = 0.0, velocityY = 0.0;
    float velocity;
    ll bullets;
    bool empty;
    float range;
    float posx, posy;
}pistol, sniper;

struct bullets {
    Texture skin;
    Sprite bullet;
    ll duck;
    bool right;
    float velocity;
    float startX;
    float range;
}bull1;

struct ducks {
    Texture skin;
    Clock myclock;
    Sprite myduck;
    int frame = 0;
    bool facingRight = true;
    bool isJumping = false;
    bool haveWeapon = false;
    bool holding = false;
    bool firing = false;
    bool dead = false;
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
vector<bullets> bulls;


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
    pistol.velocity = 25.f;
    pistol.range = 500.f;
    weaps.push_back(pistol);


    //init sniper
    sniper.skin.loadFromFile("img/sniper.png");
    sniper.weapon.setTexture(sniper.skin);
    sniper.weapon.setPosition(200.f, 660.f);
    sniper.posx = 200;
    sniper.posy = 660;
    sniper.weapon.setTextureRect(IntRect(0, 0, 33, 9));
    sniper.weapon.setScale(2.5f, 2.5f);
    sniper.empty = false;
    sniper.bullets = 4;
    sniper.velocity = 50.f;
    sniper.range = 100000.f;
    weaps.push_back(sniper);

    //init bullet
    bull1.skin.loadFromFile("img/pistol_bullet.png");
    bull1.bullet.setTexture(bull1.skin);


    //init Grave
    grave.loadFromFile("img/rip.png");
    Grave.setTexture(grave);
    Grave.setScale(0.16f, 0.1f);


    //init the background
    ground.loadFromFile("img/map.png");
    background.setTexture(ground);
    scalex = window.getSize().x * 1.0 / ground.getSize().x;
    scaley = window.getSize().y * 1.0 / ground.getSize().y;
    background.setScale(scalex, scaley);
    background.setPosition(0.f, 0.f);

    x = 1;
}

void drop_weapon(ducks& duck) {
    duck.haveWeapon = false;
    duck.myweap.velocityX = 7.f;
    if (!duck.facingRight) {
        duck.myweap.velocityX *= -1.f;
    }
    duck.myweap.velocityY = -7.f;
    weaps.push_back(duck.myweap);

    if (duck.facingRight) {
        duck.myarm.arm.setTextureRect(IntRect(0, 0, 16, 16));
        duck.myarm.arm.setScale(3.f, 3.f);
        duck.myarm.arm.setOrigin(0.f, 0.f);
    }
    else {
        duck.myarm.arm.setTextureRect(IntRect(0, 0, 16, 16));
        duck.myarm.arm.setScale(-3.f, 3.f);
        duck.myarm.arm.setOrigin(22.f, 0.f);
    }
}

void get_weapon(ducks& duck) {
    if (duck.haveWeapon) {
        drop_weapon(duck);
        return;
    }
    for (ll i = weaps.size() - 1;i >= 0;--i) {
        weapons weap = weaps[i];
        if (duck.myduck.getGlobalBounds().intersects(weap.weapon.getGlobalBounds())) {
            duck.haveWeapon = true;
            duck.myarm.arm.setTextureRect(IntRect(0, 16 * 4, 16, 16));
            if (!duck.facingRight) {
                FloatRect bounds = weap.weapon.getLocalBounds();
                weap.weapon.setOrigin(bounds.width - 5.f, 0.f);
                weap.weapon.setScale(-2.5f, 2.5f);
            }
            duck.myweap = weap;
            weaps.erase(weaps.begin() + i);
            break;
        }
    }
}

void update_bullets() {
    ll sz = bulls.size();
    for (ll i = sz - 1; i >= 0; i--) {
        if (bulls[i].right) bulls[i].bullet.move(bulls[i].velocity, 0.f);
        else bulls[i].bullet.move(-bulls[i].velocity, 0.f);

        FloatRect bulletBounds = bulls[i].bullet.getGlobalBounds();
        if (bulls[i].right) bulletBounds.left -= 50.f;
        else bulletBounds.left += 50.f;

        if (bulletBounds.intersects(duck1.myduck.getGlobalBounds())) {
            if (bulls[i].duck == 1) {
                continue;
            }
            bulls.erase(bulls.begin() + i);
            Grave.setPosition(duck1.myduck.getPosition().x, 660.f);
            cout << "dead" << endl;
            duck1.dead = true;
            continue;
        }
        if (bulletBounds.intersects(duck2.myduck.getGlobalBounds())) {
            if (bulls[i].duck == 2) {
                continue;
            }
            bulls.erase(bulls.begin() + i);
            Grave.setPosition(duck2.myduck.getPosition().x, 660.f);
            cout << "dead" << endl;
            duck2.dead = true;
            continue;
        }

        if (bulls[i].bullet.getPosition().x <= 0 || bulls[i].bullet.getPosition().x >= 1280 || bulls[i].bullet.getPosition().x - bulls[i].startX >= bulls[i].range) {
            bulls.erase(bulls.begin() + i);
        }
    }
}


void Fire(ducks& duck, ll shooter) {
    if (duck.myweap.bullets > 0) {
        duck.myweap.bullets--;
        bull1.right = duck.facingRight;
        if (duck.facingRight) bull1.bullet.setScale(0.08f, 0.08f);
        else bull1.bullet.setScale(-0.08f, 0.08f);
        bull1.bullet.setPosition(duck.myweap.weapon.getPosition().x + 30.f, duck.myweap.weapon.getPosition().y - 12.f);
        bull1.duck = shooter;
        bull1.velocity = duck.myweap.velocity;
        bull1.startX = bull1.bullet.getPosition().x;
        bull1.range = duck.myweap.range;
        bulls.push_back(bull1);
    }
}

void update_weapons() {
    for (auto& weap : weaps) {
        weap.weapon.move(weap.velocityX, weap.velocityY);
        weap.velocityY += gravity;
        if (weap.weapon.getPosition().y >= 660.f) {
            weap.weapon.setPosition(weap.weapon.getPosition().x, 660.f);
            weap.velocityX = 0.f;
            weap.velocityY = 0.f;
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
            duck.myarm.arm.setOrigin(22.f, 0.f);
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
        duck.myarm.arm.move(0.f, duck.velocityY);

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
        if (duck.myclock.getElapsedTime().asMilliseconds() > 80) {
            duck.frame = (duck.frame + 1) % 6;
            duck.myduck.setTextureRect(IntRect(duck.frame * 32, 0, 32, 32));
            duck.myclock.restart();
        }
    }
    else {
        duck.myduck.setTextureRect(IntRect(0, 0, 32, 32));
    }
    if (duck.haveWeapon) {
        update_weapon(duck);
    }
}


void update() {
    update_duck(duck1);
    update_duck(duck2);
    update_weapons();
    update_bullets();

    if (Keyboard::isKeyPressed(duck1.hold)) {
        if (!duck1.holding) {
            duck1.holding = true;
            get_weapon(duck1);
        }
    }
    else {
        duck1.holding = false;
    }
    if (Keyboard::isKeyPressed(duck2.hold)) {
        if (!duck2.holding) {
            duck2.holding = true;
            get_weapon(duck2);
        }
    }
    else {
        duck2.holding = false;
    }

    if (duck1.haveWeapon && Keyboard::isKeyPressed(duck1.fire)) {
        if (!duck1.firing) {
            duck1.firing = true;
            Fire(duck1, 1);
        }
    }
    else {
        duck1.firing = false;
    }
    if (duck2.haveWeapon && Keyboard::isKeyPressed(duck2.fire)) {
        if (!duck2.firing) {
            duck2.firing = true;
            Fire(duck2, 2);
        }
    }
    else {
        duck2.firing = false;
    }
}

void draw() {
    window.clear();
    window.draw(background);
    for (auto weap : weaps) {
        window.draw(weap.weapon);
    }
    for (auto bull : bulls) {
        window.draw(bull.bullet);
    }
    if (!duck1.dead) {
        window.draw(duck1.myduck);
        if (duck1.haveWeapon) {
            window.draw(duck1.myweap.weapon);
        }
        window.draw(duck1.myarm.arm);
    }
    else {
        window.draw(Grave);
    }
    if (!duck2.dead) {
        window.draw(duck2.myduck);
        if (duck2.haveWeapon) {
            window.draw(duck2.myweap.weapon);
        }
        window.draw(duck2.myarm.arm);
    }
    else {
        window.draw(Grave);
    }
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
