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
#include <iostream>
#include <SFML/System.hpp>
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace sf;
typedef long long ll;


using namespace sf;
using namespace std;

Menu mainMenu, settingsMenu, gameMenu, transition;
RenderWindow window(VideoMode(1280, 720), "Duck Game");
Texture grave;
Sprite Grave;
splashScreen intro;

Clock trans;

bool fullscreen = false;

int menuState = 0; // 1000 = main menu, 0 = game/levels, 1 = settings, 2 = game menu (& level selector), 3 = transition between games 
bullets bull[3];
ducks duck1, duck2;
float fact;
weapons sword, pistol, sniper, pewpew;
vector<weapons> weaps;
vector<bullets> bulls;
float gravity = 0.5f;
float jumpSpeed = -14.f;
float velocityX = 5.f;
float MaxiVelocityY = 10.f;
bool GameEnd = 0;
ll mapnum = 0, duck1Score=0, duck2Score=0;
float DUCK_SCALE;
float GUN_SCALE;
float scalex, scaley;
const float Width = 1280, Height = 720;
float duck1posx=100.f, duck1posy=680.f, duck2posx=1000.f, duck2posy=680.f;


//Fady
void init() {
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
void Fire(ducks& duck, ll shooter) {
    ll idx = duck.myweap.bull_type;
    if (duck.myweap.type == "sword") {
        duck.myweap.hit = 1;
        duck.myweap.myclock.restart();
        return;
    }
    if (duck.myweap.bullets > 0) {
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
void update_weapons() {
    for (auto& weap : weaps) {
        weap.weapon.move(weap.velocityX, weap.velocityY);
        weap.velocityY += gravity;
        if (weap.weapon.getPosition().y >= 780.f) {
            weap.weapon.setPosition(weap.weapon.getPosition().x, 780.f);
            weap.velocityX = 0.f;
            weap.velocityY = 0.f;
        }
    }
}
void update_sword(ducks& duck, ll shooter) {
    if (duck.myweap.type == "sword" && duck.myweap.hit) {
        if (duck.myweap.weapon.getGlobalBounds().intersects(duck2.myduck.getGlobalBounds()) && shooter == 1) {
            Grave.setPosition(duck2.myduck.getPosition().x, 640.f);
            GameEnd = 1;
            duck2.dead = true;
        }
        if (duck.myweap.weapon.getGlobalBounds().intersects(duck1.myduck.getGlobalBounds()) && shooter == 2) {
            Grave.setPosition(duck1.myduck.getPosition().x, 640.f);
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
    duck.myweap.velocityX = 7.f;
    if (!duck.facingRight) {
        duck.myweap.velocityX *= -1.f;
    }
    duck.myweap.velocityY = -7.f;
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
        cout << "sss";
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
            GameEnd = 1;
            duck1.dead = true;
            continue;
        }
        if (bulletBounds.intersects(duck2.myduck.getGlobalBounds())) {
            if (bulls[i].duck == 2) {
                continue;
            }
            bulls.erase(bulls.begin() + i);
            Grave.setPosition(duck2.myduck.getPosition().x, 660.f);
            GameEnd = 1;
            duck2.dead = true;
            continue;
        }

        if (bulls[i].bullet.getPosition().x <= 0 || bulls[i].bullet.getPosition().x >= 1280 || abs(bulls[i].bullet.getPosition().x - bulls[i].startX) >= bulls[i].range) {
            bulls.erase(bulls.begin() + i);
        }
    }
}
void update_Logic() {
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
    update_sword(duck1, 1);
    update_sword(duck2, 2);
}
void draw_Logic() {
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
}


// Walid
void fadeIn() {

}
void fadeOut() {

}
void updateMusicVal() {
    int musicVal = settingsMenu.menuMusic.getVolume();
    string musicText = "MUSIC VOLUME           " + to_string(musicVal);
    settingsMenu.menuText[0].setString(musicText);
}
void updateMapNum() {
    string currMap = "SELECTED  MAP     " + to_string(mapnum + 1);
    gameMenu.menuText[2].setString(currMap);
}
void updateReadiness(Menu& menu) {
    if (menu.selected == 0 && duck1.ready) {
        menu.menuText[0].setString("READY!");
    }
    else if (menu.selected == 0) {
        menu.menuText[0].setString("PLAYER 1");
    }

    if (menu.selected == 3 && duck2.ready) {
        menu.menuText[3].setString("READY!");
    }
    else if (menu.selected == 3) {
        menu.menuText[3].setString("PLAYER 2");
    }
}
void startGame(Menu& menu, ducks& duck1, ducks& duck2) {
    if ((duck1.ready && duck2.ready) && menu.selected == 1) {
        menuState = 0;
    }
}
void initTransition(ducks& duck1, ducks& duck2, int width, int height) {
    transition.font.loadFromFile("img/arcade.ttf");

    for (int i = 0; i < 3; i++) {
        transition.menuText[i].setFont(transition.font);
    }

    transition.menuText[0].setString("SCORE");
    transition.menuText[0].setCharacterSize(60.f);
    transition.menuText[0].setFillColor(Color(255, 255, 255));
    transition.menuText[0].setPosition(Vector2f(width / 2.5f, height / 4.f));

    string score1 = "PLAYER 1    " + to_string(duck1Score);
    transition.menuText[1].setCharacterSize(60.f);
    transition.menuText[1].setFillColor(Color(255, 255, 255));
    transition.menuText[1].setPosition(Vector2f(width / 2.5f, height / 4.f + 200.f));
    transition.menuText[1].setString(score1);

    string score2 = "PLAYER 2    " + to_string(duck2Score);
    transition.menuText[2].setCharacterSize(60.f);
    transition.menuText[2].setFillColor(Color(255, 255, 255));
    transition.menuText[2].setPosition(Vector2f(width / 2.5f, height / 4.f + 400.f));
    transition.menuText[2].setString(score2);

}
void drawTransition(int width, int height) {
    string score1 = "PLAYER 1     " + to_string(duck1Score);
    string score2 = "PLAYER 2     " + to_string(duck2Score);

    transition.menuText[0].setString("SCORE");
    transition.menuText[1].setString(score1);
    transition.menuText[2].setString(score2);

    for (int i = 0; i < 3; i++) {
        window.draw(transition.menuText[i]);
    }
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
void initGameMenu(int width, int height, RenderWindow& window) {

    gameMenu.font.loadFromFile("img/arcade.ttf");

    for (int i = 0; i < 4; i++) {
        gameMenu.menuText[i].setFont(gameMenu.font);
        gameMenu.menuText[i].setCharacterSize(60);
        gameMenu.menuText[i].setFillColor(Color::White);
    }

    gameMenu.menuText[0].setString("PLAYER 1");
    gameMenu.menuText[0].setPosition(Vector2f(width / 6.f - 100, height / 4.f + 400));

    gameMenu.menuText[1].setString("START");
    gameMenu.menuText[1].setPosition(Vector2f(width / 2.25f, height / 4.f + 400.f));

    string currLevel = "SELECTED  MAP     " + to_string(mapnum);
    gameMenu.menuText[2].setString(currLevel);
    gameMenu.menuText[2].setPosition(Vector2f(width / 3.f, height / 4.f + 200.f));

    gameMenu.menuText[3].setString("PLAYER 2");
    gameMenu.menuText[3].setPosition(Vector2f(width / 1.5f + 100, height / 4.f + 400));




}
void initMusic() {

    // Music
    //if (!settingsMenu.menuMusic.openFromFile("img/music.wav")) {
    //    cout << "Error loading menu music.\n";
    //}

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
    string fullscreenText = "FULLSCREEN               " + string(fullscreen ? "ON" : "OFF");
    settingsMenu.menuText[1].setString(fullscreenText);
    settingsMenu.menuText[1].setCharacterSize(60);
    settingsMenu.menuText[1].setFillColor(Color::White);
    settingsMenu.menuText[1].setPosition(Vector2f(width / 2.f, height / 4.f + 250.f));


    settingsMenu.menuText[2].setString("BACK");
    settingsMenu.menuText[2].setCharacterSize(60);
    settingsMenu.menuText[2].setFillColor(Color::White);
    settingsMenu.menuText[2].setPosition(Vector2f(width / 2.f, height / 4.f + 400));
}
void drawMenu(RenderWindow& window, Menu& menu, int itemCount) {
    window.draw(menu.background);

    for (int i = 0; i < itemCount; i++) {
        window.draw(menu.menuText[i]);
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
void fullscreenMode(RenderWindow& window) {
    fullscreen = !fullscreen;

    if (fullscreen) {
        window.create(VideoMode::getFullscreenModes()[0], "Duck Game", Style::Fullscreen);
    }
    else {
        window.create(VideoMode(Width, Height), "Duck Game", Style::Default);
    }

}


//Khaled's MAP
Texture ground_texture, background_texture, skeleton_texture, stone_texture, tree_texture, cactus1, cactus2, cactus3, bushs_texture, grass_texture, stone_block_texture;
Sprite blocks[25], skeletons[3], stones[2], tree, ccts[5], bush[3], grs[2], stone_block, background;
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
    blocks[8].setPosition(1150, 300);
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
    stone_block.setPosition(380, 478);
    stone_block.setScale(0.35, 0.35);
}
void Bullet_Collision1() {
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 25; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(blocks[j].getGlobalBounds()))
            {

                bulls.erase(bulls.begin() + i);
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
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, blocks[i].getPosition().y);
                }
                else
                {
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
void collision_weaps_Map1(RectangleShape& collider, weapons& weap)
{
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
                    weap.weapon.setPosition(weap.weapon.getPosition().x, blocks[i].getPosition().y);
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
void init_Map1()
{
    DUCK_SCALE = 2.5f;
    GUN_SCALE = 2.f;
    fact = 5.f;
    gravity = 0.3f;
    jumpSpeed = -9.f;
    velocityX = 5;
    init();
    // pistol
    pistol.fix_X = -6.f;
    pistol.fix_Y = -28.f;
    pistol.fix_hold_x = 31.f;
    pistol.fix_hold_y = -16.f;
    pistol.rev = 9.f;
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
    Grave.setScale(0.16f, 0.1f);

    background_texture.loadFromFile("img/BG.png");
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
    player1_collider.setOrigin(player1_collider.getLocalBounds().width / 2, player1_collider.getLocalBounds().height / 2);
    player2_collider.setOrigin(player2_collider.getLocalBounds().width / 2, player2_collider.getLocalBounds().height / 2);
    sniper_collider.setOrigin(sniper_collider.getLocalBounds().width / 2, sniper_collider.getLocalBounds().height / 2);
    pistol_collider.setOrigin(pistol_collider.getLocalBounds().width / 2, pistol_collider.getLocalBounds().height / 2);
    pewpew_collider.setOrigin(pewpew_collider.getLocalBounds().width / 2, pewpew_collider.getLocalBounds().height / 2);
    swrd_collider.setOrigin(swrd_collider.getLocalBounds().width / 2, swrd_collider.getLocalBounds().height / 2);
    tre();
    grounds();
    skeleton();
    stone();
    cactii();
    bushs();
    grass();
    stoneblock();
    background.setScale(1, 0.75);
    pistol.collider = pistol_collider;
    sniper.collider = sniper_collider;
    pewpew.collider = pewpew_collider;
    sword.collider = swrd_collider;

    duck1.myduck.setPosition(250, 210);
    duck2.myduck.setPosition(950, 225);

    sniper.weapon.setPosition(450.f, 415);
    pistol.weapon.setPosition(720.f, 145);
    pewpew.weapon.setPosition(360.f, 510);
    sword.weapon.setPosition(1035.f, 465);
    //spawn weaps
    weaps.push_back(pistol);
    weaps.push_back(sniper);
    weaps.push_back(pewpew);
    weaps.push_back(sword);
}
void update_Map1()
{
    duck1.onGround = false;
    duck2.onGround = false;
    background.setTexture(background_texture);
    for (int i = 0;i < 13;i++) blocks[i].setTexture(ground_texture);
    for (int i = 0;i < 3;i++) skeletons[i].setTexture(skeleton_texture);
    for (int i = 0;i < 2;i++) stones[i].setTexture(stone_texture);
    tree.setTexture(tree_texture);
    for (int i = 0;i < 2;i++) ccts[i].setTexture(cactus1);
    for (int i = 2;i < 3;i++) ccts[i].setTexture(cactus2);
    for (int i = 3;i < 5;i++) ccts[i].setTexture(cactus3);
    for (int i = 0;i < 3;i++) bush[i].setTexture(bushs_texture);
    for (int i = 0;i < 2;i++) grs[i].setTexture(grass_texture);
    stone_block.setTexture(stone_block_texture);
    player1_collider.setPosition(duck1.myduck.getPosition().x + 38, duck1.myduck.getPosition().y - 30);
    player2_collider.setPosition(duck2.myduck.getPosition().x + 38, duck2.myduck.getPosition().y - 30);

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

    collision_Map1(player1_collider, duck1);
    collision_Map1(player2_collider, duck2);
    Bullet_Collision1();
    for (int i = 0;i < weaps.size();i++)
    {
        collision_weaps_Map1(weaps[i].collider, weaps[i]);
    }
    if (duck1.myduck.getPosition().y >= 620)
    {
        duck1.dead = true;
    }
    if (duck2.myduck.getPosition().y >= 620)
    {
        duck2.dead = true;
    }
    update_Logic();
}
void draw_Map1()
{
    window.draw(background);
    for (int i = 0;i < 13;i++)  window.draw(blocks[i]);
    for (int i = 0;i < 2;i++) window.draw(stones[i]);
    for (int i = 0;i < 3;i++) window.draw(skeletons[i]);
    window.draw(tree);
    for (int i = 0;i < 5;i++) window.draw(ccts[i]);
    for (int i = 0;i < 2;i++) window.draw(grs[i]);
    window.draw(stone_block);
    draw_Logic();
}
void Map1() {
    update_Map1();
    draw_Map1();
}


//Fawzy's MAP
RectangleShape player1(Vector2f(24, 35)), player2(Vector2f(24, 35));
int spawnX = 25;
int spawnY = 27;
set<GameTile::TileType> solidTiles = {
        GameTile::Stone, GameTile::Cement, GameTile::Ground, GameTile::Branch, GameTile::Weapon
};
GameTile::GameTile() {
    loadTexture(Ground, "img/ground2.png");
    loadTexture(Tree, "img/tree2.png");
    loadTexture(Cement, "img/cement.png");
    loadTexture(Weapon, "img/weapon.png");
    loadTexture(Branch, "img/branch.png");
    loadTexture(Stone, "img/stone2.png");
    loadTexture(Root, "img/root.png");
}
void GameTile::loadTexture(TileType type, const std::string& path) {
    sf::Texture texture;
    if (texture.loadFromFile(path)) {
        textures[type] = texture;
        sprites[type].setTexture(textures[type]);
    }
}
const sf::Sprite& GameTile::getTileSprite(TileType type) {
    return sprites[type];
}
const int TILE_SIZE = 9;
GameTile::TileType charToTileType(char c) {
    switch (c) {
    case 'g': return GameTile::Ground;
    case 'r': return GameTile::Root;
    case 'c': return GameTile::Cement;
    case 's': return GameTile::Stone;
    case 'b': return GameTile::Branch;
    case 'w': return GameTile::Weapon;
    case 't': return GameTile::Tree;
    default:  return GameTile::None;
    }
}
GameTile gameTiles;
vector<string> level;
void init_Map2() {
    DUCK_SCALE = 1.5;
    GUN_SCALE = 1.2f;
    fact = 3.f;
    gravity = 0.5f;
    jumpSpeed = -10.0f;  //give it negative value
    velocityX = 2.5f;   // the duck speed
    duck1posx = 25 * TILE_SIZE;
    duck1posy = 27 * TILE_SIZE - 16;
    init();
    // pistol
    pistol.fix_X = -2.f;
    pistol.fix_Y = -17.f;
    pistol.fix_hold_x = 18.f;
    pistol.fix_hold_y = -12.f;
    pistol.rev = 11.f;
    //sniper
    sniper.fix_X = 13.f;
    sniper.fix_Y = -14.f;
    sniper.fix_hold_x = 11.f;
    sniper.fix_hold_y = -15.f;
    sniper.rev = 23.f;
    //pewpew
    pewpew.fix_X = 4.f;
    pewpew.fix_Y = -10.f;
    pewpew.fix_hold_x = 15.f;
    pewpew.fix_hold_y = -13.f;
    pewpew.rev = 16.f;
    //sword
    sword.fix_hold_x = 29.f;
    sword.fix_hold_y = -12.f;
    sword.rev = 2.f;
    //bull1
    bull[1].scalex = 0.036f;
    bull[1].scaley = 0.036f;
    //bull2
    bull[2].scalex = 0.12f;
    bull[2].scaley = 0.12f;
    //grave
    Grave.setScale(0.16f, 0.1f);

    //spawn weaps
    weaps.push_back(pistol);
    weaps.push_back(sniper);
    weaps.push_back(pewpew);
    weaps.push_back(sword);


    level = {
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                                                                                       w                            ",
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                                                                                                                    ",
    "                                           w                                                                                        ",
    "                                                                                                                                    ",
    "                                                                                                      ggggg                         ",
    "                                                                                                      ccccc                         ",
    "                                                                                                   gggccccc                         ",
    "                                                                                                   ccccccc                          ",
    "                                                                                              gggggccccc                            ",
    "                                                                                              ccccccccc                             ",
    "                                                                                              ccccccccc   tt                        ",
    "                        t             gggggggggg                                              ccccccccc   tt                        ",
    "                        t             cccccccccc                                                          tt                   w    ",
    "                        t             cccccccccc                                                          tt                        ",
    "                        t                ccccccc                                                          tttbbbbbt                 ",
    "                        t                ccccccc                           s                                t     t                 ",
    "                        t                ccccccc                                                            t     t                 ",
    "                        t                                               gggggggggg                          t     t                 ",
    "                        t                                   s           cccccccccc                          t     t                 ",
    "                        t                                               ccccccccc                           t     t                 ",
    "                        tbbbbbbb                     gggggggggggggggggggccc                                 t     t                 ",
    "                        t                            cccccccccccccccccccccc                                 t     t        ggggggggg",
    "                        t                                  ccccccccccccc                                    t     r        ccccccccc",
    "                        t                                  ccccccccccccc                                    t              ccccccccc",
    "                        t                                  ccccccccccccc                                    t  ggggggggggggcccccc   ",
    "                        t                                                                                   t  cccccccccccccccccc   ",
    "                        t                                                                                   t     ccccccccccccccc   ",
    "                        t                                                                                bbbt     ccccccccccccccc   ",
    "                        t                                                                                   t     ccccccccccccccc   ",
    "                        t                                                           s                       t                       ",
    "                        t                                                                                   t                       ",
    "                        t                    gggggg                              gggggg                     t                       ",
    "                        t                    cccccc                              cccccc                     t                       ",
    "                        t                    cccccc                              cccccc                     t                       ",
    "                        t                    cccccc      s                    gggccccccggg                  t                       ",
    "                        t                    cccccc                           cccccccccccc                  t                       ",
    "                        t                    ccccccggggggggggggggg bbt  ggggggccccccccc               bbbbbbt                       ",
    "                        t                    ccccccccccccccccccccc   t  ccccccccccccccc                     t                       ",
    "                        t                    ccccccccccccccccccccc   t  ccccccccccccccc                     t                       ",
    "                        t                    ccccccccccccccccccccc   t  ccccccccccccccc                     t            w          ",
    "                        tbbb                 ccccccccccc             t  ccccccccc                           t                       ",
    "                   w    t                ggggccccccccccc             t  ccccccccc                           t                       ",
    "                        t                ccccccccccccccc             r  ccccccccc                           t                       ",
    "                        t                                               ccc                                 t                       ",
    "                        t                                            gggccc                                 t                       ",
    "                        t                                            cccccc                                 t                 ggg   ",
    "            ggg         t                                                                             s     t                 ccc   ",
    "            ccc         r                                                                                   t                 ccc   ",
    "            ccc                                                                                     ggggggggt         ggggggggccc   ",
    "            cccggggggggggggggggggg                                                                  ccccccccr         ccccccccccc   ",
    "            cccccccccccccccccccccc                                                                    cccccc          cccccccc      ",
    "            cccccccccccccccccccccc                                            s                       ccccccggggggggggcccccccc      ",
    "            cccccccccccccccccccccc                                                                    cccccccccccccccccccccccc      ",
    "                  ccccccccccccccccggg              ggggggggggggggggggggggggggggggggg                                                ",
    "                  ccccccccccccccccccc     s        ccccccccccccccccccccccccccccccccc                                                ",
    "                  ccccccccccccccccccc              ccccccccccccccccccccccccccccccccc                                                ",
    "                        cccccccccccccggggggggggggggcccccccccccccccccccccccccccccccccgggggg                                          ",
    "                        cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc                                          ",
    "                        cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc                                          ",
    "                              cccccccccc        ccccccccc                     ccccccccccccggg                                       ",
    "                              cccccccccc        ccccccccc                     ccccccccccccccc                                       ",
    "                              cccccccccc        ccccccccc                                                                           ",
    "                                                   cccccc                                                                           ",
    "                                                   cccccc                                                                           ",
    "                                                   cccccc                                                                           ",
    "                                                                                                                            "
    };

    player1_collider.setOrigin(player1_collider.getLocalBounds().width / 2, player1_collider.getLocalBounds().height / 2);
    player2_collider.setOrigin(player2_collider.getLocalBounds().width / 2, player2_collider.getLocalBounds().height / 2);
    duck1.myduck.setPosition(250, 210);
    duck2.myduck.setPosition(950, 225);
}
void collision_Map2(RectangleShape& player, ducks& duck)
{
    FloatRect box, wall, intersection;
    box = player.getGlobalBounds();
    for (size_t y = 0; y < level.size(); ++y) {
        for (size_t x = 0; x < level[y].size(); ++x) {
            GameTile::TileType tileType = charToTileType(level[y][x]);
            if (!solidTiles.count(tileType)) continue;

            Sprite tile = gameTiles.getTileSprite(tileType);
            tile.setScale(1.125, 1.125);
            tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
            wall = tile.getGlobalBounds();

            if (box.intersects(wall))
            {
                box.intersects(wall, intersection);
                if (intersection.width > intersection.height)
                {
                    if (box.top < wall.top)
                    {
                        duck.onGround = true;
                        duck.isJumping = false;
                        duck.velocityY = 0.f;
                        duck.myduck.setPosition(duck.myduck.getPosition().x, tile.getPosition().y + 1.f);
                    }
                    else
                    {
                        duck.velocityY = 0;
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
}
void update_Map2() {
    duck1.onGround = false;
    duck2.onGround = false;
    player1.setPosition(duck1.myduck.getPosition().x + 10, duck1.myduck.getPosition().y - 35);
    player2.setPosition(duck2.myduck.getPosition().x + 10, duck2.myduck.getPosition().y - 35);
    collision_Map2(player1, duck1);
    collision_Map2(player2, duck2);
    if (duck1.myduck.getPosition().y >= 620)
    {
        duck1.dead = false; //edit later
    }
    if (duck2.myduck.getPosition().y >= 620)
    {
        duck2.dead = false; //edit later
    }
    update_Logic();
}
void draw_Map2() {
    for (size_t y = 0; y < level.size(); ++y) {
        for (size_t x = 0; x < level[y].size(); ++x) {
            char tileChar = level[y][x];
            GameTile::TileType tileType = charToTileType(tileChar);
            if (tileType != GameTile::None) {
                sf::Sprite tile = gameTiles.getTileSprite(tileType);
                tile.setPosition(static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE));
                window.draw(tile);
            }
        }
    }
    cout << duck1.myduck.getPosition().x << " " << duck1.myduck.getPosition().y << endl;
    window.draw(player1);
    draw_Logic();
}
void Map2() {
    update_Map2();
    draw_Map2();
}


// Khaled Ghareeb
struct block
{
    Sprite map_blocks;
};
block b; block obs;

const int MAX_BLOCKS = 100;
block finalground[MAX_BLOCKS];
block finalblock[MAX_BLOCKS];
block finalobs[MAX_BLOCKS];
int groundCount = 0;
int blockCount = 0;
int obsCount = 0;

Sprite backgroundGhareeb;
Texture backgroundtexture;

Texture mapblock_Texture;
Texture obstacels_texture;

RectangleShape playercollider1(Vector2f(28, 46));
RectangleShape playercollider2(Vector2f(28, 46));

RectangleShape pistolcollider3(Vector2f(28, 25));
RectangleShape snipercollider3(Vector2f(40, 25));
RectangleShape pewpewcollider3(Vector2f(40, 25));
RectangleShape swordcollider3(Vector2f(36, 20));


void groundd()
{
    for (int i = 0; i < 10; i++)
    {
        block ground;


        ground.map_blocks.setTexture(mapblock_Texture);
        ground.map_blocks.setScale(0.45, 0.17);
        ground.map_blocks.setPosition(0 + i * 150, 697);
        finalground[groundCount++] = ground;
    }
}
void obstacels_position(int x, int y)
{
    obs.map_blocks.setPosition(x, y);
    finalobs[obsCount++] = obs;

}
void block_position(int x, int y)
{

    b.map_blocks.setPosition(x, y);
    finalblock[blockCount++] = b;

}
void blocksGhareeb()
{
    groundd();

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
void handleCollision3(RectangleShape& player, block& obj, ducks& duck)
{
    FloatRect duckBounds = player.getGlobalBounds();
    FloatRect objBounds = obj.map_blocks.getGlobalBounds();

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
                duck.myduck.setPosition(duck.myduck.getPosition().x, obj.map_blocks.getPosition().y + 1.f);
                duck.onGround = 1;
                duck.isJumping = 0;
            }
            else
                duck.myduck.setPosition(duck.myduck.getPosition().x, duck.myduck.getPosition().y + intersection.height);
            duck.velocityY = 0;
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
void handle_BulletCollision3(block& obj)
{
    for (int i = bulls.size()-1;i>=0; i--)
    {

        for (int j = 0; j < MAX_BLOCKS; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(obj.map_blocks.getGlobalBounds()))
            {

                bulls.erase(bulls.begin() + i);
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
void weapon_handleCollision3(RectangleShape& weapcoll, weapons& weapon, block& obj)
{
    FloatRect weapBounds = weapon.weapon.getGlobalBounds();
    FloatRect objBounds = obj.map_blocks.getGlobalBounds();

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

        {
            if (weapBounds.top < objBounds.top)
            {
                weapon.weapon.setPosition(weapon.weapon.getPosition().x, obj.map_blocks.getPosition().y + 1.f);
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
void collision_Weapon3(RectangleShape& weap, weapons& weapon)
{
    for (int i = 0; i < blockCount; i++)
        weapon_handleCollision3(weap, weapon, finalblock[i]);
    for (int i = 0; i < groundCount; i++)
        weapon_handleCollision3(weap, weapon, finalground[i]);
    for (int i = 0; i < obsCount; i++)
        weapon_handleCollision3(weap, weapon, finalobs[i]);
}
void init_Map3()
{
    DUCK_SCALE = 2.f;
    GUN_SCALE = 1.6f;
    fact = 3.f;
    gravity = 0.35f;
    jumpSpeed = -9.5f;  //give it negative value
    velocityX = 3;   // the duck speed
    init();
    pistol.weapon.setPosition(1000, 250);
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
    Grave.setScale(0.16f, 0.1f);




    backgroundtexture.loadFromFile("img/background.jpg");
    backgroundGhareeb.setTexture(backgroundtexture);
    backgroundGhareeb.setScale(0.5, 0.5);

    mapblock_Texture.loadFromFile("img/cccc.png");

    obstacels_texture.loadFromFile("img/Block 2.png");


    b.map_blocks.setTexture(mapblock_Texture);
    b.map_blocks.setScale(0.45, 0.17);

    obs.map_blocks.setTexture(obstacels_texture);
    obs.map_blocks.setScale(0.035, 0.035);

    duck1.myduck.setPosition(180, 250);
    duck2.myduck.setPosition(530, 660);

    playercollider1.setOrigin(duck1.myduck.getLocalBounds().width / 2 - 34, duck1.myduck.getLocalBounds().height / 2 + 30);
    playercollider2.setOrigin(duck2.myduck.getLocalBounds().width / 2 - 34, duck2.myduck.getLocalBounds().height / 2 + 30);

    pistolcollider3.setOrigin(pistol.weapon.getLocalBounds().width / 2 - 8, pistol.weapon.getLocalBounds().height / 2 + 13);
    snipercollider3.setOrigin(sniper.weapon.getLocalBounds().width / 2 - 18, sniper.weapon.getLocalBounds().height / 2 + 11);
    pewpewcollider3.setOrigin(pewpew.weapon.getLocalBounds().width / 2 - 13, pewpew.weapon.getLocalBounds().height / 2 + 12);
    swordcollider3.setOrigin(sword.weapon.getLocalBounds().width / 2 - 4, sword.weapon.getLocalBounds().height / 2);

    pistolcollider3.setFillColor(Color::Green);
    snipercollider3.setFillColor(Color::Green);
    pewpewcollider3.setFillColor(Color::Green);
    swordcollider3.setFillColor(Color::Green);

    pistol.collider = pistolcollider3;
    sniper.collider = snipercollider3;
    pewpew.collider = pewpewcollider3;
    sword.collider = swordcollider3;

    
    weaps.push_back(pistol);
    weaps.push_back(sniper);
    weaps.push_back(pewpew);
    weaps.push_back(sword);
    blocksGhareeb();

}
void update3()
{
    duck1.onGround = false;
    duck2.onGround = false;


    playercollider1.setPosition(duck1.myduck.getPosition());
    playercollider2.setPosition(duck2.myduck.getPosition());

    for(ll i=0;i<weaps.size();++i)
        weaps[i].collider.setPosition(weaps[i].weapon.getPosition());

    collision3(playercollider1, duck1);
    collision3(playercollider2, duck2);

    BulletCollision3();
    for (ll i = 0;i < weaps.size();++i) 
        collision_Weapon3(weaps[i].collider, weaps[i]);

    duck1.myarm.arm.setPosition(
        duck1.myduck.getPosition().x + duck1.myduck.getGlobalBounds().width / 6,
        duck1.myduck.getPosition().y - duck1.myduck.getGlobalBounds().height / 2 - fact
    );
    duck2.myarm.arm.setPosition(
        duck2.myduck.getPosition().x + duck2.myduck.getGlobalBounds().width / 6,
        duck2.myduck.getPosition().y - duck2.myduck.getGlobalBounds().height / 2 - fact
    );


    update_Logic();

}
void draw3()
{
    window.draw(backgroundGhareeb);

    for (int i = 0; i < groundCount; i++)
        window.draw(finalground[i].map_blocks);

    for (int i = 0; i < blockCount; i++)
        window.draw(finalblock[i].map_blocks);

    for (int i = 0; i < obsCount; i++)
        window.draw(finalobs[i].map_blocks);

    /*window.draw(pistol.weapon);
    window.draw(pistolcollider3);
    window.draw(snipercollider3);
    window.draw(pewpewcollider3);
    window.draw(swordcollider3);

    window.draw(sniper.weapon);
    window.draw(pewpew.weapon);
    window.draw(sword.weapon);*/

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
void initBackground()
{
    ground.loadFromFile("img/Lv7nNS.png");
    background.setTexture(ground);
    scalex = window.getSize().x * 1.0f / ground.getSize().x;
    scaley = window.getSize().y * 1.0f / ground.getSize().y;
    background.setScale(scalex, scaley);
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
    Grave.setScale(0.16f, 0.1f);

    // SET WEAPONS POSITION
    for (int y = 0; y < 23; y++) {
        for (int x = 0; x < snowMap[y].length(); x++) {
            if (snowMap[y][x] == 'S') {
                weapPos.x = x * TILE_SIZEE;
                weapPos.y = (y + .9) * TILE_SIZEE;
                sniper.weapon.setPosition(weapPos.x, weapPos.y);
            }
            else if (snowMap[y][x] == 'W') {
                weapPos.x = x * TILE_SIZEE;
                weapPos.y = (y + .9) * TILE_SIZEE;
                sword.weapon.setPosition(weapPos.x, weapPos.y);
            }
            else if (snowMap[y][x] == 'E') {
                weapPos.x = (x - .3) * TILE_SIZEE;
                weapPos.y = (y + .9) * TILE_SIZEE;
                pewpew.weapon.setPosition(weapPos.x, weapPos.y);
            }
            else if (snowMap[y][x] == 'P') {
                weapPos.x = x * TILE_SIZEE;
                weapPos.y = (y + .9) * TILE_SIZEE;
                pistol.weapon.setPosition(weapPos.x, weapPos.y);
            }
        }
    }

    //spawn weaps
    weaps.push_back(pistol);
    weaps.push_back(sniper);
    weaps.push_back(pewpew);
    weaps.push_back(sword);

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
            }
            tileCount++;
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
                //cout << intersection.width << " " << intersection.height << endl;
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
                        //cout << "left" << endl;
                        duck.myduck.setPosition(
                            mapTiles[i].bounds.left + mapTiles[i].bounds.width - (duck.facingRight ? 16 : 22),
                            duck.myduck.getPosition().y
                        );
                    }
                }
                else if (intersection.width - 4 > intersection.height) { // Vertical collision
                    //cout << "top : " << duckBound.top << "   MAP TILE BOTTOM : " << mapTiles[i].bounds.top + mapTiles[i].bounds.height << endl;
                    //cout << mapTiles[i].bounds.top << endl;
                    if (duckBound.top < mapTiles[i].bounds.top) {
                        // Landing on ground (from above)
                        //cout << "from above" << endl;
                        duck.myduck.setPosition(
                            duck.myduck.getPosition().x,
                            mapTiles[i].tileSprite.getPosition().y + 1
                        );
                        duck.onGround = true;
                        duck.isJumping = false;
                        duck.velocityY = 0;
                    }
                    else {
                        // Hitting ceiling (from below)
                        //cout << "from below" << endl;
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
                //cout << intersection.width << " " << intersection.height << endl;
                if (intersection.width < intersection.height) {
                    if (weaponBounds.left < mapTiles[i].bounds.left) {
                        // Right collision
                        cout << "RIGHT " << endl;
                        Gun.velocityX = 0;
                        Gun.weapon.setPosition(
                            Gun.weapon.getPosition().x - intersection.width - 1,
                            Gun.weapon.getPosition().y
                        );
                    }
                    else if (weaponBounds.left > mapTiles[i].bounds.left) {
                        // Left collision
                        cout << "left" << endl;
                        //cout << intersection.width << " " << intersection.height << endl;
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
                        //cout << "from above" << endl;
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
                        //cout << "from below" << endl;
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
                cout << bulls.size() << endl;
                if (bulls[i].bullet.getGlobalBounds().intersects(mapTiles[j].bounds)) {
                    cout << "FOUND !!" << endl;
                    bulls.erase(bulls.begin() + i);
                    break;
                }
            }
        }
    }
}
void update_Map4() {
    update_Logic();
    for (auto& weap : weaps) {
        weap_collision_Map4(weap);
    }
    collision_Map4(duck1);
    collision_Map4(duck2);
    bullet_collision4();
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
Texture backgroundTexture;
Texture blockTexture;
Texture groundTexture;
Sprite blockse[9];
Sprite backgroundSprite;
RectangleShape player1_colliderh(Vector2f(27, 50)), player2_colliderh(Vector2f(27, 50));
RectangleShape pistol_colliderh(Vector2f(25, 20)), sniper_colliderh(Vector2f(55, 20)), pewpew_colliderh(Vector2f(45, 25)), swrd_colliderh(Vector2f(40, 20));

void Bullet_Collision5() {
    for (int i = bulls.size() - 1;i >= 0; i--)
    {

        for (int j = 0; j < 9; j++)
        {
            if (bulls[i].bullet.getGlobalBounds().intersects(blockse[j].getGlobalBounds()))
            {

                bulls.erase(bulls.begin() + i);
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
    blockse[2].setScale(3.5, 1);
    blockse[3].setPosition(55, 90);
    blockse[3].setScale(2, 1);
    blockse[4].setPosition(395, 175);
    blockse[4].setScale(1.7, 1);
    blockse[5].setPosition(985, 300);
    blockse[5].setScale(2, 1);
    blockse[6].setPosition(1035, 90);
    blockse[6].setScale(2, 1);
    blockse[7].setPosition(745, 175);
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
                    duck.onGround = true;
                    duck.isJumping = false;
                    duck.velocityY = 0.f;
                    duck.myduck.setPosition(duck.myduck.getPosition().x, blockse[i].getPosition().y + 1);
                }
                else
                {
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
void collision_weaps_Map5(RectangleShape& collider, weapons& weap)
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
                    weap.weapon.setPosition(weap.weapon.getPosition().x, blockse[i].getPosition().y + 1.f);
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
void init_Map5() {
    DUCK_SCALE = 2.f;
    GUN_SCALE = 1.6f;
    fact = 3.f;
    //gravity = Put_your_val;
    //jumpSpeed = Put_your_val;  //give it negative value
    //velocityX = Put_your_val;   // the duck speed
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
    Grave.setScale(0.16f, 0.1f);



    backgroundTexture.loadFromFile("img/nature.png");
    blockTexture.loadFromFile("img/blocks.png");
    blocks5();
    for (int i = 0;i < 9;i++) {
        blockse[i].setTexture(blockTexture);
    }
    backgroundSprite.setTexture(backgroundTexture);
    Vector2f windowSize = static_cast<sf::Vector2f>(window.getSize());
    Vector2f textureSize = static_cast<sf::Vector2f>(backgroundTexture.getSize());
    backgroundSprite.setScale(windowSize.x / textureSize.x, windowSize.y / textureSize.y);
    player1_colliderh.setOrigin(player1_colliderh.getLocalBounds().width / 2, player1_colliderh.getLocalBounds().height / 2);
    player2_colliderh.setOrigin(player2_colliderh.getLocalBounds().width / 2, player2_colliderh.getLocalBounds().height / 2);
    sniper_collider.setOrigin(sniper_collider.getLocalBounds().width / 2, sniper_collider.getLocalBounds().height / 2);
    pistol_collider.setOrigin(pistol_collider.getLocalBounds().width / 2, pistol_collider.getLocalBounds().height / 2);
    pewpew_collider.setOrigin(pewpew_collider.getLocalBounds().width / 2, pewpew_collider.getLocalBounds().height / 2);
    swrd_collider.setOrigin(swrd_collider.getLocalBounds().width / 2, swrd_collider.getLocalBounds().height / 2);
    pistol.collider = pistol_colliderh;
    sniper.collider = sniper_colliderh;
    pewpew.collider = pewpew_colliderh;
    sword.collider = swrd_colliderh;

    duck1.myduck.setPosition(30, 320);
    duck2.myduck.setPosition(1030, 90);

    sniper.weapon.setPosition(200, 390);
    pistol.weapon.setPosition(850, 520);
    pewpew.weapon.setPosition(750, 175);
    sword.weapon.setPosition(400, 175);

    //spawn weaps
    weaps.push_back(pistol);
    weaps.push_back(sniper);
    weaps.push_back(pewpew);
    weaps.push_back(sword);
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
        collision_weaps_Map5(weaps[i].collider, weaps[i]);
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
}
void draw_Map5() {

    window.draw(backgroundSprite);
    for (int i = 0;i < 9;i++)
        window.draw(blockse[i]);
    draw_Logic();
}
void Map5() {
    update_Map5();
    draw_Map5();
}


int main() {

    if (mapnum == 0) init_Map1();
    if (mapnum == 1) init_Map2();
    if (mapnum == 2) init_Map3();
    if (mapnum == 3) init_Map4();
    if (mapnum == 4) init_Map5();

    window.setFramerateLimit(90);
    initMainMenu(Width, Height, window);
    initTransition(duck1, duck2, Width, Height);
    initGameMenu(Width, Height, window);
    initSettingsMenu(Width, Height, window);
    initMusic();

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if (menuState == 1000) {
                    if (event.key.code == Keyboard::Up) {
                        moveUp(mainMenu, 3);
                    }
                    else if (event.key.code == Keyboard::Down) {
                        moveDown(mainMenu, 3);
                    }
                    else if (event.key.code == Keyboard::Return) {
                        if (mainMenu.selected == 0) {
                            menuState = 2;
                        }
                        else if (mainMenu.selected == 1) {
                            menuState = 1;
                        }
                        else if (mainMenu.selected == 2) {
                            window.close();
                        }
                    }
                }

                // Return to main menu 
                else if (menuState != 1000 && event.key.code == Keyboard::Escape) {
                    menuState = 1000;
                }

                else if (menuState == 1) {
                    if (event.key.code == Keyboard::Down) {
                        moveDown(settingsMenu, 3);
                    }
                    else if (event.key.code == Keyboard::Up) {
                        moveUp(settingsMenu, 3);
                    }
                    else if (event.key.code == Keyboard::Return) {

                        if (settingsMenu.selected == 1) {
                            fullscreenMode(window);

                            initMainMenu(Width, Height, window);
                            initGameMenu(Width, Height, window);
                            initSettingsMenu(Width, Height, window);
                        }

                        if (settingsMenu.selected == 2)
                            menuState = 1000;
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
                        moveDown(gameMenu, 4);
                    }
                    else if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                        moveUp(gameMenu, 4);
                    }
                    else if (event.key.code == Keyboard::Return) {
                        if (gameMenu.selected == 0) {
                            duck1.ready = !duck1.ready;
                            updateReadiness(gameMenu);
                        }
                        else if (gameMenu.selected == 3) {
                            duck2.ready = !duck2.ready;
                            updateReadiness(gameMenu);
                        }
                        else if (gameMenu.selected == 1) {
                            startGame(gameMenu, duck1, duck2);
                        }
                    }
                    else if (event.key.code == Keyboard::Right) {
                        if (gameMenu.selected == 2) {
                            mapnum++;
                            if (mapnum > 4)
                                mapnum = 0;
                            updateMapNum();
                        }
                    }
                    else if (event.key.code == Keyboard::Left) {
                        if (gameMenu.selected == 2) {
                            mapnum--;
                            if (mapnum < 0)
                                mapnum = 4;
                            updateMapNum();
                        }

                    }
                }

            }
        }

        window.clear();
        if (menuState == 1000) {
            drawMenu(window, mainMenu, 3);
        }
        else if (menuState == 0) {
            if (GameEnd) {
                if (duck1.dead) {
                    duck2Score++;
                    cout << "Duck 2 Dead" << endl;
                    cout << "Duck 1 - " << duck1Score << endl;
                    cout << "Duck 2 - " << duck2Score << endl;
                }
                else if (duck2.dead) {
                    duck1Score++;
                    cout << "Duck 2 Dead" << endl;
                    cout << "Duck 1 - " << duck1Score << endl;
                    cout << "Duck 2 - " << duck2Score << endl;
                }

                duck1.dead = false;
                duck2.dead = false;
                GameEnd = 0;
                mapnum++;
                if (mapnum == 1) mapnum++;
                mapnum %= 5;
                menuState = 3;
                trans.restart();
                if (mapnum == 0) init_Map1();
                if (mapnum == 1) init_Map2();
                if (mapnum == 2) init_Map3();
                if (mapnum == 3) init_Map4();
                if (mapnum == 4) init_Map5();
            }
            if (mapnum == 0) Map1();
            if (mapnum == 1) Map2();
            if (mapnum == 2) Map3();
            if (mapnum == 3) Map4();
            if (mapnum == 4) Map5();
        }
        else if (menuState == 1) {
            drawMenu(window, settingsMenu, 3);
        }
        else if (menuState == 2) {
            drawMenu(window, gameMenu, 4);
        }
        else if (menuState == 3) {
            drawTransition(Width, Height);
            if (trans.getElapsedTime().asMilliseconds() > 2000) {
                menuState = 0;
            }
        }

        window.display();
    }

    return 0;
}