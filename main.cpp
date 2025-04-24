#include "MainMenu.h"
//#include "Logic.h"
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

splashScreen intro;

bool fullscreen = false;

int menuState = 0; // 1000 = main menu, 0 = game, 1 = settings

struct arm {
    Texture skin;
    Sprite arm;
};

struct weapons {
    string type;
    Texture skin;
    Sprite weapon;
    float velocityX = 0.0, velocityY = 0.0;
    Clock myclock;
    float velocity;
    ll bullets;
    bool empty;
    float range;
    float posx, posy;
    float fix_X, fix_Y;
    float fix_hold_x, fix_hold_y;
    ll bull_type;
    float space = 0.f;
    float rev;
    float angle;
    bool hit = 0;
    ll fact = 1;
};

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
    float space;
};

//Fady
void init(ducks& duck1, ducks& duck2) {
    //init the first duck
    duck1.skin.loadFromFile("img/duck1.png");
    duck1.myduck.setTexture(duck1.skin);
    duck1.myduck.setTextureRect(IntRect(0, 0, 32, 32));
    duck1.myduck.setScale(3.f, 3.f);
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
    duck1.myarm.arm.setScale(2.5f, 2.5f);
    duck1.myarm.arm.setPosition(
        duck1.myduck.getPosition().x + duck1.myduck.getGlobalBounds().width / 6,
        duck1.myduck.getPosition().y - duck1.myduck.getGlobalBounds().height / 2 - 5.f
    );


    //init the second duck
    duck2.skin.loadFromFile("img/duck2.png");
    duck2.myduck.setTexture(duck2.skin);
    duck2.myduck.setTextureRect(IntRect(0, 0, 32, 32));
    duck2.myduck.setScale(-2.5f, 2.5f);
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
    duck2.myarm.arm.setScale(-3.f, 3.f);
    duck2.myarm.arm.setOrigin(22.f, 0.f);
    duck2.myarm.arm.setPosition(
        duck2.myduck.getPosition().x + duck2.myduck.getGlobalBounds().width / 6,
        duck2.myduck.getPosition().y - duck2.myduck.getGlobalBounds().height / 2 - 5.f
    );
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
    mainMenu.menuText[0].setPosition(Vector2f(1200.f, height / 4.f + 400));

    /* Second Button (SETTINGS) */
    mainMenu.menuText[1].setString("SETTINGS");
    mainMenu.menuText[1].setCharacterSize(90);
    mainMenu.menuText[1].setFillColor(Color(255, 255, 255));
    mainMenu.menuText[1].setPosition(Vector2f(1200.f, height / 4.f + 600));

    /* Third Button (EXIT) */
    mainMenu.menuText[2].setString("EXIT");
    mainMenu.menuText[2].setCharacterSize(90);
    mainMenu.menuText[2].setFillColor(Color(255, 255, 255));
    mainMenu.menuText[2].setPosition(Vector2f(1200.f, height / 4.f + 800));

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
        window.create(VideoMode(1920, 1080), "Duck Game", Style::Default);
    }

    window.setFramerateLimit(60);
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
void start(ducks& duck1, ducks& duck2)
{
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
    init(duck1, duck2);
    background.setScale(1, 0.75);
}
void update()
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
}
void draw(ducks& duck1, ducks& duck2)
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
    window.draw(duck1.myduck);
    window.draw(duck2.myduck);
    window.draw(stone_block);

   
}
void Map1(ducks& duck1, ducks& duck2) {
    update();
    draw(duck1, duck2);
}




int main() {
    ducks duck1, duck2;
    start(duck1, duck2);
    window.setFramerateLimit(60);
    initMainMenu(mainMenu.width, mainMenu.height, window);
    initGameMenu(gameMenu.width, gameMenu.height, window);
    initSettingsMenu(settingsMenu.width, settingsMenu.height, window);
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

                            initMainMenu(mainMenu.width, mainMenu.height, window);
                            initGameMenu(gameMenu.width, gameMenu.height, window);
                            initSettingsMenu(settingsMenu.width, settingsMenu.height, window);
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
            Map1(duck1,duck2);
        }
        else if (menuState == 1) {
            drawMenu(window, settingsMenu);
        }

        window.display();
    }

    return 0;
}