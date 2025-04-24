#include "MainMenu.h"
#include "Logic.h"
#include "GameTile.h"
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

Menu mainMenu;
Menu gameMenu;
Menu settingsMenu;
RenderWindow window(VideoMode(1280, 720), "Duck Game");
Texture grave;
Sprite Grave;
splashScreen intro;

bool fullscreen = false;

int menuState = 1000; // 1000 = main menu, 0 = game, 1 = settings
bullets bull[3];
ducks duck1, duck2;
weapons sword, pistol, sniper, pewpew;
vector<weapons> weaps;
vector<bullets> bulls;
const float gravity = 0.5f;
const float jumpSpeed = -14.f;
float velocityX = 5.f;
bool GameEnd = 0;
ll mapnum = 1;
float DUCK_SCALE;
float GUN_SCALE;


const float Width = 1280, Height = 720;

//Fady
void init() {

    //init the first duck
    duck1.skin.loadFromFile("img/duck1.png");
    duck1.myduck.setTexture(duck1.skin);
    duck1.myduck.setTextureRect(IntRect(0, 0, 32, 32));
    duck1.myduck.setScale(DUCK_SCALE, DUCK_SCALE);
    duck1.myduck.setOrigin(0.f, duck1.myduck.getLocalBounds().height);
    duck1.myduck.setPosition(0.f, 680.f);
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
        duck1.myduck.getPosition().y - duck1.myduck.getGlobalBounds().height / 2 - 5.f
    );


    //init the second duck
    duck2.skin.loadFromFile("img/duck2.png");
    duck2.myduck.setTexture(duck2.skin);
    duck2.myduck.setTextureRect(IntRect(0, 0, 32, 32));
    duck2.myduck.setScale(-DUCK_SCALE, DUCK_SCALE);
    duck2.myduck.setOrigin(duck2.myduck.getLocalBounds().width, duck2.myduck.getLocalBounds().height);
    duck2.myduck.setPosition(1200.f, 680.f);
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
        duck2.myduck.getPosition().y - duck2.myduck.getGlobalBounds().height / 2 - 5.f
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
    pistol.velocity = 30.f;
    pistol.range = 500.f;
    pistol.fix_X = 34.f;
    pistol.fix_Y = -37.f;
    pistol.fix_hold_x = 30.f;
    pistol.fix_hold_y = -20.f;
    pistol.bull_type = 1;
    pistol.space = 20.f;
    pistol.rev = 15.f;
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
    sniper.velocity = 50.f;
    sniper.range = 100000.f;
    sniper.fix_X = 60.f;
    sniper.fix_Y = -30.f;
    sniper.fix_hold_x = 20.f;
    sniper.fix_hold_y = -25.f;
    sniper.bull_type = 1;
    sniper.space = 15.f;
    sniper.rev = 24.f;
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
    pewpew.velocity = 15.f;
    pewpew.range = 100000.f;
    pewpew.fix_X = 23.f;
    pewpew.fix_Y = -22.f;
    pewpew.fix_hold_x = 30.f;
    pewpew.fix_hold_y = -20.f;
    pewpew.bull_type = 2;
    pewpew.space = 45.f;
    pewpew.rev = 15.f;
    pewpew.weapon.setOrigin(0, pewpew.weapon.getLocalBounds().height);
    pewpew.weapon.setPosition(300.f, 680);
    pewpew.type = "pewpew";

    //init sword
    sword.skin.loadFromFile("img/sword.png");
    sword.weapon.setTexture(sword.skin);
    sword.weapon.setTextureRect(IntRect(0, 0, 8, 23));
    sword.weapon.setScale(GUN_SCALE, GUN_SCALE);
    sword.empty = false;
    sword.fix_X = 14.f;
    sword.fix_Y = 17.f;
    sword.fix_hold_x = 58.f;
    sword.fix_hold_y = -22.f;
    sword.space = 50.f;
    sword.rev = 15.f;
    sword.type = "sword";
    sword.angle = 90.f;
    sword.weapon.setOrigin(sword.weapon.getLocalBounds().width, sword.weapon.getLocalBounds().height);
    sword.weapon.setPosition(400.f, 680);
    sword.weapon.setRotation(sword.angle);

    //init bullet
    bull[1].skin.loadFromFile("img/pistol_bullet.png");
    bull[1].bullet.setTexture(bull[1].skin);
    bull[1].scalex = 0.08f;
    bull[1].scaley = 0.08f;

    bull[2].skin.loadFromFile("img/laser_bullet1.png");
    bull[2].bullet.setTexture(bull[2].skin);
    bull[2].scalex = 0.3f;
    bull[2].scaley = 0.3f;


    //init Grave
    grave.loadFromFile("img/rip.png");
    Grave.setTexture(grave);
    Grave.setScale(0.16f, 0.1f);
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
            bull[idx].bullet.setOrigin(bull[idx].bullet.getGlobalBounds().width, 0.f);
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
        //cout << weap.velocityX << " " << weap.velocityY << " " << weap.velocity << endl;
        weap.velocityY += gravity;
        if (weap.weapon.getPosition().y >= 680.f) {
            weap.weapon.setPosition(weap.weapon.getPosition().x, 680.f);
            weap.velocityX = 0.f;
            weap.velocityY = 0.f;
        }
    }
}

void update_sword(ducks& duck, ll shooter) {
    //cout << "a7a2" << endl;
    if (duck.myweap.type == "sword" && duck.myweap.hit) {
        if (duck.myweap.weapon.getGlobalBounds().intersects(duck2.myduck.getGlobalBounds()) && shooter == 1) {
            Grave.setPosition(duck2.myduck.getPosition().x, 640.f);
            GameEnd = 1;
            cout << "dead" << endl;
            duck2.dead = true;
        }
        if (duck.myweap.weapon.getGlobalBounds().intersects(duck1.myduck.getGlobalBounds()) && shooter == 2) {
            Grave.setPosition(duck1.myduck.getPosition().x, 640.f);
            cout << "dead" << endl;
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
                if (duck.myweap.type == "sword")
                    duck.myweap.weapon.setOrigin(duck.myweap.weapon.getLocalBounds().width, duck.myweap.weapon.getLocalBounds().height);
                else
                    duck.myweap.weapon.setOrigin(0.f, duck.myweap.weapon.getLocalBounds().height);
                duck.myweap.weapon.setScale(GUN_SCALE, GUN_SCALE);
            }
        }

        if (duck.myduck.getPosition().x + velocityX <= 1280 - duckWidth * 2) {
            duck.myduck.move(velocityX, 0.f);
            duck.myarm.arm.move(velocityX, 0.f);
            duck.myweap.weapon.move(velocityX, 0.f);
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
                if (duck.myweap.type == "sword")
                    duck.myweap.weapon.setOrigin(0.f, duck.myweap.weapon.getLocalBounds().height);
                else
                    duck.myweap.weapon.setOrigin(duck.myweap.rev, duck.myweap.weapon.getLocalBounds().height);
                duck.myweap.weapon.setScale(-GUN_SCALE, GUN_SCALE);
            }
        }
        if (duck.myduck.getPosition().x - velocityX >= -duckWidth) {
            duck.myduck.move(-velocityX, 0.f);
            duck.myarm.arm.move(-velocityX, 0.f);
            duck.myweap.weapon.move(-velocityX, 0.f);
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
        duck.myweap.weapon.move(0.f, duck.velocityY);

        if (duck.velocityY < 0) {
            duck.myduck.setTextureRect(IntRect(0, 32, 32, 32));
        }
        else {
            duck.myduck.setTextureRect(IntRect(32, 32, 32, 32));
        }

        if (duck.myduck.getPosition().y >= 680.f) {
            duck.myduck.setPosition(duck.myduck.getPosition().x, 680.f);
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
        return;
    }
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
            GameEnd = 1;
            cout << "dead" << endl;
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

// Setting up a splash screen introduction
void fadeIn() {

}

void fadeOut() {

}

void updateMusicVal() {
    int musicVal = settingsMenu.menuMusic.getVolume();
    string musicText = "MUSIC VOLUME           " + to_string(musicVal);
    settingsMenu.menuText[0].setString(musicText);
}

void initMainMenu(int width, int height, RenderWindow& window) {


    
    // Loading and setting font  
    mainMenu.font.loadFromFile("img/arcade.ttf");
    mainMenu.menuText[0].setFont(mainMenu.font);
    mainMenu.menuText[1].setFont(mainMenu.font);
    mainMenu.menuText[2].setFont(mainMenu.font);

    /* First Button (PLAY) */
    mainMenu.menuText[0].setString("PLAY");
    mainMenu.menuText[0].setCharacterSize(90);
    mainMenu.menuText[0].setFillColor(Color(255, 255, 255));
    mainMenu.menuText[0].setPosition(Vector2f(width/1.5f, height / 4.f));
    cout << width << endl;

    /* Second Button (SETTINGS) */
    mainMenu.menuText[1].setString("SETTINGS");
    mainMenu.menuText[1].setCharacterSize(90);
    mainMenu.menuText[1].setFillColor(Color(255, 255, 255));
    mainMenu.menuText[1].setPosition(Vector2f(width / 1.5f, height / 4.f + 200));

    /* Third Button (EXIT) */
    mainMenu.menuText[2].setString("EXIT");
    mainMenu.menuText[2].setCharacterSize(90);
    mainMenu.menuText[2].setFillColor(Color(255, 255, 255));
    mainMenu.menuText[2].setPosition(Vector2f(width / 1.5f, height / 4.f + 400));

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
    gameMenu.menuText[0].setFont(gameMenu.font);

    gameMenu.menuText[0].setString("GAME");
    gameMenu.menuText[0].setCharacterSize(90);
    gameMenu.menuText[0].setFillColor(Color(255, 255, 255));
    gameMenu.menuText[0].setPosition(Vector2f(200.f, height / 4.f + 200.f));
}

void initMusic() {

    // Music
    if (!settingsMenu.menuMusic.openFromFile("img/music.wav")) {
        cout << "Error loading menu music.\n";
    }

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
    settingsMenu.menuText[0].setCharacterSize(90);
    settingsMenu.menuText[0].setFillColor(Color::White);
    settingsMenu.menuText[0].setPosition(Vector2f(1000.f, height / 4.f + 400));

    string fullscreenText = "FULLSCREEN               " + string(fullscreen ? "ON" : "OFF");
    settingsMenu.menuText[1].setString(fullscreenText);
    settingsMenu.menuText[1].setCharacterSize(90);
    settingsMenu.menuText[1].setFillColor(Color::White);
    settingsMenu.menuText[1].setPosition(Vector2f(1000.f, height / 4.f + 600));


    settingsMenu.menuText[2].setString("BACK");
    settingsMenu.menuText[2].setCharacterSize(90);
    settingsMenu.menuText[2].setFillColor(Color::White);
    settingsMenu.menuText[2].setPosition(Vector2f(1000.f, height / 4.f + 800));
}

void drawMenu(RenderWindow& window, Menu& menu) {
    window.draw(menu.background);
    for (int i = 0; i < 3; i++) {
        window.draw(menu.menuText[i]);
    }
}

void moveUp(Menu& menu) {
    if (menu.selected - 1 >= -1) {
        menu.menuText[menu.selected].setOutlineThickness(0);
        menu.menuText[menu.selected].setFillColor(Color(255, 255, 255));

        menu.selected--;

        if (menu.selected == -1) {
            menu.selected = 2;
        }

        menu.menuText[menu.selected].setOutlineThickness(10);
        menu.menuText[menu.selected].setOutlineColor(Color(255, 150, 0));
        menu.menuText[menu.selected].setFillColor(Color(255, 255, 255));
    }
}

void moveDown(Menu& menu) {
    if (menu.selected + 1 <= 3) {
        // Reset current button's appearance
        menu.menuText[menu.selected].setOutlineThickness(0);
        menu.menuText[menu.selected].setFillColor(Color(255, 255, 255));

        menu.selected++;

        if (menu.selected == 3) {
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
void grounds()
{
    blocks[0].setPosition(1000, 540);
    blocks[0].setScale(0.4, 0.35);
    blocks[1].setPosition(700, 375);
    blocks[1].setScale(0.4, 0.35);
    blocks[2].setPosition(220, 280);
    blocks[2].setScale(0.4, 0.35);
    blocks[3].setPosition(0, 375);
    blocks[3].setScale(0.4, 0.35);
    blocks[4].setPosition(920, 300);
    blocks[4].setScale(0.4, 0.35);
    blocks[5].setPosition(415, 190);
    blocks[5].setScale(0.4, 0.35);
    blocks[6].setPosition(325, 585);
    blocks[6].setScale(0.4, 0.35);
    blocks[7].setPosition(0, 1040);
    blocks[7].setScale(0.4, 0.35);
    blocks[8].setPosition(550, 645);
    blocks[8].setScale(0.4, 0.35);
    blocks[9].setPosition(1120, 375);
    blocks[9].setScale(0.25, 0.35);
    blocks[10].setPosition(100, 500);
    blocks[10].setScale(0.55, 0.35);
    blocks[11].setPosition(435, 440);
    blocks[11].setScale(0.4, 0.35);
    blocks[12].setPosition(800, 585);
    blocks[12].setScale(0.25, 0.35);
    blocks[13].setPosition(670, 220);
    blocks[13].setScale(0.4, 0.35);
    Vector2f pos, sc;
    for (int i = 0;i < 14;i++)
    {
        pos = blocks[i].getPosition();
        int x = pos.x;
        int y = pos.y;
        blocks[i].setPosition(x + 30, y - 150);
    }
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
void init_Map1()
{
    DUCK_SCALE = 2.5f;
    GUN_SCALE = 2.f;
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
    tre();
    grounds();
    skeleton();
    stone();
    cactii();
    bushs();
    grass();
    stoneblock();
    background.setScale(1, 0.75);
    init();
}
void update_Map1()
{
    background.setTexture(background_texture);
    for (int i = 0;i < 14;i++) blocks[i].setTexture(ground_texture);
    for (int i = 0;i < 3;i++) skeletons[i].setTexture(skeleton_texture);
    for (int i = 0;i < 2;i++) stones[i].setTexture(stone_texture);
    tree.setTexture(tree_texture);
    for (int i = 0;i < 2;i++) ccts[i].setTexture(cactus1);
    for (int i = 2;i < 3;i++) ccts[i].setTexture(cactus2);
    for (int i = 3;i < 5;i++) ccts[i].setTexture(cactus3);
    for (int i = 0;i < 3;i++) bush[i].setTexture(bushs_texture);
    for (int i = 0;i < 2;i++) grs[i].setTexture(grass_texture);
    stone_block.setTexture(stone_block_texture);
    update_Logic();
}
void draw_Map1()
{
    window.draw(background);
    for (int i = 0;i < 14;i++)
    {
        if (i == 7) continue;
        window.draw(blocks[i]);
    }
    for (int i = 0;i < 2;i++) window.draw(stones[i]);
    for (int i = 0;i < 3;i++) window.draw(skeletons[i]);
    window.draw(tree);
    for (int i = 0;i < 2;i++) window.draw(ccts[i]);
    for (int i = 2;i < 3;i++) window.draw(ccts[i]);
    for (int i = 3;i < 5;i++) window.draw(ccts[i]);
    for (int i = 0;i < 3;i++) window.draw(bush[i]);
    for (int i = 0;i < 2;i++) window.draw(grs[i]);
    window.draw(stone_block);
    draw_Logic();
}
void Map1() {
    update_Map1();
    draw_Map1();
}


//Fawzy's MAP
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
    GUN_SCALE = 1.f;
    init();
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
}

void update_Map2() {
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
    update_Logic();
    draw_Logic();
}
void Map2() {
    update_Map2();
}



int main() {
    init_Map2();
    window.setFramerateLimit(90);
    initMainMenu(Width, Height, window);
    initGameMenu(Width, Height, window);
    initSettingsMenu(Width, Height, window);
    initMusic();

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (menuState == 0) continue;
            if (event.type == Event::KeyPressed) {
                if (menuState == 1000) {
                    if (event.key.code == Keyboard::Up) {
                        moveUp(mainMenu);
                    }
                    else if (event.key.code == Keyboard::Down) {
                        moveDown(mainMenu);
                    }
                    else if (event.key.code == Keyboard::Return) {
                        if (mainMenu.selected == 0) {
                            menuState = 0;
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
                        moveDown(settingsMenu);
                    }
                    else if (event.key.code == Keyboard::Up) {
                        moveUp(settingsMenu);
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

            }
        }

        window.clear();
        if (menuState == 1000) {
            drawMenu(window, mainMenu);
        }
        else if (menuState == 0) {
            //drawMenu(window, gameMenu);
            if (GameEnd) {
                GameEnd = 0;
                mapnum++;
                mapnum %= 2;
                if (mapnum == 0) init_Map1();
                if (mapnum == 1) init_Map2();
                //if (mapnum == 2) init_Map3();
                //if (mapnum == 3) init_Map4();
                //if (mapnum == 4) init_Map5();
                // show death, and screen between rounds
            }
            if (mapnum == 0) Map1();
            if (mapnum == 1) Map2();
            //if (mapnum == 2) Map3();
            //if (mapnum == 3) Map4();
            //if (mapnum == 4) Map5();
        }
        else if (menuState == 1) {
            drawMenu(window, settingsMenu);
        }

        window.display();
    }

    return 0;
}