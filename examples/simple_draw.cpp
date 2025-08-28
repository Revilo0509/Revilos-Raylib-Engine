#include "RRE.hpp"
#include "raymath.h"

#include "./gui_layout_name.h"

using namespace RRE;

class Game : public GamePrototype {
  private:
    const int cooldown = 20;
    int cooldownTimer = 0;

  public:
    void draw() override { DrawFPS(10, 10); }
    void update() override { cooldownTimer -= 1; }
};

class Player : public Object, Updatable {
  public:
    Player(float x, float y, STexture* texture) : Object(x, y, texture) {}

    void update() {
        pos += Funcs::getTopDownPlayerMovement();
    }
};

int main() {
    Game game;

    STexture t_maja("./res/maja_nobg_small.png");
    Object maja(100, 100, &t_maja);

    STexture t_maja_red("./res/maja_nobg_small_red.png");
    Player maja_red(200, 100, &t_maja_red);

    game.run();
}