#include "RRE.hpp"

using namespace RRE;

int main() {
    GamePrototype game("[RRE] Simple Player Example");

    STexture t_maja("./resources/maja_nobg_small.png");
    PlayerPrototype maja_red(200, 100, &t_maja);

    game.run();
}