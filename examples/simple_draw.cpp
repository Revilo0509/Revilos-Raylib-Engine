#include "RRE.hpp"
#include "raymath.h"

using namespace RRE;

class Game : public GamePrototype {
  public:
    void draw() override { DrawFPS(10, 10); }
};

class Player : public Object, Updatable {
  public:
    using Object::Object;

    void update() { pos += Funcs::getTopDownPlayerMovement(); }
};

int main() {
    Game game;

    STexture t_maja("./resources/maja_nobg_small.png");
    Object maja(100, 100, &t_maja);

    STexture t_maja_red("./resources/maja_nobg_small_red.png");
    Player maja_red(200, 100, &t_maja_red);

    game.run();
}