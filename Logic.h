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
    bool onGround = false;
    bool ready = false;
    bool boom = false;
    float scale;
    ll fact = 1;
    RectangleShape collider;
};

struct bullets {
    Texture skin;
    Sprite bullet;
    ll duck;
    bool right;
    float velocity;
    float startX;
    float range;
    float scalex, scaley;
};

struct ducks {
    Texture skin;
    Clock myclock;
    Sprite myduck;
    int frame = 0;
    int score = 0;
    bool facingRight = true;
    bool isJumping = false;
    bool haveWeapon = false;
    bool holding = false;
    bool firing = false;
    bool dead = false;
    bool onGround = false;
    bool holdingJump = false;
    bool ready = false;
    weapons myweap;
    arm myarm;
    Keyboard::Key up;
    Keyboard::Key right;
    Keyboard::Key left;
    Keyboard::Key hold;
    Keyboard::Key fire;
    float velocityY = 0.f;
    float space;
    float fact;
};

template<typename T>
struct MyVector {
    T* data;
    int capacity;
    int length;

    MyVector() {
        capacity = 1;
        length = 0;
        data = new T[capacity];
    }

    void push_back(const T& value) {
        if (length == capacity) {
            capacity *= 2;
            T* new_data = new T[capacity];
            for (int i = 0; i < length; i++)
                new_data[i] = data[i];
            delete[] data;
            data = new_data;
        }
        data[length++] = value;
    }

    void erase(int index) {
        if (index < 0 || index >= length) return;
        for (int i = index; i + 1 < length; i++)
            data[i] = data[i + 1];
        length--;
    }

    T& operator[](int index) {
        return data[index];
    }

    int size() const {
        return length;
    }

    bool empty() const {
        return length == 0;
    }

    void clear() {
        length = 0;
    }
};