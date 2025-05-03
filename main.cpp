#include "MainMenu.h"
#include "Logic.h"
#include "GameTile.h"
#include "Game.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/Config.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/System.hpp>
#include <ctime>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace sf;
typedef long long ll;


using namespace sf;
using namespace std;

Menu mainMenu, settingsMenu, gameMenu, pausedMenu, transition, endScreen;
RenderWindow window(VideoMode(1280, 720), "Duck Game");
Texture grave;
Sprite Grave;
splashScreen intro;

Clock trans, SPAWN, fadeTimer, endGameTimer;
bool started = false;
int fadeDirection = 0, fadeTargetState = 1000, fadeCurrentAlpha, fadeDuration = 500;
RectangleShape fadeScreen;

SoundBuffer buffer1, buffer2, buffer3, buffer4, buffer5;
Sound gunfire, lazerfire, launch, bombb, quack;


Texture gameBackgroundTexture;
Texture smoke;
Sprite gameBackground;
Texture duck1AvatarTexture, duck2AvatarTexture, duck1AvatarReadyTexture, duck2AvatarReadyTexture;
Sprite duck1Avatar, duck2Avatar, duck1AvatarReady, duck2AvatarReady;
bool drawPaused = false;
float weapsVelocity = 7.f;

int menuState = 1000; // 1000 = main menu, 0 = game/levels, 1 = settings, 2 = game menu (& level selector), 3 = transition between games, 4 = end game
bullets bull[3];
ducks duck1, duck2;
float fact;
weapons sword, pistol, sniper, pewpew, grenade;
MyVector<weapons> weaps;
MyVector<bullets> bulls;
float gravity = 0.5f;
float jumpSpeed = -14.f;
float velocityX = 5.f;
float MaxiVelocityY = 9.f;
bool GameEnd = 0;
ll mapnum = 0, duck1Score=0, duck2Score=0;
float DUCK_SCALE;
float GUN_SCALE;
float scalex, scaley;
const float Width = 1280, Height = 720;
float duck1posx=100.f, duck1posy=680.f, duck2posx=1000.f, duck2posy=680.f;
bool wait = 0;


//Fady
void init() {

    buffer1.loadFromFile("img/gunshot.wav");
    gunfire.setBuffer(buffer1);
    buffer2.loadFromFile("img/lasershot.wav");
    lazerfire.setBuffer(buffer2);
    buffer3.loadFromFile("img/launch.wav");
    launch.setBuffer(buffer3);
    buffer4.loadFromFile("img/explode.wav");
    bombb.setBuffer(buffer4);
    buffer5.loadFromFile("img/quack.wav");
    quack.setBuffer(buffer5);

    //init the first duck
    ducks duck;
    duck1 = duck;
    duck2 = duck;
    weaps.clear();
    bulls.clear();
    duck1.skin.loadFromFile("img/duck1.png");
    duck1.myduck.setTexture(duck1.skin);
    duck1.myduck.setTextureRect(IntRect(0, 0, 32, 32));
    duck1.myduck.setScale(DUCK_SCALE, DUCK_SCALE);
    duck1.myduck.setOrigin(0.f, duck1.myduck.getLocalBounds().height);
    duck1.myduck.setPosition(duck1posx, duck1posy);
    duck1.up = Keyboard::W;
    duck1.right = Keyboard::D;
    duck1.left = Keyboard::A;
    duck1.hold = Keyboard::C;
    duck1.fire = Keyboard::V;
    //init the first duck's arm
    duck1.myarm.skin.loadFromFile("img/duck1arms.png");
    duck1.myarm.arm.setTexture(duck1.myarm.skin);
    duck1.myarm.arm.setTextureRect(IntRect(0, 0, 16, 16));
    duck1.myarm.arm.setScale(DUCK_SCALE, DUCK_SCALE);
    duck1.myarm.arm.setPosition(
        duck1.myduck.getPosition().x + duck1.myduck.getGlobalBounds().width / 6,
        duck1.myduck.getPosition().y - duck1.myduck.getGlobalBounds().height / 2 - fact
    );


    //init the second duck
    duck2.skin.loadFromFile("img/duck2.png");
    duck2.myduck.setTexture(duck2.skin);
    duck2.myduck.setTextureRect(IntRect(0, 0, 32, 32));
    duck2.myduck.setScale(-DUCK_SCALE, DUCK_SCALE);
    duck2.myduck.setOrigin(duck2.myduck.getLocalBounds().width, duck2.myduck.getLocalBounds().height);
    duck2.myduck.setPosition(duck2posx, duck2posy);
    duck2.up = Keyboard::Up;
    duck2.right = Keyboard::Right;
    duck2.left = Keyboard::Left;
    duck2.hold = Keyboard::K;
    duck2.fire = Keyboard::L;
    duck2.facingRight = false;
    //init the second duck's arm
    duck2.myarm.skin.loadFromFile("img/duck2arms.png");
    duck2.myarm.arm.setTexture(duck2.myarm.skin);
    duck2.myarm.arm.setTextureRect(IntRect(0, 0, 16, 16));
    duck2.myarm.arm.setScale(-DUCK_SCALE, DUCK_SCALE);
    duck2.myarm.arm.setOrigin(22.f, 0.f);
    duck2.myarm.arm.setPosition(
        duck2.myduck.getPosition().x + duck2.myduck.getGlobalBounds().width / 6,
        duck2.myduck.getPosition().y - duck2.myduck.getGlobalBounds().height / 2 - fact
    );


    //init pistol
    pistol.skin.loadFromFile("img/pistol.png");
    pistol.weapon.setTexture(pistol.skin);
    pistol.posx = 600;
    pistol.posy = 660;
    pistol.weapon.setTextureRect(IntRect(0, 0, 16, 10));
    pistol.weapon.setScale(GUN_SCALE, GUN_SCALE);
    pistol.empty = false;
    pistol.bullets = 10;
    pistol.velocity = 25;
    pistol.range = 500.f;
    pistol.bull_type = 1;
    pistol.weapon.setOrigin(0, pistol.weapon.getLocalBounds().height);
    pistol.weapon.setPosition(600.f, 680);
    pistol.type = "pistol";

    //init grenade
    grenade.skin.loadFromFile("img/grenade.png");
    smoke.loadFromFile("img/smoke.png");
    grenade.weapon.setTexture(grenade.skin);
    grenade.posx = 600;
    grenade.posy = 660;
    grenade.weapon.setTextureRect(IntRect(0, 0, 9, 11));
    grenade.weapon.setScale(GUN_SCALE, GUN_SCALE);
    grenade.empty = false;
    grenade.bull_type = 1;
    grenade.weapon.setOrigin(0, grenade.weapon.getLocalBounds().height);
    grenade.weapon.setPosition(600.f, 680);
    grenade.type = "grenade";


    //init sniper
    sniper.skin.loadFromFile("img/sniper.png");
    sniper.weapon.setTexture(sniper.skin);
    sniper.posx = 200;
    sniper.posy = 660;
    sniper.weapon.setTextureRect(IntRect(0, 0, 33, 9));
    sniper.weapon.setScale(GUN_SCALE, GUN_SCALE);
    sniper.empty = false;
    sniper.bullets = 4;
    sniper.velocity = 40;
    sniper.range = 100000.f;
    sniper.bull_type = 1;
    sniper.weapon.setOrigin(0, sniper.weapon.getLocalBounds().height);
    sniper.weapon.setPosition(200.f, 680);
    sniper.type = "sniper";

    //init pewpew
    pewpew.skin.loadFromFile("img/pewpewLaser.png");
    pewpew.weapon.setTexture(pewpew.skin);
    pewpew.posx = 300;
    pewpew.posy = 300;
    pewpew.weapon.setTextureRect(IntRect(0, 0, 26, 10));
    pewpew.weapon.setScale(GUN_SCALE, GUN_SCALE);
    pewpew.empty = false;
    pewpew.bullets = 20;
    pewpew.velocity = 15;
    pewpew.range = 100000.f;
    pewpew.bull_type = 2;
    pewpew.weapon.setOrigin(0, pewpew.weapon.getLocalBounds().height);
    pewpew.weapon.setPosition(300.f, 680);
    pewpew.type = "pewpew";

    //init sword
    sword.skin.loadFromFile("img/sword.png");
    sword.weapon.setTexture(sword.skin);
    sword.weapon.setTextureRect(IntRect(0, 0, 8, 23));
    sword.weapon.setScale(GUN_SCALE, GUN_SCALE);
    sword.empty = false;
    sword.type = "sword";
    sword.angle = 90.f;
    sword.weapon.setOrigin(sword.weapon.getLocalBounds().width, sword.weapon.getLocalBounds().height);
    sword.weapon.setPosition(400.f, 680);
    sword.weapon.setRotation(sword.angle);

    //init bullet
    bull[1].skin.loadFromFile("img/pistol_bullet.png");
    bull[1].bullet.setTexture(bull[1].skin);

    bull[2].skin.loadFromFile("img/laser_bullet1.png");
    bull[2].bullet.setTexture(bull[2].skin);


    //init Grave
    grave.loadFromFile("img/rip.png");
    Grave.setTexture(grave);
}
void launch_grenade(weapons& weap) {
    if (!weap.ready) {
        weap.ready = 1;
        weap.myclock.restart();
        weap.weapon.setTextureRect(IntRect(16, 0, 10, 11));
        launch.play();
    }
    return;
}
void Fire(ducks& duck, ll shooter) {
    ll idx = duck.myweap.bull_type;
    if (duck.myweap.type == "sword") {
        duck.myweap.hit = 1;
        duck.myweap.myclock.restart();
        return;
    }
    if (duck.myweap.type == "grenade") {
        launch_grenade(duck.myweap);
        return;
    }
    if (duck.myweap.bullets > 0) {
        if (idx == 1) gunfire.play();
        else lazerfire.play();
        duck.myweap.bullets--;
        bull[idx].right = duck.facingRight;
        if (duck.facingRight) {
            bull[idx].bullet.setScale(bull[idx].scalex, bull[idx].scaley);
            bull[idx].bullet.setOrigin(0.f, 0.f);
        }
        else {
            bull[idx].bullet.setScale(-1 * bull[idx].scalex, bull[idx].scaley);
            bull[idx].bullet.setOrigin(bull[idx].bullet.getLocalBounds().width, 0.f);
        }
        if (duck.facingRight) bull[idx].bullet.setPosition(duck.myweap.weapon.getPosition().x + duck.myweap.weapon.getGlobalBounds().width, duck.myweap.weapon.getPosition().y + duck.myweap.fix_Y);
        else bull[idx].bullet.setPosition(duck.myweap.weapon.getPosition().x - duck.myweap.weapon.getGlobalBounds().width + duck.myweap.fix_X, duck.myweap.weapon.getPosition().y + duck.myweap.fix_Y);
        bull[idx].duck = shooter;
        bull[idx].velocity = duck.myweap.velocity;
        bull[idx].startX = bull[idx].bullet.getPosition().x;
        bull[idx].range = duck.myweap.range;
        bulls.push_back(bull[idx]);
    }
}
void update_grenade(weapons& weap) {
    if (!weap.boom && weap.myclock.getElapsedTime().asMilliseconds() >= 1500) {
        bombb.play();
        weap.boom = 1;
        weap.weapon.setTexture(smoke);
        weap.weapon.setTextureRect(IntRect(0, 0, 44, 42));
        weap.scale = 0.1f;
        weap.weapon.setScale(weap.scale, weap.scale);
        weap.weapon.setOrigin(weap.weapon.getLocalBounds().width / 2, weap.weapon.getLocalBounds().height / 2);
    }
    else if (weap.boom) {
        if (weap.weapon.getGlobalBounds().intersects(duck1.myduck.getGlobalBounds())) {
            duck1.dead = 1;
            GameEnd = 1;
        }
        if (weap.weapon.getGlobalBounds().intersects(duck2.myduck.getGlobalBounds())) {
            duck2.dead = 1;
            GameEnd = 1;
        }
        weap.scale += 0.15;
        weap.weapon.setScale(weap.scale, weap.scale);
        if (weap.scale >= 5) {
            weap.boom = 0;
            weap.ready = 0;
            weap.scale = 0;
            weap.weapon.setScale(weap.scale, weap.scale);
        }
    }
}
void update_weapons() {
    for (int i = 0;i < weaps.size();++i) {
        weapons &weap = weaps[i];
        if (weap.type == "grenade" && weap.ready) {
            update_grenade(weap);
            if (weap.boom) {
                continue;
            }
        }
        weap.weapon.move(weap.velocityX, weap.velocityY);
        weap.velocityY += gravity;
        weap.velocityY = min(weap.velocityY, MaxiVelocityY);
        if (weap.weapon.getPosition().y >= 720.f) {
            weap.weapon.setPosition(weap.weapon.getPosition().x, 720.f);
            weap.velocityX = 0.f;
            weap.velocityY = 0.f;
        }
    }
}
void update_sword(ducks& duck, ll shooter) {
    if (duck.myweap.type == "sword" && duck.myweap.hit) {
        if (duck.myweap.weapon.getGlobalBounds().intersects(duck2.myduck.getGlobalBounds()) && shooter == 1) {
            GameEnd = 1;
            duck2.dead = true;
        }
        if (duck.myweap.weapon.getGlobalBounds().intersects(duck1.myduck.getGlobalBounds()) && shooter == 2) {
            GameEnd = 1;
            duck1.dead = true;
        }
        if (duck.facingRight) {
            if (duck.myweap.myclock.getElapsedTime().asMilliseconds() >= 5) {
                duck.myweap.angle = duck.myweap.angle + 15 * duck.myweap.fact;
                duck.myweap.weapon.setRotation(duck.myweap.angle);
                if (duck.myweap.weapon.getRotation() >= 90) {
                    duck.myweap.fact = -1;
                }
                if (duck.myweap.weapon.getRotation() <= 0) {
                    duck.myweap.fact = 1;
                    duck.myweap.hit = 0;
                }
                duck.myweap.myclock.restart();
            }
        }
        else {
            if (duck.myweap.myclock.getElapsedTime().asMilliseconds() >= 5) {
                duck.myweap.angle = duck.myweap.angle - 15 * duck.myweap.fact;
                duck.myweap.weapon.setRotation(duck.myweap.angle);
                if (duck.myweap.angle <= -90) {
                    duck.myweap.fact = -1;
                }
                if (duck.myweap.angle >= 0) {
                    duck.myweap.fact = 1;
                    duck.myweap.hit = 0;
                }
                duck.myweap.myclock.restart();
            }
        }
    }
}
void update_duck(ducks& duck) {
    bool moving = false;
    float duckWidth = duck.myduck.getGlobalBounds().width / 3;

    if (Keyboard::isKeyPressed(duck.right)) {
        moving = true;

        if (!duck.facingRight) {
            duck.facingRight = true;
            duck.myduck.setScale(DUCK_SCALE, DUCK_SCALE);
            duck.myduck.setOrigin(0.f, duck.myduck.getLocalBounds().height);
            duck.myarm.arm.setScale(DUCK_SCALE, DUCK_SCALE);
            duck.myarm.arm.setOrigin(0.f, 0.f);
            if (duck.haveWeapon) {
                duck.myweap.weapon.setScale(GUN_SCALE, GUN_SCALE);
                if (duck.myweap.type == "sword")
                    duck.myweap.weapon.setOrigin(duck.myweap.weapon.getLocalBounds().width, duck.myweap.weapon.getLocalBounds().height);
                else
                    duck.myweap.weapon.setOrigin(0.f, duck.myweap.weapon.getLocalBounds().height);
            }
        }

        if (duck.myduck.getPosition().x + velocityX <= 1280 - duckWidth * 2) {
            duck.myduck.move(velocityX, 0.f);
            duck.myarm.arm.setPosition(
                duck.myduck.getPosition().x + duck.myduck.getGlobalBounds().width / 6,
                duck.myduck.getPosition().y - duck.myduck.getGlobalBounds().height / 2 - fact
            );
            duck.myweap.weapon.setPosition(
                duck.myduck.getPosition().x + duck.myweap.fix_hold_x,
                duck.myduck.getPosition().y + duck.myweap.fix_hold_y
            );
        }

    }
    if (Keyboard::isKeyPressed(duck.left)) {
        moving = true;
        if (duck.facingRight) {
            duck.facingRight = false;
            duck.myduck.setScale(-DUCK_SCALE, DUCK_SCALE);
            duck.myduck.setOrigin(duck.myduck.getLocalBounds().width, duck.myduck.getLocalBounds().height);
            duck.myarm.arm.setScale(-DUCK_SCALE, DUCK_SCALE);
            duck.myarm.arm.setOrigin(22.f, 0.f);
            if (duck.haveWeapon) {
                duck.myweap.weapon.setScale(-GUN_SCALE, GUN_SCALE);
                duck.myweap.weapon.setOrigin(duck.myweap.rev, duck.myweap.weapon.getLocalBounds().height);
            }
        }
        if (duck.myduck.getPosition().x - velocityX >= -duckWidth) {
            duck.myduck.move(-velocityX, 0.f);
            duck.myarm.arm.setPosition(
                duck.myduck.getPosition().x + duck.myduck.getGlobalBounds().width / 6,
                duck.myduck.getPosition().y - duck.myduck.getGlobalBounds().height / 2 - fact
            );
            duck.myweap.weapon.setPosition(
                duck.myduck.getPosition().x + duck.myweap.fix_hold_x,
                duck.myduck.getPosition().y + duck.myweap.fix_hold_y
            );
        }
    }
    bool pressed = Keyboard::isKeyPressed(duck.up);
    if (duck.isJumping && !pressed) {
        duck.velocityY = max(duck.velocityY, -1.f);
    }
    if (!duck.isJumping && duck.onGround) {
        if (pressed) {
            if (!duck.holdingJump) {
                duck.isJumping = true;
                duck.velocityY = jumpSpeed;
                duck.holdingJump = true;
            }
        }
        else {
            duck.holdingJump = false;
        }
    }

    if (duck.isJumping || !duck.onGround) {
        duck.velocityY += gravity;
        duck.velocityY = min(duck.velocityY, MaxiVelocityY);
        duck.myduck.move(0.f, duck.velocityY);
        duck.myarm.arm.setPosition(
            duck.myduck.getPosition().x + duck.myduck.getGlobalBounds().width / 6,
            duck.myduck.getPosition().y - duck.myduck.getGlobalBounds().height / 2 - fact
        );
        duck.myweap.weapon.setPosition(
            duck.myduck.getPosition().x + duck.myweap.fix_hold_x,
            duck.myduck.getPosition().y + duck.myweap.fix_hold_y
        );

        if (duck.velocityY < 0) {
            duck.myduck.setTextureRect(IntRect(0, 32, 32, 32));
        }
        else {
            duck.myduck.setTextureRect(IntRect(32, 32, 32, 32));
        }

        if (duck.myduck.getPosition().y >= 720.f) {
            duck.myduck.setPosition(duck.myduck.getPosition().x, 720.f);
            duck.isJumping = false;
            duck.onGround = true;
            duck.velocityY = 0.f;
            duck.dead = 1;
            GameEnd = 1;
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
    duck.myarm.arm.setPosition(
        duck.myduck.getPosition().x + duck.myduck.getGlobalBounds().width / 6,
        duck.myduck.getPosition().y - duck.myduck.getGlobalBounds().height / 2 - fact
    );
    duck.myweap.weapon.setPosition(
        duck.myduck.getPosition().x + duck.myweap.fix_hold_x,
        duck.myduck.getPosition().y + duck.myweap.fix_hold_y
    );
}   
void drop_weapon(ducks& duck) {
    duck.haveWeapon = false;
    duck.myweap.velocityX = weapsVelocity;
    if (!duck.facingRight) {
        duck.myweap.velocityX *= -1.f;
    }
    duck.myweap.velocityY = weapsVelocity*-1;
    if (duck.myweap.type == "sword") {
        duck.myweap.weapon.setOrigin(duck.myweap.weapon.getLocalBounds().width, duck.myweap.weapon.getLocalBounds().height);
        if (duck.facingRight) duck.myweap.weapon.setRotation(90);
        else duck.myweap.weapon.setRotation(-90);
    }
    weaps.push_back(duck.myweap);

    if (duck.facingRight) {
        duck.myarm.arm.setTextureRect(IntRect(0, 0, 16, 16));
        duck.myarm.arm.setScale(DUCK_SCALE, DUCK_SCALE);
        duck.myarm.arm.setOrigin(0.f, 0.f);
    }
    else {
        duck.myarm.arm.setTextureRect(IntRect(0, 0, 16, 16));
        duck.myarm.arm.setScale(-DUCK_SCALE, DUCK_SCALE);
        duck.myarm.arm.setOrigin(22.f, 0.f);
    }
}
void get_weapon(ducks& duck) {
    if (duck.haveWeapon) {
        drop_weapon(duck);
        return;
    }
    if (weaps.size() == 0) return;
    for (ll i = weaps.size() - 1;i >= 0;--i) {
        weapons weap = weaps[i];
        if (duck.myduck.getGlobalBounds().intersects(weap.weapon.getGlobalBounds())) {
            duck.haveWeapon = true;
            duck.myarm.arm.setTextureRect(IntRect(0, 16 * 4, 16, 16));
            if (!duck.facingRight) {
                FloatRect bounds = weap.weapon.getLocalBounds();
                if (weap.type == "sword")
                    weap.weapon.setOrigin(0.f, bounds.height);
                else
                    weap.weapon.setOrigin(weap.rev, bounds.height);
                weap.weapon.setScale(-GUN_SCALE, GUN_SCALE);
            }
            if (duck.facingRight) {
                FloatRect bounds = weap.weapon.getLocalBounds();
                if (weap.type == "sword")
                    weap.weapon.setOrigin(bounds.width, bounds.height);
                else
                    weap.weapon.setOrigin(0, bounds.height);
                weap.weapon.setScale(GUN_SCALE, GUN_SCALE);
            }

            weap.weapon.setPosition(
                duck.myduck.getPosition().x + weap.fix_hold_x,
                duck.myduck.getPosition().y + weap.fix_hold_y
            );
            if (weap.type == "sword") {
                weap.weapon.setRotation(0);
                weap.angle = 0;
            }
            duck.myweap = weap;
            weaps.erase(i);
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
            bulls.erase(i);
            GameEnd = 1;
            duck1.dead = true;
            continue;
        }
        if (bulletBounds.intersects(duck2.myduck.getGlobalBounds())) {
            if (bulls[i].duck == 2) {
                continue;
            }
            bulls.erase(i);
            GameEnd = 1;
            duck2.dead = true;
            continue;
        }

        if (bulls[i].bullet.getPosition().x <= 0 || bulls[i].bullet.getPosition().x >= 1280 || abs(bulls[i].bullet.getPosition().x - bulls[i].startX) >= bulls[i].range) {
            bulls.erase(i);
        }
    }
}
void update_Logic() {
    if(!duck1.dead) update_duck(duck1);
    if(!duck2.dead) update_duck(duck2);
    update_weapons();
    update_bullets();
    if (duck1.haveWeapon && duck1.myweap.type == "grenade" && duck1.myweap.ready && !duck1.dead) {
        update_grenade(duck1.myweap);
        if (duck1.myweap.boom) {
            duck1.haveWeapon = false;
            weaps.push_back(duck1.myweap);
        }
    }
    if (duck2.haveWeapon && duck2.myweap.type == "grenade" && duck2.myweap.ready && !duck2.dead) {
        update_grenade(duck2.myweap);
        if (duck2.myweap.boom) {
            duck2.haveWeapon = false;
            weaps.push_back(duck2.myweap);
        }
    }

    if (Keyboard::isKeyPressed(duck1.hold) && !duck1.dead) {
        if (!duck1.holding) {
            duck1.holding = true;
            get_weapon(duck1);
        }
    }
    else {
        duck1.holding = false;
    }
    if (Keyboard::isKeyPressed(duck2.hold) && !duck2.dead) {
        if (!duck2.holding) {
            duck2.holding = true;
            get_weapon(duck2);
        }
    }
    else {
        duck2.holding = false;
    }


    if (duck1.haveWeapon && Keyboard::isKeyPressed(duck1.fire) && !duck1.dead) {
        if (!duck1.firing) {
            duck1.firing = true;
            Fire(duck1, 1);
        }
    }
    else {
        duck1.firing = false;
    }
    if (duck2.haveWeapon && Keyboard::isKeyPressed(duck2.fire) && !duck2.dead) {
        if (!duck2.firing) {
            duck2.firing = true;
            Fire(duck2, 2);
        }
    }
    else {
        duck2.firing = false;
    }
    if(!duck1.dead) update_sword(duck1, 1);
    if(!duck2.dead) update_sword(duck2, 2);
}
void draw_Logic() {
    for (int i = 0;i < weaps.size() ; ++i) {
        window.draw(weaps[i].weapon);
    }
    for (int i = 0;i < bulls.size();++i) {
        window.draw(bulls[i].bullet);
    }
    if (!duck1.dead) {
        window.draw(duck1.myduck);
        if (duck1.haveWeapon) {
            window.draw(duck1.myweap.weapon);
        }
        window.draw(duck1.myarm.arm);
    }
    else {
        Grave.setPosition(duck1.myduck.getPosition().x,duck1.myduck.getPosition().y);
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
        Grave.setPosition(duck2.myduck.getPosition().x, duck2.myduck.getPosition().y);
        window.draw(Grave);
    }
}


// Walid
void initFade(int width, int height) {
    fadeScreen.setPosition(0.f, 0.f);
    fadeScreen.setSize(Vector2f(width, height));
    fadeScreen.setFillColor(Color(0.f, 0.f, 0.f, 0.f));
}
void fadeIn(int targetState) {
    fadeDirection = 1;
    fadeTargetState = targetState;
    fadeCurrentAlpha = 255;
    fadeTimer.restart();
}
void fadeOut(int targetState) {
    fadeDirection = -1;
    fadeTargetState = targetState;
    fadeCurrentAlpha = 0;
    fadeTimer.restart();

}
void updateFade() {

    int currentTime = fadeTimer.getElapsedTime().asMilliseconds();

    if (fadeDirection == 0)
        return;
    else if (fadeDirection == 1) {
        fadeCurrentAlpha = 255 - (255 * currentTime / fadeDuration);
        if (fadeCurrentAlpha <= 0) {
            fadeCurrentAlpha = 0;
            fadeDirection = 0;
            menuState = fadeTargetState;
        }
    }
    else if (fadeDirection == -1) {
        fadeCurrentAlpha = 255 * currentTime / fadeDuration;
        if (fadeCurrentAlpha >= 255) {
            fadeCurrentAlpha = 255;
            fadeDirection = 0;
            menuState = fadeTargetState;
            fadeIn(fadeTargetState);
        }
    }

    fadeScreen.setFillColor(sf::Color(0, 0, 0, fadeCurrentAlpha));


}
void updateMusicVal() {
    int musicVal = settingsMenu.menuMusic.getVolume();
    string musicText = "MUSIC VOLUME           " + to_string(musicVal);
    settingsMenu.menuText[0].setString(musicText);
}
void updateMapNum() {
    string currMap = "SELECTED  MAP     " + to_string(mapnum + 1);
    gameMenu.menuText[1].setString(currMap);
}
void updateReadiness(Menu& menu) {
    quack.play();
    if (duck1.ready) {
        menu.menuText[2].setString("READY!");
    }
    else {
        menu.menuText[2].setString("PLAYER 1");
    }

    if (duck2.ready) {
        menu.menuText[3].setString("READY!");
    }
    else {
        menu.menuText[3].setString("PLAYER 2");
    }
}

void startGame(Menu& menu, ducks& duck1, ducks& duck2) {
    if ((duck1.ready && duck2.ready) && menu.selected == 0) {
        fadeIn(0);
    }
}

void endGame() {
    fadeIn(4);
}

void initEndGameScreen(int width, int height) {
    endScreen.font.loadFromFile("img/arcade.ttf");

    float scaleX, scaleY;
    endScreen.backgroundTexture.loadFromFile("img/bigTV.png");
    endScreen.background.setTexture(endScreen.backgroundTexture);

    scaleX = window.getSize().x * 1.0 / endScreen.backgroundTexture.getSize().x;
    scaleY = window.getSize().y * 1.0 / endScreen.backgroundTexture.getSize().y;

    endScreen.background.setScale(scaleX, scaleY);
    endScreen.background.setPosition(0.f, 0.f);

    for (int i = 0; i < 3; i++) {
        endScreen.menuText[i].setFont(endScreen.font);
    }

    endScreen.menuText[0].setString("WINNER!!!");
    endScreen.menuText[0].setCharacterSize(60.f);
    endScreen.menuText[0].setFillColor(Color(255, 255, 255));
    endScreen.menuText[0].setPosition(Vector2f(width / 4.5f, height / 4.f));

    endScreen.menuText[1].setString("PLAYER 1");
    endScreen.menuText[1].setCharacterSize(60.f);
    endScreen.menuText[1].setFillColor(Color(255, 255, 255));
    endScreen.menuText[1].setPosition(Vector2f(width / 4.5f, height / 4.f + 150.f));

    endScreen.menuText[2].setString("PLAYER 2");
    endScreen.menuText[2].setCharacterSize(60.f);
    endScreen.menuText[2].setFillColor(Color(255, 255, 255));
    endScreen.menuText[2].setPosition(Vector2f(width / 4.5f, height / 4.f + 150.f));
}

void initTransition(ducks& duck1, ducks& duck2, int width, int height) {
    transition.font.loadFromFile("img/arcade.ttf");

    transition.backgroundTexture.loadFromFile("img/bigTV.png");
    transition.background.setTexture(transition.backgroundTexture);

    float scaleX, scaleY;
    scaleX = window.getSize().x * 1.0 / transition.backgroundTexture.getSize().x;
    scaleY = window.getSize().y * 1.0 / transition.backgroundTexture.getSize().y;

    transition.background.setScale(scaleX, scaleY);
    transition.background.setPosition(0.f, 0.f);


    for (int i = 0; i < 3; i++) {
        transition.menuText[i].setFont(transition.font);
    }

    transition.menuText[0].setString("SCORE");
    transition.menuText[0].setCharacterSize(60.f);
    transition.menuText[0].setFillColor(Color(255, 255, 255));
    transition.menuText[0].setPosition(Vector2f(width / 4.5f, height / 4.f));

    string score1 = "PLAYER 1         " + to_string(duck1Score);
    transition.menuText[1].setCharacterSize(60.f);
    transition.menuText[1].setFillColor(Color(255, 255, 255));
    transition.menuText[1].setPosition(Vector2f(width / 4.5f, height / 4.f + 150.f));
    transition.menuText[1].setString(score1);

    string score2 = "PLAYER 2         " + to_string(duck2Score);
    transition.menuText[2].setCharacterSize(60.f);
    transition.menuText[2].setFillColor(Color(255, 255, 255));
    transition.menuText[2].setPosition(Vector2f(width / 4.5f, height / 4.f + 300.f));
    transition.menuText[2].setString(score2);

}

void drawEndScreen() {
    endScreen.menuText[0].setString("WINNER!!");

    window.draw(endScreen.background);
    window.draw(endScreen.menuText[0]);

    if (duck1Score == 10) {
        window.draw(endScreen.menuText[1]);
    }
    else if (duck2Score == 10) {
        window.draw(endScreen.menuText[2]);
    }

    if (!started) {
        endGameTimer.restart();
        started = true;
    }

    if (endGameTimer.getElapsedTime().asMilliseconds() >= 5000) {
        fadeOut(1000);
        started = false;
        duck1Score = 0;
        duck2Score = 0;
    }
}

void drawTransition(int width, int height) {
    string score1 = "PLAYER 1     " + to_string(duck1Score);
    string score2 = "PLAYER 2     " + to_string(duck2Score);

    transition.menuText[0].setString("SCORE");
    transition.menuText[1].setString(score1);
    transition.menuText[2].setString(score2);

    window.draw(transition.background);
    for (int i = 0; i < 3; i++) {
        window.draw(transition.menuText[i]);
    }
}

void initPaused(int width, int height, RenderWindow& window) {

    float charSize = 60.f;

    // Loading and setting font  
    pausedMenu.font.loadFromFile("img/arcade.ttf");

    for (int i = 0; i < 3; i++) {
        pausedMenu.menuText[i].setFont(pausedMenu.font);
    }

    /* First Button (CONTINUE) */
    pausedMenu.menuText[0].setString("CONTINUE");
    pausedMenu.menuText[0].setCharacterSize(charSize);
    pausedMenu.menuText[0].setFillColor(Color(255, 255, 255));
    pausedMenu.menuText[0].setPosition(Vector2f(width / 1.5f, height / 4.f + 100.f));

    /* Second Button (SETTINGS) */
    pausedMenu.menuText[1].setString("SETTINGS");
    pausedMenu.menuText[1].setCharacterSize(charSize);
    pausedMenu.menuText[1].setFillColor(Color(255, 255, 255));
    pausedMenu.menuText[1].setPosition(Vector2f(width / 1.5f, height / 4.f + 250.f));

    /* Third Button (QUIT) */
    pausedMenu.menuText[2].setString("QUIT");
    pausedMenu.menuText[2].setCharacterSize(charSize);
    pausedMenu.menuText[2].setFillColor(Color(255, 255, 255));
    pausedMenu.menuText[2].setPosition(Vector2f(width / 1.5f, height / 4.f + 400.f));
}

void initMainMenu(int width, int height, RenderWindow& window) {

    float charSize = 60.f;

    // Loading and setting font  
    mainMenu.font.loadFromFile("img/arcade.ttf");

    for (int i = 0; i < 3; i++) {
        mainMenu.menuText[i].setFont(mainMenu.font);
    }

    /* First Button (PLAY) */
    mainMenu.menuText[0].setString("PLAY");
    mainMenu.menuText[0].setCharacterSize(charSize);
    mainMenu.menuText[0].setFillColor(Color(255, 255, 255));
    mainMenu.menuText[0].setPosition(Vector2f(width / 1.5f, height / 4.f + 100.f));

    /* Second Button (SETTINGS) */
    mainMenu.menuText[1].setString("SETTINGS");
    mainMenu.menuText[1].setCharacterSize(charSize);
    mainMenu.menuText[1].setFillColor(Color(255, 255, 255));
    mainMenu.menuText[1].setPosition(Vector2f(width / 1.5f, height / 4.f + 250.f));

    /* Third Button (EXIT) */
    mainMenu.menuText[2].setString("EXIT");
    mainMenu.menuText[2].setCharacterSize(charSize);
    mainMenu.menuText[2].setFillColor(Color(255, 255, 255));
    mainMenu.menuText[2].setPosition(Vector2f(width / 1.5f, height / 4.f + 400.f));

    // Setting background image
    mainMenu.backgroundTexture.loadFromFile("img/menu.png");
    mainMenu.background.setTexture(mainMenu.backgroundTexture);

    float scaleX, scaleY;

    scaleX = window.getSize().x * 1.0 / mainMenu.backgroundTexture.getSize().x;
    scaleY = window.getSize().y * 1.0 / mainMenu.backgroundTexture.getSize().y;

    mainMenu.background.setScale(scaleX, scaleY);
    mainMenu.background.setPosition(0.f, 0.f);

}
void initDuckAvatars() {
    duck1AvatarTexture.loadFromFile("img/Duck1NotReady.png");
    duck1AvatarReadyTexture.loadFromFile("img/Duck1Ready.png");
    duck2AvatarTexture.loadFromFile("img/Duck2NotReady.png");
    duck2AvatarReadyTexture.loadFromFile("img/Duck2Ready.png");

    duck1Avatar.setTexture(duck1AvatarTexture);
    duck2Avatar.setTexture(duck2AvatarTexture);
    duck1AvatarReady.setTexture(duck1AvatarReadyTexture);
    duck2AvatarReady.setTexture(duck2AvatarReadyTexture);

    int avatarScaleX, avatarScaleY;
    avatarScaleX = 100.f / duck1AvatarTexture.getSize().x;
    avatarScaleY = 105.f / duck1AvatarTexture.getSize().y;

    duck1Avatar.setScale(avatarScaleX, avatarScaleY);
    duck2Avatar.setScale(avatarScaleX, avatarScaleY);
    duck1AvatarReady.setScale(avatarScaleX, avatarScaleY);
    duck2AvatarReady.setScale(avatarScaleX, avatarScaleY);

    duck1Avatar.setPosition(40.f, 595.f);
    duck2Avatar.setPosition(1145.f, 595.f);
    duck1AvatarReady.setPosition(40.f, 595.f);
    duck2AvatarReady.setPosition(1145.f, 595.f);
}
void initGameMenu(int width, int height, RenderWindow& window) {
    initDuckAvatars();

    gameBackgroundTexture.loadFromFile("img/bigTV.png");
    gameBackground.setTexture(gameBackgroundTexture);

    int scaleX2, scaleY2;
    scaleX2 = window.getSize().x * 1.0 / gameBackgroundTexture.getSize().x;
    scaleY2 = window.getSize().y * 1.0 / gameBackgroundTexture.getSize().y;

    gameBackground.setScale(scaleX2, scaleY2);
    gameBackground.setPosition(0.f, 0.f);

    gameMenu.font.loadFromFile("img/arcade.ttf");

    for (int i = 0; i < 4; i++) {
        gameMenu.menuText[i].setFont(gameMenu.font);
        gameMenu.menuText[i].setCharacterSize(60);
        gameMenu.menuText[i].setFillColor(Color::White);
    }

    gameMenu.menuText[0].setString("START");
    gameMenu.menuText[0].setPosition(Vector2f(200.f, 400.f));

    string currLevel = "SELECTED  MAP     " + to_string(mapnum);
    gameMenu.menuText[1].setString(currLevel);
    gameMenu.menuText[1].setPosition(Vector2f(200.f, 200.f));

    gameMenu.menuText[2].setString("PLAYER 1");
    gameMenu.menuText[2].setPosition(Vector2f(width / 3.25f - 100, height / 4.f + 435));


    gameMenu.menuText[3].setString("PLAYER 2");
    gameMenu.menuText[3].setPosition(Vector2f(width / 2.f + 100, height / 4.f + 435));

    gameMenu.backgroundTexture.loadFromFile("img/GameMenu.png");
    gameMenu.background.setTexture(gameMenu.backgroundTexture);

    int scaleX, scaleY;
    scaleX = window.getSize().x * 1.0 / gameMenu.backgroundTexture.getSize().x;
    scaleY = window.getSize().y * 1.0 / gameMenu.backgroundTexture.getSize().y;

    gameMenu.background.setScale(scaleX, scaleY);
    gameMenu.background.setPosition(0.f, 0.f);


}
//For debugging purposes
void getMousePos() {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

void initMusic() {

    // Music
    if (!settingsMenu.menuMusic.openFromFile("img/music.wav")) {
        cout << "Error loading menu music.\n";
    }
    settingsMenu.menuMusic.setVolume(1.f);
    settingsMenu.menuMusic.setLoop(true);
    settingsMenu.menuMusic.play();

}
void initSettingsMenu(int width, int height, RenderWindow& window) {
    settingsMenu.backgroundTexture.loadFromFile("img/menu.png");
    settingsMenu.background.setTexture(settingsMenu.backgroundTexture);

    float scaleX, scaleY;
    scaleX = window.getSize().x * 1.0 / settingsMenu.backgroundTexture.getSize().x;
    scaleY = window.getSize().y * 1.0 / settingsMenu.backgroundTexture.getSize().y;

    settingsMenu.background.setScale(scaleX, scaleY);
    settingsMenu.background.setPosition(0.f, 0.f);

    settingsMenu.font.loadFromFile("img/arcade.ttf");
    settingsMenu.menuText[0].setFont(settingsMenu.font);
    settingsMenu.menuText[1].setFont(settingsMenu.font);
    settingsMenu.menuText[2].setFont(settingsMenu.font);

    int musicValue = settingsMenu.menuMusic.getVolume();
    string musicText = "MUSIC VOLUME           " + to_string(musicValue);

    settingsMenu.menuText[0].setString(musicText);
    settingsMenu.menuText[0].setCharacterSize(60);
    settingsMenu.menuText[0].setFillColor(Color::White);
    settingsMenu.menuText[0].setPosition(Vector2f(width / 2.f, height / 4.f + 100.f));

    settingsMenu.menuText[1].setString("CONTROLS");
    settingsMenu.menuText[1].setCharacterSize(60);
    settingsMenu.menuText[1].setFillColor(Color::White);
    settingsMenu.menuText[1].setPosition(Vector2f(width / 2.f, height / 4.f + 250.f));

    settingsMenu.menuText[2].setString("BACK");
    settingsMenu.menuText[2].setCharacterSize(60);
    settingsMenu.menuText[2].setFillColor(Color::White);
    settingsMenu.menuText[2].setPosition(Vector2f(width / 2.f, height / 4.f + 400));
}
void drawDucksGameMenu() {
    if (menuState == 2 && duck1.ready) {
        window.draw(duck1AvatarReady);
    }
    else if (menuState == 2) {
        window.draw(duck1Avatar);
    }

    if (menuState == 2 && duck2.ready) {
        window.draw(duck2AvatarReady);
    }
    else if (menuState == 2) {
        window.draw(duck2Avatar);
    }
}

void drawMenu(RenderWindow& window, Menu& menu, int itemCount) {
    if (menuState == 2)
        window.draw(gameBackground);

    window.draw(menu.background);

    if (fadeDirection != 0 || fadeCurrentAlpha > 0) {
        window.draw(fadeScreen);
    }

    if (menuState == 2)
        drawDucksGameMenu();

    if (menuState == 0 && drawPaused) {
        for (int i = 0; i < itemCount; i++) {
            window.draw(menu.menuText[i]);
        }
    }
    else if (menuState != 0) {
        for (int i = 0; i < itemCount; i++) {
            window.draw(menu.menuText[i]);
        }
    }
}

void moveUp(Menu& menu, int itemCount) {
    if (menu.selected - 1 >= -1) {
        menu.menuText[menu.selected].setOutlineThickness(0);
        menu.menuText[menu.selected].setFillColor(Color(255, 255, 255));

        menu.selected--;

        if (menu.selected == -1) {
            menu.selected = itemCount - 1;
        }

        menu.menuText[menu.selected].setOutlineThickness(10);
        menu.menuText[menu.selected].setOutlineColor(Color(255, 150, 0));
        menu.menuText[menu.selected].setFillColor(Color(255, 255, 255));
    }
}
void moveDown(Menu& menu, ll itemCount) {
    if (menu.selected + 1 <= itemCount) {
        // Reset current button's appearance
        menu.menuText[menu.selected].setOutlineThickness(0);
        menu.menuText[menu.selected].setFillColor(Color(255, 255, 255));

        menu.selected++;

        if (menu.selected == itemCount) {
            menu.selected = 0;
        }

        // Set new button's appearance
        menu.menuText[menu.selected].setOutlineThickness(10);
        menu.menuText[menu.selected].setOutlineColor(Color(255, 150, 0));
        menu.menuText[menu.selected].setFillColor(Color(255, 255, 255));
    }
}


//Khaled's MAP
Texture ground_texture, background1_Texture, skeleton_texture, stone_texture, tree_texture, cactus1, cactus2, cactus3, bushs_texture, grass_texture, stone_block_texture;
Sprite blocks[25], skeletons[3], stones[2], tree, ccts[5], bush[3], grs[2], stone_block[3], Background1;
RectangleShape player1_collider(Vector2f(40, 62)), player2_collider(Vector2f(40, 62));
RectangleShape pistol_collider(Vector2f(35, 25)), sniper_collider(Vector2f(70, 20)), pewpew_collider(Vector2f(55, 25)), swrd_collider(Vector2f(50, 20));
void grounds()
{
    blocks[0].setPosition(1030, 465);
    blocks[0].setScale(0.4, 0.35);
    blocks[1].setPosition(730, 300);
    blocks[1].setScale(0.4, 0.35);
    blocks[2].setPosition(250, 205);
    blocks[2].setScale(0.4, 0.35);
    blocks[3].setPosition(30, 300);
    blocks[3].setScale(0.4, 0.35);
    blocks[4].setPosition(950, 225);
    blocks[4].setScale(0.4, 0.35);
    blocks[5].setPosition(445, 115);
    blocks[5].setScale(0.4, 0.35);
    blocks[6].setPosition(355, 510);
    blocks[6].setScale(0.4, 0.35);
    blocks[7].setPosition(580, 570);
    blocks[7].setScale(0.4, 0.35);
    blocks[8].setPosition(1180, 300);
    blocks[8].setScale(0.25, 0.35);
    blocks[9].setPosition(130, 425);
    blocks[9].setScale(0.55, 0.35);
    blocks[10].setPosition(465, 365);
    blocks[10].setScale(0.4, 0.35);
    blocks[11].setPosition(830, 510);
    blocks[11].setScale(0.25, 0.35);
    blocks[12].setPosition(700, 145);
    blocks[12].setScale(0.4, 0.35);

}
void skeleton()
{
    skeletons[0].setPosition(720, 550);
    skeletons[0].setScale(-0.5, 0.5);
    skeletons[1].setPosition(1000, 205);
    skeletons[1].setScale(0.5, 0.5);
    skeletons[2].setPosition(110, 280);
    skeletons[2].setScale(-0.5, 0.5);
}
void stone()
{
    stones[0].setPosition(590, 540);
    stones[0].setScale(0.5, 0.5);
    stones[1].setPosition(285, 170);
    stones[1].setScale(0.55, 0.55);
}
void tre()
{
    tree.setPosition(500, 40);
    tree.setScale(0.3, 0.3);
}
void cactii()
{
    ccts[0].setPosition(125, 248);
    ccts[0].setScale(0.5, 0.5);
    ccts[1].setPosition(760, 93);
    ccts[1].setScale(-0.5, 0.5);
    ccts[2].setPosition(810, 272);
    ccts[2].setScale(0.7, 0.7);
    ccts[3].setPosition(250, 370);
    ccts[3].setScale(0.6, 0.6);
    ccts[4].setPosition(1100, 410);
    ccts[4].setScale(-0.6, 0.6);
}
void bushs()
{
    bush[0].setPosition(740, 265);
    bush[0].setScale(0.5, 0.5);
    bush[1].setPosition(220, 390);
    bush[1].setScale(-0.5, 0.5);
    bush[2].setPosition(850, 110);
    bush[2].setScale(-0.5, 0.5);
}
void grass()
{
    grs[0].setPosition(1120, 437);
    grs[0].setScale(0.6, 0.6);
    grs[1].setPosition(505, 337);
    grs[1].setScale(0.6, 0.6);
}
void stoneblock()
{
    stone_block[0].setPosition(1125, 432);
    stone_block[0].setScale(0.35, 0.35);
    stone_block[1].setPosition(1125, 400);
    stone_block[1].setScale(0.35, 0.35);
    stone_block[2].setPosition(360, 477);
    stone_block[2].setScale(0.35, 0.35);;
}
void Bullet_Collision1() {
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 25; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(blocks[j].getGlobalBounds()))
            {

                bulls.erase(i);
                break;
            }
        }
    }
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 3; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(stone_block[j].getGlobalBounds()))
            {

                bulls.erase(i);
                break;
            }
        }
    }
}
void collision_Map1(RectangleShape& player_collider, ducks& duck)
{
    FloatRect box, wall, intersection;
    box = player_collider.getGlobalBounds();
    for (int i = 0;i < 13;i++)
    {
        wall = blocks[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, blocks[i].getPosition().y);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 3;i++)
    {
        wall = stone_block[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, stone_block[i].getPosition().y);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
}
void collision_weaps_Map1(weapons& weap)
{
    if (weap.type == "grenade" && weap.boom) return;
    FloatRect box, wall, intersection;
    box = weap.weapon.getGlobalBounds();
    for (int i = 0;i < 13;i++)
    {
        wall = blocks[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, blocks[i].getPosition().y + 1);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                    weap.velocityY = 0.f;
                }
            }
            else
            {
                weap.velocityX = 0.f;
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 3;i++)
    {
        wall = stone_block[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, stone_block[i].getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
}
void SPAWN1() {
    sniper_collider.setOrigin(sniper_collider.getLocalBounds().width / 2, sniper_collider.getLocalBounds().height / 2);
    pistol_collider.setOrigin(pistol_collider.getLocalBounds().width / 2, pistol_collider.getLocalBounds().height / 2);
    pewpew_collider.setOrigin(pewpew_collider.getLocalBounds().width / 2, pewpew_collider.getLocalBounds().height / 2);
    swrd_collider.setOrigin(swrd_collider.getLocalBounds().width / 2, swrd_collider.getLocalBounds().height / 2);
    sniper.weapon.setPosition(480.f, 370);
    pistol.weapon.setPosition(760.f, 145);
    grenade.weapon.setPosition(520.f, 100);
    pewpew.weapon.setPosition(620.f, 560);
    sword.weapon.setPosition(1035.f, 465);
    pistol.collider = pistol_collider;
    sniper.collider = sniper_collider;
    pewpew.collider = pewpew_collider;
    sword.collider = swrd_collider;
    //spawn weaps
    bool flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - pistol.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - pistol.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(pistol);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - sniper.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - sniper.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(sniper);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - grenade.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - grenade.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(grenade);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - pewpew.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - pewpew.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(pewpew);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - sword.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - sword.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(sword);
    SPAWN.restart();
}
void init_Map1()
{
    DUCK_SCALE = 2.5f;
    GUN_SCALE = 2.f;
    fact = 5.f;
    gravity = 0.3f;
    jumpSpeed = -9.f;
    velocityX = 5;
    weapsVelocity = 7;
    init();
    // pistol
    pistol.fix_X = -6.f;
    pistol.fix_Y = -28.f;
    pistol.fix_hold_x = 31.f;
    pistol.fix_hold_y = -16.f;
    pistol.rev = 9.f;
    // grenade
    grenade.fix_hold_x = 36.f;
    grenade.fix_hold_y = -16.f;
    grenade.rev = 6.f;
    //sniper
    sniper.fix_X = 25.f;
    sniper.fix_Y = -23.f;
    sniper.fix_hold_x = 18.f;
    sniper.fix_hold_y = -22.f;
    sniper.rev = 24.f;
    //pewpew
    pewpew.fix_X = 3.f;
    pewpew.fix_Y = -16.f;
    pewpew.fix_hold_x = 28.f;
    pewpew.fix_hold_y = -20.f;
    pewpew.rev = 13.f;
    //sword
    sword.fix_hold_x = 48.f;
    sword.fix_hold_y = -20.f;
    sword.rev = 2.f;
    //bull1
    bull[1].scalex = 0.06f;
    bull[1].scaley = 0.06f;
    //bull2
    bull[2].scalex = 0.2f;
    bull[2].scaley = 0.2f;
    //grave
    Grave.setScale(0.13f, 0.083f);
    Grave.setOrigin(0, Grave.getLocalBounds().height);

    background1_Texture.loadFromFile("img/BG.png");
    skeleton_texture.loadFromFile("img/skeleton.png");
    ground_texture.loadFromFile("img/ground.png");
    stone_texture.loadFromFile("img/Stone.png");
    tree_texture.loadFromFile("img/Tree.png");
    cactus1.loadFromFile("img/Cactus (1).png");
    cactus2.loadFromFile("img/Cactus (2).png");
    cactus3.loadFromFile("img/Cactus (3).png");
    bushs_texture.loadFromFile("img/Bush (2).png");
    grass_texture.loadFromFile("img/Grass (1).png");
    stone_block_texture.loadFromFile("img/Stoneblock.png");
    Background1.setTexture(background1_Texture);
    tre();
    grounds();
    skeleton();
    stone();
    cactii();
    bushs();
    grass();
    stoneblock();
    for (int i = 0;i < 13;i++) blocks[i].setTexture(ground_texture);
    for (int i = 0;i < 3;i++) skeletons[i].setTexture(skeleton_texture);
    for (int i = 0;i < 2;i++) stones[i].setTexture(stone_texture);
    tree.setTexture(tree_texture);
    for (int i = 0;i < 2;i++) ccts[i].setTexture(cactus1);
    for (int i = 2;i < 3;i++) ccts[i].setTexture(cactus2);
    for (int i = 3;i < 5;i++) ccts[i].setTexture(cactus3);
    for (int i = 0;i < 3;i++) bush[i].setTexture(bushs_texture);
    for (int i = 0;i < 2;i++) grs[i].setTexture(grass_texture);
    player1_collider.setOrigin(player1_collider.getLocalBounds().width / 2, player1_collider.getLocalBounds().height / 2);
    player2_collider.setOrigin(player2_collider.getLocalBounds().width / 2, player2_collider.getLocalBounds().height / 2);
    Background1.setScale(1, 0.75);
    duck1.myduck.setPosition(30, 210);
    duck2.myduck.setPosition(1170, 225);

    SPAWN1();
}
void update_Map1()
{
    duck1.onGround = false;
    duck2.onGround = false;
    for (int i = 0;i < 3;i++) stone_block[i].setTexture(stone_block_texture);
    player1_collider.setPosition(duck1.myduck.getPosition().x + 38, duck1.myduck.getPosition().y - 30);
    player2_collider.setPosition(duck2.myduck.getPosition().x + 38, duck2.myduck.getPosition().y - 30);

    collision_Map1(player1_collider, duck1);
    collision_Map1(player2_collider, duck2);
    Bullet_Collision1();
    for (int i = 0;i < weaps.size();i++)
    {
        collision_weaps_Map1(weaps[i]);
    }
    update_Logic();
    if (SPAWN.getElapsedTime().asSeconds() >= 60) {
        SPAWN1();
    }
}
void draw_Map1()
{
    window.clear();
    window.draw(Background1);
    for (int i = 0;i < 13;i++)  window.draw(blocks[i]);
    for (int i = 0;i < 3;i++) window.draw(stone_block[i]);
    for (int i = 0;i < 2;i++) window.draw(stones[i]);
    for (int i = 0;i < 3;i++) window.draw(skeletons[i]);
    window.draw(tree);
    for (int i = 0;i < 5;i++) window.draw(ccts[i]);
    for (int i = 0;i < 2;i++) window.draw(grs[i]);
    draw_Logic();
}
void Map1() {
    update_Map1();
    draw_Map1();
}

//Fawzy's MAP
RectangleShape player1coll(Vector2f(19, 29));
RectangleShape player2coll(Vector2f(19, 29));
Texture GroundT, BackGroundT, TreeT, StoneT, CementT, RanT, BranchT, SideT;
Sprite CementF[250], GroundF[250], TreeF[250], StoneF[250], RanF[250], BranchF[250], SideF[250], BackGroundF;

void PosGround() {
    // 1
    GroundF[0].setPosition(300, 300);
    GroundF[1].setPosition(315, 300);
    GroundF[2].setPosition(330, 300);
    GroundF[3].setPosition(345, 300);
    GroundF[4].setPosition(360, 300);
    GroundF[5].setPosition(375, 300);
    GroundF[6].setPosition(390, 300);
    GroundF[7].setPosition(405, 300);
    GroundF[8].setPosition(420, 300);
    GroundF[9].setPosition(435, 300);
    // 2
    GroundF[10].setPosition(800, 300);
    GroundF[11].setPosition(815, 300);
    GroundF[12].setPosition(830, 300);
    GroundF[13].setPosition(845, 300);
    GroundF[14].setPosition(860, 300);
    GroundF[15].setPosition(875, 300);
    GroundF[16].setPosition(890, 300);
    GroundF[17].setPosition(905, 300);
    GroundF[18].setPosition(920, 300);
    GroundF[19].setPosition(935, 300);
    // 3
    GroundF[20].setPosition(580, 400);
    GroundF[21].setPosition(595, 400);
    GroundF[22].setPosition(610, 400);
    GroundF[23].setPosition(625, 400);
    GroundF[24].setPosition(640, 400);
    // 4
    GroundF[25].setPosition(595, 225);
    GroundF[26].setPosition(610, 225);
    GroundF[27].setPosition(625, 225);
    // 5
    GroundF[28].setPosition(530, 550);
    GroundF[29].setPosition(545, 550);
    GroundF[30].setPosition(560, 550);
    GroundF[31].setPosition(575, 550);
    GroundF[32].setPosition(590, 550);
    GroundF[33].setPosition(515, 550);
    GroundF[34].setPosition(520, 550);
    GroundF[35].setPosition(535, 550);
    GroundF[36].setPosition(590, 550);
    GroundF[37].setPosition(605, 550);
    GroundF[38].setPosition(620, 550);
    GroundF[39].setPosition(635, 550);
    GroundF[40].setPosition(650, 550);
    GroundF[41].setPosition(665, 550);
    GroundF[42].setPosition(680, 550);
    GroundF[43].setPosition(695, 550);
    GroundF[44].setPosition(710, 550);
    // 6
    GroundF[45].setPosition(725, 485);
    GroundF[46].setPosition(740, 485);
    GroundF[47].setPosition(755, 485);
    GroundF[48].setPosition(770, 485);
    GroundF[49].setPosition(785, 485);
    GroundF[50].setPosition(800, 485);
    GroundF[51].setPosition(815, 485);
    GroundF[52].setPosition(830, 485);
    GroundF[53].setPosition(845, 485);
    GroundF[54].setPosition(860, 485);
    GroundF[55].setPosition(875, 485);
    GroundF[56].setPosition(890, 485);
    GroundF[57].setPosition(905, 485);
    GroundF[58].setPosition(920, 485);
    GroundF[59].setPosition(935, 485);
    GroundF[60].setPosition(950, 485);
    GroundF[61].setPosition(965, 485);
    GroundF[62].setPosition(980, 485);
    GroundF[63].setPosition(995, 485);
    // 7
    GroundF[64].setPosition(1010, 400);
    GroundF[65].setPosition(1025, 400);
    GroundF[66].setPosition(1040, 400);
    GroundF[67].setPosition(1055, 400);
    GroundF[68].setPosition(1070, 400);
    GroundF[69].setPosition(1085, 400);
    GroundF[70].setPosition(1100, 400);
    // 8
    GroundF[71].setPosition(500, 485);
    GroundF[72].setPosition(500, 485);
    GroundF[73].setPosition(485, 485);
    GroundF[74].setPosition(470, 485);
    GroundF[75].setPosition(455, 485);
    GroundF[76].setPosition(440, 485);
    GroundF[77].setPosition(435, 485);
    GroundF[78].setPosition(420, 485);
    GroundF[79].setPosition(405, 485);
    GroundF[80].setPosition(390, 485);
    GroundF[81].setPosition(375, 485);
    GroundF[82].setPosition(360, 485);
    GroundF[83].setPosition(345, 485);
    GroundF[84].setPosition(330, 485);
    GroundF[85].setPosition(315, 485);
    GroundF[86].setPosition(300, 485);
    GroundF[87].setPosition(295, 485);
    GroundF[88].setPosition(280, 485);
    GroundF[89].setPosition(265, 485);
    // 9
    GroundF[90].setPosition(250, 400);
    GroundF[91].setPosition(235, 400);
    GroundF[92].setPosition(220, 400);
    GroundF[93].setPosition(205, 400);
    GroundF[94].setPosition(195, 400);
    GroundF[95].setPosition(180, 400);
    GroundF[96].setPosition(165, 400);

    GroundF[97].setPosition(935, 225);
    GroundF[98].setPosition(950, 225);
    GroundF[99].setPosition(965, 195);
    GroundF[100].setPosition(980, 195);
    GroundF[101].setPosition(995, 195);
    GroundF[102].setPosition(1010, 195);

    GroundF[103].setPosition(315, 225);
    GroundF[104].setPosition(300, 225);
    GroundF[105].setPosition(285, 195);
    GroundF[106].setPosition(270, 195);
    GroundF[107].setPosition(255, 195);
    GroundF[108].setPosition(240, 195);
}
void PosBranch() {
    BranchF[0].setPosition(166, 195);
    BranchF[0].setScale(-1, 1);
    BranchF[1].setPosition(166, 320);
    BranchF[1].setScale(-1, 1);
    BranchF[2].setPosition(1114, 195);
    BranchF[3].setPosition(1114, 320);
}
void PosSide() {
    SideF[0].setPosition(300, 300);
    SideF[0].setScale(-1, 1);
    SideF[1].setPosition(450, 300);
    SideF[1].setScale(1, 1);
    SideF[2].setPosition(800, 300);
    SideF[2].setScale(-1, 1);
    SideF[3].setPosition(950, 300);
    SideF[3].setScale(1, 1);
    SideF[4].setPosition(580, 400);
    SideF[4].setScale(-1, 1);
    SideF[5].setPosition(655, 400);
    SideF[5].setScale(1, 1);
    SideF[6].setPosition(595, 225);
    SideF[6].setScale(-1, 1);
    SideF[7].setPosition(640, 225);
    SideF[7].setScale(1, 1);
    SideF[8].setPosition(1115, 400);
    SideF[8].setScale(1, 1);
    SideF[9].setPosition(165, 400);
    SideF[9].setScale(-1, 1);
}
void PosCement() {
    CementF[0].setPosition(530, 565);
    CementF[1].setPosition(545, 565);
    CementF[2].setPosition(560, 565);
    CementF[3].setPosition(575, 565);
    CementF[4].setPosition(590, 565);
    CementF[5].setPosition(515, 565);
    CementF[6].setPosition(520, 565);
    CementF[7].setPosition(535, 565);
    CementF[8].setPosition(590, 565);
    CementF[9].setPosition(605, 565);
    CementF[10].setPosition(620, 565);
    CementF[11].setPosition(635, 565);
    CementF[12].setPosition(650, 565);
    CementF[13].setPosition(665, 565);
    CementF[14].setPosition(680, 565);
    CementF[15].setPosition(695, 565);
    CementF[16].setPosition(710, 565);
    CementF[17].setPosition(530, 580);
    CementF[18].setPosition(545, 580);
    CementF[19].setPosition(560, 580);
    CementF[20].setPosition(575, 580);
    CementF[21].setPosition(590, 580);
    CementF[22].setPosition(515, 580);
    CementF[23].setPosition(520, 580);
    CementF[24].setPosition(535, 580);
    CementF[25].setPosition(590, 580);
    CementF[26].setPosition(605, 580);
    CementF[27].setPosition(620, 580);
    CementF[28].setPosition(635, 580);
    CementF[29].setPosition(650, 580);
    CementF[30].setPosition(665, 580);
    CementF[31].setPosition(680, 580);
    CementF[32].setPosition(695, 580);
    CementF[33].setPosition(710, 580);

    CementF[34].setPosition(725, 500);
    CementF[35].setPosition(740, 500);
    CementF[36].setPosition(755, 500);
    CementF[37].setPosition(770, 500);
    CementF[38].setPosition(785, 500);
    CementF[39].setPosition(800, 500);
    CementF[40].setPosition(725, 515);
    CementF[41].setPosition(740, 515);
    CementF[42].setPosition(755, 515);
    CementF[43].setPosition(770, 515);
    CementF[44].setPosition(785, 515);
    CementF[45].setPosition(800, 515);
    CementF[46].setPosition(725, 530);
    CementF[47].setPosition(740, 530);
    CementF[48].setPosition(755, 530);
    CementF[49].setPosition(770, 530);
    CementF[50].setPosition(785, 530);
    CementF[51].setPosition(800, 530);
    CementF[52].setPosition(725, 545);
    CementF[53].setPosition(740, 545);
    CementF[54].setPosition(755, 545);
    CementF[55].setPosition(770, 545);
    CementF[56].setPosition(725, 560);
    CementF[57].setPosition(740, 560);
    CementF[58].setPosition(755, 560);
    CementF[59].setPosition(770, 560);
    CementF[60].setPosition(725, 575);
    CementF[61].setPosition(740, 575);
    CementF[62].setPosition(755, 575);
    CementF[63].setPosition(770, 575);

    CementF[64].setPosition(500, 500);
    CementF[65].setPosition(485, 500);
    CementF[66].setPosition(470, 500);
    CementF[67].setPosition(455, 500);
    CementF[68].setPosition(440, 500);
    CementF[69].setPosition(425, 500);
    CementF[70].setPosition(500, 515);
    CementF[71].setPosition(485, 515);
    CementF[72].setPosition(470, 515);
    CementF[73].setPosition(455, 515);
    CementF[74].setPosition(440, 515);
    CementF[75].setPosition(425, 515);
    CementF[76].setPosition(500, 530);
    CementF[77].setPosition(485, 530);
    CementF[78].setPosition(470, 530);
    CementF[79].setPosition(455, 530);
    CementF[80].setPosition(440, 530);
    CementF[81].setPosition(425, 530);
    CementF[82].setPosition(500, 545);
    CementF[83].setPosition(485, 545);
    CementF[84].setPosition(470, 545);
    CementF[85].setPosition(455, 545);
    CementF[86].setPosition(500, 560);
    CementF[87].setPosition(485, 560);
    CementF[88].setPosition(470, 560);
    CementF[89].setPosition(455, 560);
    CementF[90].setPosition(500, 575);
    CementF[91].setPosition(485, 575);
    CementF[92].setPosition(470, 575);
    CementF[93].setPosition(455, 575);

    CementF[94].setPosition(1010, 415);
    CementF[95].setPosition(1025, 415);
    CementF[96].setPosition(1040, 415);
    CementF[97].setPosition(1055, 415);
    CementF[98].setPosition(1070, 415);
    CementF[99].setPosition(1085, 415);
    CementF[100].setPosition(1100, 415);
    CementF[101].setPosition(1010, 430);
    CementF[102].setPosition(1025, 430);
    CementF[103].setPosition(1040, 430);
    CementF[104].setPosition(1055, 430);
    CementF[105].setPosition(1070, 430);
    CementF[106].setPosition(1085, 430);
    CementF[107].setPosition(1100, 430);
    CementF[108].setPosition(1010, 445);
    CementF[109].setPosition(1025, 445);
    CementF[110].setPosition(1040, 445);
    CementF[111].setPosition(1055, 445);
    CementF[112].setPosition(1070, 445);
    CementF[113].setPosition(1010, 460);
    CementF[114].setPosition(1025, 460);
    CementF[115].setPosition(1040, 460);
    CementF[116].setPosition(1055, 460);
    CementF[117].setPosition(1070, 460);
    CementF[118].setPosition(1010, 475);
    CementF[119].setPosition(1025, 475);
    CementF[120].setPosition(1040, 475);
    CementF[121].setPosition(1055, 475);
    CementF[122].setPosition(1070, 475);

    CementF[123].setPosition(250, 415);
    CementF[124].setPosition(235, 415);
    CementF[125].setPosition(220, 415);
    CementF[126].setPosition(205, 415);
    CementF[127].setPosition(190, 415);
    CementF[128].setPosition(175, 415);
    CementF[129].setPosition(160, 415);
    CementF[130].setPosition(250, 430);
    CementF[131].setPosition(235, 430);
    CementF[132].setPosition(220, 430);
    CementF[133].setPosition(205, 430);
    CementF[134].setPosition(190, 430);
    CementF[135].setPosition(175, 430);
    CementF[136].setPosition(160, 430);
    CementF[137].setPosition(250, 445);
    CementF[138].setPosition(235, 445);
    CementF[139].setPosition(220, 445);
    CementF[140].setPosition(205, 445);
    CementF[141].setPosition(190, 445);
    CementF[142].setPosition(250, 460);
    CementF[143].setPosition(235, 460);
    CementF[144].setPosition(220, 460);
    CementF[145].setPosition(205, 460);
    CementF[146].setPosition(190, 460);
    CementF[147].setPosition(250, 475);
    CementF[148].setPosition(235, 475);
    CementF[149].setPosition(220, 475);
    CementF[150].setPosition(205, 475);
    CementF[151].setPosition(190, 475);

    CementF[152].setPosition(965, 210);
    CementF[153].setPosition(980, 210);
    CementF[154].setPosition(995, 210);
    CementF[155].setPosition(1010, 210);
    CementF[156].setPosition(965, 225);
    CementF[157].setPosition(980, 225);
    CementF[158].setPosition(995, 225);
    CementF[159].setPosition(1010, 225);

    CementF[160].setPosition(285, 210);
    CementF[161].setPosition(270, 210);
    CementF[162].setPosition(255, 210);
    CementF[163].setPosition(240, 210);
    CementF[164].setPosition(285, 225);
    CementF[165].setPosition(270, 225);
    CementF[166].setPosition(255, 225);
    CementF[167].setPosition(240, 225);
}
void PosTree() {
    TreeF[0].setPosition(165, 380);
    TreeF[0].setScale(1.5, 1);
    TreeF[1].setPosition(165, 360);
    TreeF[1].setScale(1.5, 1);
    TreeF[2].setPosition(165, 340);
    TreeF[2].setScale(1.5, 1);
    TreeF[3].setPosition(165, 320);
    TreeF[3].setScale(1.5, 1);
    TreeF[4].setPosition(165, 300);
    TreeF[4].setScale(1.5, 1);
    TreeF[5].setPosition(165, 280);
    TreeF[5].setScale(1.5, 1);
    TreeF[6].setPosition(165, 260);
    TreeF[6].setScale(1.5, 1);
    TreeF[7].setPosition(165, 240);
    TreeF[7].setScale(1.5, 1);
    TreeF[8].setPosition(165, 220);
    TreeF[8].setScale(1.5, 1);
    TreeF[9].setPosition(165, 200);
    TreeF[9].setScale(1.5, 1);
    TreeF[10].setPosition(165, 180);
    TreeF[10].setScale(1.5, 1);
    TreeF[11].setPosition(165, 160);
    TreeF[11].setScale(1.5, 1);
    TreeF[12].setPosition(165, 140);
    TreeF[12].setScale(1.5, 1);

    TreeF[13].setPosition(1115, 380);
    TreeF[13].setScale(-1.5, 1);
    TreeF[14].setPosition(1115, 360);
    TreeF[14].setScale(-1.5, 1);
    TreeF[15].setPosition(1115, 340);
    TreeF[15].setScale(-1.5, 1);
    TreeF[16].setPosition(1115, 320);
    TreeF[16].setScale(-1.5, 1);
    TreeF[17].setPosition(1115, 300);
    TreeF[17].setScale(-1.5, 1);
    TreeF[18].setPosition(1115, 280);
    TreeF[18].setScale(-1.5, 1);
    TreeF[19].setPosition(1115, 260);
    TreeF[19].setScale(-1.5, 1);
    TreeF[20].setPosition(1115, 240);
    TreeF[20].setScale(-1.5, 1);
    TreeF[21].setPosition(1115, 220);
    TreeF[21].setScale(-1.5, 1);
    TreeF[22].setPosition(1115, 200);
    TreeF[22].setScale(-1.5, 1);
    TreeF[23].setPosition(1115, 180);
    TreeF[23].setScale(-1.5, 1);
    TreeF[24].setPosition(1115, 160);
    TreeF[24].setScale(-1.5, 1);
    TreeF[25].setPosition(1115, 140);
    TreeF[25].setScale(-1.5, 1);
}
void PosStone() {
    StoneF[0].setPosition(405, 288);
    StoneF[0].setScale(1, 1);
    StoneF[1].setPosition(845, 288);
    StoneF[1].setScale(-1, 1);
    StoneF[2].setPosition(785, 473);
    StoneF[2].setScale(-1, 1);
    StoneF[3].setPosition(455, 473);
    StoneF[3].setScale(1, 1);
}
void PosRan() {
    RanF[0].setPosition(610, 300);
    RanF[0].setScale(1, 1);
}
void init_Map2() {
    DUCK_SCALE = 1.2f;
    GUN_SCALE = 0.96f;
    fact = 3.f;
    gravity = 0.24f;
    jumpSpeed = -7.2f;  //give it negative value
    velocityX = 3.f;   // the duck speed
    weapsVelocity = 5;
    init();
    // pistol
    pistol.fix_X = -5.f;
    pistol.fix_Y = -15.f;
    pistol.fix_hold_x = 14.f;
    pistol.fix_hold_y = -8.f;
    pistol.rev = 11.f;
    //sniper
    sniper.fix_X = 10.f;
    sniper.fix_Y = -12.f;
    sniper.fix_hold_x = 8.f;
    sniper.fix_hold_y = -11.f;
    sniper.rev = 24.f;
    //pewpew
    pewpew.fix_X = -2.f;
    pewpew.fix_Y = -8.f;
    pewpew.fix_hold_x = 13.f;
    pewpew.fix_hold_y = -10.f;
    pewpew.rev = 14.f;
    //sword
    sword.fix_hold_x = 23.f;
    sword.fix_hold_y = -10.f;
    sword.rev = 2.f;
    //bull1
    bull[1].scalex = 0.029f;
    bull[1].scaley = 0.029f;
    //bull2
    bull[2].scalex = 0.096f;
    bull[2].scaley = 0.096f;
    //grave
    Grave.setScale(0.0624f, 0.04f);
    Grave.setOrigin(0, Grave.getLocalBounds().height);

    BackGroundT.loadFromFile("BackGroundT.png");
    GroundT.loadFromFile("GroundT.png");
    StoneT.loadFromFile("StoneT.png");
    CementT.loadFromFile("CementT.png");
    RanT.loadFromFile("RanT.png");
    TreeT.loadFromFile("TreeT.png");
    BranchT.loadFromFile("BranchT.png");
    SideT.loadFromFile("SideT.png");

    BackGroundF.setPosition(0, 0);
    BackGroundF.setScale(12, 10);
    PosBranch();
    PosCement();
    PosGround();
    PosRan();
    PosSide();
    PosStone();
    PosTree();

    duck1.myduck.setPosition(330, 266);
    duck2.myduck.setPosition(880, 266);
    sniper.weapon.setPosition(250, 100);
    pistol.weapon.setPosition(980, 100);
    pewpew.weapon.setPosition(600, 100);
    sword.weapon.setPosition(600, 500);
    weaps.push_back(pistol);
    weaps.push_back(sniper);
    weaps.push_back(pewpew);
    weaps.push_back(sword);


}
void Bullet_Collision2() {
    for (int i = bulls.size() - 1;i >= 0; i--)
    {
        for (int j = 0; j < 109; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(GroundF[j].getGlobalBounds()))
            {
                bulls.erase(i);
                break;
            }
        }
        for (int j = 0; j < 168; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(CementF[j].getGlobalBounds()))
            {
                bulls.erase(i);
                break;
            }
        }
        for (int j = 0; j < 10; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(SideF[j].getGlobalBounds()))
            {
                bulls.erase(i);
                break;
            }
        }
        for (int j = 0; j < 4; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(StoneF[j].getGlobalBounds()))
            {
                bulls.erase(i);
                break;
            }
        }
        for (int j = 0; j < 1; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(RanF[j].getGlobalBounds()))
            {
                bulls.erase(i);
                break;
            }
        }
    }
}
void collision_weaps_Map2(weapons& weap)
{
    FloatRect box, wall, intersection;
    box = weap.weapon.getGlobalBounds();
    for (int i = 0; i < 109;i++)
    {
        wall = GroundF[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, GroundF[i].getPosition().y + 1.f);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                    weap.velocityY = 0.f;
                }
            }
            else
            {
                weap.velocityX = 0.f;
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
    box = weap.weapon.getGlobalBounds();
    for (int i = 0; i < 10;i++)
    {
        wall = SideF[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, SideF[i].getPosition().y + 1.f);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                    weap.velocityY = 0.f;
                }
            }
            else
            {
                weap.velocityX = 0.f;
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
    box = weap.weapon.getGlobalBounds();
    for (int i = 0; i < 168;i++)
    {
        wall = CementF[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, CementF[i].getPosition().y + 1.f);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                    weap.velocityY = 0.f;
                }
            }
            else
            {
                weap.velocityX = 0.f;
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
    box = weap.weapon.getGlobalBounds();
    for (int i = 0; i < 4;i++)
    {
        wall = StoneF[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, StoneF[i].getPosition().y + 1.f);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                    weap.velocityY = 0.f;
                }
            }
            else
            {
                weap.velocityX = 0.f;
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
    box = weap.weapon.getGlobalBounds();
    for (int i = 0; i < 4;i++)
    {
        wall = BranchF[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, BranchF[i].getPosition().y + 1.f);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                    weap.velocityY = 0.f;
                }
            }
            else
            {
                weap.velocityX = 0.f;
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
    box = weap.weapon.getGlobalBounds();
    for (int i = 0; i < 1;i++)
    {
        wall = RanF[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, RanF[i].getPosition().y + 1.f);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                    weap.velocityY = 0.f;
                }
            }
            else
            {
                weap.velocityX = 0.f;
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
}
void collision_Map2(RectangleShape& player_collider, ducks& duck)
{
    FloatRect box, wall, intersection;
    box = player_collider.getGlobalBounds();
    for (int i = 0; i < 109;i++)
    {
        wall = GroundF[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, GroundF[i].getPosition().y + 1.f); // added 1.f
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left <= wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
    box = player_collider.getGlobalBounds();
    for (int i = 0; i < 168;i++)
    {
        wall = CementF[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width < intersection.height)
            {
                if (box.left <= wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
            else
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, CementF[i].getPosition().y + 1.f);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
        }
    }
    box = player_collider.getGlobalBounds();
    for (int i = 0; i < 10;i++)
    {
        wall = SideF[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, SideF[i].getPosition().y + 1.f);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left <= wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
    box = player_collider.getGlobalBounds();
    for (int i = 0; i < 4;i++)
    {
        wall = StoneF[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, StoneF[i].getPosition().y + 1.f);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left <= wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
    box = player_collider.getGlobalBounds();
    for (int i = 0; i < 4;i++)
    {
        wall = BranchF[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, BranchF[i].getPosition().y + 1.f);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left <= wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
    box = player_collider.getGlobalBounds();
    for (int i = 0; i < 1;i++)
    {
        wall = RanF[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, RanF[i].getPosition().y + 1.f);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left <= wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
}
void update_Map2() {
    BackGroundF.setTexture(BackGroundT);
    for (int i = 0; i < 250; i++) GroundF[i].setTexture(GroundT);
    for (int i = 0; i < 250; i++) CementF[i].setTexture(CementT);
    for (int i = 0; i < 250; i++) TreeF[i].setTexture(TreeT);
    for (int i = 0; i < 250; i++) BranchF[i].setTexture(BranchT);
    for (int i = 0; i < 250; i++) SideF[i].setTexture(SideT);
    for (int i = 0; i < 250; i++) RanF[i].setTexture(RanT);
    for (int i = 0; i < 250; i++) StoneF[i].setTexture(StoneT);
    player1coll.setPosition(duck1.myduck.getPosition().x + 9.5, duck1.myduck.getPosition().y - 28);
    player2coll.setPosition(duck2.myduck.getPosition().x + 9.5, duck2.myduck.getPosition().y - 28);
    for (int i = 0;i < weaps.size();i++)
    {
        if (weaps[i].type == "pistol")
        {
            weaps[i].collider.setPosition(weaps[i].weapon.getPosition().x + 17, weaps[i].weapon.getPosition().y - 10);
        }
        else if (weaps[i].type == "sniper")
        {
            weaps[i].collider.setPosition(weaps[i].weapon.getPosition().x + 33, weaps[i].weapon.getPosition().y - 10);
        }
        else if (weaps[i].type == "pewpew")
        {
            weaps[i].collider.setPosition(weaps[i].weapon.getPosition().x + 25, weaps[i].weapon.getPosition().y - 10);
        }
        else
        {
            weaps[i].collider.setPosition(weaps[i].weapon.getPosition().x + 23, weaps[i].weapon.getPosition().y - 8);
        }
    }
    Bullet_Collision2();
    for (int i = 0; i < weaps.size(); i++) {
        collision_weaps_Map2(weaps[i]);
    }
    duck1.onGround = false;
    duck2.onGround = false;
    collision_Map2(player1coll, duck1);
    collision_Map2(player2coll, duck2);
    update_Logic();
}
void draw_Map2() {
    window.draw(BackGroundF);
    for (int i = 0; i < 109; i++) window.draw(GroundF[i]);
    for (int i = 0; i < 168; i++) window.draw(CementF[i]);
    for (int i = 0; i < 26; i++) window.draw(TreeF[i]);
    for (int i = 0; i < 4; i++) window.draw(BranchF[i]);
    for (int i = 0; i < 10; i++) window.draw(SideF[i]);
    for (int i = 0; i < 1; i++) window.draw(RanF[i]);
    for (int i = 0; i < 4; i++) window.draw(StoneF[i]);


    draw_Logic();
}
void Map2() {
    update_Map2();
    draw_Map2();
}


// Khaled Ghareeb
Sprite blocks3; Sprite obs;

const int MAX_BLOCKS = 50;
Sprite finalground[MAX_BLOCKS];
Sprite finalblock[MAX_BLOCKS];
Sprite finalobs[MAX_BLOCKS];
int groundCount = 0;
int blockCount = 0;
int obsCount = 0;

Sprite backgroundmap3;
Texture backgroundtexture;

Texture mapblock_Texture;
Texture obstacels_texture;

RectangleShape playercollider1(Vector2f(28, 46));
RectangleShape playercollider2(Vector2f(28, 46));

void SPAWN3() {
    playercollider1.setOrigin(duck1.myduck.getLocalBounds().width / 2 - 34, duck1.myduck.getLocalBounds().height / 2 + 30);
    playercollider2.setOrigin(duck2.myduck.getLocalBounds().width / 2 - 34, duck2.myduck.getLocalBounds().height / 2 + 30);

    pistol.weapon.setPosition(60, 466);
    pewpew.weapon.setPosition(1240, 460);
    sniper.weapon.setPosition(750, 366);
    sword.weapon.setPosition(875, 535);
    grenade.weapon.setPosition(645, 135);

    //spawn weaps
    bool flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - pistol.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - pistol.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(pistol);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - sniper.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - sniper.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(sniper);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - pewpew.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - pewpew.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(pewpew);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - sword.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - sword.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(sword);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - grenade.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - grenade.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(grenade);
    SPAWN.restart();
}
void map3_groundd()
{
    for (int i = 0; i < 10; i++)
    {

        blocks3.setPosition(i * 150, 697);
        finalground[groundCount++] = blocks3;
    }
}
void obstacels_position(int x, int y)
{
    obs.setPosition(x, y);
    finalobs[obsCount++] = obs;

}
void block_position(int x, int y)
{

    blocks3.setPosition(x, y);
    finalblock[blockCount++] = blocks3;

}
void map3_positions()
{
    map3_groundd();

    block_position(295, 545);
    block_position(0, 595);
    block_position(1060, 465);
    block_position(0, 470);
    block_position(190, 370);
    block_position(400, 370);
    block_position(610, 370);
    block_position(820, 370);
    block_position(1058, 620);
    block_position(800, 540);
    block_position(0, 270);
    block_position(1050, 270);
    block_position(290, 200);
    block_position(550, 130);
    block_position(800, 180);

    obstacels_position(265, 332);
    obstacels_position(300, 332);
    obstacels_position(700, 619);
    obstacels_position(661, 658);
    obstacels_position(700, 658);
    obstacels_position(98, 431);
    obstacels_position(1100, 390);
    obstacels_position(1100, 427);
    obstacels_position(950, 331);
    obstacels_position(460, 161);
}
void handleCollision3(RectangleShape& player, Sprite& obj, ducks& duck)
{
    FloatRect duckBounds = player.getGlobalBounds();
    FloatRect objBounds = obj.getGlobalBounds();

    if (duckBounds.intersects(objBounds))
    {
        FloatRect intersection;
        duckBounds.intersects(objBounds, intersection);

        if (intersection.width < intersection.height)
        {
            if (duckBounds.left < objBounds.left)
                duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
            else
                duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
        }
        else
        {
            if (duckBounds.top < objBounds.top)
            {
                if (duck.velocityY < 0) {
                    return;
                }
                duck.myduck.setPosition(duck.myduck.getPosition().x, obj.getPosition().y + 1.f);
                duck.onGround = 1;
                duck.isJumping = 0;
                duck.velocityY = 0;
            }
            else {
                if (duck.velocityY > 0) {
                    return;
                }
                duck.velocityY = 0;
                duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
            }
        }
    }
}
void collision3(RectangleShape& player, ducks& duck)
{
    for (int i = 0; i < blockCount; i++)
        handleCollision3(player, finalblock[i], duck);

    for (int i = 0; i < groundCount; i++)
        handleCollision3(player, finalground[i], duck);

    for (int i = 0; i < obsCount; i++)
        handleCollision3(player, finalobs[i], duck);
}
void handle_BulletCollision3(Sprite& obj)
{
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < MAX_BLOCKS; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(obj.getGlobalBounds()))
            {

                bulls.erase(i);
                break;
            }
        }
    }
}
void BulletCollision3()
{
    for (int i = 0; i < obsCount; i++)
        handle_BulletCollision3(finalobs[i]);
    for (int i = 0; i < blockCount; i++)
        handle_BulletCollision3(finalblock[i]);
    for (int i = 0; i < groundCount; i++)
        handle_BulletCollision3(finalground[i]);
}
void weapon_handleCollision3(weapons& weapon, Sprite& obj)
{
    if (weapon.type == "grenade" && weapon.boom) return;
    FloatRect weapBounds = weapon.weapon.getGlobalBounds();
    FloatRect objBounds = obj.getGlobalBounds();

    if (weapBounds.intersects(objBounds))
    {
        FloatRect intersection;
        weapBounds.intersects(objBounds, intersection);

        if (intersection.width < intersection.height)
        {
            if (weapBounds.left < objBounds.left)
                weapon.weapon.setPosition(weapon.weapon.getPosition().x - intersection.width, weapon.weapon.getPosition().y);
            else
                weapon.weapon.setPosition(weapon.weapon.getPosition().x + intersection.width, weapon.weapon.getPosition().y);
            weapon.velocityX = 0;
        }

        else {
            if (weapBounds.top < objBounds.top)
            {
                weapon.weapon.setPosition(weapon.weapon.getPosition().x, obj.getPosition().y + 1.f);
                weapon.velocityY = 0;
                weapon.velocityX = 0;
                weapon.onGround = true;

            }
            else
            {
                weapon.weapon.setPosition(weapon.weapon.getPosition().x, weapon.weapon.getPosition().y + intersection.height);
                weapon.velocityY = 0;
            }

        }
    }
}
void collision_Weapon3(weapons& weapon)
{
    for (int i = 0; i < blockCount; i++)
        weapon_handleCollision3(weapon, finalblock[i]);
    for (int i = 0; i < groundCount; i++)
        weapon_handleCollision3(weapon, finalground[i]);
    for (int i = 0; i < obsCount; i++)
        weapon_handleCollision3(weapon, finalobs[i]);
}
void init_Map3()
{
    DUCK_SCALE = 2.f;
    GUN_SCALE = 1.6f;
    fact = 3.f;
    gravity = 0.05f;
    jumpSpeed = -4.f;  //give it negative value
    velocityX = 1.5;   // the duck speed
    weapsVelocity = 2;
    init();
    // pistol
    pistol.fix_X = -4.f;
    pistol.fix_Y = -23.f;
    pistol.fix_hold_x = 25.f;
    pistol.fix_hold_y = -12.f;
    pistol.rev = 10.f;
    // grenade
    grenade.fix_hold_x = 29.f;
    grenade.fix_hold_y = -12.f;
    grenade.rev = 6.f;
    //sniper
    sniper.fix_X = 17.f;
    sniper.fix_Y = -18.f;
    sniper.fix_hold_x = 14.f;
    sniper.fix_hold_y = -16.f;
    sniper.rev = 23.f;
    //pewpew
    pewpew.fix_X = 7.f;
    pewpew.fix_Y = -13.f;
    pewpew.fix_hold_x = 18.f;
    pewpew.fix_hold_y = -15.f;
    pewpew.rev = 17.f;
    //sword
    sword.fix_hold_x = 38.f;
    sword.fix_hold_y = -14.f;
    sword.rev = 2.f;
    //bull1
    bull[1].scalex = 0.048f;
    bull[1].scaley = 0.048f;
    //bull2
    bull[2].scalex = 0.16f;
    bull[2].scaley = 0.16f;
    //grave
    Grave.setScale(0.107f, 0.067f);
    Grave.setOrigin(0, Grave.getLocalBounds().height);

    backgroundtexture.loadFromFile("img/background.jpg");
    mapblock_Texture.loadFromFile("img/cccc.png");
    obstacels_texture.loadFromFile("img/Block 2.png");


    backgroundmap3.setTexture(backgroundtexture);
    backgroundmap3.setScale(0.5, 0.5);

    blocks3.setTexture(mapblock_Texture);
    blocks3.setScale(0.45, 0.17);

    obs.setTexture(obstacels_texture);
    obs.setScale(0.035, 0.035);

    duck1.myduck.setPosition(180, 250);
    duck2.myduck.setPosition(530, 660);

    SPAWN3();
    map3_positions();

}
void update3()
{
    duck1.onGround = false;
    duck2.onGround = false;

    playercollider1.setPosition(duck1.myduck.getPosition());
    playercollider2.setPosition(duck2.myduck.getPosition());

    for (ll i = 0;i < weaps.size();++i)
        weaps[i].collider.setPosition(weaps[i].weapon.getPosition());

    collision3(playercollider1, duck1);
    collision3(playercollider2, duck2);

    BulletCollision3();
    for (ll i = 0;i < weaps.size();++i)
        collision_Weapon3(weaps[i]);

    duck1.myarm.arm.setPosition(
        duck1.myduck.getPosition().x + duck1.myduck.getGlobalBounds().width / 6,
        duck1.myduck.getPosition().y - duck1.myduck.getGlobalBounds().height / 2 - fact
    );
    duck2.myarm.arm.setPosition(
        duck2.myduck.getPosition().x + duck2.myduck.getGlobalBounds().width / 6,
        duck2.myduck.getPosition().y - duck2.myduck.getGlobalBounds().height / 2 - fact
    );


    update_Logic();
    if (SPAWN.getElapsedTime().asSeconds() >= 60) {
        SPAWN3();
    }

}
void draw3()
{
    window.draw(backgroundmap3);

    for (int i = 0; i < groundCount; i++)
        window.draw(finalground[i]);

    for (int i = 0; i < blockCount; i++)
        window.draw(finalblock[i]);

    for (int i = 0; i < obsCount; i++)
        window.draw(finalobs[i]);


    draw_Logic();

}
void Map3() {
    update3();
    draw3();
}


// Abdullah
string snowMap[23] = {
    "........................................",
    "........................................",
    "........................................",
    "........................................",
    "........................................",
    "........................................",
    "........................................",
    ".._..1...........................2..._..",
    "..|____....._......S......_......____|..",
    "......|....<|.....<=>.....|>.....|......",
    "......|.....|.............|......|......",
    "......|.....|.............|......|......",
    "......|....W|.............|E.....|......",
    "......|...<~|++++.._..++++|=>....|......",
    "......|..........................|......",
    "......|..........................|......",
    "......|............P.............|......",
    "......|++++++++++++++++++++++++++|......",
    "........................................",
    "........................................",
    "........................................",
    "........................................",
    "........................................"
};
Sprite background;
void SPAWN4() {

    for (int y = 0; y < 23; y++) {
        for (int x = 0; x < snowMap[y].length(); x++) {
            bool flag = 1;
            if (snowMap[y][x] == 'S') {
                weapPos.x = x * TILE_SIZEE;
                weapPos.y = (y + .9) * TILE_SIZEE;
                sniper.weapon.setPosition(weapPos.x, weapPos.y);
                for (ll i = 0;i < weaps.size();++i) {
                    if (abs(weaps[i].weapon.getPosition().x - sniper.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - sniper.weapon.getPosition().y) <= 20) {
                        flag = 0;
                    }
                }
                if (flag) weaps.push_back(sniper);
            }
            else if (snowMap[y][x] == 'W') {
                weapPos.x = x * TILE_SIZEE;
                weapPos.y = (y + .9) * TILE_SIZEE;
                sword.weapon.setPosition(weapPos.x, weapPos.y);
                for (ll i = 0;i < weaps.size();++i) {
                    if (abs(weaps[i].weapon.getPosition().x - sword.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - sword.weapon.getPosition().y) <= 20) {
                        flag = 0;
                    }
                }
                if (flag) weaps.push_back(sword);
            }
            else if (snowMap[y][x] == 'E') {
                weapPos.x = (x - .3) * TILE_SIZEE;
                weapPos.y = (y + .9) * TILE_SIZEE;
                pewpew.weapon.setPosition(weapPos.x, weapPos.y);
                for (ll i = 0;i < weaps.size();++i) {
                    if (abs(weaps[i].weapon.getPosition().x - pewpew.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - pewpew.weapon.getPosition().y) <= 20) {
                        flag = 0;
                    }
                }
                if (flag) weaps.push_back(pewpew);
            }
            else if (snowMap[y][x] == 'P') {
                weapPos.x = x * TILE_SIZEE;
                weapPos.y = (y + .9) * TILE_SIZEE;
                pistol.weapon.setPosition(weapPos.x, weapPos.y);
                for (ll i = 0;i < weaps.size();++i) {
                    if (abs(weaps[i].weapon.getPosition().x - pistol.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - pistol.weapon.getPosition().y) <= 20) {
                        flag = 0;
                    }
                }
                if (flag) weaps.push_back(pistol);
            }
        }
    }
    
    SPAWN.restart();
}
void initBackground()
{
    ground.loadFromFile("img/Lv7nNS.png");
    background.setTexture(ground);
    background.setScale(1.7f, 1.7f);
    background.setPosition(0.f, 0.f);
}
void init_Map4()
{
    initBackground();
    tileCount = 0;  // track how many tiles we've added
    weapCount = 0;
    DUCK_SCALE = 2.f;
    GUN_SCALE = 1.6f;
    fact = 3.f;
    gravity = 0.4;
    MaxiVelocityY = 12.f;
    jumpSpeed = -10.5f;  //give it negative value
    velocityX = 3.5f;   // the duck speed
    weapsVelocity = 7;

    // SET DUCKS POSISTIONS 
    for (int y = 0; y < 23; y++) {
        for (int x = 0; x < snowMap[y].length(); x++) {
            if (snowMap[y][x] == '1') {
                duck1posx = x * TILE_SIZEE;
                duck1posy = (y - 1.2) * TILE_SIZEE;
            }
            else if (snowMap[y][x] == '2') {
                duck2posx = x * TILE_SIZEE;
                duck2posy = (y - 1.2) * TILE_SIZEE;
            }
        }
    }

    init();
    // pistol
    pistol.fix_X = -4.f;
    pistol.fix_Y = -23.f;
    pistol.fix_hold_x = 25.f;
    pistol.fix_hold_y = -12.f;
    pistol.rev = 10.f;
    //sniper
    sniper.fix_X = 17.f;
    sniper.fix_Y = -18.f;
    sniper.fix_hold_x = 14.f;
    sniper.fix_hold_y = -16.f;
    sniper.rev = 23.f;
    //pewpew
    pewpew.fix_X = 7.f;
    pewpew.fix_Y = -13.f;
    pewpew.fix_hold_x = 18.f;
    pewpew.fix_hold_y = -15.f;
    pewpew.rev = 17.f;
    //sword
    sword.fix_hold_x = 38.f;
    sword.fix_hold_y = -14.f;
    sword.rev = 2.f;
    //bull1
    bull[1].scalex = 0.048f;
    bull[1].scaley = 0.048f;
    //bull2
    bull[2].scalex = 0.16f;
    bull[2].scaley = 0.16f;
    //grave
    Grave.setScale(0.107f, 0.067f);
    Grave.setOrigin(0, Grave.getLocalBounds().height);

    SPAWN4();

    ice.loadFromFile("img/iceBlocks.png");
    snow.loadFromFile("img/snowBlocks.png");
    wood.loadFromFile("img/woodBlocks.png");

    int scaleX = 0, scaleY = 0;
    bool isSnow = false;

    for (int y = 0; y < sizeof(snowMap) / sizeof(snowMap[0]); y++) {
        for (int x = 0; x < snowMap[y].length(); x++) {
            if (tileCount >= 1000) break;
            char tileChar = snowMap[y][x];
            bool isWood = false;
            // determine which part of the texture to use
            IntRect texRect;
            bool valid = true;

            switch (tileChar) {
            case '_':
                scaleX = 16;
                scaleY = 14;
                isSnow = true;
                texRect = IntRect(63, 0, scaleX, scaleY);
                break;
            case '+':
                scaleX = 31;
                scaleY = 14;
                isSnow = false;
                texRect = IntRect(0, 0, scaleX, scaleY);
                break;
            case '=':
                scaleX = 12;
                scaleY = 9;
                isWood = true;
                texRect = IntRect(0, 48, scaleX, scaleY);
                break;
            case '~':
                scaleX = 12;
                scaleY = 9;
                isWood = true;
                texRect = IntRect(4, 32, scaleX, scaleY);
                break;
            case '>':
                scaleX = 15;
                scaleY = 9;
                isWood = true;
                texRect = IntRect(0, 48, scaleX, scaleY);
                break;
            case '<':
                scaleX = 15;
                scaleY = 9;
                isWood = true;
                texRect = IntRect(0, 32, scaleX, scaleY);
                break;
            case '|':
                scaleX = 12;
                scaleY = 14;
                isSnow = false;
                texRect = sf::IntRect(34, 95, scaleX, scaleY);
                break;
            case '.':
                break;
            case '1':
                break;
            case '2':
                break;
            case '*':
                break;
            case 'S':
                break;
            case 'W':
                break;
            case 'E':
                break;
            case 'P':
                break;
            default:
                valid = false;
                break;
            }

            if (valid) {
                if (isSnow) mapTiles[tileCount].tileSprite.setTexture(snow);
                else if (isWood) mapTiles[tileCount].tileSprite.setTexture(wood);
                else mapTiles[tileCount].tileSprite.setTexture(ice);
                mapTiles[tileCount].tileSprite.setTextureRect(texRect);
                mapTiles[tileCount].tileSprite.setScale(
                    static_cast<float>(TILE_SIZEE) / scaleX,  // scale X down to 32px
                    static_cast<float>(TILE_SIZEE) / scaleY   // scale Y down to 32px
                );
                mapTiles[tileCount].tileSprite.setPosition(x * TILE_SIZEE, y * TILE_SIZEE);
                if (tileChar == '.' || tileChar == '1' || tileChar == '2' || tileChar == '*' ||
                    tileChar == 'S' || tileChar == 'P' || tileChar == 'E' || tileChar == 'W')
                {
                    mapTiles[tileCount].tileSprite.setScale(
                        static_cast<float>(TILE_SIZEE),  // scale X down to 32px
                        static_cast<float>(TILE_SIZEE)    // scale Y down to 32px
                    );
                    mapTiles[tileCount].isCollidable = false;
                }
                else mapTiles[tileCount].isCollidable = true;
                mapTiles[tileCount].bounds = mapTiles[tileCount].tileSprite.getGlobalBounds();
                if (tileChar == '.' || tileChar == '1' || tileChar == '2' || tileChar == '*' ||
                    tileChar == 'S' || tileChar == 'P' || tileChar == 'E' || tileChar == 'W') {
                    mapTiles[tileCount].bounds.width = 32;
                    mapTiles[tileCount].bounds.height = 32;
                }
                if(mapTiles[tileCount].isCollidable) tileCount++;
            }
        }
    }
}
void collision_Map4(ducks& duck)
{
    FloatRect duckBound = duck.myduck.getGlobalBounds();
    if (duck.facingRight) {
        duckBound.left += 16;
        duckBound.top += 19;
        duckBound.width -= 37;
        duckBound.height -= 19;
    }
    else {
        duckBound.left += 22;
        duckBound.top += 19;
        duckBound.width -= 37;
        duckBound.height -= 19;
    }

    duck.onGround = false; // to make duck fall when walk on unCollidable block

    for (int i = 0; i < tileCount; i++) {
        if (!mapTiles[i].isCollidable) continue;
        else {
            if (duckBound.intersects(mapTiles[i].bounds)) {
                FloatRect intersection;
                duckBound.intersects(mapTiles[i].bounds, intersection);
                if (intersection.width < intersection.height - 4) {
                    if (duckBound.left < mapTiles[i].bounds.left) {
                        // Right collision
                        duck.myduck.setPosition(
                            mapTiles[i].bounds.left - duckBound.width - (duck.facingRight ? 16 : 22),
                            duck.myduck.getPosition().y
                        );
                    }
                    else {
                        // Left collision
                        duck.myduck.setPosition(
                            mapTiles[i].bounds.left + mapTiles[i].bounds.width - (duck.facingRight ? 16 : 22),
                            duck.myduck.getPosition().y
                        );
                    }
                }
                else if (intersection.width - 4 > intersection.height) { // Vertical collision
                    if (duckBound.top < mapTiles[i].bounds.top) {
                        if (duck.velocityY < 0) {
                            continue;
                        }
                        // Landing on ground (from above)
                        duck.myduck.setPosition(
                            duck.myduck.getPosition().x,
                            mapTiles[i].tileSprite.getPosition().y + 1
                        );
                        duck.onGround = true;
                        duck.isJumping = false;
                        duck.velocityY = 0;
                    }
                    else {
                        if (duck.velocityY > 0) {
                            continue;
                        }
                        // Hitting ceiling (from below)
                        duck.myduck.setPosition(
                            duck.myduck.getPosition().x,
                            mapTiles[i].bounds.top + mapTiles[i].bounds.height + 46
                        );
                        duck.velocityY = 0;
                    }
                }
            }
        }
    }
    duck.myarm.arm.setPosition(
        duck.myduck.getPosition().x + duck.myduck.getGlobalBounds().width / 6,
        duck.myduck.getPosition().y - duck.myduck.getGlobalBounds().height / 2 - fact
    );
    duck.myweap.weapon.setPosition(
        duck.myduck.getPosition().x + duck.myweap.fix_hold_x,
        duck.myduck.getPosition().y + duck.myweap.fix_hold_y
    );
}
void weap_collision_Map4(weapons& Gun) {
    FloatRect weaponBounds = Gun.weapon.getGlobalBounds();
    Gun.onGround = false;
    for (int i = 0; i < tileCount; i++) {
        if (!mapTiles[i].isCollidable) continue;
        else {
            if (weaponBounds.intersects(mapTiles[i].bounds)) {
                FloatRect intersection;
                weaponBounds.intersects(mapTiles[i].bounds, intersection);
                if (intersection.width < intersection.height) {
                    if (weaponBounds.left < mapTiles[i].bounds.left) {
                        // Right collision
                        Gun.velocityX = 0;
                        Gun.weapon.setPosition(
                            Gun.weapon.getPosition().x - intersection.width - 1,
                            Gun.weapon.getPosition().y
                        );
                    }
                    else if (weaponBounds.left > mapTiles[i].bounds.left) {
                        // Left collision
                        Gun.velocityX = 0;
                        Gun.weapon.setPosition(
                            Gun.weapon.getPosition().x + intersection.width + 1, // +16
                            Gun.weapon.getPosition().y
                        );
                    }
                }
                else if (intersection.width > intersection.height) { // Vertical collision                    
                    if (weaponBounds.top < mapTiles[i].bounds.top) {
                        // Landing on ground (from above)
                        Gun.weapon.setPosition(
                            Gun.weapon.getPosition().x,
                            mapTiles[i].tileSprite.getPosition().y + 1 // adjust accurate positioning
                        );
                        Gun.onGround = true;
                        Gun.velocityY = 0;
                        Gun.velocityX = 0;
                    }
                    else {
                        // Hitting ceiling (from below)
                        Gun.weapon.setPosition(
                            Gun.weapon.getPosition().x,
                            mapTiles[i].bounds.top + mapTiles[i].bounds.height + 47
                        );
                        Gun.velocityY = 0;
                    }
                }
            }
        }
    }

}
void bullet_collision4() {
    for (int i = bulls.size() - 1;i >= 0;--i) {
        for (int j = 0; j < tileCount; j++) {
            if (!mapTiles[j].isCollidable) continue;
            else {
                if (bulls[i].bullet.getGlobalBounds().intersects(mapTiles[j].bounds)) {
                    bulls.erase(i);
                    break;
                }
            }
        }
    }
}
void update_Map4() {
    update_Logic();
    for (int i = 0;i < weaps.size();++i) {
        weapons& weap = weaps[i];
        weap_collision_Map4(weap);
    }
    collision_Map4(duck1);
    collision_Map4(duck2);
    bullet_collision4();
    if (SPAWN.getElapsedTime().asSeconds() >= 60) {
        SPAWN4();
    }
}
void draw4() {
    window.draw(background);
    for (int i = 0; i < tileCount; i++) {
        window.draw(mapTiles[i].tileSprite);
    }
    //FloatRect weaponBounds = pewpew.weapon.getGlobalBounds();
    //RectangleShape test;
    //test.setSize(Vector2f(weaponBounds.width, weaponBounds.height));
    //test.setFillColor(Color::Red);
    //test.setPosition(weaponBounds.left, weaponBounds.top);  // Use the modified position
    //window.draw(test);
    draw_Logic();
}
void Map4() {
    update_Map4();
    draw4();
}


//Hekal
Texture BackgroundTexture;
Texture blockTexture;
Texture groundTexture;
Sprite blockse[9];
Sprite BackgroundSprite;
RectangleShape player1_colliderh(Vector2f(27, 50)), player2_colliderh(Vector2f(27, 50));
RectangleShape pistol_colliderh(Vector2f(25, 20)), sniper_colliderh(Vector2f(55, 20)), pewpew_colliderh(Vector2f(45, 25)), swrd_colliderh(Vector2f(40, 20));

void Bullet_Collision5() {
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 9; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(blockse[j].getGlobalBounds()))
            {

                bulls.erase(i);
                break;
            }
        }
    }
}
void blocks5() {
    blockse[0].setPosition(155, 520);
    blockse[0].setScale(2.25, 1);
    blockse[1].setPosition(25, 320);
    blockse[1].setScale(2, 1);
    blockse[2].setPosition(415, 390);
    blockse[2].setScale(4.5, 1);
    blockse[3].setPosition(55, 90);
    blockse[3].setScale(2, 1);
    blockse[4].setPosition(395, 175);
    blockse[4].setScale(1.7, 1);
    blockse[5].setPosition(985, 300);
    blockse[5].setScale(2, 1);
    blockse[6].setPosition(985, 90);
    blockse[6].setScale(2, 1);
    blockse[7].setPosition(645, 175);
    blockse[7].setScale(1.7, 1);
    blockse[8].setPosition(845, 520);
    blockse[8].setScale(2.25, 1);
}
void collision_Map5(RectangleShape& player_collider, ducks& duck)
{
    FloatRect box, wall, intersection;
    box = player_collider.getGlobalBounds();
    for (int i = 0;i < 9;i++)
    {
        wall = blockse[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, blockse[i].getPosition().y + 1);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
}
void collision_weaps_Map5( weapons& weap)
{
    FloatRect box, wall, intersection;
    box = weap.weapon.getGlobalBounds();
    for (int i = 0;i < 9;i++)
    {
        wall = blockse[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, blockse[i].getPosition().y + 1.f);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                }
            }
            else
            {
                weap.velocityX = 0.f;
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
}
void SPAWN5() {

    sniper_collider.setOrigin(sniper_collider.getLocalBounds().width / 2, sniper_collider.getLocalBounds().height / 2);
    pistol_collider.setOrigin(pistol_collider.getLocalBounds().width / 2, pistol_collider.getLocalBounds().height / 2);
    pewpew_collider.setOrigin(pewpew_collider.getLocalBounds().width / 2, pewpew_collider.getLocalBounds().height / 2);
    swrd_collider.setOrigin(swrd_collider.getLocalBounds().width / 2, swrd_collider.getLocalBounds().height / 2);
    sniper.weapon.setPosition(200, 510);
    pistol.weapon.setPosition(850, 520);
    pewpew.weapon.setPosition(750, 175);
    sword.weapon.setPosition(400, 175);
    //grenade.weapon.setPosition(350, 175);
    pistol.collider = pistol_colliderh;
    sniper.collider = sniper_colliderh;
    pewpew.collider = pewpew_colliderh;
    sword.collider = swrd_colliderh;

    //spawn weaps
    bool flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - pistol.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - pistol.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(pistol);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - sniper.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - sniper.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(sniper);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - pewpew.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - pewpew.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(pewpew);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - sword.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - sword.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(sword);
    SPAWN.restart();
}
void init_Map5() {
    DUCK_SCALE = 2.f;
    GUN_SCALE = 1.6f;
    fact = 3.f;
    gravity = 0.5;
    jumpSpeed = -14;  //give it negative value
    velocityX = 4.f;   // the duck speed
    weapsVelocity = 7;
    init();
    // pistol
    pistol.fix_X = -4.f;
    pistol.fix_Y = -23.f;
    pistol.fix_hold_x = 25.f;
    pistol.fix_hold_y = -12.f;
    pistol.rev = 10.f;
    // grenade
    grenade.fix_hold_x = 25.f;
    grenade.fix_hold_y = -12.f;
    grenade.rev = 6.f;
    //sniper
    sniper.fix_X = 17.f;
    sniper.fix_Y = -18.f;
    sniper.fix_hold_x = 14.f;
    sniper.fix_hold_y = -16.f;
    sniper.rev = 23.f;
    //pewpew
    pewpew.fix_X = 7.f;
    pewpew.fix_Y = -13.f;
    pewpew.fix_hold_x = 18.f;
    pewpew.fix_hold_y = -15.f;
    pewpew.rev = 17.f;
    //sword
    sword.fix_hold_x = 38.f;
    sword.fix_hold_y = -14.f;
    sword.rev = 2.f;
    //bull1
    bull[1].scalex = 0.048f;
    bull[1].scaley = 0.048f;
    //bull2
    bull[2].scalex = 0.16f;
    bull[2].scaley = 0.16f;
    //grave
    Grave.setScale(0.107f, 0.067f);
    Grave.setOrigin(0, Grave.getLocalBounds().height);



    BackgroundTexture.loadFromFile("img/nature.png");
    blockTexture.loadFromFile("img/blocks.png");
    blocks5();
    for (int i = 0;i < 9;i++) {
        blockse[i].setTexture(blockTexture);
    }
    BackgroundSprite.setTexture(BackgroundTexture);
    Vector2f windowSize = static_cast<sf::Vector2f>(window.getSize());
    Vector2f textureSize = static_cast<sf::Vector2f>(BackgroundTexture.getSize());
    BackgroundSprite.setScale(2.3, 2);
    player1_colliderh.setOrigin(player1_colliderh.getLocalBounds().width / 2, player1_colliderh.getLocalBounds().height / 2);
    player2_colliderh.setOrigin(player2_colliderh.getLocalBounds().width / 2, player2_colliderh.getLocalBounds().height / 2);
    duck1.myduck.setPosition(30, 320);
    duck2.myduck.setPosition(1030, 90);
    SPAWN5();

}
void update_Map5() {
    duck1.onGround = 0;
    duck2.onGround = 0;
    player1_colliderh.setPosition(duck1.myduck.getPosition().x + 32, duck1.myduck.getPosition().y - 25);
    player2_colliderh.setPosition(duck2.myduck.getPosition().x + 32, duck2.myduck.getPosition().y - 25);
    for (int i = 0;i < weaps.size();i++)
    {
        if (weaps[i].type == "pistol")
        {
            weaps[i].collider.setPosition(weaps[i].weapon.getPosition().x, weaps[i].weapon.getPosition().y - 20);
        }
        else if (weaps[i].type == "sniper")
        {
            weaps[i].collider.setPosition(weaps[i].weapon.getPosition().x, weaps[i].weapon.getPosition().y - 20);
        }
        else if (weaps[i].type == "pewpew")
        {
            weaps[i].collider.setPosition(weaps[i].weapon.getPosition().x, weaps[i].weapon.getPosition().y - 20);
        }
        else
        {
            weaps[i].collider.setPosition(weaps[i].weapon.getPosition().x, weaps[i].weapon.getPosition().y - 15);
        }
    }
    collision_Map5(player1_colliderh, duck1);
    collision_Map5(player2_colliderh, duck2);
    Bullet_Collision5();
    for (int i = 0;i < weaps.size();i++)
    {
        collision_weaps_Map5(weaps[i]);
    }
    if (duck1.onGround)
    {
        duck1.isJumping = 0;
    }
    else
    {
        duck1.isJumping = 1;
    }
    if (duck2.onGround)
    {
        duck2.isJumping = 0;
    }
    else
    {
        duck2.isJumping = 1;
    }
    update_Logic();
    if (SPAWN.getElapsedTime().asSeconds() >= 60) {
        SPAWN5();
    }
}
void draw_Map5() {

    window.draw(BackgroundSprite);
    for (int i = 0;i < 9;i++)
        window.draw(blockse[i]);
    draw_Logic();
}
void Map5() {
    update_Map5();
    draw_Map5();
}

//khaled`s MAP 2
Sprite nblocks[12], pillars[15], right_edges[8], left_edges[8], bases[15], middle_blocks[10], boxes[8], green_barrel, red_barrel, fence1, fence2[3], fence3, doorlocked[2], switch6[2], spikes[12], acid1[5], acid2[4], acid_skel, lower_row[20], upper_row[10], rback_ground[8][10], acid_blocks[4];
Texture blocks_texture6, pillar_texture6, right_edges_texture6, left_edges_texture6, bases_texture6, middle_blocks_texture6, base_redge6, base_ledge6, boxes_texture6, red_barrel_texture, green_barrel_texture, fence1_texture6, fence2_texture6, fence3_texture6, doorlocked_texture6, switch_texture6, spikes_texture6, acid1_texture6, acid2_texture6, acid_skel_texture6, lower_row_texture, lupper_row_texture, upper_row_texture, rbackground_texture1, rbackground_texture2, acid_blocks_texture6;
RectangleShape player1_collider6(Vector2f(25, 40)), player2_collider62(Vector2f(25, 40));
void SPAWN6() {
    sniper_collider.setOrigin(sniper_collider.getLocalBounds().width / 2, sniper_collider.getLocalBounds().height / 2);
    pistol_collider.setOrigin(pistol_collider.getLocalBounds().width / 2, pistol_collider.getLocalBounds().height / 2);
    pewpew_collider.setOrigin(pewpew_collider.getLocalBounds().width / 2, pewpew_collider.getLocalBounds().height / 2);
    swrd_collider.setOrigin(swrd_collider.getLocalBounds().width / 2, swrd_collider.getLocalBounds().height / 2);
    sniper.weapon.setPosition(320.f, 110);
    pistol.weapon.setPosition(625.f, 420);
    grenade.weapon.setPosition(640.f, 80);
    pewpew.weapon.setPosition(620.f, 160);
    sword.weapon.setPosition(920.f, 110);
    pistol.collider = pistol_collider;
    sniper.collider = sniper_collider;
    pewpew.collider = pewpew_collider;
    sword.collider = swrd_collider;
    //spawn weaps
    bool flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - pistol.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - pistol.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(pistol);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - sniper.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - sniper.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(sniper);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - grenade.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - grenade.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(grenade);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - pewpew.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - pewpew.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(pewpew);
    flag = 1;
    for (ll i = 0;i < weaps.size();++i) {
        if (abs(weaps[i].weapon.getPosition().x - sword.weapon.getPosition().x) <= 20 && abs(weaps[i].weapon.getPosition().y - sword.weapon.getPosition().y) <= 20) {
            flag = 0;
        }
    }
    if (flag) weaps.push_back(sword);
    SPAWN.restart();
}
void set_grounds()
{
    nblocks[0].setPosition(340, 600);
    nblocks[0].setScale(0.4, 0.2);
    nblocks[1].setPosition(440, 600);
    nblocks[1].setScale(0.4, 0.2);
    nblocks[2].setPosition(540, 600);
    nblocks[2].setScale(0.4, 0.2);
    nblocks[3].setPosition(640, 600);
    nblocks[3].setScale(0.4, 0.2);
    nblocks[4].setPosition(740, 600);
    nblocks[4].setScale(0.4, 0.2);
    nblocks[5].setPosition(840, 600);
    nblocks[5].setScale(0.4, 0.2);
    nblocks[6].setPosition(340, 330);
    nblocks[6].setScale(0.4, 0.15);
    nblocks[7].setPosition(840, 330);
    nblocks[7].setScale(0.4, 0.15);
    nblocks[8].setPosition(440, 93);
    nblocks[8].setScale(0.4, 0.15);
    nblocks[9].setPosition(540, 93);
    nblocks[9].setScale(0.4, 0.15);
    nblocks[10].setPosition(640, 93);
    nblocks[10].setScale(0.4, 0.15);
    nblocks[11].setPosition(740, 93);
    nblocks[11].setScale(0.4, 0.15);
}
void set_left_edges()
{
    left_edges[0].setPosition(240, 600);
    left_edges[0].setScale(0.4, 0.2);
    left_edges[1].setPosition(345, 472);
    left_edges[1].setScale(0.2, 0.15);
    left_edges[2].setPosition(985, 530);
    left_edges[2].setScale(0.2, 0.15);
    left_edges[3].setPosition(140, 472);
    left_edges[3].setScale(0.45, 0.15);
    left_edges[4].setPosition(220, 330);
    left_edges[4].setScale(0.5, 0.15);
    left_edges[5].setPosition(190, 370);
    left_edges[5].setScale(0.2, 0.15);
    left_edges[6].setPosition(200, 140);
    left_edges[6].setScale(0.2, 0.15);
}
void set_right_edges()
{
    right_edges[0].setPosition(940, 600);
    right_edges[0].setScale(0.4, 0.2);
    right_edges[1].setPosition(890, 472);
    right_edges[1].setScale(0.2, 0.15);
    right_edges[2].setPosition(250, 530);
    right_edges[2].setScale(0.2, 0.15);
    right_edges[3].setPosition(1030, 472);
    right_edges[3].setScale(0.45, 0.15);
    right_edges[4].setPosition(940, 330);
    right_edges[4].setScale(0.5, 0.15);
    right_edges[5].setPosition(1045, 370);
    right_edges[5].setScale(0.2, 0.15);
    right_edges[6].setPosition(1030, 140);
    right_edges[6].setScale(0.2, 0.15);
}
void set_pillars()
{
    pillars[0].setPosition(250, 472);
    pillars[0].setScale(0.02, 0.5);
    pillars[1].setPosition(1030, 472);
    pillars[1].setScale(0.02, 0.5);
    pillars[2].setPosition(630, 550);
    pillars[2].setScale(0.07, 0.195);
    pillars[3].setPosition(390, 472);
    pillars[3].setScale(0.02, 0.3);
    pillars[4].setPosition(890, 472);
    pillars[4].setScale(0.02, 0.3);
    pillars[5].setPosition(1045, 348);
    pillars[5].setScale(0.02, 0.3);
    pillars[6].setPosition(235, 348);
    pillars[6].setScale(0.02, 0.3);
    /*pillars[7].setPosition(390, 280);   //not pillar
    pillars[7].setScale(0.2, 0.2);
    pillars[8].setPosition(840, 280);   //not pillar
    pillars[8].setScale(0.2, 0.2);
    pillars[9].setPosition(390, 230);   //not pillar
    pillars[9].setScale(0.2, 0.2);
    pillars[10].setPosition(840, 230);   //not pillar
    pillars[10].setScale(0.2, 0.2);*/
    pillars[7].setPosition(250, 130);
    pillars[7].setScale(0.02, 0.4);
    pillars[8].setPosition(1030, 130);
    pillars[8].setScale(0.02, 0.4);
}
void set_base()
{
    bases[0].setPosition(390, 538);
    bases[0].setScale(0.4, 0.1);
    bases[1].setPosition(490, 538);
    bases[1].setScale(0.4, 0.1);
    bases[2].setPosition(590, 538);
    bases[2].setScale(0.4, 0.1);
    bases[3].setPosition(690, 538);
    bases[3].setScale(0.4, 0.1);
    bases[4].setPosition(790, 538);
    bases[4].setScale(0.4, 0.1);
    bases[5].setPosition(440, 330);
    bases[5].setScale(0.4, 0.15);
    bases[6].setPosition(540, 330);
    bases[6].setScale(0.4, 0.15);
    bases[7].setPosition(640, 330);
    bases[7].setScale(0.4, 0.15);
    bases[8].setPosition(740, 330);
    bases[8].setScale(0.4, 0.15);
    bases[9].setPosition(240, 110);
    bases[9].setScale(0.4, 0.15);
    bases[10].setPosition(340, 110);
    bases[10].setScale(0.45, 0.15);
    bases[11].setPosition(830, 110);
    bases[11].setScale(0.45, 0.15);
    bases[12].setPosition(940, 110);
    bases[12].setScale(0.4, 0.15);

}
void set_middle_blocks()
{
    middle_blocks[0].setPosition(460, 450);
    middle_blocks[0].setScale(0.2, 0.1);
    middle_blocks[1].setPosition(770, 450);
    middle_blocks[1].setScale(0.2, 0.1);
    middle_blocks[2].setPosition(615, 470);
    middle_blocks[2].setScale(0.2, 0.1);
    middle_blocks[3].setPosition(573, 210);
    middle_blocks[3].setScale(0.5, 0.1);
    middle_blocks[4].setPosition(80, 300);
    middle_blocks[4].setScale(0.3, 0.1);
    middle_blocks[5].setPosition(10, 230);
    middle_blocks[5].setScale(0.2, 0.1);
    middle_blocks[6].setPosition(100, 180);
    middle_blocks[6].setScale(0.2, 0.1);
    middle_blocks[7].setPosition(1125, 300);
    middle_blocks[7].setScale(0.3, 0.1);
    middle_blocks[8].setPosition(1220, 230);
    middle_blocks[8].setScale(0.2, 0.1);
    middle_blocks[9].setPosition(1145, 180);
    middle_blocks[9].setScale(0.2, 0.1);

}
void set_boxes()
{
    boxes[0].setPosition(140, 435);
    boxes[0].setScale(0.15, 0.15);
    boxes[1].setPosition(1110, 435);
    boxes[1].setScale(0.15, 0.15);
    boxes[2].setPosition(335, 280);
    boxes[2].setScale(0.2, 0.2);
    boxes[3].setPosition(895, 280);
    boxes[3].setScale(0.2, 0.2);
    boxes[4].setPosition(80, 262);
    boxes[4].setScale(0.15, 0.15);
    boxes[5].setPosition(1165, 262);
    boxes[5].setScale(0.15, 0.15);
    boxes[6].setPosition(310, 575);
    boxes[6].setScale(0.1, 0.1);
    boxes[7].setPosition(950, 575);
    boxes[7].setScale(0.1, 0.1);
}
void set_decorations()
{
    red_barrel.setPosition(610, 22);
    red_barrel.setScale(0.3, 0.3);
    green_barrel.setPosition(620, 162);
    green_barrel.setScale(0.2, 0.2);
    fence1.setPosition(450, 17);
    fence1.setScale(0.3, 0.3);
    fence2[0].setPosition(527, 17);
    fence2[0].setScale(0.3, 0.3);
    fence2[1].setPosition(604, 17);
    fence2[1].setScale(0.3, 0.3);
    fence2[2].setPosition(681, 17);
    fence2[2].setScale(0.3, 0.3);
    fence3.setPosition(758, 17);
    fence3.setScale(0.3, 0.3);
    doorlocked[0].setPosition(590, 554);
    doorlocked[0].setScale(0.1, 0.1);
    doorlocked[1].setPosition(660, 554);
    doorlocked[1].setScale(0.1, 0.1);
    switch6[0].setPosition(570, 562);
    switch6[0].setScale(0.15, 0.15);
    switch6[1].setPosition(700, 562);
    switch6[1].setScale(0.15, 0.15);
}
void set_spikes()
{
    spikes[0].setPosition(395, 505);
    spikes[0].setScale(0.35, 0.2);
    spikes[1].setPosition(480, 505);
    spikes[1].setScale(0.35, 0.2);
    spikes[2].setPosition(630, 505);
    spikes[2].setScale(-0.35, 0.2);
    spikes[3].setPosition(630, 505);
    spikes[3].setScale(0.35, 0.2);
    spikes[4].setPosition(780, 505);
    spikes[4].setScale(-0.35, 0.2);
    spikes[5].setPosition(785, 505);
    spikes[5].setScale(0.4, 0.2);
}
void set_acid()
{
    acid1[0].setPosition(440, 240);
    acid1[0].setScale(0.3, 0.2);
    acid2[0].setPosition(440, 277);
    acid2[0].setScale(0.3, 0.21);
    acid1[1].setPosition(517, 240);
    acid1[1].setScale(0.3, 0.2);
    acid2[1].setPosition(517, 277);
    acid2[1].setScale(0.3, 0.21);
    acid1[2].setPosition(671, 240);
    acid1[2].setScale(-0.3, 0.2);
    acid2[2].setPosition(671, 277);
    acid2[2].setScale(-0.3, 0.21);
    acid1[3].setPosition(671, 240);
    acid1[3].setScale(0.3, 0.2);
    acid_skel.setPosition(671, 265);
    acid_skel.setScale(0.3, 0.3);
    acid1[4].setPosition(748, 240);
    acid1[4].setScale(0.36, 0.2);
    acid2[3].setPosition(748, 277);
    acid2[3].setScale(0.36, 0.21);
}
void set_acid_blocks()
{
    acid_blocks[0].setPosition(390, 280);
    acid_blocks[0].setScale(0.2, 0.2);
    acid_blocks[1].setPosition(840, 280);
    acid_blocks[1].setScale(0.2, 0.2);
    acid_blocks[2].setPosition(390, 230);
    acid_blocks[2].setScale(0.2, 0.2);
    acid_blocks[3].setPosition(840, 230);
    acid_blocks[3].setScale(0.2, 0.2);
}
void set_lower_row()
{
    lower_row[0].setPosition(0, 648);
    lower_row[0].setScale(0.5, 0.28);
    lower_row[1].setPosition(128, 648);
    lower_row[1].setScale(0.5, 0.28);
    lower_row[2].setPosition(256, 648);
    lower_row[2].setScale(0.5, 0.28);
    lower_row[3].setPosition(384, 648);
    lower_row[3].setScale(0.5, 0.28);
    lower_row[4].setPosition(512, 648);
    lower_row[4].setScale(0.5, 0.28);
    lower_row[5].setPosition(640, 648);
    lower_row[5].setScale(0.5, 0.28);
    lower_row[6].setPosition(768, 648);
    lower_row[6].setScale(0.5, 0.28);
    lower_row[7].setPosition(896, 648);
    lower_row[7].setScale(0.5, 0.28);
    lower_row[8].setPosition(1024, 648);
    lower_row[8].setScale(0.5, 0.28);
    lower_row[9].setPosition(1152, 648);
    lower_row[9].setScale(0.5, 0.28);
}
void set_rest()
{
    for (int i = 0;i < 8;i++)
    {
        for (int j = 0;j < 10;j++)
        {
            rback_ground[i][j].setPosition((j * 128), (1 + i) * 72);
            rback_ground[i][j].setScale(0.5, 0.28);
        }
    }
}
void set_upper_row()
{
    upper_row[0].setPosition(0, 0);
    upper_row[0].setScale(0.5, 0.28);
    upper_row[1].setPosition(128, 0);
    upper_row[1].setScale(0.5, 0.28);
    upper_row[2].setPosition(256, 0);
    upper_row[2].setScale(0.5, 0.28);
    upper_row[3].setPosition(384, 0);
    upper_row[3].setScale(0.5, 0.28);
    upper_row[4].setPosition(512, 0);
    upper_row[4].setScale(0.5, 0.28);
    upper_row[5].setPosition(640, 0);
    upper_row[5].setScale(0.5, 0.28);
    upper_row[6].setPosition(768, 0);
    upper_row[6].setScale(0.5, 0.28);
    upper_row[7].setPosition(896, 0);
    upper_row[7].setScale(0.5, 0.28);
    upper_row[8].setPosition(1024, 0);
    upper_row[8].setScale(0.5, 0.28);
    upper_row[9].setPosition(1152, 0);
    upper_row[9].setScale(0.5, 0.28);
}
void set_background()
{
    set_lower_row();
    set_upper_row();
    set_rest();
}
void collision_Map6(RectangleShape& player_collider, ducks& duck)
{
    FloatRect box, wall, intersection;
    box = player_collider.getGlobalBounds();
    for (int i = 0;i < 12;i++)
    {
        wall = nblocks[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, nblocks[i].getPosition().y);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 7;i++)
    {
        wall = left_edges[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, left_edges[i].getPosition().y);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 7;i++)
    {
        wall = right_edges[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, right_edges[i].getPosition().y);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }

    for (int i = 0;i < 4;i++)
    {
        wall = acid_blocks[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, acid_blocks[i].getPosition().y);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 9;i++)
    {
        wall = pillars[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, pillars[i].getPosition().y);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 13;i++)
    {
        wall = bases[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, bases[i].getPosition().y);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 10;i++)
    {
        wall = middle_blocks[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, middle_blocks[i].getPosition().y);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 8;i++)
    {
        wall = boxes[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    if (duck.velocityY < 0) {
                        continue;
                    }
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, boxes[i].getPosition().y);
                }
                else
                {
                    if (duck.velocityY > 0) {
                        continue;
                    }
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x - intersection.width, duck.myduck.getPosition().y);
                }
                else
                {
                    duck.myduck.setPosition(duck.myduck.getPosition().x + intersection.width, duck.myduck.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 5;i++)
    {
        wall = acid1[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            duck.dead = true;
            GameEnd = 1;
        }
    }
    for (int i = 0;i < 6;i++)
    {
        wall = spikes[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            duck.dead = true;
            GameEnd = 1;
        }
    }
}
void collision_weaps_Map6(weapons& weap)
{
    if (weap.type == "grenade" && weap.boom) return;
    FloatRect box, wall, intersection;
    box = weap.weapon.getGlobalBounds();

    for (int i = 0;i < 4;i++)
    {
        wall = acid_blocks[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, nblocks[i].getPosition().y + 1);
                }
                else
                {
                    weap.velocityY = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }

    for (int i = 0;i < 12;i++)
    {
        wall = nblocks[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, nblocks[i].getPosition().y + 1);
                }
                else
                {
                    weap.velocityY = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 7;i++)
    {
        wall = left_edges[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, left_edges[i].getPosition().y + 1);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 7;i++)
    {
        wall = right_edges[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, right_edges[i].getPosition().y + 1);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 13;i++)
    {
        wall = pillars[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, pillars[i].getPosition().y + 1);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 13;i++)
    {
        wall = bases[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, bases[i].getPosition().y + 1);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 10;i++)
    {
        wall = middle_blocks[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, middle_blocks[i].getPosition().y + 1);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
    for (int i = 0;i < 8;i++)
    {
        wall = boxes[i].getGlobalBounds();
        if (box.intersects(wall))
        {
            box.intersects(wall, intersection);
            if (intersection.width > intersection.height)
            {
                if (box.top < wall.top)
                {
                    weap.onGround = true;
                    weap.velocityY = 0.f;
                    weap.velocityX = 0.f;
                    weap.weapon.setPosition(weap.weapon.getPosition().x, boxes[i].getPosition().y + 1);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x, weap.weapon.getPosition().y + intersection.height);
                }
            }
            else
            {
                if (box.left < wall.left)
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x - intersection.width, weap.weapon.getPosition().y);
                }
                else
                {
                    weap.weapon.setPosition(weap.weapon.getPosition().x + intersection.width, weap.weapon.getPosition().y);
                }
            }
        }
    }
}
void Bullet_Collision6() {

    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 4; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(acid_blocks[j].getGlobalBounds()))
            {

                bulls.erase(i);
                break;
            }
        }
    }
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 12; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(nblocks[j].getGlobalBounds()))
            {

                bulls.erase(i);
                break;
            }
        }
    }
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 7; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(right_edges[j].getGlobalBounds()))
            {

                bulls.erase(i);
                break;
            }
        }
    }
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 7; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(left_edges[j].getGlobalBounds()))
            {

                bulls.erase(i);
                break;
            }
        }
    }
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 13; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(pillars[j].getGlobalBounds()))
            {

                bulls.erase(i);
                break;
            }
        }
    }
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 13; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(bases[j].getGlobalBounds()))
            {

                bulls.erase(i);
                break;
            }
        }
    }
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 10; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(middle_blocks[j].getGlobalBounds()))
            {

                bulls.erase(i);
                break;
            }
        }
    }
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 8; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(boxes[j].getGlobalBounds()))
            {

                bulls.erase(i);
                break;
            }
        }
    }
}
void init_Map6()
{
    DUCK_SCALE = 1.7f;
    GUN_SCALE = 1.364f;
    fact = 3.4f;
    gravity = 0.272f;
    jumpSpeed = -6.12f;
    velocityX = 3.4;
    weapsVelocity = 5.f;
    init();
    // pistol
    pistol.fix_X = -3.f;
    pistol.fix_Y = -19.f;
    pistol.fix_hold_x = 21.f;
    pistol.fix_hold_y = -10.9f;
    pistol.rev = 10.f;
    // grenade
    grenade.fix_hold_x = 25.f;
    grenade.fix_hold_y = -10.f;
    grenade.rev = 4.f;
    //sniper
    sniper.fix_X = 17.f;
    sniper.fix_Y = -16.f;
    sniper.fix_hold_x = 12.f;
    sniper.fix_hold_y = -14.f;
    sniper.rev = 24.f;
    //pewpew
    pewpew.fix_X = 7.f;
    pewpew.fix_Y = -11.f;
    pewpew.fix_hold_x = 16.f;
    pewpew.fix_hold_y = -13.f;
    pewpew.rev = 18.f;
    //sword
    sword.fix_hold_x = 33.f;
    sword.fix_hold_y = -13.f;
    sword.rev = 1.f;
    //bull1
    bull[1].scalex = 0.041f;
    bull[1].scaley = 0.041f;
    //bull2
    bull[2].scalex = 0.136f;
    bull[2].scaley = 0.136f;
    //grave
    Grave.setScale(0.088f, 0.056f);
    Grave.setOrigin(0, Grave.getLocalBounds().height);
    blocks_texture6.loadFromFile("img/Tile (13).png");
    pillar_texture6.loadFromFile("img/Block 2 pill.png");
    right_edges_texture6.loadFromFile("img/Tile (14).png");
    left_edges_texture6.loadFromFile("img/Tile (12).png");
    bases_texture6.loadFromFile("img/Tile (10).png");
    middle_blocks_texture6.loadFromFile("img/Tile (15).png");
    base_redge6.loadFromFile("img/Tile (7).png");
    base_ledge6.loadFromFile("img/Tile (7).png");
    boxes_texture6.loadFromFile("img/Box.png");
    red_barrel_texture.loadFromFile("img/Barrel (1).png");
    green_barrel_texture.loadFromFile("img/Barrel (2).png");
    fence1_texture6.loadFromFile("img/Fence (1).png");
    fence2_texture6.loadFromFile("img/Fence (2).png");
    fence3_texture6.loadFromFile("img/Fence (3).png");
    doorlocked_texture6.loadFromFile("img/DoorLocked.png");
    switch_texture6.loadFromFile("img/Switch (2).png");
    spikes_texture6.loadFromFile("img/Spike.png");
    acid1_texture6.loadFromFile("img/Acid (1).png");
    acid2_texture6.loadFromFile("img/Acid (2).png");
    acid_skel_texture6.loadFromFile("img/acid_skel.png");
    lower_row_texture.loadFromFile("img/BGTile (7).png");
    upper_row_texture.loadFromFile("img/BGTile (2).png");
    lupper_row_texture.loadFromFile("img/BGTile (1).png");
    rbackground_texture1.loadFromFile("img/BGTile (5).png");
    rbackground_texture2.loadFromFile("img/BGTile (6).png");
    acid_blocks_texture6.loadFromFile("img/Tile (5).png");
    set_grounds();
    set_pillars();
    set_right_edges();
    set_left_edges();
    set_base();
    set_middle_blocks();
    set_boxes();
    set_decorations();
    set_spikes();
    set_acid();
    set_acid_blocks();
    set_background();
    player1_collider6.setOrigin(player1_collider.getLocalBounds().width / 2, player1_collider.getLocalBounds().height / 2);
    player2_collider62.setOrigin(player2_collider.getLocalBounds().width / 2, player2_collider.getLocalBounds().height / 2);
    duck1.myduck.setPosition(590, 600);
    duck2.myduck.setPosition(660, 600);
    SPAWN6();
}
void update_Map6()
{
    duck1.onGround = false;
    duck2.onGround = false;
    for (int i = 0;i < 13;i++) nblocks[i].setTexture(blocks_texture6);
    for (int i = 0;i < 15;i++) pillars[i].setTexture(pillar_texture6);
    for (int i = 0;i < 8;i++) left_edges[i].setTexture(left_edges_texture6);
    for (int i = 0;i < 8;i++) right_edges[i].setTexture(right_edges_texture6);
    for (int i = 0;i < 15;i++) bases[i].setTexture(bases_texture6);
    for (int i = 0;i < 10;i++) middle_blocks[i].setTexture(middle_blocks_texture6);
    for (int i = 0;i < 8;i++) boxes[i].setTexture(boxes_texture6);
    green_barrel.setTexture(green_barrel_texture);
    red_barrel.setTexture(red_barrel_texture);
    fence1.setTexture(fence1_texture6);
    for (int i = 0;i < 3;i++) fence2[i].setTexture(fence2_texture6);
    fence3.setTexture(fence3_texture6);
    for (int i = 0;i < 2;i++) doorlocked[i].setTexture(doorlocked_texture6);
    for (int i = 0;i < 2;i++) switch6[i].setTexture(switch_texture6);
    for (int i = 0;i < 10;i++) spikes[i].setTexture(spikes_texture6);
    for (int i = 0;i < 5;i++) acid1[i].setTexture(acid1_texture6);
    for (int i = 0;i < 4;i++) acid2[i].setTexture(acid2_texture6);
    acid_skel.setTexture(acid_skel_texture6);
    for (int i = 0;i < 20;i++) lower_row[i].setTexture(lower_row_texture);
    for (int i = 0;i < 10;i++)
    {
        if (i % 2) upper_row[i].setTexture(upper_row_texture);
        else upper_row[i].setTexture(lupper_row_texture);
    }
    for (int i = 0;i < 8;i++)
    {
        for (int j = 0;j < 10;j++)
        {
            if (i % 2)
            {
                if (j % 2) rback_ground[i][j].setTexture(rbackground_texture1);
                else rback_ground[i][j].setTexture(rbackground_texture2);
            }
            else
            {
                if (j % 2) rback_ground[i][j].setTexture(rbackground_texture2);
                else rback_ground[i][j].setTexture(rbackground_texture1);
            }
        }
    }
    for (int i = 0;i < 4;i++) acid_blocks[i].setTexture(acid_blocks_texture6);
    player1_collider6.setPosition(duck1.myduck.getPosition().x + 35, duck1.myduck.getPosition().y - 7);
    player2_collider62.setPosition(duck2.myduck.getPosition().x + 35, duck2.myduck.getPosition().y - 7);
    collision_Map6(player1_collider6, duck1);
    collision_Map6(player2_collider62, duck2);
    for (int i = 0;i < weaps.size();i++)
    {
        collision_weaps_Map6(weaps[i]);
    }
    Bullet_Collision6();
    update_Logic();
    if (SPAWN.getElapsedTime().asSeconds() >= 60) {
        SPAWN6();
    }
}
void draw_Map6()
{
    for (int i = 0;i < 10;i++) window.draw(lower_row[i]);
    for (int i = 0;i < 10;i++) window.draw(upper_row[i]);
    for (int i = 0;i < 8;i++)
    {
        for (int j = 0;j < 10;j++) window.draw(rback_ground[i][j]);
    }
    for (int i = 0;i < 12;i++) window.draw(nblocks[i]);
    for (int i = 0;i < 5;i++) window.draw(acid1[i]);
    for (int i = 0;i < 4;i++) window.draw(acid2[i]);
    window.draw(acid_skel);
    for (int i = 0;i < 9;i++) window.draw(pillars[i]);
    for (int i = 0;i < 7;i++) window.draw(left_edges[i]);
    for (int i = 0;i < 7;i++) window.draw(right_edges[i]);
    for (int i = 0;i < 13;i++) window.draw(bases[i]);
    for (int i = 0;i < 10;i++) window.draw(middle_blocks[i]);
    for (int i = 0;i < 8;i++) window.draw(boxes[i]);
    for (int i = 0;i < 4;i++) window.draw(acid_blocks[i]);
    window.draw(green_barrel);
    window.draw(red_barrel);
    window.draw(fence1);
    for (int i = 0;i < 3;i++) window.draw(fence2[i]);
    window.draw(fence3);
    for (int i = 0;i < 2;i++) window.draw(doorlocked[i]);
    for (int i = 0;i < 2;i++) window.draw(switch6[i]);
    for (int i = 0;i < 6;i++) window.draw(spikes[i]);
    draw_Logic();
}
void Map6() {
    update_Map6();
    draw_Map6();
}



int main() {

    if (mapnum == 0) init_Map1();
    if (mapnum == 1) init_Map2();
    if (mapnum == 2) init_Map3();
    if (mapnum == 3) init_Map4();
    if (mapnum == 4) init_Map5();
    if (mapnum == 5) init_Map6();

    window.setFramerateLimit(90);
    initMainMenu(Width, Height, window);
    initGameMenu(Width, Height, window);
    initSettingsMenu(Width, Height, window);
    initPaused(Width, Height, window);
    initTransition(duck1, duck2, Width, Height);
    initEndGameScreen(Width, Height);
    initFade(Width, Height);
    initMusic();
    updateMapNum();

    Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event) && !drawPaused) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if (menuState == 1000) {
                    duck1Score = 0;
                    duck2Score = 0;
                    duck1.ready = 0;
                    duck2.ready = 0;
                    mapnum = 0;
                    if (event.key.code == Keyboard::Up) {
                        moveUp(mainMenu, 3);
                    }
                    else if (event.key.code == Keyboard::Down) {
                        moveDown(mainMenu, 3);
                    }
                    else if (event.key.code == Keyboard::Return) {
                        if (mainMenu.selected == 0) {
                            fadeOut(2);
                        }
                        else if (mainMenu.selected == 1) {
                            fadeOut(1);
                        }
                        else if (mainMenu.selected == 2) {
                            window.close();
                        }
                    }
                }

                // Return to main menu 
                else if ((menuState != 1000) && event.key.code == Keyboard::Escape) {
                    fadeOut(1000);
                }
                else if (menuState == 1) {
                    if (event.key.code == Keyboard::Down) {
                        moveDown(settingsMenu, 3);
                    }
                    else if (event.key.code == Keyboard::Up) {
                        moveUp(settingsMenu, 3);
                    }
                    else if (event.key.code == Keyboard::Return) {
                        if (settingsMenu.selected == 2)
                            fadeOut(1000);
                    }
                    else if (event.key.code == Keyboard::Right) {
                        if (settingsMenu.selected == 0) {
                            int newMusicVal = min((int)settingsMenu.menuMusic.getVolume() + 5, 100);
                            settingsMenu.menuMusic.setVolume(newMusicVal);
                            updateMusicVal();
                        }
                    }
                    else if (event.key.code == Keyboard::Left) {
                        if (settingsMenu.selected == 0) {
                            int newMusicVal = max((int)settingsMenu.menuMusic.getVolume() - 5, 0);
                            settingsMenu.menuMusic.setVolume(newMusicVal);
                            updateMusicVal();
                        }
                    }
                }
                else if (menuState == 2) {
                    if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                        moveDown(gameMenu, 2);
                    }
                    else if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                        moveUp(gameMenu, 2);
                    }
                    else if (event.key.code == Keyboard::Return) {
                        if (gameMenu.selected == 0) {
                            startGame(gameMenu, duck1, duck2);
                            if (mapnum == 0) init_Map1();
                            if (mapnum == 1) init_Map2();
                            if (mapnum == 2) init_Map3();
                            if (mapnum == 3) init_Map4();
                            if (mapnum == 4) init_Map5();
                            if (mapnum == 5) init_Map6();
                        }
                    }
                    else if (event.key.code == Keyboard::Right) {
                        if (gameMenu.selected == 1) {
                            mapnum++;
                            if (mapnum > 5)
                                mapnum = 0;
                        }
                    }
                    else if (event.key.code == Keyboard::Left) {
                        if (gameMenu.selected == 1) {
                            mapnum--;
                            if (mapnum < 0)
                                mapnum = 5;
                        }

                    }
                    else if (event.key.code == Keyboard::E) {
                        duck1.ready = !duck1.ready;
                        updateReadiness(gameMenu);
                    }
                    else if (event.key.code == Keyboard::P) {
                        duck2.ready = !duck2.ready;
                        updateReadiness(gameMenu);
                    }
                }

            }
        }

        updateFade();

        window.clear();
        if (menuState == 1000) {
            drawMenu(window, mainMenu, 3);
            duck1Score = 0;
            duck2Score = 0;
            duck1.ready = 0;
            duck2.ready = 0;
            mapnum = 0;
            gameMenu.menuText[2].setString("PLAYER 1");
            gameMenu.menuText[3].setString("PLAYER 2");
        }
        else if (menuState == 0) {
            if (GameEnd) {
                if (duck1.dead) {
                    duck2Score++;
                }
                else if (duck2.dead) {
                    duck1Score++;
                }

                GameEnd = 0;
                menuState = 3;
                trans.restart();
            }
            if (mapnum == 0) Map1();
            if (mapnum == 1) Map2();
            if (mapnum == 2) Map3();
            if (mapnum == 3) Map4();
            if (mapnum == 4) Map5();
            if (mapnum == 5) Map6();

            /*
            if (event.key.code == Keyboard::Escape) {
                drawPaused = !drawPaused;
                drawMenu(window, pausedMenu, 3);
            }
            */
        }
        else if (menuState == 1) {
            drawMenu(window, settingsMenu, 3);
        }
        else if (menuState == 2) {
            updateMapNum();
            drawMenu(window, gameMenu, 4);
        }
        else if (menuState == 3) {
            if (trans.getElapsedTime().asMilliseconds() > 1000) {
                drawTransition(Width, Height);
                duck1.dead = false;
                duck2.dead = false;
                GameEnd = 0;
            }
            else {
                if (mapnum == 0) Map1();
                if (mapnum == 1) Map2();
                if (mapnum == 2) Map3();
                if (mapnum == 3) Map4();
                if (mapnum == 4) Map5();
                if (mapnum == 5) Map6();
                GameEnd = 0;
            }
            if (trans.getElapsedTime().asMilliseconds() > 3000) {
                if (duck1Score == 10 || duck2Score == 10) {
                    menuState = 4;
                }
                else {
                    menuState = 0;
                    mapnum++;
                    mapnum %= 6;
                    GameEnd = 0;
                    if (mapnum == 0) init_Map1();
                    if (mapnum == 1) init_Map2();
                    if (mapnum == 2) init_Map3();
                    if (mapnum == 3) init_Map4();
                    if (mapnum == 4) init_Map5();
                    if (mapnum == 5) init_Map6();
                    if (mapnum == 0) Map1();
                    if (mapnum == 1) Map2();
                    if (mapnum == 2) Map3();
                    if (mapnum == 3) Map4();
                    if (mapnum == 4) Map5();
                    if (mapnum == 5) Map6();
                }
            }
        }
        else if (menuState == 4) {
            drawEndScreen();
        }

        window.display();
    }

    return 0;
}